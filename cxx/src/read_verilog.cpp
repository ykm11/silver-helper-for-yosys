#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include "read_verilog.hpp"
#include "gate_struct.hpp"
#include "regrex_patterns.hpp"


int parse(std::vector<std::string> &result,
        const std::vector<Gate> &gates, const std::vector<Dff> &registers, 
        const std::vector<std::string> &inputs, const std::vector<std::string> &outputs) {
    std::vector<std::string> wires;

    std::smatch m;
    std::string identifier;
    std::vector<Gate> n_gates(gates);
    std::vector<Dff> n_registers(registers);
    size_t num_input_val = 0;
    size_t num_output_val = 0;
    size_t range_start, range_end;

    // for inputs
    for (size_t i = 0; i < inputs.size(); i++) {
        // input a[n:m];
        if (std::regex_search(inputs[i], m, RegexPatterns::match_range)){
            identifier = m[1];
            range_start = std::stol(m[3]);
            range_end = std::stol(m[2]);
            for (size_t i = range_start; i <= range_end; i++) {
                wires.push_back(identifier + "[" + std::to_string(i) + "]");

                if (identifier == "refreshing" || identifier == "randomness") {
                    result.push_back("ref " + std::to_string(result.size()));
                } else {
                    result.push_back("in " + 
                            std::to_string(result.size()) +
                            " " + 
                            std::to_string(num_input_val) +
                            "_" +
                            std::to_string(i)
                            );
                }
            }
        } else {
            // input a;
            identifier = inputs[i];
            wires.push_back(identifier);
            if (identifier == "refreshing" || identifier == "randomness") {
                result.push_back("ref " + std::to_string(result.size()));
            } else {
                result.push_back("in " + 
                        std::to_string(result.size()) +
                        " " + 
                        std::to_string(num_input_val) +
                        "_0"
                        );
            }
        }
        num_input_val++;
    }

    size_t current_gate_num, current_register_num;
    size_t count_loop = 0;
    while (n_gates.size() > 0 || n_registers.size() > 0) {
        current_gate_num = n_gates.size();
        current_register_num = n_registers.size();

        for (size_t i = 0; i < 3; i++) {
            for (int i = n_gates.size()-1; i >= 0; i--) {
                auto in_a = std::find(wires.begin(), wires.end(), n_gates[i].g_in[0]);
                auto in_b = std::find(wires.begin(), wires.end(), n_gates[i].g_in[1]);

                if (in_a != wires.end() && in_b != wires.end()) {
                    result.push_back(
                            n_gates[i].logic + 
                            " " +
                            std::to_string(std::distance(wires.begin(), in_a)) +
                            " " +
                            std::to_string(std::distance(wires.begin(), in_b))
                            );

                    wires.push_back(n_gates[i].g_out[0]);
                    n_gates.erase(n_gates.begin() + i);
                }
            }
        }
        for (int i = n_registers.size()-1; i >= 0; i--) {
            auto in = std::find(wires.begin(), wires.end(), n_registers[i].in);

            if (in != wires.end()) {
                result.push_back(
                        "reg " +
                        std::to_string(std::distance(wires.begin(), in))
                        );

                wires.push_back(n_registers[i].out);
                n_registers.erase(n_registers.begin() + i);
            }
        }

        if (current_gate_num == n_gates.size() && 
                current_register_num == n_registers.size()) {
            count_loop++;

            if (count_loop > TIME_OUT) {
                abort();
            }
        } else{
            count_loop = 0;
        }
    }

    // for outputs
    for (size_t i = 0; i < outputs.size(); i++) {
        // a[n:m]
        if (std::regex_search(outputs[i], m, RegexPatterns::match_range) ){
            range_start = std::stol(m[3]);
            range_end = std::stol(m[2]);

            for (size_t i = range_start; i <= range_end; i++) {
                identifier = m[1].str() + "[" + std::to_string(i) + "]";
                auto in = std::find(wires.begin(), wires.end(), identifier);

                if (in != wires.end()) {
                    result.push_back(
                        "out " +
                        std::to_string(std::distance(wires.begin(), in)) + 
                        " " + 
                        std::to_string(num_output_val) +
                        "_" +
                        std::to_string(i)
                        );
                }
            }
        } else {
            // a
            auto in = std::find(wires.begin(), wires.end(), outputs[i]);
            if (in != wires.end()) {
            result.push_back(
                "out " +
                std::to_string(std::distance(wires.begin(), in)) + 
                " " + 
                std::to_string(num_output_val) +
                "_0"
                );        
            }
        }
        num_output_val++;
    }


    return 0;
}

int read_verilog(std::vector<Gate> &gates, std::vector<Dff> &registers, 
        std::vector<std::string> &inputs, std::vector<std::string> &outputs,
        std::string& filename) {
    std::ifstream file(filename);
    // No check for opening the file

    size_t num_input_val = 0;
    size_t word_start, word_end;

    gates.reserve(128);
    registers.reserve(128);

    std::smatch m;

    std::string line;
    std::string ope;
    std::string text = "";
    std::vector<std::string> words;
    words.reserve(5);

    while (std::getline(file, line)) {
        line = std::regex_replace(line, RegexPatterns::match_comment, ""); // remove comment-out generated by yosys
        if (line.size() == 0) continue;

        text += trim(line);
        if (text.at(text.size()-1) == ';') {
            text.erase(text.size()-1);
            word_start = 0;
            word_end = 0;

            while ((word_end = text.find(' ', word_start)) != std::string::npos) {
                if (word_end != word_start) { // no continuous spaces
                    words.push_back(text.substr(word_start, word_end - word_start));
                }
                word_start = word_end + 1;  // starting point to the next word
            }
            // add the last word
            if (word_start < text.length()) {
                words.push_back(text.substr(word_start));
            }


            ope = words[0];
            std::transform(ope.begin(), ope.end(), ope.begin(), ::tolower);
            if (ope == "input") {
                if (words[words.size()-1] == "clk" || words[words.size()-1] == "rst") {
                    text = "";
                    words.clear();
                    continue;
                }

                if (words.size() < 3) { // input a
                    inputs.push_back(words[1]);

                } else {                // input [n:m] a
                    inputs.push_back(words[2] + words[1]);
                }
#if DEBUG
                printf("%s\n", words[words.size()-1].c_str());
#endif
                num_input_val++;

            } else if (ope == "output") {
                if (words.size() < 3) { // output a
                    outputs.push_back(words[1]);
                } else {                // output [n:m] a
                    outputs.push_back(words[2] + words[1]);
                }
#if DEBUG
                printf("%s\n", words[words.size()-1].c_str());
#endif

 
            } else if (ope == "dff") {
                if (std::regex_search(text, m, RegexPatterns::match_reg)) {
#if DEBUG
                    printf("Reg: %s, %s\n", 
                            m[1].str().c_str(), 
                            m[2].str().c_str());
#endif
                    Dff reg;
                    reg.in  = m[1].str();
                    reg.out = m[2].str();
                    registers.push_back(reg);
                }
            } else if (ope == "and" || ope == "xor") {
                if (std::regex_search(text, m, RegexPatterns::match_gate)) {
#if DEBUG
                    printf("%s: %s, %s, %s\n", 
                            ope.c_str(),
                            m[1].str().c_str(), 
                            m[2].str().c_str(), 
                            m[3].str().c_str());
#endif

                    Gate gate;
                    std::vector<std::string> gin = {m[1], m[2]};
                    std::vector<std::string> gout = {m[3]};
                    gate.logic = ope;
                    gate.g_in = gin;
                    gate.g_out = gout;
                    gates.push_back(gate);
                }
            }
            text = "";
            words.clear();
        }

    }
    file.close();

    return 0;
}
