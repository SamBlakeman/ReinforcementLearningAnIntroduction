#include <iostream>
#include "Racetrack.h"

using namespace std;

int main()
{
	Racetrack rt_game = Racetrack();

	rt_game.MonteCarloES();

	cout << "Program Finished...\n";

	return 0;
}