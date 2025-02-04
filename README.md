# A helper module for SILVER to support the yosys synthesis
This module helps you parse the synthesized Verilog file generated by Yosys and creates a netlist that SILVER can process.
Although SILVER can handle synthesized Verilog files in non-netlist format by adding annotations such as (* SILVER="attribute" *) to the code, we believe the netlist format is more suitable not only for SILVER but also for other verification tools.
The concept of this module is to provide a solution for converting Verilog source code into a netlist format.  

A directory LIB and a yosys script synthe.ys are forked/copied from [SILVER](https://github.com/Chair-for-Security-Engineering/SILVER/tree/master/yosys) and have been modified by Haruka Hirata. Additionally, a shell script synthesize.sh is from [PROLEAD](https://github.com/ChairImpSec/PROLEAD/tree/main/yosys/syn) (and has been modified too). The shell script is useful than the yosys script, we thus recommend you to use this.

We have tested for verilog implementations of DOM1, DOM2, DOM3, CMS2, and CMS3 so far. Those implmentations are just for unit tests, and we are not sure that how the module works for large complicated modules.

A module which you want to test must be described as `MyAND(input [NUM_BIT-1:0] share-0, .., input [NUM_BIT-1:0] share-d, output ..);`, so you are not allowed combine d-shares input to one input value with d * NUM_BIT bit, e.g., `MyAND(input [15:0] share); // [15:8] -> share 0 and [7:0] -> share 1`. Some netlist we have conducted the test are included in a directory `test_2` along with verilog files.

## Quick start
Set $VERILOG_FILES and $TOP_MODULE according to your environment in `synth.ys`. Then execute commands like below;

```bash
yosys -s synth.ys
python interpreter.py result_netlist.v -o output.nl
```
or
```bash
chmod +x synthesize.sh
./synthesize.sh verilog $PATH_TO_VERILOG_DIR $TOP_MODULE
python interpreter.py result_netlist.v -o output.nl
```

#### C++ implementation
We provide C++ implementation of this module in a directory `cxx`. You can move to the directory, then execume `make` command to build it.
```bash
cd cxx
make
./convert $SYNTHESIZED_VERILOG_FILE
```
!CAUTION!
THERE IS NO ERROR HANDLING MECHANISM.  



We note that the resulting netlists are not identical for Python and C++ implmentation. However, the result of the verification by SILVER is same.  
Our Yosys version is `Yosys 0.48+47 (git sha1 cbb95cb51, g++ 13.2.0 -fPIC -O3)`, but the older Yosys would also work.
