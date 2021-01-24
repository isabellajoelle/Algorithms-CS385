/*******************************************************************************
* Name        : inversioncounter.cpp
* Author      : Isabella Cruz
* Version     : 1.0
* Date        : October 30th, 2020
* Description : Counts the number of inversions in an array.
* Pledge      : I pledge my honor thatI have abided by the Stevens Honor System.
*******************************************************************************/
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cctype>
#include <cstring>

using namespace std;

// Function prototype.
static long mergesort(int array[], int scratch[], int low, int high);
/**
* Counts the number of inversions in an array in theta(n^2) time.
*/

long count_inversions_slow(int array[], int length) {
	// TODO
	long counter = 0;

	for(int i = 0; i < length; i++){
		for(int j = i+1; j < length; j++){
			if(array[i] > array[j]){
				counter++;
			}
		}
	}

	return counter;
	}

// Counts the number of inversions in an array in theta(n lg n) time.

long count_inversions_fast(int array[], int length) {
	// TODO
	// Hint: Use mergesort!
	vector<int> scratch;
	scratch.resize(length);

	return mergesort(array, &scratch[0], 0, length-1);
}

static long mergesort(int array[], int scratch[], int low, int high) {
	long counter = 0;
	if( low < high){
		long mid = low + (high - low) / 2;
		counter += mergesort(array, scratch, low, mid);
		counter += mergesort(array, scratch, mid+1, high);
		int L = low;
		int H = mid + 1;
		for(int k = low; k <= high; k++){
			if((L <= mid) && ( (H > high) || (array[L] <= array[H]))){
				scratch[k] = array[L];
				L+=1;
			}
			else{
				scratch[k] = array[H];
				H+=1;
				counter = counter + ((mid - L) + 1);
			}
		}
		for(int j = low; j <= high; j++){
			array[j] = scratch[j];
		}
	}
	return counter;
}

int main(int argc, char *argv[]) {
	// TODO: parse command-line argument
	if(argc > 2){
		cerr << "Usage: ./inversioncounter [slow]";
		return 1;
	}

	if(argv[1]!= 0){
		char slow[] ="slow";
		if(strcmp(slow, argv[1]) != 0){
			cerr << "Error: Unrecognized option '" << argv[1] << "'.";
			return 1;
		}
	}

	cout << "Enter sequence of integers, each followed by a space: " << flush;
	istringstream iss;
	int value, index = 0;
	vector<int> values;
	string str;
	str.reserve(11);
	char c;
	while (true) {
		c = getchar();
		const bool eoln = c == '\r' || c == '\n';
		if (isspace(c) || eoln) {
			if (str.length() > 0) {
				iss.str(str);
				if (iss >> value) {
					values.push_back(value);
                } else {
                	cerr << "Error: Non-integer value '" << str
                		 << "' received at index " << index << "." << endl;
                	return 1;
                }
				iss.clear();
				++index;
			}
			if (eoln) {
				break;
			}
			str.clear();
		} else {
			str += c;
		}
	}
	// TODO: produce output

	if(values.empty()){
		cerr << "Error: Sequence of integers not received.";
		return 1;
	}

	if(argv[1] == 0){
		cout << "Number of inversions: " << count_inversions_fast(&values[0], values.size());
	}

	else{
		cout << "Number of inversions: " << count_inversions_slow(&values[0], values.size());
		return 0;
	}

	}


