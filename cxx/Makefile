CXX=g++
CXXFLAGS=-O3 -Wall -Wextra
INC_DIR=./inc
SRC=./src
SRCS = $(wildcard $(SRC)/*.cpp)

%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -I $(INC_DIR) -c $< -o $@ 

all: read_verilog.o regex_patterns.o convert.o
	$(CXX) $(CXXFLAGS) -I $(INC_DIR) $? -o convert

clean:
	$(RM) convert a.out *.o 

.PHONY:
	clean
