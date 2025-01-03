# A helper module for SILVER to support the yosys synthesis
This module helps you to parse the synthesized verilog file by yosys and make a netlist correspoinding to SILVER tool.


A directory LIB and a yosys script synthe.ys are forked/copied from [SILVER](https://github.com/Chair-for-Security-Engineering/SILVER/tree/master/yosys) and have been modified by ykm11. Additionally, a shell script synthesize.sh is from [PROLEAD](https://github.com/ChairImpSec/PROLEAD/tree/main/yosys/syn) (and has been modified too). The shell script is useful than the yosys script, we thus recommend you to use this.

We have tested for verilog implementations of DOM1, DOM2, DOM3, CMS2, and CMS3 so far. Those implmentations are just for unit tests, and we are not sure that how the module works for large complicated modules.

The current implementation of the module support a representation like `MyAND([NUM_SHARES_X-1:0] X, [NUM_SHARES_Y-1:0] Y);`, so MyAND is 1-bit N-shares.
When the representaion is `MyAND([NUM_BIT-1:0] share-0, .., [NUM_BIT-1:0] share-d);`, you need to fix the codes. Please see lines regarding processings for inputs.  

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
