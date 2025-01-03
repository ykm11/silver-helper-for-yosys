CXX=g++
CXXFLAGS=-O3 -Wall -Wextra
INC_DIR=./inc
SRC=./src
SRCS = $(wildcard $(SRC)/*.cpp)
BUILD_DIR=./build

$(BUILD_DIR)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -I $(INC_DIR) -c $< -o $@ 

all: $(BUILD_DIR)/read_verilog.o $(BUILD_DIR)/regex_patterns.o $(BUILD_DIR)/convert.o
	$(CXX) $(CXXFLAGS) -I $(INC_DIR) $? -o convert

clean:
	$(RM) convert a.out $(BUILD_DIR)/*.o 

.PHONY:
	clean
