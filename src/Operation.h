/*************/
/*
 * File: Operation.h
 * Author: Jason Tran, Andrew Camps
 * NetID: ichikasuto, andrewcamps
 * Date: October 20, 2017
 */

#ifndef Operation_h
#define Operation_h

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <cctype>

#include "Output.h"
#include "Input.h"
#include "Wire.h"

#define NUM_INPUTS 3
#define NUM_SIZES 6

using namespace std;

class Wire;
class Input;

class Operation {
public:
    enum Operations { REG, ADD, SUB, MUL, COMP_GT, COMP_LT, COMP_EQ, MUX2x1, SHR, SHL, DIV, MOD, INC, DEC };
    Operation();
    Operations getOperation() { return operation; }
    void setOperation(Operations op) {operation = op; }
    int getOpID() { return operationID; }
    void setOpID(int ID) { operationID = ID; }
    void calcWidth();
    double getDelay();
    void setSign();
    bool isUnsigned() { return usigned; }
    string toString();
    Output *outNext;
    Wire *wireNext;
    Wire *inWire[NUM_INPUTS];
    Input *inInput[NUM_INPUTS];
private:
    Operations operation;
    string sign;
    bool usigned;
    int operationID;
    int width;
    static double reg_delay[];
    static double add_delay[];
    static double sub_delay[];
    static double mul_delay[];
    static double comp_delay[];
    static double mux_delay[];
    static double shr_delay[];
    static double shl_delay[];
    static double div_delay[];
    static double mod_delay[];
    static double inc_delay[];
    static double dec_delay[];
};

#endif
