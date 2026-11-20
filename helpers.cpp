#include "helpers.h"
#define SIZE 200

//This function process the raw data and calls the corresponding category of operation processing
// It also calls the error checking to make sure its valid
void processInstruction(ifstream& fp, int& length, uint32_t** m, uint32_t** r, int* opArr, int& flagZ){
    string fpString, operation, operand1 = "err", operand2 = "err", operand3 = "err";
    char tempChar;
    int opCount, flagC = 0, flagN = 0, flagV =0, sort;
    bool err, isBranch = false;

    while(getline(fp, fpString)){
        stringstream ss(fpString);
        ss >> operation;
        ss.get(tempChar);
        if(tempChar == ' '){
            opCount = 1; 
            ss >> operand1;
        }

        ss.get(tempChar);
        if(tempChar == ' '){
            opCount = 2; 
            ss >> operand2;
        }

        ss.get(tempChar);
        if(tempChar == ' '){
            opCount = 3; 
            ss >> operand3;
        }
        
        ss.get(tempChar);
        if(tempChar == ' '){
            opCount = 4; 
        }

        cleanUp(operand1, operand2, operand3);
        err = errorCheck(operation, operand1, operand2, operand3, opArr[length]);
        opSort(operation, sort);
        if(sort == 0){
            isBranch = false;
            if(err == true){
                arithOp(r, operation, operand1, operand2, operand3, flagZ, err, flagN, flagC, flagV);
            }
            display(r, m, operation, operand1, operand2, operand3, flagZ, err, isBranch, flagN, flagC, flagV);
        }
        else if(sort == 1){
            isBranch = false;
            if(err == true){
                bitwiseOp(r, operation, operand1, operand2, operand3, err, flagN, flagZ, flagC, flagV);
            }
            display(r, m, operation, operand1, operand2, operand3, flagZ, err, isBranch, flagN, flagC, flagV);
        }
        else if(sort == 2){
            isBranch = false;
            if(err == true){
                dataTrans(r, m, operation, operand1, operand2, err, flagN, flagC, flagZ, flagV);
            }
            display(r, m, operation, operand1, operand2, operand3, flagZ, err, isBranch, flagN, flagC, flagV);
        }
        else if(sort == 3){
            isBranch = true;
            if(err == true){
                branchOp(operation, operand1, flagZ);
            }
            display(r, m, operation, operand1, operand2, operand3, flagZ, err, isBranch, flagN, flagC, flagV);
            
        }
        else if(sort == 4){
            isBranch = false;
            if(err == true){
                shiftOp(r, operation, operand1, operand2, operand3,err, flagN, flagZ, flagC, flagV);
            }
            display(r, m, operation, operand1, operand2, operand3, flagZ, err, isBranch, flagN, flagC, flagV);
        }
        else{
            cout << "Invalid operation error" << endl;
        }
    
        operation = "NULL";
        opCount = 0;
        
        operand1 = "err";
        operand2 = "err";
        operand3 = "err";
        
        length++;
    }

}

//This handles the logic for properly displaying output depending on which operation and if it triggered errors
void display(uint32_t** r, uint32_t** m, string operation, string operand1, string operand2, string operand3, int flagZ, bool err, bool isBranch, int flagN, int flagC, int flagV){
    uint32_t temp;
    cout << operation  << " ||||| " << operand1;
        if(operand2 != " ||||| "){
            cout << " ||||| " << operand2;
        }
        if(operand3 != " ||||| " && operand3 != ""){
           cout << " ||||| " << operand3;
        }
        cout << endl;
        cout << "N = " << flagN <<  " Z = " << flagZ << " C = " << flagC <<  " V = " << flagV << endl;
    if(err == true && isBranch == false){
        cout << "Register array:" << endl;
        for(int i = 0; i < 12; i++){
            temp = *r[i];
            cout << "R" << std::dec << i << " = 0x" << std::hex << temp << "|||";
        }
        cout << endl << "Memory Array:" << endl;
        
        for(int i = 0; i < 5; i++){
            cout << "0x" << *m[i] << ",  ";
        }
    }
    cout << endl << endl;
}

//this cleans up and properly seperates the operand variables
void cleanUp(string& op1, string& op2, string& op3){
    string temp1 = "", temp2 = "", temp3 = "", rework;
    int splitCount = 1;
    bool fixEnd = false;
    
    if (op2 != "err") op1 += op2;
    if (op3 != "err") op1 += op3;

    for(int i = 0; i < op1.length(); i++){
        char c = op1[i];
        
        if(c != ' ' && c != '#' && c != ','){
            if(splitCount == 1){
                temp1 += c;
            }
            if(splitCount == 2){
                temp2 += c;
            }
            if(splitCount == 3){
                temp3 += c;
            }
        }
        
        if(c == ','){
            splitCount++;
        }
    }
    
    op1 = temp1;
    op2 = temp2;
    op3 = temp3;
}

//Helper function to handle inserting into register and memory arrays
int findRegNum(string s){
    if(s == "R0"){
        return 0;
    }
    if(s == "R1"){
        return 1;
    }
    if(s == "R2"){
        return 2;
    }
    if(s == "R3"){
        return 3;
    }
    if(s == "R4"){
        return 4;
    }
    if(s == "R5"){
        return 5;
    }
    if(s == "R6"){
        return 6;
    }
    if(s == "R7"){
        return 7;
    }
    if(s == "R8"){
        return 8;
    }
    if(s == "R9"){
        return 9;
    }
    if(s == "R10"){
        return 10;
    }
    if(s == "R11"){
        return 11;
    }
    if(s == "0x100"){
        return 0;
    }
    if(s == "0x104"){
        return 1;
    }
    if(s == "0x108"){
        return 2;
    }
    if(s == "0x10c"){
        return 3;
    }
    if(s == "0x120"){
        return 4;
    }
    return 13;
}

//converts operands values into their proper values, then stores them to temp so as not to lose any data
void conversion(uint32_t** rArr, string op2, string op3, uint32_t& temp1, uint32_t& temp2){
    int regNum; 
    string manip;

    if(op2.size() >= 1 && op2[0] == 'R'){
        regNum = findRegNum(op2);
        temp1 = *rArr[regNum];
    }
    else if(!op2.empty() && op2[1] == 'x'){
        manip = op2;
        if(manip.size() >= 1 && manip[0] == '#'){
            manip = manip.substr(1);
        }
        if(manip.find("0x") != string::npos){
            manip = manip.substr(2);
            temp1 = stoi(manip, nullptr, 16);
        }
        else{
            temp1 = stoi(manip,nullptr,10);
        }
    }
    else if(!op2.empty()){
        manip = op2;
        if(manip[0] == '#'){
            manip = manip.substr(1);
        }
        if(!manip.empty() && isdigit(manip[0])){
            temp1 = stoi(manip, nullptr, 10);
        }
    }


    if(!op3.empty() && op3[0] == 'R'){
        regNum = findRegNum(op3);
        temp2 = *rArr[regNum];
    }
    else if(!op3.empty() && op3[1] == 'x'){
        manip = op3;
        if(!op3.empty() && manip[0] == '#'){
            manip = manip.substr(1);
        }
        if(manip.find("0x") != string::npos){
            manip = manip.substr(2);
            temp2 = stoi(manip, nullptr, 16);
        }
        else{
            temp2 = stoi(manip,nullptr,10);
        }
    }
    else{
        manip = op3;
        if(!op3.empty() && manip[0] == '#'){
            manip = manip.substr(1);
        }
        if(isdigit(manip[0])){
            temp2 = stoi(manip, nullptr, 10);
        }
    }
    

}

//This function takes in ADD, MOV, SUB and CMP operations and performs them accordingly.
void arithOp(uint32_t** rArr, string operation, string op1, string op2, string op3, int& z, bool& err, int& n, int& c, int& v){
    uint32_t temp1, temp2, result;
    int regNum = 0, cond;
    conditionalRoute(operation, n, z, c, v, cond);

    if(operation[0] == 'A' && cond == 1){
        regNum = findRegNum(op1);
        conversion(rArr, op2, op3, temp1, temp2);
        if(op2[0] == 'R'){
            *rArr[regNum] = (temp1+temp2);
            if(operation.length() > 3 && operation[3] == 'S'){
                result = (temp1+temp2);
                if(result < temp1){
                    c = 1;
                }
                else{
                    c = 0;
                }
                if(result == 0){
                    z = 1;
                }
                else{
                    z = 0;
                }
                if(result >= 2147483648 && result <= 4294967295){
                    n = 1;
                }
                else{ 
                    n = 0;
                }
                if(result >= 2147483648 && (temp1 <= 2147483648 && temp2 <= 2147483648)){
                    v = 1;
                }
                else{
                    v = 0;
                }
                cout << "FLAGS: " << n << z << c << v << endl;
            }
        }
        else{
            cout << "Error: operand 2 can not be an immediate value." << endl;
            err = false;
        }

    }
    if(operation[0] == 'M' && cond == 1){
        regNum = findRegNum(op1);
        conversion(rArr, op2, op3, temp1, temp2);
        if(regNum >= 0 && regNum <= 11 && operation[2] == 'V'){
            *rArr[regNum] = temp1;
        }
        else if(regNum >= 0 && regNum <= 11 && operation[2] == 'N'){
            *rArr[regNum] = ~temp1;
        }
        else{
            cout << "cannot insert into a register that does not exist" << endl;
            err = false;
        }
    }


    if(operation[0] == 'S' && cond == 1){
        regNum = findRegNum(op1);
        conversion(rArr, op2, op3, temp1, temp2);
        if(op2[0] == 'R'){
            *rArr[regNum] = (temp1-temp2);
            if(operation.length() > 3 && operation[3] == 'S'){
                result = (temp1-temp2);
                if(temp1 >= temp2){
                    c = 1;
                }
                else{
                    c = 0;
                }
                if(result == 0){
                    z = 1;
                }
                else{
                    z = 0;
                }
                if(result >= 2147483648 && result <= 4294967295){
                    n = 1;
                }
                else{ 
                    n = 0;
                }
                if((temp1 < 2147483648 && temp2 >= 2147483648 && result >= 2147483648)|| (temp1 >=2147483648 && temp2 <2147483648 && result < 2147483648)){
                    v = 1;
                }
                else{
                    v = 0;
                }
            }
        }
        else{
            err = false;
            cout << "Error: operand 2 can not be an immediate value" << endl;
        }
    }
    if(operation[0] == 'C' && cond == 1){
        regNum = findRegNum(op1);
        conversion(rArr, op1, op2, temp1, temp2);
        if(op1[0] == 'R'){
            result = (temp1-temp2);
            if(temp1 >= temp2){
                c = 1;
            }
            else{
                c = 0;
            }
            if(result == 0){
                z = 1;
            }
            else{
                z = 0;
            }
            if(result >= 2147483648 && result <= 4294967295){
                n = 1;
            }
            else{ 
                n = 0;
            }
            if((temp1 < 2147483648 && temp2 >= 2147483648 && result >= 2147483648)|| (temp1 >=2147483648 && temp2 <2147483648 && result < 2147483648)){
                v = 1;
            }
            else{
                v = 0;
            }
        }
        else{
            err = false;
            cout << "Error: operand 1 can not be an immediate value" << endl;
        }
    }
}

//This function takes AND, OR, and XOR, and performs those operations and stores in destination register.
void bitwiseOp(uint32_t** r, string operation, string op1, string op2, string op3, bool& err, int& n, int& z, int& c, int& v){
    uint32_t temp1, temp2;
    int regNum = 0, cond;
    conditionalRoute(operation, n, z, c, v, cond);
    if(operation[0] == 'A' && cond == 1){
        if(op2[0] == 'R'){
            regNum = findRegNum(op1);
            conversion(r, op2, op3, temp1, temp2);
            *r[regNum] = (temp1 & temp2);
        }
        else{
            err = false;
            cout << "Error: Operand 2 can not be an immediate value." << endl;
        }
    }
    if(operation[0] == 'E' && cond == 1){
        if(op2[0] == 'R'){
            regNum = findRegNum(op1);
            conversion(r, op2, op3, temp1, temp2);
            *r[regNum] = (temp1 ^ temp2);
        }
        else{
            err = false;
            cout << "Error: Operand 2 can not be an immediate value." << endl;
        }
    }
    if(operation[0] == 'O' && cond == 1){
        if(op2[0] == 'R'){
            regNum = findRegNum(op1);
            conversion(r, op2, op3, temp1, temp2);
            *r[regNum] = (temp1 | temp2);
        }
        else{
            err = false;
            cout << "Error: Operand 2 can not be an immediate value." << endl;
        }
    }
}

//This function takes and executes LOAD and STORE functions.
void dataTrans(uint32_t** r, uint32_t** m, string operation, string op1, string op2, bool& err, int& n, int& c, int& z, int& v){
    int regNum, cond;
    string tempop2;
    uint32_t temp1, temp2;
    conditionalRoute(operation, n, z, c, v, cond);
    
    if(operation[0] == 'L' && cond == 1){
        if(op2[0] == '['){
            tempop2 = op2.substr(1, op2.length() - 2);
            regNum = findRegNum(tempop2);
            if(regNum >=0 && regNum <=11){
                temp1 = *r[regNum];
                if(temp1 == 0x100){
                    tempop2 = "0x100";
                }
                if(temp1 == 0x104){
                    tempop2 = "0x104";
                }
                if(temp1 == 0x108){
                    tempop2 = "0x108";
                }
                if(temp1 == 0x10c){
                    tempop2 = "0x10c";
                }
                if(temp1 == 0x110){
                    tempop2 = "0x110";
                }
                regNum = findRegNum(tempop2);
                temp2 = *m[regNum];
                regNum = findRegNum(op1);
                if(regNum >=0 && regNum <= 11){
                    *r[regNum] = temp2;
                    cout << "finsihed properly" << endl;
                }
                else{
                    err = false;
                    cout << "Destination register of LOAD is invalid." << endl;
                }
            }
            else{
                err = false;
                cout << "formatting error" << endl;
            }
        }
        else{
            err = false;
            cout << "Memory address is out of range." << endl;
        }
    }

    else if(operation[0] == 'S' && cond == 1){
        if(op2[0] == '['){
            regNum = findRegNum(op1);
            if(regNum >= 0 && regNum <=11){
                temp2 = *r[regNum];
                tempop2 = op2.substr(1, op2.length() - 2);
                regNum = findRegNum(tempop2);
                if(regNum >= 0 && regNum <= 11){
                    temp1 = *r[regNum];
                    
                    if(temp1 == 0x100){
                        regNum = 0;
                    }
                    
                    if(temp1 == 0x104){
                        regNum = 1;
                    }
                    
                    if(temp1 == 0x108){
                        regNum = 2;
                    }
                    
                    if(temp1 == 0x10c){
                        regNum = 3;
                    }
                    
                    if(temp1 == 0x110){
                        regNum = 4;
                    }
                    
                    if(regNum >= 0 && regNum <=4){
                        *m[regNum] = temp2;
                    }
                    
                    else{
                        cout << "Memory is out of range" << endl;
                        err = false;
                    }
                }

                else{
                    err = false;
                    cout << "register is out of range" << endl;
                }
            }
            
            else{
                err = false;
                cout << "register is out of range" << endl;
            }
        }
        
        else{
            cout << "Data trans func failed catastrophically!" << endl;
        }
    }
}

//This takes a branching statement and outputs the corresponding statement to itself and the z flag.
void branchOp(string operation, string operand, int flagZ){
    if(operation == "BEQ" && flagZ == 1){
        cout << "Branch will be taken to <" << operand << ">" << endl;
    }
    else if(operation == "BEQ" && flagZ == 0){
        cout << "Branch will not be taken to <" << operand << ">" << endl;
    }
    else if(operation == "BNE" && flagZ == 0){
        cout << "Branch will be taken to <" << operand << ">" << endl;
    }
    else if(operation == "BNE" && flagZ == 1){
        cout << "Branch will not be taken to <" << operand << ">" << endl;
    }
    else if(operation == "BAL"){
        cout << "Branch will be taken to <" << operand << ">" << endl;
    }
    else{
        cout << "There was an error displaying the branching operation." << endl;
    }
}

//This function takes the operation and opNum, and if it is the incorrect amount prints an error statement and returns corresponding bool.
bool errorCheck(string op, string o1, string o2, string o3, int opNum){
    if(opNum > 3 || opNum == 0){
        cout << "Error: incorrect operand ammount" << endl;
        return false;
    }
    else if(opNum != 3 && ((op[0] == 'A'  && op[2] == 'D') || (op[0] == 'S' && op[2] == 'B') || (op[0] == 'O' && op[2] == 'R') || (op[0] == 'E' && op[2] == 'R')||((op[0] == 'L') && (op[1] == 'S') &&(op[2] == 'L' || op[2] == 'R')))){
        cout << "error in operand amount" << endl;
        return false;
    }
    else if(opNum != 2 && ((op[0] == 'M' && (op[2] == 'V' || op[2] =='N')) || (op[0] == 'C' && op[2] == 'P') || (op[0] == 'L' && op[1] == 'D' && op[2] == 'R') || (op[0] == 'S' && op[2] == 'R'))){
         cout << "error in operand amount" << endl;
        return false;
    }
    else if(opNum != 1 && (op == "BNE" || op == "BAL" || op == "BEQ")){
         cout << "error in operand amount" << endl;
        return false;
    }
    else{
        return true;
    }
    cout << "Debugging: catastrophic error in boolErrorCheck()" << endl; 
    return false;
}

//This is a function that finds how many operands are in each statement. 
void opCount(int* opArr, ifstream& fptr){
    string fpstring, tempStore;
    char c;
    int count =0, length = 0;
    while(getline(fptr, fpstring)){
        count = 0;
        stringstream ss(fpstring);
        while(ss.get(c) && c != '\n'){
            if(c == ','){
                count++;
            }
        }
        opArr[length] = count + 1;
        length++;
    }
}

//This is a function that performs the appropriate shift operations
void shiftOp(uint32_t** r, string operation, string op1, string op2, string op3, bool& err, int& n, int& z, int& c, int& v){
    uint32_t temp1, temp2, temp3;
    int regNum = 0, cond;
    conditionalRoute(operation, n, z, c, v, cond);
    if(operation[2] == 'L' && cond == 1){
        if(op2[0] == 'R'){
            regNum = findRegNum(op1);
            conversion(r, op2, op3, temp1, temp2);
            temp3 = temp1 << temp2;
            *r[regNum] = temp3;
        }
        else{
            err = false;
            cout << "Error: Operand 2 can not be an immediate value." << endl;
        }
    }
    if(operation[2] == 'R' && cond == 1){
        if(op2[0] == 'R'){
            regNum = findRegNum(op1);
            conversion(r, op2, op3, temp1, temp2);
            temp3 = temp1 >> temp2;
            *r[regNum] = temp3;
        }
        else{
            err = false;
            cout << "Error: Operand 2 can not be an immediate value." << endl;
        }
    }
}