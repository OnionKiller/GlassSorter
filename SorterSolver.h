#pragma once
#include <queue>
#include <vector>
#include "SortingProblemSolution.h"
#include "StateProvider.h"


struct SolverParams
{
	uint16_t depth = 30;
};

class SorterSolver
{
public:
	SorterSolver(SolverParams parameters);
	void setup(SortingProblemState initial_state);
	void solveBreadthFirst();
	// if a solution reaches max depth, it decayes, the number of these are listed there
	size_t decayed_solutions = 0;
	// count all inspected solutions
	size_t inspected_solutions = 0;
	// count dead ends
	size_t dead_solutions = 0;
	//coutn queue additions
	size_t q_additions = 0;
	//is solved
	bool solved = false;
	SortingProblemSolution get_solution();
	std::vector<SortingProblemState> get_states();
	SortingProblemSolution get_best_solution();
private:
	std::queue<std::shared_ptr<SortingProblemSolution>> _state_list;
	//create new follow up solutions from the state provider (this is the key)
	std::vector<std::shared_ptr<SortingProblemSolution>> _create_possible_solutions(std::shared_ptr<SortingProblemSolution> base_solutions);
	//process one que element
	void _process_queue();
	//check state if fully homogenous
	void _broadcast_stop();
	bool _stop = false;
	std::shared_ptr<SortingProblemSolution> _solution;
	std::shared_ptr<SortingProblemSolution> _best_solution;
	std::unique_ptr<SafeStateProvider> _state_provider;
	uint16_t _depth = 30;
	size_t _best_score = 0;
};

