// Little code explaining the working of functors

#include <iostream>

struct addX {
	addX (int x) : x(x) {}	// Member List

	int operator()(int y) const { 
		return x + y; 
	}	

private: 
	int x;

};

int main() {
	addX add27(27);
	std::cout << add27(9) << "\n";
	addX add99(99);
	std::cout << add99(9) << "\n";
	return 0;
}