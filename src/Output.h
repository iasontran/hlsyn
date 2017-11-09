/*************/
/*
 * File: Output.h
 * Author: Jason Tran, Andrew Camps
 * NetID: ichikasuto, andrewcamps
 * Date: October 20, 2017
 */

#ifndef Output_h
#define Output_h

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>

using namespace std;

class Output {
private:
    bool usigned;
    int dataWidth;
    string name;
    double outDelay;
public:
    Output(string name, int width, bool usigned);
    string getName() { return name; }
    int getWidth() { return dataWidth; }
    bool isUnsigned() { return usigned; }
    double getDelay() { return outDelay; }
    void setDelay(double delay) { outDelay = delay; }
    string toString();

};

#endif
