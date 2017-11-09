/*************/
/*
 * File: Output.cpp
 * Author: Jason Tran, Andrew Camps
 * NetID: ichikasuto, andrewcamps
 * Date: October 21, 207
 *
 * Description: Output object type. Contains to string method to output verilog code for an output type
 */

#include "Output.h"

/**
 * Constructor
 */
Output::Output(string name, int width, bool usigned){
    this->name = name;
    this->dataWidth = width;
    this->usigned = usigned;
    this->outDelay = -1;
}

/**
 * To string method outputs verilog code syntax
 */
string Output::toString(){
    string width = "";
    string sign = "";
    if(this->dataWidth > 1){
        width = "[" + to_string(this->dataWidth - 1) + ":0] ";
    }
    
    if(!this->isUnsigned()){
        sign = "signed ";
    }
    
    return "\toutput " + sign + width + this->name + ";";
}
