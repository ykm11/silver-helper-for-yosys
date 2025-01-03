#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdio>

#include "read_verilog.hpp"
#include "gate_struct.hpp"
#include "regrex_patterns.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Give me a verilog file");
        return 1;
    }
    FILE *outFile = nullptr;
    if (argc > 2) {
        outFile = fopen(argv[2], "w");
        if (outFile == nullptr) {
            perror("The specified file for the output couldn't be opened");
            return 1;
        }
    }

    std::string filename = argv[1];

    std::vector<Gate> gates;
    std::vector<Dff> registers;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
    std::vector<std::string> result;

    gates.reserve(128);
    registers.reserve(128);
    inputs.reserve(128);
    outputs.reserve(128);
    result.reserve(256);

    read_verilog(gates, registers, inputs, outputs, filename);
    std::sort(gates.begin(), gates.end(), 
        [](const Gate &x, const Gate &y) {
            if (x.logic == y.logic) {
                if (x.g_in[0] == y.g_in[0]) {
                    return x.g_in[1] > y.g_in[1];
                }
                return x.g_in[0] > y.g_in[0];
            }
            return x.logic > y.logic;
        });

    parse(result, gates, registers, inputs, outputs);

    if (outFile == nullptr) {
        for (size_t i = 0; i < result.size(); i++) {
            printf("%s\n", result[i].c_str());
        }
    } else {
        for (size_t i = 0; i < result.size(); i++) {
            fprintf(outFile, "%s\n", result[i].c_str());
        }
        printf("The resulting output has been written in %s\n", argv[2]);
    }
}
