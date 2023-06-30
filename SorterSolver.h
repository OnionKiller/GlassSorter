#pragma once
#include "SortingProblem.h"
#include <queue>
#include <vector>


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
private:
	std::queue<SortingProblemSolution> _state_list;
	//create new follow up states
	std::vector<changePair> _create_possible_changes(SortingProblemState base_state);
	void _remove_oscillations(std::vector<changePair>& change_list, SortingProblemSolution& solution);
	bool _check_if_state_is_recursive(SortingProblemState state);
	// apply changes, and upload them to the state queue
	void _apply_changes(std::vector<changePair> changes, SortingProblemSolution& base_state);
	//process one que element
	void _process_queue();
	//check state if fully homogenous
	bool _check_fully_homogen(SortingProblemState state);
	void _broadcast_stop();
	bool _stop = false;
	std::unique_ptr<SortingProblemSolution> _solution;
	std::vector<SortingProblemState> _reached_states;
	uint16_t _depth = 30;

};

