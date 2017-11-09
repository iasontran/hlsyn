/*************/
/*
 * File: Input.h
 * Author: Jason Tran, Andrew Camps
 * NetID: ichikasuto, andrewcamps
 * Date: October 20, 2017
 */

#ifndef Input_h
#define Input_h

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cstring>
#include <string>
#include <cctype>

#include "Operation.h"

using namespace std;

class Operation;

class Input {
private:
    bool usigned;
    int dataWidth;
    string name;
public:
    Input(string name, int width, bool usigned);
    string getName() { return name; }
    int getWidth() { return dataWidth; }
    bool isUnsigned() { return usigned; }
    string toString();
    vector<Operation *> toOperations;
};

#endif /* Input_h */
