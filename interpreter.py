import argparse
import re

TIMEOUT = 10

match_comment = re.compile(r"[\(/]\*.+\*[\)/]") # for yosys comment-out
match_range = re.compile(r"\[(\d+?):(\d+?)\]")
match_gate_2i1o = re.compile(r"\(\.A\((.+)\),\.B\((.+)\),\.Y\((.+)\)\)")
match_reg = re.compile(r"\.D\((.+)\),\.Q\((.+)\)\)")

def interpre(f_name):
    res = []
    inputs = []
    outputs = []
    gates = []
    registers = []

    with open(f_name) as f:
        text = ""
        input_val_num = 0
        for line in f:
            line = line.strip()
            substituted_line = match_comment.sub("", line)
            if len(substituted_line) == 0:
                continue

            text += substituted_line
            if text[-1] == ";":
                text = text.strip(";")
                op = text.split()
                operand = op[0]
                length = match_range.search(op[1])

                inout_start = 0
                inout_end = 0
                if length:
                    inout_start = int(length[2])
                    inout_end = int(length[1])

                if operand == "input":
                    if op[-1].lower() in ["clk", "rst"]: # identifier of the clock signal and reset should be "clk" and "rst", respectively
                        text = ""
                        continue

                    for i in range(inout_start, inout_end+1):
                        if inout_end == 0:
                            inputs.append(f"{op[-1]}")
                        else:
                            inputs.append(f"{op[-1]}[{i}]")

                        if op[-1].lower() in ["refreshing", "randomness"]:
                            res.append(f"ref {len(res)}")
                        else:
                            res.append(f"in {len(res)} {input_val_num}_{i}")
                            #res.append(f"in {len(res)} {i}_{input_val_num}") # when the input is represented as ([N:0] share 1, [N:0] share 2, ..)

                    if op[-1].lower() not in ["refreshing", "randomness"]:
                        input_val_num += 1

                elif operand == "output":
                    if inout_end == 0:
                        outputs.append(op[-1])
                        text = ""
                        continue
                    for i in range(inout_start, inout_end+1):
                        outputs.append(f"{op[-1]}[{i}]")

                elif operand in ["AND", "XOR"]:
                    ## ex: AND _0_ (.A(b[0]),.B(a[1]),.Y(c2_0))
                    d = {}
                    d["gate"] = op[0].lower()

                    m = match_gate_2i1o.search(op[2])
                    if not m:
                        print("[*] Gate not found")
                        assert(False)

                    d["in"] = sorted([m[1], m[2]])
                    d["out"] = [m[3]]
                    gates.append(d)

                elif operand == "DFF":
                    m = match_reg.search(op[2])
                    if not m:
                        print("[*] Register not found")
                        assert(False)
                    d = {}
                    d["in"] = m[1]
                    d["out"] = m[2]
                    registers.append(d)

                text = ""
                
    return res, inputs, outputs, gates, registers


def make_silver_syntax(res, inputs, outputs, gates, registers):
    wires = inputs.copy()
    gates = sorted(gates, key=lambda x: (x["gate"], x["in"][0], x["in"][1]))

    loop_count = 0
    current_num_gates = len(gates)
    current_num_registers = len(registers)
    while len(gates) > 0  or len(registers) > 0:

        for _ in range(3):
            for i in range(len(gates)-1, -1, -1):
                gate = gates[i]
                gate_in = gate["in"]
                gate_out = gate["out"][0]
                if set(gate_in).issubset(wires):
                    
                    gate_in_num = [str(wires.index(wire)) for wire in gate_in]
                    t = f"{gate['gate']} {' '.join(gate_in_num)}"

                    res.append(t)
                    wires.append(gate_out)
                    gates.pop(i)

        for i in range(len(registers)-1, -1, -1):
            register = registers[i]
            reg_in = register["in"]
            reg_out = register["out"]
                
            if reg_in in wires:
                reg_in_num = wires.index(reg_in)
                t = f"reg {reg_in_num}"

                res.append(t)
                wires.append(reg_out)
                registers.pop(i)


        current_num_gates_tmp = len(gates)
        current_num_registers_tmp = len(registers)
        if current_num_registers == current_num_registers_tmp and \
                current_num_gates == current_num_gates_tmp:
                    loop_count += 1
        else:
            current_num_gates = current_num_gates_tmp
            current_num_registers = current_num_registers_tmp
            loop_count = 0

        if loop_count == TIMEOUT:
            print("[*] TIME OUT")
            assert(False)


    out_share_num = 0
    for i in range(len(outputs)-1, -1, -1):
        out = outputs[i]
            
        if out in wires:
            reg_in_num = wires.index(out)
            t = f"out {reg_in_num} {len(outputs)-1-i}_{out_share_num}"
            
            res.append(t)
            wires.append(out)
            outputs.pop(i)

            out_share_num += 1

    return res


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("infile", help="Verilog file (synthesized by yosys)")
    parser.add_argument("-o", "--outfile", help="Filename or path to write results.")
    args = parser.parse_args()
    infile = args.infile


    res, inputs, outputs, gates, registers = interpre(infile)
    res = make_silver_syntax(res, inputs, outputs, gates, registers)
    if args.outfile:
        with open(args.outfile, "w", newline="\n") as f:
            for line in res:
                f.write(line + "\n")
    else:
        for line in res:
            print(line)

