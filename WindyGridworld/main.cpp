#include <iostream>
#include "WindyGridworld.h"

using namespace std;

int main()
{
	WindyGridworld wg_game = WindyGridworld();

	wg_game.SARSA();

	cout << "Program Finished...\n";

	return 0;
}