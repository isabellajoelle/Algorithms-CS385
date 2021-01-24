/*******************************************************************************
 * Name    : waterjugpuzzle.cpp
 * Author  : Isabella Cruz
 * Version : 1.0
 * Date    : 10/19/20
 * Description : Solves the water jug puzzle
 * Pledge : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <sstream>
#include <cctype>
#include <queue>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

struct State {
	int a, b, c;
	string directions;
	State *parent;

	State(int _a, int _b, int _c, string _directions) :
		a{_a}, b{_b}, c{_c}, directions{_directions}, parent{nullptr} { };

	// String representation of state in tuple form.

	string to_string() {
		ostringstream oss;
		oss << "(" << a << ", " << b << ", " << c << ")";
		return oss.str();
	}

};

queue<State> q;

bool can_pour(int from, int to, int capTo){
	if( from == 0){
		return false;
	}else if( to == capTo ){
		return false;
	}else{
		return true;
	}
}

string poured(int from, int to){
	int n = from - to;
	string str = to_string(n);
	if(n == 1){
		return str + " gallon";
	}else{
	return str + " gallons";
	}
}

vector<int> pour(int from, int to, int capTo){

	while(from != 0){
		if(to == capTo){
			break;
		}
		from--;
		to++;
	}
	vector<int> returnVec = {from, to};
	return returnVec;
}

vector<string> bfs(int a, int b, int c, int aG, int bG, int cG){

	vector<int> pourVec; vector<string> printVec;

	bool **array = new bool*[a+1];

	for (int i = 0; i < a+1; ++i) {
		   array[i] = new bool[b+1];
		   fill(array[i], array[i] + (b+1), false);
	}

    //add new State(a, b, c) to queue
	State *goal_State = new State(aG,bG,cG, "");
	State *initial_State = new State(0,0,c, "");
	initial_State->directions = "Initial state. " + initial_State->to_string();
	initial_State->parent = nullptr;
	q.push(*initial_State);

    //while (queue is not empty)
	while(!(q.empty())){
		//  current <- front of queue
		State *current_State = new State(0, 0, 0, "");
		State *last_State = new State(0, 0, 0, "");
		State *new_State = new State(0, 0, 0, "");
		*current_State = q.front();
		//pop from the queue
		q.pop();

		  //if current equals goal_state
		if( (current_State->a == goal_State->a ) && (current_State->b == goal_State->b ) && (current_State->c == goal_State->c )){

			while(current_State->parent != nullptr){
				printVec.push_back(current_State->directions + " " + current_State->to_string());
				*current_State = *current_State->parent;
			}

			printVec.push_back(initial_State->directions);
			return printVec;

			for (int i = 0; i < a+1; ++i) {
				delete [] array[i];
			}
			delete [] array;
			delete initial_State;
			delete current_State;
			delete last_State;
			delete new_State;
			delete goal_State;
		}

		if(array[current_State->a][current_State->b] == true){
			//current has been seen
			delete current_State;
			delete last_State;
			delete new_State;
			continue;
		}

		else{
		//mark current as having been visited
		array[current_State->a][current_State->b] = true;

        //try the 6 ways to pour water, pushing new States to the queue
		if(can_pour(current_State->c, current_State->a, a)){
			*last_State = *current_State;
			pourVec = pour(current_State->c,current_State->a, a);
			new_State->a = pourVec.at(1);
			new_State->c = pourVec.at(0);
			new_State->b = current_State->b;
			new_State->directions = "Pour " + poured(current_State->c, pourVec.at(0)) + " from C to A.";
			new_State->parent = last_State;
			q.push(*new_State);
		}
		if(can_pour(current_State->b, current_State->a, a)){
			*last_State = *current_State;
			pourVec = pour(current_State->b,current_State->a, a);
			new_State->a = pourVec.at(1);
			new_State->b = pourVec.at(0);
			new_State->c = current_State->c;
			new_State->directions = "Pour " + poured(current_State->b, pourVec.at(0)) + " from B to A.";
			new_State->parent = last_State;
			q.push(*new_State);
		}
		if(can_pour(current_State->c, current_State->b, b)){
			*last_State = *current_State;
			pourVec = pour(current_State->c,current_State->b, b);
			new_State->b = pourVec.at(1);
			new_State->c = pourVec.at(0);
			new_State->a = current_State->a;
			new_State->directions = "Pour " + poured(current_State->c, pourVec.at(0)) + " from C to B.";
			new_State->parent = last_State;
			q.push(*new_State);
		}
		if(can_pour(current_State->a, current_State->b, b)){
			*last_State = *current_State;
			pourVec = pour(current_State->a,current_State->b, b);
			new_State->a = pourVec.at(0);
			new_State->b = pourVec.at(1);
			new_State->c = current_State->c;
			new_State->directions = "Pour " + poured(current_State->a, pourVec.at(0)) + " from A to B.";
			new_State->parent = last_State;
			q.push(*new_State);
		}
		if(can_pour(current_State->b, current_State->c, c)){
			*last_State = *current_State;
			pourVec = pour(current_State->b,current_State->c, c);
			new_State->b = pourVec.at(0);
			new_State->c = pourVec.at(1);
			new_State->a = current_State->a;
			new_State->directions = "Pour " + poured(current_State->b, pourVec.at(0)) + " from B to C.";
			new_State->parent = last_State;
			q.push(*new_State);
		}
		if(can_pour(current_State->a, current_State->c, c)){
			*last_State = *current_State;
			pourVec = pour(current_State->a, current_State->c, c);
			new_State->a = pourVec.at(0);
			new_State->c = pourVec.at(1);
			new_State->b = current_State->b;
			new_State->directions = "Pour " + poured(current_State->a, pourVec.at(0)) + " from A to C.";
			new_State->parent = last_State;
			q.push(*new_State);
		}
	}

		current_State = NULL;
		last_State = NULL;
		new_State = NULL;
		delete current_State;
		delete last_State;
		delete new_State;
	}

	for (int i = 0; i < a+1; ++i) {
		delete [] array[i];
	}
	delete [] array;
	delete initial_State;
	delete goal_State;

    printVec.push_back("No solution.");
    return printVec;
}

int main(int argc, char *argv[]){
	double a, b, c, aG, bG, cG;
	vector<string> print;

	if(argc != 7 ){
		cout << "Usage: ./waterjugpuzzle <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>";
		return 1;
	}

	istringstream iss;

	iss.str(argv[1]);
	if(!(iss >> a)){
		cout << "Error: Invalid capacity '" << argv[1] << "' for jug A.";
		return 1;
	}
	iss >> a;
	if(a < 0){
			cout << "Error: Invalid capacity '" << argv[1] << "' for jug A.";
			return 1;
	}
	iss.clear();
	iss.str(argv[2]);
	if(!(iss >> b)){
		cout << "Error: Invalid capacity '" << argv[2] << "' for jug B.";
		return 1;
	}
	iss >> b;
	if(b < 0){
		cout << "Error: Invalid capacity '" << argv[2] << "' for jug B.";
		return 1;
	}
	iss.clear();
	iss.str(argv[3]);
	if(!(iss >> c)){
		cout << "Error: Invalid capacity '" << argv[3] << "' for jug C.";
		return 1;
	}
	iss >> c;
	if(c <= 0){
			cout << "Error: Invalid capacity '" << argv[3] << "' for jug C.";
			return 1;
	}
	iss.clear();
	iss.str(argv[4]);
	if(!(iss >> aG)){
		cout << "Error: Invalid goal '" << argv[4] << "' for jug A.";
		return 1;
	}
	iss >> aG;
	if(aG < 0){
		cout << "Error: Invalid goal '" << argv[4] << "' for jug A.";
		return 1;
	}
	iss.clear();
	iss.str(argv[5]);
	if(!(iss >> bG)){
		cout << "Error: Invalid goal '" << argv[5] << "' for jug B.";
		return 1;
	}
	iss >> bG;
	if(bG < 0){
		cout << "Error: Invalid goal '" << argv[5] << "' for jug B.";
		return 1;
	}
	iss.clear();
	iss.str(argv[6]);
	if(!(iss >> cG)){
		cout << "Error: Invalid goal '" << argv[6] << "' for jug C.";
		return 1;
	}
	iss >> cG;
	if(cG < 0){
		cout << "Error: Invalid goal '" << argv[6] << "' for jug C.";
		return 1;
	}else if(aG > a){
		cout << "Error: Goal cannot exceed capacity of jug A.";
		return 1;
	}else if(bG > b){
		cout << "Error: Goal cannot exceed capacity of jug B.";
		return 1;
	}else if(cG > c){
		cout << "Error: Goal cannot exceed capacity of jug C.";
		return 1;
	}else if( c != (aG+bG+cG)){
		cout << "Error: Total gallons in goal state must be equal to the capacity of jug C.";
		return 1;
	}else{

		print = bfs(a, b, c, aG, bG, cG);
		reverse(print.rbegin(), print.rend());

		for(size_t i = 0; i < print.size(); i++){
			cout << print.at(i) << endl;
		}

		return 0;
	}
}


