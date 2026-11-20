#include "conditionals.h"

void conditionalRoute(string operation, int& N, int& Z, int& C, int& V, int& condE){
    if(operation[3] == 'S'){
        condE = 1;
    }
    else if(operation[3] == 'G'){
        if(operation[4] == 'T'){
            if(Z == 0 && N == V){
                condE = 1;
            }
            else{
                condE = 0;
            }
        }
        if(operation[4] == 'E'){
            if(N == V){
                condE = 1;
            }
            else{
                condE = 0;
            }
        }
    }
    else if(operation[3] == 'L'){
        if(operation[4] == 'T'){
            if(N != V){
                condE = 1;
            }
            else{
                condE = 0;
            }
        }
        if(operation[4] == 'E'){
            if(Z == 1 || N != V){
                condE = 1;
            }
            else{
                condE = 0;
            }
        }
    }
    else if(operation[3] == 'E'){
        if(Z == 1){
            condE = 1;
        }
        else{
            condE = 0;
        }
    }
    else if(operation[3] == 'N'){
        if(Z == 0){
            condE = 1;
        }
        else{
            condE = 0;
        }
    }
    else{
        condE = 1;
    }
}

void opSort(string op, int& sort){
    //operation == "ADD" || operation == "SUB" || operation == "CMP" || operation == "MOV"
    if((op[0] == 'A' && op[1] == 'D' && op[2] == 'D') || (op[0] == 'S' && op[2] == 'B') || (op[0] == 'C' && op[2] == 'P') || (op[0] == 'M' && op[2] == 'V')){
        sort = 0;
    }
    //operation == "AND" || operation == "OR" || operation == "EOR"
    else if((op[0] == 'A' && op[2] == 'D') || (op[0] == 'O' && op[2] == 'R') || (op[0] == 'E' && op[2] == 'R')){
        sort = 1;
    }
    //operation == "LOAD" || operation == "STORE"
    else if((op[0] == 'L' && op[2] == 'R') || (op[0] == 'S' && op[2] == 'R')){
        sort = 2;
    }
    //operation == "BAL" || operation == "BEQ" || operation == "BNE"
    else if ((op[0] == 'B') && (op[2] == 'L' || op[2] == 'Q' || op[2] == 'E')){
        sort = 3;
    }
    else{
        sort = 10;
    }
}