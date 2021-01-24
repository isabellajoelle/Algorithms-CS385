/*******************************************************************************
 * Name    : stairclimber.cpp
 * Author  : Isabella Cruz
 * Version : 1.0
 * Date    : October 2, 2020
 * Description : Lists the number of ways to climb n stairs
 * Pledge : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

vector<vector<int> > get_ways(int num_stairs) {
	// TODO: Return a vector of vectors of ints representing
	// the different combinations of ways to climb num_stairs
	// stairs, moving up either 1, 2, or 3 stairs at a time.
	vector< vector<int> > ways = {};

	//if you aren't climbing up any stairs just add an empty vector
	if( num_stairs <= 0 ){
		ways.push_back({});
		return ways;
	}

	else{
		// can only climb up stairs 1, 2, or 3 at a time (iterates through every way to scale le stairs
		for(int i = 1; i < 4; i++){
			if( num_stairs >= i){
				//recursive call
				vector<vector<int> > result = get_ways(num_stairs - i);
				//converting .size() to int by going from auto -> int
				auto size = result.size();
				for(int j = 0; j < (int)size; j++){
					//pushing # of stairs scaled into result vector
					result[j].push_back(i);
				}
				//appending the result and ways vectors
				ways.insert(ways.end(), result.begin(), result.end());
			}
		}

		return ways;
	}
}

//taken from sieve assignment to find the width of the total number of ways
//used in display_ways
int num_digits(int num){

	int counter = 1;

	while( (num / 10)  >= 1){
		counter ++;
		num /= 10;
	}

	return counter;
}

void display_ways(const vector< vector<int> > &ways) {
	// TODO: Display the ways to climb stairs by iterating over
	// the vector of vectors and printing each combination.
	auto size = ways.size();

	//if there is only 1 stair print this
	if( size == 1){
		cout << size << " way to climb " << ways[0].size() << " stair.";
		cout << endl;
	}

	//if there is more than 1 stair print this
	if(size > 1){
		cout << size << " ways to climb " << ways[0].size() << " stairs.";
		cout << endl;
	}

	// get the number of digits of total number of ways so we can align everything
	int n = num_digits(size);

	//iterates through every vector in the initial vector
	for(size_t i= 0; i < size ; i++){
		//prints out the number of the way about to be printed and aligns it
		cout << setw(n) << i+1 << ". [";
		//iterates through nested vectors BACKWARDS
		for(size_t j = ways[i].size(); j--;){
			//if j == 0 we close the printed brackets
			if( j == 0){
				cout << ways[i][j] << "]";
			}
			//printing each member of the nested vector and adding a comma + space afterwards
			else{
			cout << ways[i][j] << ", ";
			}
		}
		cout << endl;
	}


}

int main(int argc, char * const argv[]) {
	int n;

	//checks for correct amount of inputs
	if(argc != 2){
		cout << "Usage: ./stairclimber <number of stairs>";
		return 1;

	}

	istringstream iss(argv[1]);

	//checks if the input is an integer
	if(!(iss >> n)){
		cerr << "Error: Number of stairs must be a positive integer." << endl;
		return 1;
	}

	iss >> n;

	//checks if the input is greater than 0
	if(n <= 0){
			cerr << "Error: Number of stairs must be a positive integer." << endl;
			return 1;
	}

	//prints ways
	display_ways(get_ways(n));

	return 0;

}


