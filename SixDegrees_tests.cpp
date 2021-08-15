/**
 ** unit_tests.cpp
 **
 ** COMP15 Spring
 ** Project Two: Six Degrees of Collaboration
 **
 ** Written by: Lucas Maley (lmaley01)
 ** Date: April 21st, 2021
 **
 ** Purpose: Uses the unit testing framework written by Matthew Russel to 
 ** individually test functions
 **/

#include <cassert>
#include <fstream>
#include <iostream>
#include "SixDegrees.h"

//tests the constructor for the Six Degrees class
//requires artist.txt to exist
void constructor_SD() {
    
    ifstream infile;
    
    infile.open("artists.txt");
    
    if (infile.fail()) {
        cerr << "artists.txt cannot be opened.\n";
        return;
    }
    
    ifstream commandFile;
    commandFile.open("unittestinput.txt");
    if (commandFile.fail()) {
        cerr << "unittestinput.txt cannot be opened.\n";
        exit(EXIT_FAILURE);
    }
    
    cout << "about to print contents of artists.txt as a graph\n";
    SixDegrees SixDegreesStandard(infile, commandFile, cout);
    SixDegreesStandard.print();
    
    infile.close(); 
    
    cout << "\n\n\n";
    
    //empty artist file
    infile.open("empty.txt");
    
    if (infile.fail()) {
        cerr << "empty.txt cannot be opened.\n";
        return;
    }
    
    cout << "about to print contents of empty.txt as a graph\n";
    SixDegrees SixDegreesEmpty(infile, commandFile, cout);
    SixDegreesEmpty.print();
    
    commandFile.close();
    infile.close(); 
}

//tests the get_vertex_neighbors function in the Collab Graph class
//relies on graph being made public in CollabGraph to access it
void get_vertex_neighbors_CG() {
    
    ifstream infile;
    
    infile.open("artists.txt");
    
    if (infile.fail()) {
        cerr << "artists.txt cannot be opened.\n";
        return;
    }
    
    ifstream commandFile;
    commandFile.open("unittestinput.txt");
    if (commandFile.fail()) {
        cerr << "unittestinput.txt cannot be opened.\n";
        exit(EXIT_FAILURE);
    }
    
    cout << "about to print contents of artists.txt as a graph\n";
    SixDegrees SixDegreesStandard(infile, commandFile, cout);
    SixDegreesStandard.print();
    
    commandFile.close();
    infile.close(); 
    
    cout << "This line should be succeeded by Kanye West\n"; 
    cout << SixDegreesStandard.graph.graph.at("Kanye West")
                                                ->artist.get_name() << endl;
    
    vector<Artist> neighbors = SixDegreesStandard.graph.get_vertex_neighbors(
        SixDegreesStandard.graph.graph.at("Kanye West")->artist
    );
    
    cout << "Kanye West expected neighbors: " << 
    SixDegreesStandard.graph.graph.at("Kanye West")->neighbors.size() << endl;
    cout << "Kanye West's neighbors: ";
    if (neighbors.empty()) {
        cout << "none\n";
    } else {
        for (unsigned long i = 0; i < neighbors.size(); i++) {
            cout << neighbors.at(i) << " ";
        }
    }
    cout << endl;

    neighbors = SixDegreesStandard.graph.get_vertex_neighbors(
        SixDegreesStandard.graph.graph.at("Da Brat")->artist
    );
    
    cout << "Da Brat expected neighbors: " << 
    SixDegreesStandard.graph.graph.at("Da Brat")->neighbors.size() << endl;
    cout << "Da Brat's neighbors: ";
    if (neighbors.empty()) {
        cout << "none\n";
    } else {
        for (unsigned long i = 0; i < neighbors.size(); i++) {
            cout << neighbors.at(i) << " ";
        }
    }
    cout << endl;
}

//tests the report_path function in the Collab Graph class
//has to manually set predecessors as no traversal has been implemented yet
void report_path_CG() {
    //set_predecessor();
    ifstream infile;
    
    infile.open("artists.txt");
    
    if (infile.fail()) {
        cerr << "artists.txt cannot be opened.\n";
        return;
    }
    
    ifstream commandFile;
    commandFile.open("unittestinput.txt");
    if (commandFile.fail()) {
        cerr << "unittestinput.txt cannot be opened.\n";
        exit(EXIT_FAILURE);
    }
    
    //cout << "about to print contents of artists.txt as a graph\n";
    SixDegrees SixDegreesStandard(infile, commandFile, cout);
    //SixDegreesStandard.print();
    
    cout << "expecting path from Kendrick Lamar to Zayn to go through Sia:\n";
    Artist source = SixDegreesStandard.graph.graph.at("Kendrick Lamar")
                                                                    ->artist;
    Artist dest = SixDegreesStandard.graph.graph.at("ZAYN")->artist;
    
    SixDegreesStandard.graph.graph.at("ZAYN")->predecessor = 
    SixDegreesStandard.graph.graph.at("Sia");
    
    SixDegreesStandard.graph.graph.at("Sia")->predecessor = 
    SixDegreesStandard.graph.graph.at("Kendrick Lamar");
    
    stack<Artist> path = SixDegreesStandard.graph.report_path(source, dest);
    
    while (!path.empty()) {
        cout << path.top().get_name() << " ";
        path.pop();
    }
    
    
    cout << endl << endl;
    cout << "expecting direct path from Katy Perry to DaBaby:\n";
    //path when source and dest are neighbors
    source = SixDegreesStandard.graph.graph.at("Katy Perry")->artist;
    dest = SixDegreesStandard.graph.graph.at("DaBaby")->artist;
    
    SixDegreesStandard.graph.graph.at("DaBaby")->predecessor = 
    SixDegreesStandard.graph.graph.at("Katy Perry");
    
    path = SixDegreesStandard.graph.report_path(source, dest);
    
    while (!path.empty()) {
        cout << path.top().get_name() << " ";
        path.pop();
    }
    
    
    cout << endl << endl;
    cout << "expecting no path to exist:\n";
    
    source = SixDegreesStandard.graph.graph.at("Kanye West")->artist;
    dest = SixDegreesStandard.graph.graph.at("Michael Jackson")->artist;
    
    path = SixDegreesStandard.graph.report_path(source, dest);
    
    if (path.empty()) {
        cout << "no path exists\n";
    }
    
    cout << endl << endl;
    cout << "testing source = dest; expecting no path to exist:\n";
    
    source = SixDegreesStandard.graph.graph.at("Aretha Franklin")->artist;
    dest = SixDegreesStandard.graph.graph.at("Aretha Franklin")->artist;
    
    path = SixDegreesStandard.graph.report_path(source, dest);
    
    if (path.empty()) {
        cout << "no path exists\n";
    }
    
    commandFile.close();
    infile.close(); 
}

void print_path_SD() {
    
    ifstream infile;
    
    infile.open("artists.txt");
    
    if (infile.fail()) {
        cerr << "artists.txt cannot be opened.\n";
        return;
    }
    
    ifstream commandFile;
    commandFile.open("unittestinput.txt");
    if (commandFile.fail()) {
        cerr << "unittestinput.txt cannot be opened.\n";
        exit(EXIT_FAILURE);
    }
    
    
    //cout << "about to print contents of artists.txt as a graph\n";
    SixDegrees SixDegreesStandard(infile, commandFile, cout);
    
    Artist source = SixDegreesStandard.graph.graph.at("Alicia Keys")
                                                                    ->artist;
    Artist dest = SixDegreesStandard.graph.graph.at("will.i.am")->artist;
    
    SixDegreesStandard.source = "Alicia Keys";
    SixDegreesStandard.dest = "will.i.am";
    
    SixDegreesStandard.graph.graph.at("Usher")->predecessor = 
    SixDegreesStandard.graph.graph.at("Alicia Keys");
    
    SixDegreesStandard.graph.graph.at("will.i.am")->predecessor = 
    SixDegreesStandard.graph.graph.at("Usher");
    
    SixDegreesStandard.print_path();
    
    commandFile.close();
    infile.close(); 
}

void bfs_SD() {
    ifstream infile;
    
    infile.open("artists.txt");
    
    if (infile.fail()) {
        cerr << "artists.txt cannot be opened.\n";
        return;
    }
    
    ifstream commandFile;
    commandFile.open("bfs.txt");
    if (commandFile.fail()) {
        cerr << "bfs.txt cannot be opened.\n";
        exit(EXIT_FAILURE);
    }
    
    SixDegrees SixDegreesStandard(infile, commandFile, cout);
    
    commandFile.close();
    infile.close(); 
}


