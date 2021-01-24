/*******************************************************************************
 * Name    : sieve.cpp
 * Author  : Isabella Cruz
 * Version : 1.0
 * Date    : September 18, 2020
 * Description : Sieve of Eratosthenes
 * Pledge : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

class PrimesSieve{
public:
	PrimesSieve(int limit) : is_prime_(new bool[limit + 1]), limit_{limit} {
		sieve();
	}

	~PrimesSieve(){
		delete [] is_prime_;
	}

	int num_primes() const {
		return num_primes_;
	}

	void display_primes() const;

private:
	//Instance Variables
	bool * const is_prime_;
	const int limit_;
	int num_primes_, max_prime_;
	const int max_prime_width = num_digits(max_prime_);

	//Method Declarations
	int count_num_primes() const;
	void sieve();
	static int num_digits(int num);
};

void PrimesSieve::display_primes() const{

	//setting max prime width and primes per row
	int primes_per_row = 80 / (PrimesSieve::max_prime_width +1);

	//tracks the amount of primes per line as we iterate through
	int prime_counter = 0;

	//if the code fits on one line
	if(PrimesSieve::num_primes_ <= primes_per_row){
		for(int i = 0; i <= PrimesSieve::limit_+1; i++){
			if(PrimesSieve::is_prime_[i]){
				//if this is the highest prime
				if(i == PrimesSieve::max_prime_){
					break;
				}
				cout << i << " ";
			}
		}

		cout << PrimesSieve::max_prime_;

	}

	else{
		for(int i = 0; i <= PrimesSieve::limit_+1; i++){
			//if i is prime
			if(PrimesSieve::is_prime_[i]){
				prime_counter++;

				//if i is the max prime, break out of the for loop
				if(i == PrimesSieve::max_prime_){
					//printing the last prime w/o a space after it
					cout << PrimesSieve::max_prime_;
					break;
				}

				// checking widths of current prime and max prime
				if(max_prime_width >= num_digits(i)){
					//printing the primes
					cout << setw(max_prime_width) << i;
					// checks if we are at the max # of primes in one line
					if(prime_counter == primes_per_row){
						cout << endl;
						prime_counter = 0;
					}

					else{
					cout << " ";
					}
				}

			}
		}
	}

}

int PrimesSieve::count_num_primes() const{

	int counter = 0;

	for(int i = 0; i <= PrimesSieve::limit_ + 1; i++){
		if(PrimesSieve::is_prime_[i]){
			counter++;
		}
	}

	return counter;
}

void PrimesSieve::sieve(){

	//setting 0 and 1 in array to false
	PrimesSieve::is_prime_[0] = {false};
	PrimesSieve::is_prime_[1] = {false};

	//setting all bool values index 2 - limit + 1 to true
	for(int i = 2; i<= PrimesSieve::limit_; i++){
		PrimesSieve::is_prime_[i] = {true};
	}

	//iterating through 2-limit
	for(int i = 2; i <= sqrt(PrimesSieve::limit_); i++){
		if(PrimesSieve::is_prime_[i]){
			for(int j = pow(i,2); j <= PrimesSieve::limit_; j += i){
				PrimesSieve::is_prime_[j] = {false};
			}
		}
	}

	//finding the max prime and initializing max_prime_

	for(int i = PrimesSieve::limit_; i >= 0 ; i--){
			if(PrimesSieve::is_prime_[i]){
				 PrimesSieve::max_prime_= i;
				 break;
			}
	}


	//intializing num_primes_
	PrimesSieve::num_primes_ = count_num_primes();

}

int PrimesSieve::num_digits(int num){

	int counter = 1;

	while( (num / 10)  >= 1){
		counter ++;
		num /= 10;
	}

	return counter;
}

int main(){
	cout << "**************************** " <<  "Sieve of Eratosthenes" <<
			" ****************************" << endl;

	cout << "Search for primes up to: ";
	string limit_str;
	cin >> limit_str;
	int limit;

	// Use stringstream for conversion. Don't forget to #include <sstream>
	istringstream iss(limit_str);

	// Check for error.
	if ( !(iss >> limit) ) {
		cerr << "Error: Input is not an integer." << endl;
		return 1;
	}
	if (limit < 2) {
		cerr << "Error: Input must be an integer >= 2." << endl;
		return 1;
	}

	// TODO: write code that uses your class to produce the desired output.
	cout << endl;
	cout << "Number of primes found: " << PrimesSieve(limit).num_primes() << endl;
	cout << "Primes up to " << limit << ":" << endl;
	PrimesSieve(limit).display_primes();


	return 0;
}
