#include "WindyGridworld.h"

WindyGridworld::WindyGridworld()
{
	num_actions = 8;
	grid_width = 10;
	grid_height = 7;

	wind_values = {0, 0, 0, 1, 1, 1, 2, 2, 1, 0};
	start_loc = {3, 0};
	goal_loc = {3, 7};

	move_reward = -1;
	epsilon = .1;
	alpha = .1;
	num_episodes = 1000;

	state_action_values = vector<vector<vector<double>>>(grid_height, vector<vector<double>>(grid_width, vector<double>(num_actions, 0)));
	policy = vector<vector<int>>(grid_height, vector<int>(grid_width, 0));

	return;
}

void WindyGridworld::SARSA()
{
	for (int ep = 0; ep < num_episodes; ++ep)
	{
		void SimulateEpisode();
	}

	return;
}

void WindyGridworld::SimulateEpisode()
{
	vector<int> s = { (rand() % grid_height), (rand() % grid_width) };

	bool bEpisodeOver = false;

	while (!bEpisodeOver)
	{
		// Check for goal location
		if (s == goal_loc)
		{
			bEpisodeOver = true;
			break;
		}
		else
		{
			// Choose action
			AgentActions action = ChooseAction(s);

			// Take step


			// Update state action values


			// Set new state and action

		}
	}


	return;
}


AgentActions WindyGridworld::ChooseAction(vector<int> s)
{
	float prob = (rand() % 100) / 100.0;
	AgentActions action;

	if (prob < epsilon)
	{
		action = AgentActions(rand() % num_actions);
	}
	else
	{
		action = AgentActions(policy[s[0]][s[1]]);
	}

	return action;
}


pair<int, vector<int>> WindyGridworld::EnvironmentStep(vector<int> s, AgentActions a)
{
	int r;
	vector<int> s_p;


	return make_pair(r, s_p);
}