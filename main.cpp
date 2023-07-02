// SorterSolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Glass.h"
#include "SorterSolver.h"

int main()
{
	std::cout << std::boolalpha;

	auto solver = SorterSolver(SolverParams{ 30 });

	auto initial_state = SortingProblemState();
	initial_state.glasses = {
		Glass(4,3,2,1),
		Glass(7,6,5,3),
		Glass(4,10,9,8),
		Glass(3,9,2,10),
		Glass(7,11,5,1),
		Glass(3,4,12,7),
		Glass(12,9,5,8),
		Glass(2,1,7,11),
		Glass(4,11,6,6),
		Glass(9,6,5,12),
		Glass(12,10,8,11),
		Glass(8,1,10,2),
		Glass(),
		Glass(),
	};

	initial_state.glasses = {
	Glass(1,2,1,1),
	Glass(2,2,2,1),
	Glass(),
	Glass(),
	Glass(),
	Glass(),
	};

	initial_state.glasses = {
		Glass(1,2,3,4),
		Glass(1,3,2,4),
		Glass(4,2,3,1),
		Glass(4,3,2,1),
		Glass(),
		Glass(),
		Glass(),
		Glass(),
	};

	std::cout<< "Initial State:" << std::endl;
	for (const auto& glass : initial_state.glasses) {
		std::cout << glass << std::endl;
	}
	solver.setup(initial_state);
	solver.solveBreadthFirst();

	std::cout << "Decayed: " << solver.decayed_solutions << std::endl;
	std::cout << "Dead ends: " << solver.dead_solutions << std::endl;
	std::cout << "Queue addition count: " << solver.q_additions << std::endl;

	auto states = solver.get_states();
	std::cout << "State hash size: " << states.size() << std::endl;
	std::cout << "Could solve:" << solver.solved << std::endl
		<< "Inspected:" << solver.inspected_solutions 
		<< std::endl;

	for (auto i = 0; i < 20; i++) {
		std::cout << states[i] << std::endl;
	}
	if (solver.solved) 
	{
		auto solution = solver.get_solution();
		for (auto I : solution.changes) {
			std::cout << '{' << I.first << ',' << I.second << "} ";
		}
		std::cout << std::endl;
		for (const auto& glass : solution.current.glasses) {
			std::cout << glass << std::endl;
		}
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
