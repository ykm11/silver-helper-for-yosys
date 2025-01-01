#include <regex>

#include "regrex_patterns.hpp"

namespace RegexPatterns {
    const std::regex match_gate("\\(\\.A\\((.+)\\),\\.B\\((.+)\\),\\.Y\\((.+)\\)\\)");
    const std::regex match_reg("\\.D\\((.+)\\),\\.Q\\((.+)\\)\\)");
}
