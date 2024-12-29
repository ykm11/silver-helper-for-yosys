# A helper module for SILVER to support the yosys synthesis

A directory LIB and a yosys script synthe.ys are forked/copied from [SILVER repository](https://github.com/Chair-for-Security-Engineering/SILVER/tree/master/yosys) and have been modified by ykm11.

## Quick start
Set $VERILOG_FILES and $TOP_MODULE according to your environment. Then execute commands like below;

```bash
yosys -s synth.ys
python interpreter.py result_netlist.v -o output.nl
```
