/**
 ** createbfstest.cpp
 ** Comp15 Project 2: Six Degrees of Collaboration
 ** Written by: Lucas Maley (lmaley01)
 ** April 28th, 2021
 **
 ** Program to create extensive commandFile to test bfs and dfs
 **/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main() {
    
    //declares, opens, and checks data file for failure
    ifstream dataFile;
    dataFile.open("artists.txt");
    if (dataFile.fail()) {
        cerr << "artists.txt cannot be opened.\n";
        exit(EXIT_FAILURE);
    }
    
    //declares and opens output file
    ofstream outputFile;
    outputFile.open("bfstestinput.txt");
    
    string temp;
    vector<string> artists;
    
    //iterates through data file, storing artists in an array
    getline(dataFile, temp);
    artists.push_back(temp);
    while (!dataFile.eof()) {
        while (temp != "*" and temp != "") {
            getline(dataFile, temp);
        }
        getline(dataFile, temp);
        artists.push_back(temp);
    }
    artists.pop_back(); //accounts for reading final "*"
    
    //outputs every possible combination to output file
    for (unsigned long i = 0; i < artists.size(); i++) {
        for (unsigned long j = 0; j < artists.size(); j++) {
            outputFile << "bfs\n";
            outputFile << artists[i] << "\n";
            outputFile << artists[j] << "\n";
        }
    }
    
    //closes data file and output file
    outputFile.close();
    dataFile.close();
}