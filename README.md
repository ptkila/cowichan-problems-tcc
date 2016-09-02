# cowichan-problems-tcc

Repositório para implementação dos problemas cowichan para meu trabalho de conclusão de curso.

Definição dos problemas:

gauss: Gaussian Elimination
This module solves a matrix equation AX=V for a dense, symmetric, diagonally dominant matrix A and an arbitrary vector non-zero V using explicit reduction. Input matrices are required to be symmetric and diagonally dominant in order to guarantee that there is a well-formed solution to the equation.

Inputs

matrix: the real matrix A.
target: the real vector V.
Outputs

solution: a real vector containing the solution X.

hull: Convex Hull
Given a set of (x,y) points, this toy finds those that lie on the convex hull, removes them, then finds the convex hull of the remainder. This process continues until no points are left. The output is a list of points in the order in which they were removed, i.e., the first section of the list is the points from the outermost convex hull, the second section is the points that lay on the next hull, and so on.

Inputs

original: the vector of input points.
Outputs

ordered: the vector of output points (a permutation of the input).

invperc: Invasion Percolation
Invasion percolation models the displacement of one fluid (such as oil) by another (such as water) in fractured rock. In two dimensions, this can be simulated by generating an N×N grid of random numbers in the range [1..R], and then marking the center cell of the grid as filled. In each iteration, one examines the four orthogonal neighbors of all filled cells, chooses the one with the lowest value (i.e., the one with the least resistance to filling), and fills it in.

Filling begins at the central cell of the matrix (rounding down for even-sized axes). The simulation continues until some fixed percentage of cells have been filled, or until some other condition (such as the presence of trapped regions) is achieved. The fractal structure of the filled and unfilled regions is then examined to determine how much oil could be recovered.

The naïve way to implement this is to repeatedly scan the array. A much faster technique is to maintain a priority queue of unfilled cells which are neighbors of filled cells. This latter technique is similar to the list-based methods used in some cellular automaton programs, and is very difficult to parallelize effectively.

Inputs

matrix: an integer matrix.
nfill: the number of points to fill.
Outputs

mask: a Boolean matrix filled with True (showing a filled cell) or False (showing an unfilled cell).

life: Game of Life
This module simulates the evolution of Conway's Game of Life, a two-dimensional cellular automaton. At each time step, this module must count the number of live (True) cells in the 8-neighborhood of each cell using circular boundary conditions. If a cell has 3 live neighbors, or has 2 live neighbors and is already alive, it is alive in the next generation. In any other situation, the cell becomes, or stays, dead.

Inputs

matrix: a Boolean matrix representing the Life world.
numgen: the number of generations to simulate.
Outputs

matrix: a Boolean matrix representing the world after simulation.

mandel: Mandelbrot Set Generation
This module generates the Mandelbrot Set for a specified region of the complex plane. Given initial coordinates (x0, y0), the Mandelbrot Set is generated by iterating the equation:

x'	=	x2 — y2 + y0
y'	=	2xy + x0
until either an iteration limit is reached, or the values diverge. The iteration limit used in this module is 150 steps; divergence occurs when x2 + y2 becomes 2.0 or greater. The integer value of each element of the matrix is the number of iterations done. The values produced should depend only on the size of the matrix and the seed, not on the number of processors or threads used.

Inputs

nrows, ncols: the number of rows and columns in the output matrix.
x0, y0: the real coordinates of the lower-left corner of the region to be generated.
dx, dy: the extent of the region to be generated.
Outputs

matrix: an integer matrix containing the iteration count at each point in the region.

norm: Point Location Normalization
This module normalizes point coordinates so that all points lie within the unit square [0..1]×[0..1]. If xmin and xmax are the minimum and maximum x coordinate values in the input vector, then the normalization equation is

xi'	=	(xi — xmin)/(xmax — xmin)
y coordinates are normalized in the same fashion.

Inputs

points: a vector of point locations.
Outputs

points: a vector of normalized point locations.

outer: Outer Product
This module turns a vector containing point positions into a dense, symmetric, diagonally dominant N×N matrix by calculating the distances between each pair of points. It also constructs a real vector whose values are the distance of each point from the origin. Each matrix element Mi,j such that i ≠ j is given the value di,j, the Euclidean distance between point i and point j. The diagonal values Mi,i are then set to N times the maximum off-diagonal value to ensure that the matrix is diagonally dominant. The value of the vector element vi is set to the distance of point i from the origin, which is given by √(xi2 + yi2).

Inputs

points: a vector of (x,y) points, where x and y are the point's position.
Outputs

matrix: a real matrix, whose values are filled with inter-point distances.
vector: a real vector, whose values are filled with origin-to-point distances.
product: Matrix-Vector Product
Given a matrix A, a vector V, and an assumed solution X to the equation AX=V, this module calculates the actual product AX=V', and then finds the magnitude of the error.

Inputs

matrix: the real matrix A.
actual: the real vector V.
candidate: the real vector X.
Outputs

e: the largest absolute value in the element-wise difference of V and V'.

randmat: Random Number Generation
This module fills a matrix with pseudo-random integers. The values produced must depend only on the size of the matrix and the seed, not on the number of processors or threads used.

Inputs

nrows, ncols: the number of rows and columns in the output matrix.
s: the random number generation seed.
Outputs

matrix: an integer matrix filled with random values.

shuffle: Two-Dimensional Shuffle
This module divides the values in a rectangular two-dimensional integer matrix into two halves along one axis, shuffles them, and then repeats this operation along the other axis. Values in odd-numbered locations are collected at the low end of each row or column, while values in even-numbered locations are moved to the high end. An example transformation is:

a	b	c	d		a	c	b	d
e	f	g	h	→	i	k	j	l
i	j	k	l		e	g	f	h
Note that how an array element is moved depends only on whether its location index is odd or even, not on whether its value is odd or even.

Inputs

matrix: an integer matrix.
Outputs

matrix: an integer matrix containing shuffled values.

sor: Successive Over-Relaxation
This module solves a matrix equation AX=V for a dense, symmetric, diagonally dominant matrix A and an arbitrary vector non-zero V using successive over-relaxation.

Inputs

matrix: the square real matrix A.
target: the real vector V.
tolerance: the solution tolerance, e.g., 10-6.
Outputs

solution: a real vector containing the solution X.

thresh: Histogram Thresholding
This module performs histogram thresholding on an image. Given an integer image I and a target percentage p, it constructs a binary image B such that Bi,j is set if no more than p percent of the pixels in I are brighter than Ii,j. The general idea is that an image's histogram should have two peaks, one centered around the average foreground intensity, and one centered around the average background intensity. This program attempts to set a threshold between the two peaks in the histogram and select the pixels above the threshold.

Inputs

matrix: the integer matrix to be thresholded.
percent: the minimum percentage of cells to retain.
Outputs

mask: a Boolean matrix whose values are True where the value of a cell in the input image is above the threshold, and False otherwise.
vecdiff: Vector Difference
This module finds the maximum absolute elementwise difference between two vectors of real numbers.

Inputs

left: the first vector.
right: the second vector.
Outputs

maxdiff: the largest absolute difference between any two corresponding vector elements.

winnow: Weighted Point Selection
This module converts a matrix of integers to a vector of points represented as x and y coordinates. Each location where mask is True becomes a candidate point, with a weight equal to the integer value in matrix at that location and x and y coordinates equal to its row and column indices. These candidate points are then sorted into increasing order by weight, and N evenly-spaced points selected to create the result vector.

Inputs

matrix: an integer matrix whose values are used as weights.
mask: a Boolean matrix of the same size showing which points are eligible for consideration.
nelts: the number of points to select.
Outputs

points: an N-vector of (x,y) points.
