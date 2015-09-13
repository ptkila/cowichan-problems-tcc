#include "cpp11.hpp"

int main () {

	Cpp11 *cpp = new Cpp11();
	cpp->randmat(2, 2, 10);
	return 0;
}