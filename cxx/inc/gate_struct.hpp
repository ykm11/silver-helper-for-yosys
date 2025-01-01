#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class Dff {
    public:
        std::string in;
        std::string out;

        Dff() : in("NULL"), out("NULL") {}

        Dff(std::string &dff_in, std::string &dff_out) : in(dff_in), out(dff_out) {
        }
};

class Gate {
    public:
    //private:
        std::string logic;
        std::vector<std::string> g_in;
        std::vector<std::string> g_out;

    //public:
        Gate() {
            logic = "NULL";
            g_in.reserve(2); // NULL
            g_out.reserve(1);
        }
        Gate(std::string &logic_name, std::vector<std::string> &gin, 
               std::vector<std::string> &gout) : logic(logic_name) {
            std::transform(logic.begin(), logic.end(), logic.begin(), ::tolower);
            g_in.resize(gin.size());
            g_out.resize(gout.size());
            std::copy(gin.begin(), gin.end(), g_in.begin());
            std::copy(gout.begin(), gout.end(), g_out.begin());
        }
};

class Gate_2i1o {
    private:
        std::string logic;
        std::vector<std::string> g_in;
        std::vector<std::string> g_out;

    public:
        Gate_2i1o() {
            logic = "";
            g_in.reserve(2);
            g_out.reserve(1);
        }
        Gate_2i1o(std::string logic_name, std::vector<std::string> &gin, 
               std::vector<std::string> &gout) {
            std::transform(logic_name.begin(), logic_name.end(), logic.begin(), ::tolower);
            g_in.resize(2);
            g_out.resize(1);
            g_in[0] = gin[0];
            g_in[1] = gin[1];
            g_out[0] = gout[0];
        }
};

