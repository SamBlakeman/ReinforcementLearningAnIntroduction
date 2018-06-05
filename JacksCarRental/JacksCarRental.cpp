
#include <iostream>
#include <algorithm>
#include <cmath>
#include "JacksCarRental.h"

using namespace std;


JacksCarRental::JacksCarRental()
{
	num_locations = 2;
	rental_reward = 10;
	movement_cost = 2;

	lambda_rentals = { 3, 4 };
	lambda_returns = { 3, 2 };

	max_cars = 20;
	max_to_move = 5;

	discount = .9;

	poisson_upper = 10;

	state_values = vector<vector<double>>(max_cars + 1, vector<double>(max_cars + 1, 0.0)); // Start with state values of 0
	policy = vector<vector<int>>(max_cars + 1, vector<int>(max_cars + 1, 0)); // Start with policy of moving no cars

	CalculateRentalProbabilities();
	CalculateReturnProbabilities();

}

/*
void JacksCarRental::SimulateDay()
{
	int rentals = 0;

	// Check how many cars Jack can rent
	
	
	
	int returns = 0;

	// Add the returns at the end of the day ready for the next day

	// Cap number at 20

	// Make decision to move cars overnight


	return;
}
*/

void JacksCarRental::CalculateRentalProbabilities()
{
	rental_probabilities = vector<vector<double>>(2, vector<double>(poisson_upper + 1, 0));

	for (int loc = 0; loc < 2; ++loc)
	{
		double sum = 0;

		for (int i = 0; i <= poisson_upper; ++i)
		{
			rental_probabilities[loc][i] = (pow(double(lambda_rentals[loc]), i) / Factorial(i)) * exp(double(-lambda_rentals[loc]));
			sum += rental_probabilities[loc][i];
		}
	}
}


void JacksCarRental::CalculateReturnProbabilities()
{
	return_probabilities = vector<vector<double>>(2, vector<double>(poisson_upper + 1, 0));

	for (int loc = 0; loc < 2; ++loc)
	{
		double sum = 0;

		for (int i = 0; i <= poisson_upper; ++i)
		{
			return_probabilities[loc][i] = (pow(double(lambda_returns[loc]), i) / Factorial(i)) * exp(double(-lambda_returns[loc]));;
			sum += return_probabilities[loc][i];
		}
	}
}


void JacksCarRental::PerformPolicyIteration()
{
	bool bPolicy_stable = false;
	int iter = 0;

	do
	{
		// Policy Evaluation
		cout << "Policy Evaluation Step: " << iter << endl;
		PolicyEvaluation();

		// Policy Improvement
		cout << "Policy Improvement Step: " << iter << endl;
		bPolicy_stable = PolicyImprovement();

		PrintPolicy();

		++iter;

	} while (!bPolicy_stable);

	return;
}

void JacksCarRental::PolicyEvaluation()
{
	bool bEvaluation_finished = false;

	while(!bEvaluation_finished)
	{
		double theta = .1;
		double diff = 0;

		for (int num_loc1 = 0; num_loc1 <= max_cars; ++num_loc1)
		{
			for (int num_loc2 = 0; num_loc2 <= max_cars; ++num_loc2)
			{
				double old_value = state_values[num_loc1][num_loc2];
				int action = policy[num_loc1][num_loc2];
				
				double new_value = ExpectedReturn(num_loc1, num_loc2, action);
				state_values[num_loc1][num_loc2] = new_value;
				diff = max(diff, abs(old_value - new_value));
			}
		}

		cout << "Diff: " << diff << endl;

		if (diff < theta)
		{
			bEvaluation_finished = true;
		}
	}

	return;
}


bool JacksCarRental::PolicyImprovement()
{
	bool bPolicy_stable = true;

	for (int num_loc1 = 0; num_loc1 <= max_cars; ++num_loc1)
	{
		for (int num_loc2 = 0; num_loc2 <= max_cars; ++num_loc2)
		{
			double current_action = policy[num_loc1][num_loc2];
			vector<double> action_values((max_to_move * 2) + 1, 0);

			for (int action = -max_to_move; action <= max_to_move; ++action)
			{
				if ((action >= 0 && num_loc1 >= action) || (action < 0 && num_loc2 >= abs(action)))
				{
					action_values[action + max_to_move] = ExpectedReturn(num_loc1, num_loc2, action);
				}
				else
				{
					action_values[action + max_to_move] = -9999999999;
				}
			}

			int best_action = distance(action_values.begin(), max_element(action_values.begin(), action_values.end())) - max_to_move;
			policy[num_loc1][num_loc2] = best_action;

			if (int(current_action) != int(best_action))
			{
				bPolicy_stable = false;
			}
		}
	}

	return bPolicy_stable;
}


double JacksCarRental::ExpectedReturn(int num_loc1, int num_loc2, double action)
{
	double expected_return = -movement_cost * abs(action); // cost of action

	int morning_loc1 = int(min(num_loc1 - action, double(max_cars))); // number at the start of the day at loc 1
	int morning_loc2 = int(min(num_loc2 + action, double(max_cars))); // number at the start of the day at loc 2

	// Iterate over the possible rental scenarios
	for (int rent_num_loc1 = 0; rent_num_loc1 <= poisson_upper; ++rent_num_loc1)
	{
		for (int rent_num_loc2 = 0; rent_num_loc2 <= poisson_upper; ++rent_num_loc2)
		{
			double rent_prob = rental_probabilities[0][rent_num_loc1] * rental_probabilities[1][rent_num_loc2];

			double total_rent_loc1 = min(morning_loc1, rent_num_loc1);
			double total_rent_loc2 = min(morning_loc2, rent_num_loc2);

			double rewards = (total_rent_loc1 + total_rent_loc2) * rental_reward;

			// Iterate over the possible return scenarios
			for (int return_num_loc1 = 0; return_num_loc1 <= poisson_upper; ++return_num_loc1)
			{
				for (int return_num_loc2 = 0; return_num_loc2 <= poisson_upper; ++return_num_loc2)
				{
					double return_prob = return_probabilities[0][return_num_loc1] * return_probabilities[1][return_num_loc2];

					double evening_loc1 = min(morning_loc1 - total_rent_loc1 + return_num_loc1, double(max_cars));
					double evening_loc2 = min(morning_loc2 - total_rent_loc2 + return_num_loc2, double(max_cars));

					expected_return += rent_prob * return_prob * (rewards + (discount * state_values[evening_loc1][evening_loc2]));
				}
			}
		}

	}

	return expected_return;
}



unsigned int JacksCarRental::Factorial(unsigned int n)
{
	if (n == 0)
		return 1;
	return n * Factorial(n - 1);
}

void JacksCarRental::PrintPolicy()
{
	cout << "Policy: " << endl;

	for (int num_loc1 = 0; num_loc1 <= max_cars; ++num_loc1)
	{
		for (int num_loc2 = 0; num_loc2 <= max_cars; ++num_loc2)
		{
			int action = policy[num_loc1][num_loc2];

			if (action < 0)
			{
				cout << policy[num_loc1][num_loc2] << '|';
			}
			else
			{
				cout << ' ' << policy[num_loc1][num_loc2] << '|';
			}
			
		}

		cout << endl;
	}
}