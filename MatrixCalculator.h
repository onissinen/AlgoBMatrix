#pragma once

#define _SCL_SECURE_NO_WARNINGS

#include "stdafx.h"
#include <set>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <random>
#include <chrono>
#include <iterator>
#include <array>
#include <chrono>
#include <exception>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/SparseCore>

using namespace std;
using namespace std::chrono;
using namespace Eigen;

///This class takes a set of 7 values out of which
//it will calculate the inverted matrices using the following logic
//1. Choose 4 unique items from 7 unique items (there are 35 ways to do that).
//2. Choose 3 unique items from 4 unique items(there are 4).
//3. From the 4 + 3 items produce all possible 7 x 7 matrices (35 x 4 = 140 ways)
//4. Invert the matrices and store them

class MatrixCalculator
{
public:
	typedef double MatrixValueType;

	MatrixCalculator(const vector<MatrixValueType>& values); //throws and exception if values are not unique or if there are more/less than 7
	~MatrixCalculator();

	void printMatrices() const;

	static const int MatrixDimension = 7;
	static const int FirstRoundUniqueItems = 4;
	static const int SecondRoundUniqueItems = 3;	

private:
	void calculate(const vector<MatrixValueType>& values); //Calculate inverted matrices

	vector<Matrix<MatrixValueType, MatrixDimension, MatrixDimension>> _originalMatrices;
	vector<Matrix<MatrixValueType, MatrixDimension, MatrixDimension>> _invMatrices;
};

