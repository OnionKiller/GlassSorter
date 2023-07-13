#include "SorterSolver.h"

SorterSolver::SorterSolver(SolverParams parameters)
{
	_depth = parameters.depth;
}

void SorterSolver::setup(SortingProblemState initial_state)
{
	if (!_state_list.empty()) {
		//maybe warn about clearing the queue
		for (auto i = _state_list.size(); i-- > 0;)
			_state_list.pop();
	}
	auto initial_solution = SortingProblemSolution(_depth, std::make_shared<SortingProblemState>(initial_state));
	_state_list.push(std::make_unique<SortingProblemSolution>(initial_solution));
	_best_solution = std::make_unique<SortingProblemSolution>(initial_solution);
	_state_provider = std::make_unique<StateProvider>(StateProvider(initial_state));

}

void SorterSolver::solveBreadthFirst()
{
	if (_state_list.empty())
		//maybe notify, that you have to call setup first
		return;
	while (!_stop)
	{
		_process_queue();
	}
}

SortingProblemSolution SorterSolver::get_solution()
{
	return *_solution;
}

std::vector<SortingProblemState> SorterSolver::get_states()
{
	return _state_provider->get_states();
}

SortingProblemSolution SorterSolver::get_best_solution()
{
	return *_best_solution;
}



std::vector<std::shared_ptr<SortingProblemSolution>> SorterSolver::_create_possible_solutions(std::shared_ptr<SortingProblemSolution> base_solutions)
{
	auto r_ =  std::vector<std::shared_ptr<SortingProblemSolution>>();
	auto size = base_solutions->current->size();
	for (uint16_t from = 0; from < size; from++)
	for (uint16_t to   = 0; to   < size;   to++)
	{
		if (from == to)
			continue;
		changePair change = { to,from };
		auto new_state = _state_provider->get_new_state(base_solutions->current, change);
		if (new_state.get() != nullptr) {
			auto new_solution = SortingProblemSolution(*base_solutions,change,new_state);
			r_.push_back(std::make_shared<SortingProblemSolution>(new_solution));
		}
	}
	return r_;
}

void SorterSolver::_process_queue()
{
	if (_state_list.empty()) {
		_broadcast_stop();
		return;
	}
	auto state = std::move(_state_list.front());
	_state_list.pop();
	inspected_solutions++;


	if (state->changes.size() > _best_score) {
		_best_score = state->changes.size();
		_best_solution = state;
	}

	//check if state is fully homogenous
	auto finisher = state->current->is_homogen();
	if (finisher) {
		solved = true;
		_solution = state;
		_broadcast_stop();
		return;
	}

	//check if solution is in decay
	auto decayed = state->is_decaying();
	if (decayed) {
		decayed_solutions++;
		return;
	}

	// generate possible followups
	auto new_solutions = _create_possible_solutions(state);
	if (new_solutions.empty()) {
		// dead solution
		dead_solutions++;
		return;
	}
	for (auto& I : new_solutions) {
		q_additions++;
		_state_list.push(I);
	}
}

void SorterSolver::_broadcast_stop()
{
	_stop = true;
}
