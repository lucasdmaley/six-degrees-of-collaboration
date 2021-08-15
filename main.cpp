/**
 ** main.cpp
 ** Comp15 Project 2: Six Degrees of Collaboration
 ** Written by: Lucas Maley (lmaley01)
 ** April 28th, 2021
 **
 ** Driver for the program (contains the main function)
 **/
 
 #include <iostream>
 #include <fstream>
 #include <stdlib.h>
 #include "SixDegrees.h"

using namespace std;

int main(int argc, char *argv[])
{    
    //checks if number of arguments given is correct
    if (argc < 2 or argc > 4) {
        cerr << "Usage: ./SixDegrees dataFile [commandFile] [outputFile]\n";
        return 1;
    }
    
    //declares, opens, and checks for errors in file containing graph data
    ifstream dataFile;
    dataFile.open(argv[1]);
    if (dataFile.fail()) {
        cerr << argv[1] << " cannot be opened.\n";
        exit(EXIT_FAILURE);
    }
    
    if (argc == 2) { //if only dataFile given, calls relevant constructor
        SixDegrees ArtistGraph(dataFile, cin, cout);
    } else { //declares, opens, and checks for errors in command file
        ifstream commandFile;
        commandFile.open(argv[2]);
        if (commandFile.fail()) {
            cerr << argv[2] << " cannot be opened.\n";
            exit(EXIT_FAILURE);
        }
        if (argc == 3) { //calls constructor for dataFile and commandFile
            SixDegrees ArtistGraph(dataFile, commandFile, cout);
        } else { //declares and opens output file, calls constructor
            ofstream outputFile;
            outputFile.open(argv[3]);
            SixDegrees ArtistGraph(dataFile, commandFile, outputFile);
            outputFile.close(); 
        }
        commandFile.close(); //closing the files
    }
    dataFile.close();
    
    return 0;
}
