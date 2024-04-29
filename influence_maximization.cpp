/*
PROGRAM: influence_maximization.cpp
AUTHOR: JOSEPH M. ABRUZZO

DESCRIPTION: This program implements the greedy algorithm of Kempe, Kleinberg, and
			 Tardos (2003) to approximately solve the Influence
			 Maximization Problem. Given a directory containing edgelists 
			 corresponding to directed acyclic cascades in a network, this
			 program reads the edgelists into a vector of adjacency lists. It
			 then runs the greedy algorithm with these cascades as input,
			 greedily building an approximately optimal influential set.
*/





#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <chrono>
#include <set>
#include <queue>
#include <map>

using namespace std;




// Constant chars for identifying comment lines in edgelist files
const char POUND = '#';
const char PERCENT = '%';

// Constant int for user to specify number of influential nodes desired
const int PARAM_K = 1;

// Constant string for user to specify directory of cascade files
const string CASCADE_DIRECTORY = "/path/to/cascades/";





/*
Function: print_set
Input: Set of integers
Output: None

Description: Helper function that prints a set of integers to the console.
*/
void print_set(set<int>& S)
{

	cout << "{";

	int count = 0;

	for (int s : S) {

		cout << to_string(s);

		count++;

		if (count < S.size()) {
			cout << ", ";
		}

	}

	cout << "}";

}




/*
Function: reachable_from
Input: map from integers to vectors of integers, set of integers
Output: none

Description: Given a map that represents a cascade of influence through a network,
			 finds the total number of nodes influenced by a seed set of nodes S 
			 using breadth-first search.
*/
int reachable_from(map<int, vector<int> >& A, set<int>& S)
{

	// initialize count of nodes reachable from seed set S in cascade A
	int r = 0;

	// initialize queue and map required to implement breadth-first search
	queue<int> Q;
	map<int, bool> explored;

	// for each seed node in S, do:
	for (int s : S) {

		// increment reachable count by one, as seed nodes can always reach
		// themselves
		r++;

		// add seed node to BFS queue
		Q.push(s);

		// mark each node in seed set explored
		explored[s] = true;
	}

	// while the queue is not empty, do
	while (!Q.empty()) {

		// set u equal to the node at the front of the queue and pop the queue
		int u = Q.front();
		Q.pop();

		// for each node v reachable via an outgoing edge from u, do
		for (int v : A[u]) {

			// if v has not been explored, do
			if (!explored[v]) {

				// add v to the queue
				Q.push(v);

				// mark v explored
				explored[v] = true;

				// increment the number of nodes reachable from the seed set by one
				r++;

			}

		}

	}

	// return number of nodes reachable in cascade A from seed set S
	return r;

}





/*
Function: calculate_influence
Input: vector of maps, set of integers
Output: double

Description: Given a vector of maps representing information cascades. For each
cascade, calculates the influence of a seed set of nodes S. Averages the numbers
representing the influence of S under each cascade and returns this as the 
overall influence of S.
*/
double calculate_influence(vector<map<int, vector<int> > >& cascades, set<int>& S)
{

	// initialize double to store final influence value
	double influence = 0.0;

	// for each cascade in the cascade vector, do
	for (map<int, vector<int> > A : cascades) {

		// calculate the number of reachable nodes from S in the cascade A (i.e.,
		// the influence of S in A)
		int influence_A = reachable_from(A, S);

		// add the influence of S in A to the total influence value
		influence += influence_A;

	}

	// divide total influence value by number of cascades to obtain final
	// influence value
	influence = influence / cascades.size();

	// return final influence value
	return influence;

}





/*
Function: create_cascade
Input: set of ints, map from ints to vectors of ints, string
Output: none

Description: Given a set of ints representing all the nodes in all the cascades
in the dataset, a map that will represent a single cascade as an adjacency list, 
and a string representing a file name. Reads the edgelist specified in the 
cascade .txt file and puts this information into the map. Also adds each node in 
the cascade file to the set of all nodes in all the cascades.
*/
void create_cascade(set<int>& V, map<int, vector<int> >& A, string graph_file_name)
{

	// initialize ifstream corresponding to the cascade file name
	ifstream infile(graph_file_name.c_str());

	// while the .txt file still has lines, do
	string line;
	while(getline(infile, line))
	{

		// if the current line is not a comment line and is not empty
		if (!(line.at(0) == POUND || line.at(0) == PERCENT) && !(line == "")) {
			istringstream iss(line);
			int from;
			int to;

			// read nodes in line
			iss >> from >> to;

			// add nodes to map representing adjacency list
			A[from].push_back(to);

			// add nodes to set of all nodes in all the cascades
			V.insert(to);
			V.insert(from);
			
		}

	}

}





/*
Function: get_cascade_vector
Input: set of ints, vector of maps
Output: none

Description: Given a set of ints representing all the nodes in all the cascades
in the dataset and a vector of maps that will contain all of the cascades in
the dataset. Collects the file names in the directory containing the cascade
files. Reads the information in each cascade file into a map and adds this map 
to the cascade vector.
*/
void get_cascade_vector(set<int>& V, vector<map<int, vector<int> > >& cascades)
{

	// initialize empty vector of strings to contain cascade file names
	vector<string> graph_file_names;

	// for each file in the cascade directory specified by the user, do
	for (auto file : filesystem::directory_iterator(CASCADE_DIRECTORY)) {

		// get file path string
		string file_path = file.path();

		// if the file is a .txt file, add the file path to the vector of cascade
		// file paths
		if (file_path.find(".txt") != -1) {
			graph_file_names.push_back(file_path);
		}

	}

	// for each file path in the vector of cascade file paths
	for (string graph_file_name : graph_file_names) {

		// initalize a map that will represent the information in the cascade
		// file as an adjacency list
		map<int, vector<int> > A;

		// populate the map with the information in the cascade file
		// also add any new nodes in the current cascade to the set of all nodes in all the cascades
		create_cascade(V, A, graph_file_name);

		// add the map representing the cascade to the vector of cascades
		cascades.push_back(A);

	}

}




/*
Function: main
Input: none
Output: 0

Description: Main function that runs the program.
*/
int main()
{
	// intialize a set to store all the nodes in all the cascades
	set<int> V;

	// initialize a vector of maps to store the adjacency lists representing all
	// the cascades in the directory provided by the user
	vector<map<int, vector<int> > > cascades;

	cout << endl << "READING CASCADES..." << endl;

	// get the information in the cascade files and store it in the vector of 
	// adjacency lists
	// one adjacency list per cascade file
	get_cascade_vector(V, cascades);

	cout << endl << "CASCADES READ! NUMBER OF CASCADES: " << to_string(cascades.size()) << endl;

	cout << endl << "RUNNING GREEDY ALGORITHM..." << endl;

	auto start = chrono::high_resolution_clock::now();

	// initialize a set to store the approximately optimal set of influencers
	set<int> S;

	// initialize double to store the previous total influence of the set
	double previous_influence = 0.0;

	// for K iterations corresponding to the K nodes to be selected, do
	for (int iter=0; iter<PARAM_K; iter++) {

		// initialize doubles and int to store the maximum change in the 
		// objective function in this iteration, the maximum influence of a set
		// in this iteration, and the node corresponding to the maximally influential
		// node this iteration given the approximately optimal set so far
		double max_delta = -1.0;
		double max_influence = -1.0;
		int max_delta_node = -1;

		// for each node u in all the cascades, do
		for (int u : V) {

			// if u is not already in the approximately optimal set,
			if (S.find(u) == S.end()) {

				// create a copy of the approximately optimal set and add u
				set<int> T = S;
				T.insert(u);

				// calculate the influence of this new set
				double influence_T = calculate_influence(cascades, T);

				// calculate the change in the objective function when u is
				// added to the approximately optimal set 
				double delta = influence_T - previous_influence;

				// if this change is larger than the maximum change this iteration,
				// update the maximum change to be the change corresponding to u,
				// update the maximum influence of a set this iteration to be the
				// influence of the approximately optimal set plus u, and update
				// the maximally influential node given the approximately
				// optimal set this iteration to be u
				if (delta > max_delta) {
					max_delta = delta;
					max_influence = influence_T;
					max_delta_node = u;
				}

			}

		}

		// add the maximally influential node to the approximately optimal set
		S.insert(max_delta_node);

		// update the previous influence value to be the influence of this new set
		previous_influence = max_influence;

	}

	cout << endl << "GREEDY ALGORITHM FINISHED!" << endl;

	// print the approximately optimal set
	cout << endl << "APPROXIMATELY OPTIMAL SET (SIZE " << to_string(PARAM_K) << "): "; 
	print_set(S);
	cout << endl;
	
	// print the infleunce of the approximately optimal set
	cout << endl << "INFLUENCE OF APPROX. OPTIMAL SET (NUMBER OF NODES): " << to_string(previous_influence) << endl;

	auto stop = chrono::high_resolution_clock::now();

	auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

	// print the total time the program took in seconds
	cout << endl << "TIME (SEC): " << duration.count() / 1000.0 << endl << endl;

	return 0;
}