
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <numeric>
#include <iterator>
#include "Racetrack.h"

using namespace std;


Racetrack::Racetrack()
{
	racetrack_width = 25;
	racetrack_height = 65;

	start_start = 7;
	start_length = 10;

	ConstructRacetrack();
	PrintTrack();

	actions[0] = { 0, -1 };
	actions[1] = { 0, 0 };
	actions[2] = { 0, 1 };
	actions[3] = { 1, -1 };
	actions[4] = { 1, 0 };
	actions[5] = { 1, 1 };
	actions[6] = { -1, -1 };
	actions[7] = { -1, 0 };
	actions[8] = { -1, 1 };

	num_actions = 9;

	state_action_values = vector<vector<vector<double>>>(racetrack_height, vector<vector<double>>(racetrack_width, vector<double>(num_actions, 0)));
	returns = vector<vector<vector<vector<int>>>>(racetrack_height, vector<vector<vector<int>>>(racetrack_width, vector<vector<int>>(num_actions, vector<int>())));
	policy = vector<vector<int>>(racetrack_height, vector<int>(racetrack_width, 4));

	step_reward = -1;
	off_track_reward = -5;
	max_vel = 5;
	epsilon = .1;
	num_episodes = 10000;

}


void Racetrack::ConstructRacetrack()
{
	// Initialise with all 1s
	racetrack = vector<vector<int>>(racetrack_height, vector<int>(racetrack_width, 1));

	// Input 2s for the start and 3s finish line
	for (int i = 0; i < start_length; ++i)
	{
		racetrack[i][racetrack_width - 1] = 3;
		racetrack[racetrack_height - 1][start_start + i] = 2;
	}

	// Input 0s for the track boundaries

	for (int row = 0; row < racetrack_height; ++row)
	{
		for (int col = 0; col < racetrack_width; ++col)
		{
			if (row + col < 7)
			{
				racetrack[row][col] = 0;
			}
			else if (row > 9 && col > start_start + start_length - 1)
			{
				racetrack[row][col] = 0;
			}
			else if (row > racetrack_height / 2 && col < start_start && col + 0.2 *(racetrack_height - row) < 8)
			{
				racetrack[row][col] = 0;
			}
		}
	}

}

void Racetrack::PrintTrack()
{
	cout << "Current Track:" << endl << endl;

	for (int row = 0; row < racetrack_height; ++row)
	{
		for (int col = 0; col < racetrack_width; ++col)
		{
			cout << racetrack[row][col] << '|';
		}

		cout << endl;
	}

	cout << endl;
	
}


void Racetrack::MonteCarloES()
{
	for (int iter = 0; iter < num_episodes; ++iter)
	{
		episode ep = GenerateEpisode();
		UpdateStateActionValues(ep);
		UpdatePolicy(ep);
	}

	PrintPolicy();

	return;
}



episode Racetrack::GenerateEpisode()
{
	episode ep;

	int row = racetrack_height - 1;
	int col = (rand() % start_length) + start_start;

	vector<int> vel(2, 0);

	bool bEpisodeOver = false;

	while (!bEpisodeOver)
	{
		ep.states.push_back({ row, col });

		float prob = (rand() % 100) / 100.0;

		int action_num;
		vector<int> action;

		if (prob < epsilon)
		{
			action_num = rand() % num_actions;
			action = actions[action_num];
		}
		else
		{
			action_num = policy[row][col];
			action = actions[action_num];
		}

		ep.actions.push_back(action_num);

		int new_vel_y = vel[0] + action[0];
		int new_vel_x = vel[1] + action[1];

		// Check for max in x velocity
		if (new_vel_x > max_vel)
		{
			new_vel_x = max_vel;
		}

		// Check for positive x velocity
		if (new_vel_x < 0)
		{
			new_vel_x = 0;
		}

		// Check for max in y velocity
		if (new_vel_y > max_vel)
		{
			new_vel_y = max_vel;
		}

		// Check for positive y velocity
		if (new_vel_y < 0)
		{
			new_vel_y = 0;
		}

		// Check that the car is still moving
		if (new_vel_x == 0 && new_vel_y == 0)
		{
			new_vel_x = 1;
			new_vel_y = 1;
			//TODO do something when the car has stopped
		}

		vel[0] = new_vel_y;
		vel[1] = new_vel_x;

		int new_row = row - vel[0];
		int new_col = col + vel[1];

		int reward = step_reward;

		// Check for crossing the finish line
		if (new_row >=0 && new_row < start_length && new_col >= racetrack_width)
		{
			reward = 0;
			bEpisodeOver = true;
		}
		else
		{
			// Check whether the car is still on the track and move accordingly
			if (new_row < 0 || new_col >= racetrack_width || new_col < 0 || racetrack[new_row][new_col] == 0)
			{
				reward = off_track_reward;
				
				int row_inc = row - 1;
				int col_inc = col + 1;

				if (row_inc >= 0)
				{
					if (racetrack[row_inc][col] != 0)
					{
						new_row = row_inc;
						new_col = col;

						vel[0] = 1;
						vel[1] = 0;
					}
				}
				else
				{
					new_row = row;
					new_col = col_inc;

					vel[0] = 0;
					vel[1] = 1;
				}

				// Check for crossing the finish line again after neccessary adjustments
				if (new_row >= 0 && new_row < start_length && new_col >= racetrack_width)
				{
					reward = 0;
					bEpisodeOver = true;
				}
			}
		}
		row = new_row;
		col = new_col;

		ep.rewards.push_back(reward);
	}

	PrintEpisode(ep);

	return ep;
}




void Racetrack::PrintEpisode(episode ep)
{
	cout << "Episode Trajectory: " << endl;

	vector<vector<int>> trajectory = racetrack;

	for (int t = 0; t < ep.states.size(); ++t)
	{
		trajectory[ep.states[t][0]][ep.states[t][1]] = 9;
	}

	for (int row = 0; row < racetrack_height; ++row)
	{
		for (int col = 0; col < racetrack_width; ++col)
		{
			if (trajectory[row][col] == 9)
			{
				cout << "*|";
			}
			else
			{
				cout << trajectory[row][col] << '|';
			}
		}

		cout << endl;
	}

	cout << endl;
}


void Racetrack::UpdateStateActionValues(episode ep)
{
	for (int t = 0; t < ep.states.size(); ++t)
	{
		int row = ep.states[t][0];
		int col = ep.states[t][1];
		int action = ep.actions[t];

		int ret = GetReturn(t, ep);
		returns[row][col][action].push_back(ret);

		vector<int> returns_vec = returns[row][col][action];

		state_action_values[row][col][action] = 1.0 * accumulate(returns_vec.begin(), returns_vec.end(), 0LL) / returns_vec.size();
	}


}


int Racetrack::GetReturn(int t, episode ep)
{
	int ret = 0;

	for (int tp = t; tp < ep.states.size(); ++tp)
	{
		ret += ep.rewards[tp];
	}

	return ret;
}

void Racetrack::UpdatePolicy(episode ep)
{
	for (int t = 0; t < ep.states.size(); ++t)
	{
		vector<double> action_values = state_action_values[ep.states[t][0]][ep.states[t][1]];

		policy[ep.states[t][0]][ep.states[t][1]] = int(distance(action_values.begin(), max_element(action_values.begin(), action_values.end())));
	}

	return;
}


void Racetrack::PrintPolicy()
{

	cout << "Policy: " << endl;

	for (int row = 0; row < racetrack_height; ++row)
	{
		for (int col = 0; col < racetrack_width; ++col)
		{
			cout << policy[row][col] << '|';
		}

		cout << endl;
	}

	cout << endl;

	return;
}