import argparse
import re

TIMEOUT = 10

match_comment = re.compile(r"[\(/]\*.+\*[\)/]") # for yosys comment-out
#match_range = re.compile(r"\[(\d+?):(\d+?)\]")
match_range = re.compile(r"([a-zA-Z0-9]+?)\[(\d+?):(\d+?)\]")
match_gate_2i1o = re.compile(r"\(\.A\((.+)\),\.B\((.+)\),\.Y\((.+)\)\)")
match_reg = re.compile(r"\.D\((.+)\),\.Q\((.+)\)\)")

def read_verilog(f_name):
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
                text = text.rstrip(";")
                op = text.split()
                operand = op[0]

                if operand == "input":
                    name_value = op[-1]
                    if name_value.lower() in ["clk", "rst"]: # identifier of the clock signal and reset should be "clk" and "rst", respectively
                        text = ""
                        continue

                    if len(op) < 3: # input a
                        inputs.append(f"{name_value}")
                    else:           # input [N:M] a
                        inputs.append(f"{name_value}{op[1]}")

                elif operand == "output":
                    name_value = op[-1]

                    if len(op) < 3:
                        outputs.append(f"{name_value}")
                    else:
                        outputs.append(f"{name_value}{op[1]}")


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
                
    return inputs, outputs, gates, registers

def make_silver_syntax(inputs, outputs, gates, registers):
    wires = []
    res = []
    gates = sorted(gates, key=lambda x: (x["gate"], x["in"][0], x["in"][1]))

    # make input wires
    num_input_val = 0
    for input_val in inputs:
        identifier = match_range.search(input_val)
        if identifier: # array input

            if identifier[1] in ["refreshing", "randomness"]:
                range_start = int(identifier[3])
                range_end = int(identifier[2])
                for i in range(range_start, range_end+1):
                    wires.append(f"{identifier[1]}[{i}]")
                    res.append(f"ref {len(res)}")
            else:
                range_start = int(identifier[3])
                range_end = int(identifier[2])
                for i in range(range_start, range_end+1):
                    wires.append(f"{identifier[1]}[{i}]")
                    #res.append(f"in {len(res)} {num_input_val}_{i}")
                    res.append(f"in {len(res)} {i}_{num_input_val}")

        else: # mono input
            wires.append(input_val)

            if input_val in ["refreshing", "randomness"]:
                res.append(f"ref {len(res)}")

            else:
                res.append(f"in {len(res)} {num_input_val}_0")

        num_input_val += 1

    loop_count = 0
    current_num_gates = len(gates)
    current_num_registers = len(registers)
    while len(gates) > 0  or len(registers) > 0:

        for _ in range(5):
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
            print("Registers:", registers)
            print("Gate:", gates)
            assert(False)


    num_output_val = 0
    for output_val in outputs:
        identifier = match_range.search(output_val)
        
        if identifier:
            range_start = int(identifier[3])
            range_end = int(identifier[2])
            for i in range(range_start, range_end+1):
                reg_in_num = wires.index(f"{identifier[1]}[{i}]")
                t = f"out {reg_in_num} {i}_{num_output_val}"
                #t = f"out {reg_in_num} {num_output_val}_{i}"
                res.append(t)

        else:
            reg_in_num = wires.index(f"{output_val}")
            t = f"out {reg_in_num} 0_{num_output_val}"
            #t = f"out {reg_in_num} {num_output_val}_0"
            res.append(t)

        num_output_val += 1

    return res


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("infile", help="Verilog file (synthesized by yosys)")
    parser.add_argument("-o", "--outfile", help="Filename or path to write results.")
    args = parser.parse_args()
    infile = args.infile


    inputs, outputs, gates, registers = read_verilog(infile)
    res = make_silver_syntax(inputs, outputs, gates, registers)
    if args.outfile:
        with open(args.outfile, "w", newline="\n") as f:
            for line in res:
                f.write(line + "\n")
    else:
        for line in res:
            print(line)

