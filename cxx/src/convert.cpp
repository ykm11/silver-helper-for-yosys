#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include "read_verilog.hpp"
#include "gate_struct.hpp"
#include "regrex_patterns.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Give me a netlist file");
        return 1;
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

    analyze(result, gates, registers, inputs, outputs);

    for (size_t i = 0; i < result.size(); i++) {
        printf("%s\n", result[i].c_str());
    }
}
