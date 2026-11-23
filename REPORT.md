HOW TO RUN CODE/ COMMANDS:

This code was compiled and ran successfully on a windows 11 laptop in vscode using WSL in an ubuntu environment.

make: compiles the program.

./execute: runs the program

make clean: removes all .o files and execute.

There is no user interface, and this program requires the "pp3_input.txt" file to run. If you would like to edit and test new implementations, please edit that text file.

LOGIC/FUNCTIONALITY OF OPERATIONS:

conditional route: takes the flags and the operation. If an operation has a condition it meets, returns 1. If it doesnt meet the condition, returns 0. If there is no condition, returns 1. if the operation has an appended s, also returns 1.

ADD: Before the operation, runs conditional route. If there is no conditional or it meets the condition, it continues the chain (else it skips the chain). Finds the location of register 1 in rArr. then coverts the values in op2 and op3 (regardless if they are registers or immediates) into hex. If op2 is a register and not an immediate value, the destination register is updated to hold the value of temp1+temp (the two hex values). Otherwise, outputs an error. In addition, if it has S at the end, it will update all of the  relevant flags. 

MOV/MVN: Before the operation, runs conditional route. If there is no conditional or it meets the condition, it continues the chain (else it skips the chain). Finds the location of register 1 in rArr. then coverts the values in op2 and op3 (regardless if they are registers or immediates) into hex, though op3 is disregarded in this case. If the destination register is a valid register and the command is MOV, stores the value in the temp1 (the hex of op2) into the destination register. Else if the command is MVN, it instead stores the inverse int he register using bitwise xor. Otherwise, outputs an error. In addition, if it has S at the end, it will update all of the relevabt flags. 

SUB: Before the operation, runs conditional route. If there is no conditional or it meets the condition, it continues the chain (else it skips the chain). Finds the location of register 1 in rArr. then coverts the values in op2 and op3 (regardless if they are registers or immediates) into hex. If op2 is a register and not an immediate value, the destination register is updated to hold the value of temp1+temp (the two hex values). Otherwise, outputs an error. In addition, if it has S at the end, it will update all of the  relevant flags. 

CMP:Before the operation, runs conditional route. If there is no conditional or it meets the condition, it continues the chain (else it skips the chain).Finds the location of register 1 in rArr. then coverts the values in op2 and op3 (regardless if they are registers or immediates) into hex. CMP will calculate all of the flags and update them in the same manner as SUB does, just without storing the result. 

AND: Before the operation, runs conditional route. If there is no conditional or it meets the condition, it continues the chain (else it skips the chain). If operand2 is a register and not an immediate value, finds the location of op1 in the array, converts op2 and op3 into hex (stored in temp1 and temp2), and then performs bitwise AND on temp 1 and temp2, which is then stored in the corresponding location in the array. otherwise, outputs an error. In addition, if it has S at the end, it will update all of the  relevant flags. 

XOR:Before the operation, runs conditional route. If there is no conditional or it meets the condition, it continues the chain (else it skips the chain). If operand2 is a register and not an immediate value, finds the location of op1 in the array, converts op2 and op3 into hex (stored in temp1 and temp2), and then performs bitwise XOR on temp 1 and temp2, which is then stored in the corresponding location in the array. otherwise, outputs an error. In addition, if it has S at the end, it will update all of the  relevant flags.

OR: Before the operation, runs conditional route. If there is no conditional or it meets the condition, it continues the chain (else it skips the chain). If operand2 is a register and not an immediate value, finds the location of op1 in the array, converts op2 and op3 into hex (stored in temp1 and temp2), and then performs bitwise OR on temp 1 and temp2, which is then stored in the corresponding location in the array. otherwise, outputs an error. In addition, if it has S at the end, it will update all of the  relevant flags.

LDR: Before the operation, runs conditional route. If there is no conditional or it meets the condition, it continues the chain (else it skips the chain). We have to format it properly, so we reformat op2 into a temporart variable. We find the corresponding register number of this, and then store the data in it into a variable. Since we are working with memory now, we must do some more processing to find which memory number corresponds to the memory value stored in the register. Once we find this, we grab the data stored in that memory address, and then store it in the register in op1.

STR: Before the operation, runs conditional route. If there is no conditional or it meets the condition, it continues the chain (else it skips the chain). We find the regnum of op1 initially as well as store the corresponding data into temp2, before we start on op2. We have to format it properly, so we reformat op2 into a temporart variable. We find the corresponding register number of this, and then store the data in it into a variable. Since we are working with memory now, we must do some more processing to find which memory number corresponds to the memory value stored in the register. Once we find this, we store the data in temp2 into the corresponding memory location

LSL/LSR: Before the operation, runs conditional route. If there is no conditional or it meets the condition, it continues the chain (else it skips the chain). Both instructions only diverge at the end, so they both follow these same operations: if op2 is not an immediate value, they find the regnum of op1, and then perform a conversion for formatting. They will then perform birwise shifts left/right, respectivaly. Finally, the value from this shift is stored in the register found earlier. In addition, if it has S at the end, it will update all of the  relevant flags.

While branches are present, they are a holdover and can be ignored as they are not implemented properly.

INVALID CASES: If the statement has too few or too many operands, it will print incorrect operand amount. The dataTrans func outputs 2 errors: memory address out of range, and tregister is invalid. if the memory address is less than 0x100 and more than 0x110, it will return an error. If register is less then 0 or greater than 11, it returns an error as well. In bitwiseOp, there is one invalid case: if operand 2 is not a register value. If it is anything else, it automatically prints an error. in arithOp, there are 2 invalid cases: if the oeprand is an immediate value (operand 2 for ADD and SUB, and operand 1 for CMP), and if the register is above r11 or below r0 for MOV. Finally, in processInstruction, if an input operation is not named properly, it will output an error statement. There are miscellaneous error statements that are mainly meant for bug testing. These can be ignored.

Shortcoming/issues: There are probably some off by one errors in the flag values, but from my testing with the given test file, it works properly. another issues is str/ldr will not work if the value stored in the  second register is not a memory address. 