/*************/
/*
 * File: Operation.cpp
 * Author: Jason Tran, Andrew Camps
 * NetID: ichikasuto, andrewcamps
 * Date: October 21, 207
 *
 * Description: Operation objects that defines an operation type. Has various functions to calculate the delay of an operation and output
 * verilog code for a particular operation.
 */

#include "Operation.h"

double Operation::reg_delay[NUM_SIZES] = { 2.616, 2.644, 2.879, 3.061, 3.602, 3.966 };
double Operation::add_delay[NUM_SIZES] = { 2.704, 3.713, 4.924, 5.638, 7.270, 9.566 };
double Operation::sub_delay[NUM_SIZES] = { 3.024, 3.412, 4.890, 5.569, 7.253, 9.566 };
double Operation::mul_delay[NUM_SIZES] = { 2.438, 3.651, 7.453, 7.811, 12.395, 15.354 };
double Operation::comp_delay[NUM_SIZES] = { 3.031, 3.934, 5.949, 6.256, 7.264, 8.416 };
double Operation::mux_delay[NUM_SIZES] = { 4.083, 4.115, 4.815, 5.623, 8.079, 8.766 };
double Operation::shr_delay[NUM_SIZES] = { 3.644, 4.007, 5.178, 6.460, 8.819, 11.095 };
double Operation::shl_delay[NUM_SIZES] = { 3.614, 3.980, 5.152, 6.549, 8.565, 11.220 };
double Operation::div_delay[NUM_SIZES] = { 0.619, 2.144, 15.439, 33.093, 86.312, 243.233 };
double Operation::mod_delay[NUM_SIZES] = { 0.758, 2.149, 16.078, 35.563, 88.142, 250.583 };
double Operation::inc_delay[NUM_SIZES] = { 1.792, 2.218, 3.111, 3.471, 4.347, 6.200 };
double Operation::dec_delay[NUM_SIZES] = { 1.792, 2.218, 3.108, 3.701, 4.685, 6.503 };

/**
 * Constructor
 */
Operation::Operation(){
    for(int i = 0; i < NUM_INPUTS; i++){
        inWire[i] = NULL;
        inInput[i] = NULL;
    }
    
    outNext = NULL;
    wireNext = NULL;
}

/**
 * Gets the width parameter of an operation based on various parameters layed out in the spec sheet.
 * All operations use the width of the output wire except comparator type operation
 */
void Operation::calcWidth(){
    int width0;
    int width1;
    
    if(operation == COMP_GT || operation == COMP_LT || operation == COMP_EQ){
        width0 = inWire[0] == NULL ? inInput[0]->getWidth() : inWire[0]->getWidth();
        width1 = inWire[1] == NULL ? inInput[1]->getWidth() : inWire[1]->getWidth();
        this->width = width0 > width1 ? width0 : width1;
    }else{
        this->width = outNext == NULL ? wireNext->getWidth() : outNext->getWidth();
    }
}

/**
 * Sets the sign of the operation type based on the input wires to operation
 */
void Operation::setSign() {
    bool usigned = true;
    
    for(int i = 0; i < NUM_INPUTS; i++){
        if(inWire[i] != NULL){
            if(!inWire[i]->isUnsigned()){
                usigned = false;
                break;
            }
        }
        if(inInput[i] != NULL){
            if(!inInput[i]->isUnsigned()){
                usigned = false;
                break;
            }
        }
    }
    
    if(outNext != NULL){
        if(!outNext->isUnsigned())
        usigned = false;
    }
    if(wireNext != NULL){
        if(!wireNext->isUnsigned()){
            usigned = false;
        }
    }
    
    this->usigned = usigned;
    sign = usigned ? "" : "S";
}

/**
 * Gets the delay of a particular operation based off of the width. Delays came from spec sheet
 */
double Operation::getDelay(){
    int index;
    
    switch(width){
        case 1:
            index = 0;
            break;
        case 2:
            index = 1;
            break;
        case 8:
            index = 2;
            break;
        case 16:
            index = 3;
            break;
        case 32:
            index = 4;
            break;
        case 64:
            index = 5;
            break;
    }
    
    switch(operation){
        case INC:
            return inc_delay[index];
        case DEC:
            return dec_delay[index];
        case ADD:
            return add_delay[index];
        case SUB:
            return sub_delay[index];
        case MUL:
            return mul_delay[index];
        case COMP_GT:
        case COMP_LT:
        case COMP_EQ:
            return comp_delay[index];
        case SHR:
            return shr_delay[index];
        case SHL:
            return shl_delay[index];
        case DIV:
            return div_delay[index];
        case MOD:
            return mod_delay[index];
        case MUX2x1:
            return mux_delay[index];
        case REG:
            return reg_delay[index];
    }
    
    return 0;
}

/**
 * To string method that outputs each operation in verilog syntax with correct bit sizes for inputs and outputs as well
 * as mix sign types
 */
string Operation::toString(){
    Wire *outW = NULL;
    Output *out = NULL;
    Input *in0 = NULL;
    Wire *in0w = NULL;
    Input *in1 = NULL;
    Wire *in1w = NULL;
    Input *in2 = NULL;
    Wire *in2w = NULL;
    string outName;
    string in0n;
    string in1n;
    string in2n;
    
    /* Input/Output initialization */
    if(outNext == NULL){
        outW = wireNext;
    }else{
        out = outNext;
    }
    if(inWire[0] == NULL){
        in0 = inInput[0];
    }else{
        in0w = inWire[0];
    }
    if(inWire[1] == NULL){
        if(inInput[1] != NULL){
            in1 = inInput[1];
        }
    }else{
        in1w = inWire[1];
    }
    if(inWire[2] == NULL){
        if(inInput[2] != NULL){
            in2 = inInput[2];
        }
    }else{
        in2w = inWire[2];
    }
    
    /* Getting proper bit widths and sign types */
    outName = out == NULL ? outW->getName() : out->getName();
    in0n = in0 == NULL ? in0w->getName() : in0->getName();
    if(in0w != NULL){
        if(in0w->getWidth() == width){
            if(!this->isUnsigned() && in0w->isUnsigned()){
                in0n = "$signed(" + in0n + ")";
            }
        }else if(in0w->getWidth() > width){
            if(!this->isUnsigned() && in0w->isUnsigned()){
                in0n = "$signed(" + in0n + "[" + to_string(width - 1) + ":0])";
            }else{
                in0n = in0n + "[" + to_string(width - 1) + ":0]";
            }
        }else{
            if(!in0w->isUnsigned()){
                in0n = "{{" + to_string(width - in0w->getWidth()) + "{" + in0n + "[" + to_string(in0w->getWidth() - 1) + "]" + "}}, " + in0n + "}";
            }else{
                if(!this->isUnsigned()){
                    in0n = "$signed({" + to_string(width - in0w->getWidth()) + "'b0, " + in0n + "})";
                }else{
                    in0n = "{" + to_string(width - in0w->getWidth()) + "'b0, " + in0n + "}";
                }
            }
        }
    }else{
        if(in0->getWidth() == width){
            if(!this->isUnsigned() && in0->isUnsigned()){
                in0n = "$signed(" + in0n + ")";
            }
        }else if(in0->getWidth() > width){
            if(!this->isUnsigned() && in0->isUnsigned()){
                in0n = "$signed(" + in0n + "[" + to_string(width - 1) + ":0])";
            }else{
                in0n = in0n + "[" + to_string(width - 1) + ":0]";
            }
        }else{
            if(!in0->isUnsigned()){
                in0n = "{{" + to_string(width - in0->getWidth()) + "{" + in0n + "[" + to_string(in0->getWidth() - 1) + "]" + "}}, " + in0n + "}";
            }else{
                if(!this->isUnsigned()){
                    in0n = "$signed({" + to_string(width - in0->getWidth()) + "'b0, " + in0n + "})";
                }else{
                    in0n = "{" + to_string(width - in0->getWidth()) + "'b0, " + in0n + "}";
                }
            }
        }
    }
    
    if(operation != INC && operation != DEC && operation != REG && operation != SHL && operation != SHR){
        in1n = in1 == NULL ? in1w->getName() : in1->getName();
        if(in1w != NULL){
            if(in1w->getWidth() == width){
                if(!this->isUnsigned() && in1w->isUnsigned()){
                    in1n = "$signed(" + in1n + ")";
                }
            }else if(in1w->getWidth() > width){
                if(!this->isUnsigned() && in1w->isUnsigned()){
                    in1n = "$signed(" + in1n + "[" + to_string(width - 1) + ":0])";
                }else{
                    in1n = in1n + "[" + to_string(width - 1) + ":0]";
                }
            }else{
                if(!in1w->isUnsigned()){
                    in1n = "{{" + to_string(width - in1w->getWidth()) + "{" + in1n + "[" + to_string(in1w->getWidth() - 1) + "]" + "}}, " + in1n + "}";
                }else{
                    if(!this->isUnsigned()){
                        in1n = "$signed({" + to_string(width - in1w->getWidth()) + "'b0, " + in1n + "})";
                    }else{
                        in1n = "{" + to_string(width - in1w->getWidth()) + "'b0, " + in1n + "}";
                    }
                }
            }
        }else{
            if(in1->getWidth() == width){
                if(!this->isUnsigned() && in1->isUnsigned()){
                    in1n = "$signed(" + in1n + ")";
                }
            }else if(in1->getWidth() > width){
                if(!this->isUnsigned() && in1->isUnsigned()){
                    in1n = "$signed(" + in1n + "[" + to_string(width - 1) + ":0])";
                }else{
                    in1n = in1n + "[" + to_string(width - 1) + ":0]";
                }
            }else{
                if(!in1->isUnsigned()){
                    in1n = "{{" + to_string(width - in1->getWidth()) + "{" + in1n + "[" + to_string(in1->getWidth() - 1) + "]" + "}}, " + in1n + "}";
                }else{
                    if(!this->isUnsigned()){
                        in1n = "$signed({" + to_string(width - in1->getWidth()) + "'b0, " + in1n + "})";
                    }else{
                        in1n = "{" + to_string(width - in1->getWidth()) + "'b0, " + in1n + "}";
                    }
                }
            }
        }
    }
    
    /* Printing operations */
    switch(operation){
        case INC:
            return "\t" + sign + "INC #(" + to_string(width) + ") inc" + to_string(getOpID()) + "(" + in0n + ", " + outName + ");";
        case DEC:
            return "\t" + sign + "DEC #(" + to_string(width) + ") dec" + to_string(getOpID()) + "(" + in0n + ", " + outName + ");";
        case ADD:
            return "\t" + sign + "ADD #(" + to_string(width) + ") add" + to_string(getOpID()) + "(" + in0n + ", " + in1n + ", " + outName + ");";
        case SUB:
            return "\t" + sign + "SUB #(" + to_string(width) + ") sub" + to_string(getOpID()) + "(" + in0n + ", " + in1n + ", " + outName + ");";
        case MUL:
            return "\t" + sign + "MUL #(" + to_string(width) + ") mul" + to_string(getOpID()) + "(" + in0n + ", " + in1n + ", " + outName + ");";
        case COMP_GT:
            outName = out == NULL ? outW->getName() : out->getName();
            if(outW != NULL){
                if(outW->getWidth() > 1){
                    outName = outName + "[0]";
                }
            }else{
                if(out->getWidth() > 1){
                    outName = outName + "[0]";
                }
            }
            return "\t" + sign + "COMP #(" + to_string(width) + ") comp_gt" + to_string(getOpID()) + "(" + in0n + ", " + in1n + ", " + outName + ", , " + ");";
        case COMP_LT:
            outName = out == NULL ? outW->getName() : out->getName();
            if(outW != NULL){
                if(outW->getWidth() > 1){
                    outName = outName + "[0]";
                }
            }else{
                if(out->getWidth() > 1){
                    outName = outName + "[0]";
                }
            }
            return "\t" + sign + "COMP #(" + to_string(width) + ") comp_lt" + to_string(getOpID()) + "(" + in0n + ", " + in1n + ", , " + outName + ", " + ");";
        case COMP_EQ:
            outName = out == NULL ? outW->getName() : out->getName();
            if(outW != NULL){
                if(outW->getWidth() > 1){
                    outName = outName + "[0]";
                }
            }else{
                if(out->getWidth() > 1){
                    outName = outName + "[0]";
                }
            }
            return "\t" + sign + "COMP #(" + to_string(width) + ") comp_eq" + to_string(getOpID()) + "(" + in0n + ", " + in1n + ", , , " + outName + ");";
        case SHR:
            in1n = in1 == NULL ? in1w->getName() : in1->getName();
            return "\t" + sign + "SHR #(" + to_string(width) + ") shr" + to_string(getOpID()) + "(" + in0n + ", " + in1n + ", " + outName + ");";
        case SHL:
            in1n = in1 == NULL ? in1w->getName() : in1->getName();
            return "\t" + sign + "SHL #(" + to_string(width) + ") shl" + to_string(getOpID()) + "(" + in0n + ", " + in1n + ", " + outName + ");";
        case DIV:
            return "\t" + sign + "DIV #(" + to_string(width) + ") div" + to_string(getOpID()) + "(" + in0n + ", " + in1n + ", " + outName + ");";
        case MOD:
            return "\t" + sign + "MOD #(" + to_string(width) + ") mod" + to_string(getOpID()) + "(" + in0n + ", " + in1n + ", " + outName + ");";
        case MUX2x1:
            in0n = in0 == NULL ? in0w->getName() : in0->getName();
            if(in0w != NULL){
                if(in0w->getWidth() > 1){
                    in0n = in0n + "[0]";
                }
            }else{
                if(in0->getWidth() > 1){
                    in0n = in0n + "[0]";
                }
            }
            in2n = in2 == NULL ? in2w->getName() : in2->getName();
            if(in2w != NULL){
                if(in2w->getWidth() == width){
                    if(!this->isUnsigned() && in2w->isUnsigned()){
                        in2n = "$signed(" + in2n + ")";
                    }
                }else if(in2w->getWidth() > width){
                    if(!this->isUnsigned() && in2w->isUnsigned()){
                        in2n = "$signed(" + in2n + "[" + to_string(width - 1) + ":0])";
                    }else{
                        in2n = in2n + "[" + to_string(width - 1) + ":0]";
                    }
                }else{
                    if(!in2w->isUnsigned()){
                        in2n = "{{" + to_string(width - in2w->getWidth()) + "{" + in2n + "[" + to_string(in2w->getWidth() - 1) + "]" + "}}, " + in2n + "}";
                    }else{
                        if(!this->isUnsigned()){
                            in2n = "$signed({" + to_string(width - in2w->getWidth()) + "'b0, " + in2n + "})";
                        }else{
                            in2n = "{" + to_string(width - in2w->getWidth()) + "'b0, " + in2n + "}";
                        }
                    }
                }
            }else{
                if(in2->getWidth() == width){
                    if(!this->isUnsigned() && in2->isUnsigned()){
                        in2n = "$signed(" + in2n + ")";
                    }
                }else if(in2->getWidth() > width){
                    if(!this->isUnsigned() && in2->isUnsigned()){
                        in2n = "$signed(" + in2n + "[" + to_string(width - 1) + ":0])";
                    }else{
                        in2n = in2n + "[" + to_string(width - 1) + ":0]";
                    }
                }else{
                    if(!in2->isUnsigned()){
                        in2n = "{{" + to_string(width - in2->getWidth()) + "{" + in2n + "[" + to_string(in2->getWidth() - 1) + "]" + "}}, " + in2n + "}";
                    }else{
                        if(!this->isUnsigned()){
                            in2n = "$signed({" + to_string(width - in2->getWidth()) + "'b0, " + in2n + "})";
                        }else{
                            in2n = "{" + to_string(width - in2->getWidth()) + "'b0, " + in2n + "}";
                        }
                    }
                }
            }
            return "\t" + sign + "MUX2x1 #(" + to_string(width) + ") mux" + to_string(getOpID()) + "(" + in0n + ", " + in1n + ", " + in2n + ", " + outName + ");";
        case REG:
            in1n = inInput[1]->getName();
            in2n = inInput[2]->getName();
            return "\t" + sign + "REG #(" + to_string(width) + ") reg" + to_string(getOpID()) + "(" + in0n + ", " + in1n + ", " + in2n + ", " + outName + ");";
    }
    
    return NULL;
}

