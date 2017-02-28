AlgoBMatrix

AlgoBMatrix is a simple Matrix calculation project written to calculate inverted 7x7 matrices from 7 unique values. 
It uses two external libraries, boost and Eigen. Any recent version of boost should do.
Eigen is used to create the matrices and to calculate LU decomposition and the inverted matrix.
The program was compiled with VS2013 only for convinience purposes, there is no OS specific logic intended.

Main logic of the program is as follows:
1. Read in / create 7 unique values
2. From the 7 unique values, create all possible combinations of 4 values (there are 35)
3. For each of the 4 unique values chosen in step 2, choose 3 and create all possible combinations of the 3 values (there are 4)
4. This gives a total of 140 different combinations of the 7 values. 
Then, for each permutation of the 7 values, populate a 7x7 matrix pseudo-randomly that can be inverted.
5. Invert the matrix and store it in memory with the inverse matrix

The program can be invoked in two ways
Input values - With 7 given input values, e.g command "AlgoBMatrix --input 0.0,1.0,2.0,3.0,4.0,5.0,6.0"
Random values - With 7 random values between -1.0 and 1.0 by simply ignoring command line options, e.g command "AlgoBMatrix" 

There are three main source files:
AlgoBMatrix.cpp - Simple command line option logic with main function
MatrixCalculator.h/.cpp - Class that encapsulates all of matrix creation logic. It has only two interface functions, calculate and printMatrices. 
calculate takes a vector with 7 unique values as parameter and printMatrices prints the output to standard out.

This program is probably not the best or the optimal way to create inverted matrices, but I'd like to sum up this work with one of Shakespeare's quotes:
"Though this be madness, yet there is method in 't"