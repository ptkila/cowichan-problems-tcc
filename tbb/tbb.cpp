#include "tbb.h"

int main () {

	TBB *tbb = new TBB();
	tbb->randmat(2,2,10);
	return 0;
	
}