/*************/
/*
 * File: Wire.cpp
 * Author: Jason Tran, Andrew Camps
 * NetID: ichikasuto, andrewcamps
 * Date: October 21, 207
 *
 * Description: Wire object that is used for constructing graph. Has to string method to output verilog code
 */

#include "Wire.h"

/**
 * Constructor
 */
Wire::Wire(string name, int width, bool usigned, bool reg){
    this->name = name;
    this->dataWidth = width;
    this->usigned = usigned;
    this->reg = reg;
    this->outDelay = -1;
}

/**
 * To string method outputs verilog code syntax
 */
string Wire::toString(){
    string width = "";
    string sign = "";
    if(this->dataWidth > 1){
        width = "[" + to_string(this->dataWidth - 1) + ":0] ";
    }
    
    if(!this->isUnsigned()){
        sign = "signed ";
    }
    
    return "\twire " + sign + width + this->name + ";";
}
