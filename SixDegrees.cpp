/**
 ** SixDegrees.cpp
 ** Comp15 Project 2: Six Degrees of Collaboration
 ** Written by: Lucas Maley (lmaley01)
 ** April 28th, 2021
 **
 ** Implementation file for the Six Degrees class
 **/

#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#include <unordered_map>

#include "Artist.h"
#include "CollabGraph.h"
#include "SixDegrees.h"

using namespace std;

/*********************************************************************
 ******************** public function definitions ********************
 *********************************************************************/


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: basic constructor
 * @purpose: initialize a CollabGraph instance
 *
 * @preconditions: none
 * @postconditions: none
 *
 * @parameters: input stream of artist information with which to populate
 *              graph, input stream of the file containing commands, output 
 *              stream of the file to send program output to
 */
SixDegrees::SixDegrees(istream &dataFile, istream &commandFile, 
                       ostream &outputFile) {
    
    populate_graph(dataFile);
    
    output_stream = &outputFile;
    
    run_commands(commandFile);
    
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function:destructor
 * @purpose: deallocates all dynamically allocated memory
 *
 * @preconditions: none
 * @postconditions: none
 *
 * @parameters: none
 */
SixDegrees::~SixDegrees() {

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* @function: print
* @purpose: prints a CollabGraph instance
*
* @preconditions: none
* @postconditions: none
*
* @parameters: none
*/
void SixDegrees::print() {
    graph.print_graph(cout);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* @function: populate_graph
* @purpose: populates the graph with info contained in the dataFile
*
* @preconditions: none
* @postconditions: none
*
* @parameters: input stream (already opened, with no errors while opening)
*/
void SixDegrees::populate_graph(istream &infile) {
    string temp;
    std::vector<Artist> artistsAdded;
    
    while (not infile.eof()) {
        getline(infile, temp);  
        Artist tempArtist(temp); //gets artist name and creates Artist instance
        if (tempArtist.get_name() != "") {
            graph.insert_vertex(tempArtist); //inserts Artist into graph
        }
        
        while (temp != "*" and temp != "") {
            tempArtist.add_song(temp);  //gets songs and adds them to vector
            getline(infile, temp);      //within artist
        }
        artistsAdded.push_back(tempArtist);
    }
    
    string collaboration = "";
    for (unsigned long i = 0; i < artistsAdded.size(); i++) {
        for (unsigned long j = 0; j < artistsAdded.size(); j++) {
            if (i != j) {
                collaboration = 
                        artistsAdded[i].get_collaboration(artistsAdded[j]);
                if (collaboration != "") {
                    graph.insert_edge(artistsAdded[i], artistsAdded[j],
                                      collaboration); //if a collaboration 
                } //exists between artists, inserts edge between them
            }
        }
    }   
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* @function: run_commands
* @purpose: contains query loop which calls relevant functions given commands
*
* @preconditions: none
* @postconditions: none
*
* @parameters: input stream of commands
*/
void SixDegrees::run_commands(istream &input_stream) {
    string input;
    
    //checks if commands are still to be given or if quit has been called
    while (getline(input_stream, input) && input != "quit") {
        if (input != "bfs" && input != "dfs" && input != "not") {
            *output_stream << input 
                           << " is not a command. Please try again.\n";
        } else {
            getline(input_stream, source); //gets the source and destination
            getline(input_stream, dest);   //artists
            if (input == "bfs") {
                bfs(); //calls breadth first search
            } else if (input == "dfs") {
                dfs(); //calls depth first search
            } else if (input == "not") {
                getline(input_stream, input);
                while (input != "*") { //takes in artists to be excluded
                    excluded_artists.push_back(input);
                    getline(input_stream, input);
                } 
                excl(); //cals exclusive search
            }
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* @function: bfs
* @purpose: traverses the graph with a breadth first search and outputs 
*           relevant path/message
*
* @preconditions: none
* @postconditions: none
*
* @parameters: none
*/
void SixDegrees::bfs() {
    int bfs_value = bfs_traversal();
    if (bfs_value >= 0) {
        print_path(); //if a valid path exists, prints path
    } else if (bfs_value == -2) {
        *output_stream << "A path does not exist between \"" << source
        <<"\" and \"" << dest << "\".\n";
    }
    graph.clear_metadata(); //clears metadata used during traversal/printing
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* @function: bfs_traversal
* @purpose: implementation of breadth first search traversal
*
* @preconditions: none
* @postconditions: predecessors of artists in graph reflect path to source
*
* @parameters: none
*/
int SixDegrees::bfs_traversal() {
    if (not artists_known()) //if artists aren't known, returns -1
        return -1;
    queue<Artist> to_explore; //queue of nodes to explore
    queue<Artist> neighbors_to_explore; //neighbors to explore
    neighbors_to_explore.push(Artist(source)); //begins by adding source
    int curr_dist = 0;
    
    while (not neighbors_to_explore.empty()) {
        neighbors_to_explore.swap(to_explore); //swaps contents of queues
        while (not to_explore.empty()) {
            vector<Artist> neighbors = //stores neighbors of node to explore
                             graph.get_vertex_neighbors(to_explore.front());
            for (unsigned long i = 0; i < neighbors.size(); i++) {
                graph.set_predecessor(neighbors[i], to_explore.front());
                if (not graph.is_marked(neighbors[i])) {
                    neighbors_to_explore.push(neighbors[i]);
                } //adds neighbor to queue to explore
                if (neighbors[i].get_name() == dest) {
                    return curr_dist;
                } //if destination has been reached, returns length of path
            }
            graph.mark_vertex(to_explore.front());
            to_explore.pop();
        } //sets Artist to "visited" to ensure infinite loop isn't reached
        curr_dist++;
    }
    return -2; //if no path reaches destination, returns -2
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* @function: dfs
* @purpose: traverses the graph with a depth first search and outputs 
*           relevant path/message
*
* @preconditions: none
* @postconditions: none
*
* @parameters: none
*/
void SixDegrees::dfs() {
    if (artists_known()) {
        bool path_exists = false;
        dfs_traversal(source, "", path_exists); //calls traversal
        
        if (not path_exists)
            path_nonexistent();
        else
            print_path();
        
        //clears metadata used during traversal/printing
        graph.clear_metadata();
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* @function: dfs_traversal
* @purpose: implementation of depth first search traversal
*
* @preconditions: none
* @postconditions: predecessors of artists in graph reflect path to source
*
* @parameters: name of current artist and previous artist (both strings),
*              and a boolean which keeps track of whether the path exists
*/
void SixDegrees::dfs_traversal(string curr_artist, string prev_artist,
                               bool &path_exists) {
    graph.mark_vertex(curr_artist);
    if (curr_artist != source) //sets predecessor of current artist
        graph.set_predecessor(Artist(curr_artist), Artist(prev_artist));
    
    if (curr_artist == dest) { //base case: reached destination
        path_exists = true;
        return;
    } 
    
    vector<Artist> neighbors = graph.get_vertex_neighbors(Artist(curr_artist));
    
    for (unsigned long i = 0; i < neighbors.size(); i++) {
        if (not graph.is_marked(neighbors[i])) {
            dfs_traversal(neighbors[i].get_name(), curr_artist, path_exists);
        } //recursively calls dfs_traversal on neighbors of the current artist
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* @function: excl
* @purpose: traverses the graph with a depth first search, excluding the 
*           artists specified and outputs relevant path/message
*
* @preconditions: none
* @postconditions: none
*
* @parameters: none
*/
void SixDegrees::excl() {
    
    if (artists_known()) {
        for (unsigned long i = 0; i < excluded_artists.size(); i++) {
            graph.mark_vertex(excluded_artists[i]);
        }
        
        bfs();
    }
    
    excluded_artists.clear();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* @function: artists_known
* @purpose: checks through all artists given by user to verify if they're in 
*           the graph, and prints relevant message
*
* @preconditions: none
* @postconditions: if invalid artists given, relevant message printed to output
*                  stream
*
* @parameters: none
*/
bool SixDegrees::artists_known() {
    bool all_known = true;
    
    if (source == dest) {
        all_known = path_nonexistent();
    }
    
    if (not graph.is_vertex(Artist(source))) //check source
        all_known = artist_unknown(source);
    
    if (not graph.is_vertex(Artist(dest))) //check destination
        all_known = all_known = artist_unknown(dest);
    
    //check excluded artists for existence & if they are source or destination
    for (unsigned long i = 0; i < excluded_artists.size(); i++) {
        if ( not graph.is_vertex(excluded_artists[i]) )
            all_known = artist_unknown(excluded_artists[i]);
        if (excluded_artists[i] == source or excluded_artists[i] == dest)
            all_known = path_nonexistent();
    }
    
    return all_known;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* @function: artist_unknown
* @purpose: prints artist unknown message to output stream
*
* @preconditions: none
* @postconditions: message printed to output stream, returns false
*
* @parameters: name of the artist which isn't in the graph (string)
*/
bool SixDegrees::artist_unknown(string artist_name) {

    *output_stream << "\"" << artist_name 
    << "\" was not found in the dataset :(\n";
    return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* @function: path_nonexistent
* @purpose: prints path nonexistent message to output stream
*
* @preconditions: none
* @postconditions: message printed to output stream, returns false
*
* @parameters: none
*/
bool SixDegrees::path_nonexistent() {
    *output_stream << "A path does not exist between \"" << source
    <<"\" and \"" << dest << "\".\n";
    return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* @function: print_path
* @purpose: prints path to output stream
*
* @preconditions: traversal completed which sets predecessors from dest to
*                 source
* @postconditions: path printed to output stream
*
* @parameters: none
*/
void SixDegrees::print_path() {
    stack<Artist> path = graph.report_path(source, dest);
    stack<Artist> collaborators = path;
    
    if (collaborators.empty()) 
        return; //avoids segfault
    
    collaborators.pop();
    
    int artists_in_path = collaborators.size();
    
    //iterates through path, printing to output stream
    for (int i = 0; i < artists_in_path; i++) {
        *output_stream << "\"" << path.top().get_name() << "\" collaborated " 
        << "with \"" << collaborators.top().get_name() << "\" in \"" <<
        graph.get_edge(path.top(), collaborators.top()) << "\"." << endl;
        
        path.pop();
        collaborators.pop();
    }
    *output_stream << "***\n";
}
