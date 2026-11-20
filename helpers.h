#include "conditionals.h"

void cleanUp(string&, string&, string&);

void display(uint32_t**, uint32_t**, string, string, string, string, int, bool, bool, int, int, int);

void processInstruction(ifstream&, int&, uint32_t**, uint32_t**, int*, int&);

void arithOp(uint32_t**, string, string, string, string, int&, bool&, int&, int&, int&);

void conversion(uint32_t**, string, string, uint32_t&, uint32_t&);

int findRegNum(string);

//need to update ORR, EOR
void bitwiseOp(uint32_t**, string, string, string, string, bool&, int&, int&, int&, int&);

void dataTrans(uint32_t**, uint32_t**, string, string, string, bool&, int&, int&, int&,int&);

void branchOp(string, string, int);

bool errorCheck(string, string, string, string, int);

void opCount(int*, ifstream&);