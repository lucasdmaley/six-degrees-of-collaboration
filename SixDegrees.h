/**
 ** SixDegrees.h
 ** Comp15 Project 2: Six Degrees of Collaboration
 ** Written by: Lucas Maley (lmaley01)
 ** April 28th, 2021
 **
 ** Header file for the Six Degrees class
 **/

#ifndef __SIX_DEGREES__
#define __SIX_DEGREES__

#include <iostream>
#include <stack>
#include <vector>
#include <unordered_map>
#include <queue>

#include "CollabGraph.h"
#include "Artist.h"

using namespace std;

class SixDegrees {

public:

    /* Constructors */
    SixDegrees(istream &dataFile);
    SixDegrees(istream &dataFile, istream &commandFile) ;
    SixDegrees(istream &dataFile, istream &commandFile, ostream &outputFile);
    
    /* Destructor */
    ~SixDegrees();

    /* print function */
    void print();
    
private:
    
    /* CollabGraph instance */
    CollabGraph graph;
    std::vector<string> excluded_artists;
    
    /* names of source and destination artists for different pathing */
    string source, dest;
    
    //output file stream for program
    ostream *output_stream;
    
    /* private helper function to populate a graph */
    void populate_graph(istream &infile);
    
    /* query loop and helper functions for each command */
    void run_commands(istream &input_stream);
    void bfs();
    int bfs_traversal();
    void dfs();
    void dfs_traversal(string curr_artist, string prev_artist, 
                       bool &path_exists);
    void excl();
    
    bool artists_known();
    bool artist_unknown(string artist_name);
    bool path_nonexistent();
    void print_path();
};

#endif /* __COLLAB_GRAPH__ */
