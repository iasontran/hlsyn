/**************************************************************************************************/

/*
 * File: main.cpp
 * Author: Jason Tran, Andrew Camps
 * NetID: ichikasuto, andrewcamps
 * Date: October 20, 2017
 *
 * Description: Takes in file name arguments. First arg is highlevel code which is converted to verilog code. Critical path
 * of module is then calculated
 *
 */
/**************************************************************************************************/

#include "Module.h"

#define INFILE 1
#define OUTFILE 2

using namespace std;

/**
 * Main function to start program. Reads file for high level code, outputs verilog code and calculates critical
 * path for verilog module
 */
int main(int argc, char *argv[]) {
    Module *module;
    
    /* If arguments found is not 3, display user statement and exit */
    if (argc != 3) {
        cout << "Usage : dpgen netlistFile verilogFile" << endl;
        return EXIT_FAILURE;
    }

    /* Initialize and build module datapath from input file */
    string name = argv[OUTFILE];
    if(name.find(".") != string::npos){
        name.erase(name.begin() + name.find("."), name.end());
    }
    string::size_type index;
    while((index = name.find('/')) != string::npos){
        name.erase(name.begin(), name.begin() + index + 1);
    }
    module = new Module(name);
    if (!module->buildModule(argv[INFILE])) {
        cout << "Failed to build module from input file : " << argv[INFILE] << endl;
        return EXIT_FAILURE;
    }
    
    /* Output datapath module to .v file */
    if(!module->outputModule(argv[OUTFILE])){
        cout << "Failed to create output module file : " << argv[OUTFILE] << endl;
        return EXIT_FAILURE;
    }
    
    /* Print critical path delay of the module */
    double criticalPath = module->criticalPathDelay();
    if(criticalPath == -1){
        cout << "Failed to calculate critical path" << endl;
    }
    cout << "Critical Path : " << criticalPath << " ns" << endl;
    
    return 0;
}
