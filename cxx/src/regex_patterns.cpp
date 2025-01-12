#include <regex>

#include "regrex_patterns.hpp"

namespace RegexPatterns {
    const std::regex match_gate("\\(\\.A\\((.+)\\),\\.B\\((.+)\\),\\.Y\\((.+)\\)\\)");
    const std::regex match_reg("\\.D\\((.+)\\),\\.Q\\((.+)\\)\\)");
    const std::regex match_comment("[\\(/]\\*.+\\*[\\)/]");
    const std::regex match_range("([a-zA-Z0-9]+?)\\[(\\d+?):(\\d+?)\\]");
    //const std::regex match_range("\\[(\\d+?):(\\d+?)\\]");
}

