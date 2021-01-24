/*******************************************************************************
 * Name    : shortestpaths.cpp
 * Author  : Isabella Cruz
 * Version : 1.0
 * Date    : 12/7/20
 * Description : Solves for the shortest path using Floyd's Algorithm
 * Pledge : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <array>
#include <cctype>
#include <limits>

using namespace std;

//returns the number of digits in a long
long len(long num){
	long counter = 1;
	while( (num / 10)  >= 1){
		counter ++;
		num /= 10;
	}
	return counter;
}

//checks if a string is a number
bool is_number(string s){
	for(size_t i = 0; i < s.size(); i++){
		if(!(isdigit(s[i]))){
			return false;
		}
	 }
	return true;
}

//converts a char to a string
string char_to_string(char c){
	string s;
	s.push_back(c);
	return s;
}

//returns am error string
string error_string(string line, int start_index){
	string error_string;
	size_t i = start_index;
	while((i != line.size()) && (!(isspace(line.at(i))))){
	     error_string += line.at(i);
	     i++;
	}
	return error_string;
}

//backtracks through the intermediate values used to create the shortest path
string backtracker(long** matrix, int from, int to, int s_to, int counter){
	if(from == to){
		return char_to_string(static_cast<char>(from + 'A'));
	}else if(matrix[from][to] == numeric_limits<long>::max() && counter == 0){
		return char_to_string(static_cast<char>(from + 'A')) + " -> " + char_to_string(static_cast<char>(to + 'A'));
	}else if(matrix[from][to] == numeric_limits<long>::max() && counter != 0){
		if(to == s_to){
			return char_to_string(static_cast<char>(to + 'A'));
		}else{
			return char_to_string(static_cast<char>(to + 'A')) + " -> ";
		}
	}else if(counter == 0){
		return  char_to_string(static_cast<char>(from + 'A')) + " -> " + backtracker(matrix, from, matrix[from][to], s_to, counter+1) + backtracker(matrix, matrix[from][to], to, s_to, counter+1);
	}else{
		return backtracker(matrix, from, matrix[from][to], s_to, counter+1) + backtracker(matrix, matrix[from][to], to, s_to, counter+1);
	}
}

// tells if the vertex is valid
bool valid_vertex(char v, int vtCount){
	int last_L = 64 + vtCount;

	if( (v > last_L) || !(isalpha(v))){
		return false;
	}
	return true;
}

//prints out a matrix and it's label
void display_table(long** const matrix, const string &label, const bool use_letters = false, int const num_vertices = 0) {

	long INF = numeric_limits<long>::max();

	cout << label << endl;
	long max_val = 0;
	for (int i = 0; i < num_vertices; i++) {
		for (int j = 0; j < num_vertices; j++) {
			long cell = matrix[i][j];
			if (cell < INF && cell > max_val) {
				max_val = matrix[i][j];
			}
		}
	}
	int max_cell_width = use_letters ? len(max_val) :
			len(max(static_cast<long>(num_vertices), max_val));
	cout << ' ';
	for (int j = 0; j < num_vertices; j++) {
		cout << setw(max_cell_width + 1) << static_cast<char>(j + 'A');
	}
	cout << endl;
	for (int i = 0; i < num_vertices; i++) {
		cout << static_cast<char>(i + 'A');
		for (int j = 0; j < num_vertices; j++) {
			cout << " " << setw(max_cell_width);
			if (matrix[i][j] == INF) {
				cout << "-";
			} else if (use_letters) {
				cout << static_cast<char>(matrix[i][j] + 'A');
			} else {
				cout << matrix[i][j];
			}
		}
		cout << endl;
	}
	cout << endl;
}

//Floyd's Algorithm; prints the matrix of shortest paths, the matrix of intermediate vertices, and the shortest path that exists between ever possible pair of vertices
void Floyd(long** matrix, int vertex_count){

	//create the matrix of intermediate vertices
	long **intermediate_values = new long*[vertex_count];
	for (int i = 0; i < vertex_count; ++i) {
		intermediate_values[i] = new long[vertex_count];
		fill(intermediate_values[i], intermediate_values[i] + vertex_count, numeric_limits<long>::max());
	}

	//initialize iterator to keep track of current row/col being tested as an intermediate
	int iterator = 0;

	//brunt of the algorithm
	while(iterator < vertex_count){
		for(int i = 0; i < vertex_count; i++){
			if(i == iterator){
				continue;
			}
			for(int j = 0; j < vertex_count; j++){
				if(j == iterator || i==j){
					continue;
				}
				if((matrix[i][j] > (matrix[iterator][j] + matrix[i][iterator])) && ( 0 < (matrix[iterator][j] + matrix[i][iterator]))){
					matrix[i][j] = matrix[iterator][j] + matrix[i][iterator];
					intermediate_values[i][j] = iterator;
				}
			}
		}
		iterator++;
	}

	//prints the matrix of path lengths and intermediate vertices
	display_table(matrix, "Path lengths:", false, vertex_count);
	display_table(intermediate_values, "Intermediate vertices:", true, vertex_count);

	//prints the shortest path between every possible pair of vertices
	for(int i = 0; i < vertex_count; i++){
		for(int j = 0; j < vertex_count; j++){
			if(matrix[i][j] == numeric_limits<long>::max()){
				cout << static_cast<char>(i + 'A') << " -> " << static_cast<char>(j + 'A') << ", distance: infinity, path: none" << endl;
			}
			else{
				cout << static_cast<char>(i + 'A') << " -> " << static_cast<char>(j + 'A') << ", distance: " << matrix[i][j] << ", path: " << backtracker(intermediate_values, i, j, j, 0) << endl;
			}
		}
	}

	//deletes the intermediate_values matrix
	for (int i = 0; i < vertex_count; ++i) {
	      delete [] intermediate_values[i];
	}
	delete [] intermediate_values;
}


int main(int argc, const char *argv[]) {
    // Make sure the right number of command line arguments exist.
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    // Create an ifstream object.
    ifstream input_file(argv[1]);
    // If it does not exist, print an error message.
    if (!input_file) {
        cerr << "Error: Cannot open file '" << argv[1] << "'." << endl;
        return 1;
    }
    // Add read errors to the list of exceptions the ifstream will handle.
    input_file.exceptions(ifstream::badbit);
    string line, vertex_count;
    try {
        unsigned int line_number = 2;
        getline(input_file, vertex_count);

        //checks the data given for the number of vertices is actually a number
        if(!(is_number(vertex_count))){
        	cerr << "Error: Invalid number of vertices '" << vertex_count << "' on line 1." << endl;
        	return 1;
        }

        int vC = stoi(vertex_count);
        char last_L = 64 + vC;

        //checks if # of vertices is valid
        if(vC < 1){
        	cerr << "Error: Invalid number of vertices '" << vC << "' on line 1." << endl;
        	return 1;
        }

        //creates the distance matrix
        long **distance_matrix = new long*[vC];
        for (int i = 0; i < vC; ++i) {
            distance_matrix[i] = new long[vC];
            fill(distance_matrix[i], distance_matrix[i] + vC, numeric_limits<long>::max());
        }

        while (getline(input_file, line)) {
        	// make sure edge data is valid
        	if(line.size() < 4){
        		cerr << "Error: Invalid edge data '" << line << "' on line " << line_number << "." << endl;
        		return 1;
        	}

        	//checking if the start vertex is valid
        	if((!(valid_vertex(line.at(0), vC))) || (!(isspace(line.at(1))))){
        		cerr << "Error: Starting vertex '" << error_string(line, 0) << "' on line " << line_number << " is not among valid values A-" << last_L << "." << endl;
        		return 1;
        	}

        	//checking if the end vertex is valid
        	if((!(valid_vertex(line.at(2), vC))) || (!(isspace(line.at(3))))){
        		cerr << "Error: Ending vertex '" << error_string(line, 2) << "' on line " << line_number << " is not among valid values A-" << last_L << "." << endl;
        		return 1;
        	}

        	//checks if edge weight is a valid number
        	if((!(isdigit(line.at(4)))) || (isdigit(line.at(4)) && line.at(4) == 48)){
        		cerr << "Error: Invalid edge weight '" << error_string(line, 4) <<"' on line " << line_number << "." << endl;
        		return 1;
        	}
        	//puts given edge weight in the distance matrix
        	else{
        		string edge_weight;
        		size_t i = 4;
        		while(i != line.size()){
        			edge_weight += line.at(i);
        			i++;
        		}
        	    distance_matrix[line.at(0)-65][line.at(2)-65] = stoi(edge_weight);
        	}
            ++line_number;
        }
        // Don't forget to close the file.
        input_file.close();
        // sets values in the major diagonal of the distance matrix to 0
        for(int i = 0; i < vC; i++){
        	distance_matrix[i][i] = 0;
        }

        //print the distance matrix
        display_table(distance_matrix, "Distance matrix:", false, vC);
        //Run Floyd's algorithm on the distance matrix
        Floyd(distance_matrix, vC);

        //delete the distance matrix
        for (int i = 0; i < vC; ++i) {
        	delete [] distance_matrix[i];
        }
        delete [] distance_matrix;
        return 0;

    } catch (const ifstream::failure &f) {
        cerr << "Error: An /O error occurred reading '" << argv[1] << "'." << endl;
        return 1;
    }

}
