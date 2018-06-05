#pragma once

#include <vector>

using namespace std;

class JacksCarRental
{
public:

	// Constructor
	JacksCarRental();

	// Methods
	//void SimulateDay();

	void PerformPolicyIteration();

	// Getters


	// Setters

private:

	// Methods
	void CalculateRentalProbabilities();
	void CalculateReturnProbabilities();

	void PolicyEvaluation();
	bool PolicyImprovement();

	double ExpectedReturn(int num_loc1, int num_loc2, double action);

	unsigned int Factorial(unsigned int n);
	void PrintPolicy();


	// Variables
	int num_locations;
	int rental_reward;
	int movement_cost;
	vector<int> lambda_rentals;
	vector<int> lambda_returns;
	int max_cars;
	int max_to_move;
	double discount;
	int poisson_upper;

	vector<vector<double>> state_values;
	vector<vector<int>> policy;

	vector<vector<double>> rental_probabilities;
	vector<vector<double>> return_probabilities;

};