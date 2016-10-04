clauses:

	private:
		Variables are not initialised, i.e. they start with random values like any other local automatic variable (and they are often implemented using automatic variables on the stack of each thread).

	firstprivate: 
		Specifies that each thread should have its own instance of a variable, and that the variable should be initialized with the value of the variable, because it exists before the parallel construct.

	lastprivate: 
		Specifies that the enclosing context's version of the variable is set equal to the private version of whichever thread executes the final iteration (for-loop construct) or last section (#pragma sections).