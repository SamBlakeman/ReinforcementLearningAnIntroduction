#pragma once

#include<vector>

using namespace std;

enum AgentActions
{
	Up, Down, Left, Right, UpLeft, UpRight, DownLeft, DownRight
};

class WindyGridworld
{
public:

	// Constructor
	WindyGridworld();

	// Methods
	void SARSA();
	
	// Getters

	
	// Setters


private:

	// Methods
	void SimulateEpisode();
	AgentActions ChooseAction(vector<int> s);
	pair<int, vector<int>> EnvironmentStep(vector<int> s, AgentActions a);

	// Variables
	int num_actions;
	int grid_width;
	int grid_height;

	vector<int> wind_values;
	vector<int> start_loc;
	vector<int> goal_loc;

	int move_reward;
	float epsilon;
	float alpha;
	int num_episodes;

	vector<vector<vector<double>>> state_action_values;
	vector<vector<int>> policy;
};