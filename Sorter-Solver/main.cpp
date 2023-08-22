// SorterSolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Glass.h"
#include "SorterSolver.h"

int main()
{
	std::cout << std::boolalpha;

	auto solver = SorterSolver(SolverParams{ 50 });

	std::vector<Glass> init = {
		Glass(1,2,3,4),
		Glass(3,5,6,7),
		Glass(8,9,10,4),
		Glass(10,2,9,3),
		Glass(1,5,11,7),
		Glass(7,12,4,3),
		Glass(8,5,9,12),
		Glass(11,7,1,2),
		Glass(6,6,11,4),
		Glass(12,5,6,9),
		Glass(11,8,10,12),
		Glass(2,10,1,8),
		Glass(),
		Glass(),
	};
	/*
	init = {
		Glass(1,2,1,4),
		Glass(1,2,2,4),
		Glass(1,2,4,4),
		Glass(13,7,13,7),
		Glass(13,13,7,7),
		Glass(),
		Glass(),
	};*/
	auto initial_state = SortingProblemState(init);

	/*
	initial_state.glasses = {
	Glass(1,2,3,4),
	Glass(4,2,3,4),
	Glass(1,2,3,1),
	Glass(3,1,4,2),
	Glass(),
	Glass(),
	Glass(),
	Glass(),
	};
	*/

	std::cout << initial_state << std::endl;

	solver.setup(initial_state);
	solver.solveBreadthFirst();

	std::cout << "Could solve:" << solver.solved << std::endl
		<< "Inspected:" << solver.inspected_solutions << std::endl
		<< "Wrong chnges:" << solver.wrong_change << std::endl
		<< "---------------------------------------" << std::endl;
	if (solver.solved) 
	{
		auto solution = solver.get_solution();
		for (auto I : solution.changes) {
			std::cout << '{' << I.first << ',' << I.second << "} ";
		}
		std::cout << std::endl;
		std::cout << *solution.current << std::endl;
	}
	else
	{
		std::cout << "Decayed: " << solver.decayed_solutions << std::endl;
		std::cout << "Dead ends: " << solver.dead_solutions << std::endl;
		std::cout << "Queue addition count: " << solver.q_additions<< std::endl;
		//reached states
		auto states =  solver.get_states();
		for (auto i = states.size();i --> states.size()-30; )
			std::cout << states[i]<< std::endl;
	}

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
