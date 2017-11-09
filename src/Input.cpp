/*************/
/*
 * File: Input.cpp
 * Author: Jason Tran, Andrew Camps
 * NetID: ichikasuto, andrewcamps
 * Date: October 21, 207
 *
 * Description: Input object that creates an input type to help build a graph. Has a toString method to help
 * output to a file
 */

#include "Input.h"

/**
 * Constructor
 */
Input::Input(string name, int width, bool usigned){
    this->name = name;
    this->dataWidth = width;
    this->usigned = usigned;
}

/**
 * toString method that returns a string in format of verilog syntax
 */
string Input::toString(){
    string width = "";
    string sign = "";
    if(this->dataWidth > 1){
        width = "[" + to_string(this->dataWidth - 1) + ":0] ";
    }
    
    if(!this->isUnsigned()){
        sign = "signed ";
    }
    
    return "\tinput " + sign + width + this->name + ";";
}
