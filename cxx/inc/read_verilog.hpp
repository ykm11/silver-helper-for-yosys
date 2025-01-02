#pragma once

#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include "gate_struct.hpp"
#include "regrex_patterns.hpp"

const size_t TIME_OUT = 10;

int read_verilog(std::vector<Gate>&, std::vector<Dff>&, 
        std::vector<std::string>&, std::vector<std::string>&, 
        std::string&);

int analyze(std::vector<std::string>&,
        const std::vector<Gate>&, const std::vector<Dff>&, 
        const std::vector<std::string>&, const std::vector<std::string>&);

inline std::string trim(const std::string& str) {
    auto start = str.find_first_not_of(" \t\n\r\f\v"); // 空白以外の最初の位置
    if (start == std::string::npos) return ""; // 全部空白の場合は空文字列を返す

    auto end = str.find_last_not_of(" \t\n\r\f\v"); // 空白以外の最後の位置
    return str.substr(start, end - start + 1); // 必要な部分を切り取る
}
