#include <iostream>
#include "JacksCarRental.h"

using namespace std;

int main()
{
	JacksCarRental jcr_game = JacksCarRental();

	jcr_game.PerformPolicyIteration();
	//jcr_game.PerformValueIteration();

	cout << "Program Finished...\n";

	return 0;
}