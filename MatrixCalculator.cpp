#include "stdafx.h"
#include "MatrixCalculator.h"

#include <cmath>

MatrixCalculator::MatrixCalculator(const vector<MatrixValueType>& values)
{
	//TODO this could be calculated
	static const int noOutputMatrices = 140;
	_originalMatrices.reserve(noOutputMatrices);
	_invMatrices.reserve(noOutputMatrices);

	//Check size
	if (values.size() != MatrixDimension)
	{
		string ex = "Input not valid - total of=" + std::to_string(values.size()) + " values given, expecting=" + std::to_string(MatrixDimension);
		throw std::exception(ex.c_str());
	}

	//Check uniqueness
	set<MatrixValueType> s(begin(values), end(values));
	if(s.size() != values.size())
	{
		throw std::exception("Input not valid - not all input values are unique");
	}

	calculate(values);
}

MatrixCalculator::~MatrixCalculator()
{
}

void MatrixCalculator::printMatrices() const
{
	int i = 1;

	for (const Matrix<MatrixValueType, MatrixDimension, MatrixDimension>& m : _invMatrices)
	{
		cout << "Original Matrix:" << i << endl << std::fixed << _originalMatrices[i-1] << endl;
		cout << "Inverted Matrix:" << i++ << endl << std::fixed << m << endl;
	}
}

void MatrixCalculator::calculate(const vector<MatrixValueType>& unique_7)
{
	int count = 0;

	//1. Pick 4 out of the 7	
	vector<MatrixValueType> unique_4;
	unique_4.reserve(FirstRoundUniqueItems);
	const int K = FirstRoundUniqueItems;
	const int N = MatrixDimension;	
	std::string bitmask4(K, 1); // K leading 1's
	bitmask4.resize(N, 0); // N-K trailing 0's	

	// Copy values from input vector unique_4 and permute bitmask
	do {
		for (int i = 0; i < N; ++i) // [0..N-1] integers
		{
			if (bitmask4[i])
				unique_4.push_back(unique_7[i]);
		}

		//2. Pick 3 out of the 4
		vector<MatrixValueType> unique_3;
		unique_3.reserve(SecondRoundUniqueItems);		
		std::string bitmask3(SecondRoundUniqueItems, 1); // K leading 1's
		bitmask3.resize(K, 0); // N-K trailing 0's	

		// Copy values from unique_4 to create unique_3 and permute bitmask
		do {
				for (int ind = 0; ind < K; ++ind) // [0..N-1] integers
				{
					if (bitmask3[ind])
						unique_3.push_back(unique_4[ind]);
				}

				//3. Create Matrices, invert it and store it				
				//Swap the values around if they are in the same order to reduce chance of singular matrix
				if (unique_3[0] == unique_4[0] && unique_3[1] == unique_4[1] && unique_3[2] == unique_4[2])
				{
					unique_3[0] = unique_4[2];
					unique_3[1] = unique_4[0];
					unique_3[2] = unique_4[1];
				}

				int matrixNotFoundTries = 0;
				bool matrixFound = false;

				while (!matrixFound)
				{
					vector<MatrixValueType> matrixValues;
					matrixValues.reserve(MatrixDimension);
					copy(begin(unique_4), end(unique_4), back_inserter(matrixValues));
					copy(begin(unique_3), end(unique_3), back_inserter(matrixValues));
					
					if (!matrixFound && matrixNotFoundTries > 0)
					{
						//we've tried to inverse but the values were linearly dependant
						//shuffle the matrix a bit randomly to get something non-linear
						random_shuffle(matrixValues.begin(), matrixValues.end());
					}

					int i = 0;
					Matrix<MatrixValueType, MatrixDimension, MatrixDimension> resultMat;

					//Rotate the rows to the right by one to get a matrix with an inverse, otherwise rows are always linear
					resultMat << matrixValues[0], matrixValues[1], matrixValues[2], matrixValues[3], matrixValues[4], matrixValues[5], matrixValues[6],
						matrixValues[1], matrixValues[0], matrixValues[2], matrixValues[3], matrixValues[4], matrixValues[5], matrixValues[6],
						matrixValues[2], matrixValues[1], matrixValues[0], matrixValues[3], matrixValues[4], matrixValues[5], matrixValues[6],
						matrixValues[3], matrixValues[2], matrixValues[1], matrixValues[0], matrixValues[4], matrixValues[5], matrixValues[6],
						matrixValues[4], matrixValues[3], matrixValues[2], matrixValues[1], matrixValues[0], matrixValues[5], matrixValues[6],
						matrixValues[5], matrixValues[4], matrixValues[3], matrixValues[2], matrixValues[1], matrixValues[0], matrixValues[6],
						matrixValues[6], matrixValues[5], matrixValues[4], matrixValues[3], matrixValues[2], matrixValues[1], matrixValues[0];

					//copy(begin(matrixValues), end(matrixValues), ostream_iterator<MatrixValueType>(cout, " "));

					//Do LU decomposition to find out whether the matrix can be inverted
					FullPivLU<Matrix<MatrixValueType, MatrixDimension, MatrixDimension>> lu_decomp(resultMat);

					if (lu_decomp.isInvertible())
					{
						//cout << "Try no=" << matrixNotFoundTries << ", Determinant is NON-zero, Matrix=" << resultMat << endl;

						//invert matrix
						const Matrix<MatrixValueType, MatrixDimension, MatrixDimension> inverseMat = resultMat.inverse();
						
						//4. Store original and inverted version
						_originalMatrices.emplace_back(resultMat);
						_invMatrices.emplace_back(inverseMat);						
						matrixFound = true;
						++matrixNotFoundTries;
					}
					else
					{
						//cout << "Try no=" << matrixNotFoundTries << ", Determinant is zero, Matrix = " << resultMat << endl;
						++matrixNotFoundTries;
					}

					++count;					
				}

				unique_3.clear();

			} while (std::prev_permutation(bitmask3.begin(), bitmask3.end()));

			unique_4.clear();
			
		} while (std::prev_permutation(bitmask4.begin(), bitmask4.end()));

		//cout << "Permutation count=" << count << endl;
}
