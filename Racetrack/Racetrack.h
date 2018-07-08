#pragma once

#include <vector>
#include <map>
#include <string>

using namespace std;

struct episode 
{
	vector<vector<int>> states;
	vector<int> actions;
	vector<int> rewards;
};

class Racetrack
{
public:

	// Constructor
	Racetrack();

	// Methods
	void MonteCarloES();

	// Getters


	// Setters

private:

	// Methods
	void ConstructRacetrack();
	void PrintTrack();

	episode GenerateEpisode();
	void PrintEpisode(episode ep);
	void UpdateStateActionValues(episode ep);
	int GetReturn(int t, episode ep);
	void UpdatePolicy(episode ep);
	void PrintPolicy();

	// Variables
	int racetrack_width;
	int racetrack_height;

	int start_start;
	int start_length;

	vector<vector<int>> racetrack;

	int num_actions;
	map<int, vector<int>> actions;

	vector<vector<vector<double>>> state_action_values;
	vector<vector<vector<vector<int>>>> returns;
	vector<vector<int>> policy;

	int step_reward;
	int off_track_reward;
	int max_vel;
	double epsilon;
	int num_episodes;

};