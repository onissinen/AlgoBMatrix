// AlgoBmatrix.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MatrixCalculator.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <random>
#include <chrono>
#include <iterator>

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/SparseCore>

using namespace std;
using namespace std::chrono;
using namespace boost::program_options;
using namespace Eigen;

int _tmain(int argc, _TCHAR* argv[])
{	
	//Possible options: 
	//--input <input values separated by comma>
	//
	//Default option is to randomise values between 0.0 and 1.0
	string inputValuesStr;
	vector<string> inputValues; 
	bool randomise = true;

	try
	{		
		options_description desc{ "Options" };
		desc.add_options()					
			("input", value<string>(&inputValuesStr), "Input values to create matrix")
			("unreg", "Unrecognized options");

		variables_map vm;
		store(parse_command_line(argc, argv, desc), vm);
		notify(vm);		

		if (vm.count("input"))
		{
			inputValuesStr = vm["input"].as<string>();
			randomise = false;
		}

		boost::split(inputValues, inputValuesStr, boost::is_any_of(","));		
	}
	catch (const error &ex)
	{
		std::cerr << ex.what() << endl;
		std::cout << ex.what() << endl;
		return 1;
	}
	
	vector<double> unique_7;
	unique_7.reserve(MatrixCalculator::MatrixDimension);

	//Initialise randomiser	
	if (randomise)
	{
		const double lower_bound = -1.0;
		const double upper_bound = 1.0;
		random_device rand_dev;          // Use random_device to get a random seed.
		mt19937 rand_engine(rand_dev()); // mt19937 is a good pseudo-random number generator.	
		//uniform_int_distribution<> unif(lower_bound, upper_bound); //TODO FOR INTEGERS
		uniform_real_distribution<double> unif(lower_bound, upper_bound);

		do
		{
			//Resize to 7 in case numbers were not unique
			unique_7.resize(7);

			//Put random data in
			for_each(begin(unique_7), end(unique_7), [&rand_engine, &unif](double& d){ d = unif(rand_engine); });

			//Erase duplicates - this is VERY unlikely to happen but hey
			unique_7.erase(unique(unique_7.begin(), unique_7.end()), unique_7.end());
		} while (unique_7.size() != 7);
	}
	else
	{
		//Simply sanity checks
		if (inputValues.size() != MatrixCalculator::MatrixDimension)
		{
			std::cout << "Number of input values=" << inputValues.size() << " differs from expected=" << MatrixCalculator::MatrixDimension << endl;
			return 1;
		}

		//Insert input
		for (const string& s : inputValues)
			unique_7.push_back(stod(s));
	}

	cout << "Unique 7 values are:" << endl;
	copy(begin(unique_7), end(unique_7), ostream_iterator<double>(cout, " "));
	cout << endl;	
	
	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	//Initialise and calculate inverted matrices	
	MatrixCalculator mc(unique_7);	

	high_resolution_clock::time_point t2 = high_resolution_clock::now();	
	std::cout << "Total duration " << std::fixed << duration_cast<chrono::milliseconds>(t2 - t1).count() << " milliseconds" << endl;

	//Print output
	mc.printMatrices();

	return 0;
}

