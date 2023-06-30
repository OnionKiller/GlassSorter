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
	auto initial_solution = SortingProblemSolution(_depth, initial_state);
	_state_list.push(std::move(initial_solution));

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

std::vector<changePair> SorterSolver::_create_possible_changes(SortingProblemState base_state)
{
	auto r_ =  std::vector<changePair>();

	auto set_size = base_state.glasses.size();

	for (auto i_to = 0; i_to < set_size;i_to++) 
	for (auto i_from = 0; i_from < set_size; i_from++) {
		if (i_to == i_from)
			continue;
		const auto& gto = base_state.glasses[i_to];
		const auto& gfrom = base_state.glasses[i_from];

		if (gto.accepts_combine(gfrom))
			r_.push_back({ i_to,i_from });
	}

	return r_;
}

void SorterSolver::_remove_oscillations(std::vector<changePair>& change_list, SortingProblemSolution& solution)
{
	if (solution.changes.empty())
		return;
	std::pair<uint16_t, uint16_t> inverse_change = { solution.changes.back().second,solution.changes.back().first };
	//check if it's a simple oscillation
	change_list.erase(
		std::remove_if(change_list.begin(), change_list.end(), [&](const changePair& change) {
			return change == inverse_change;
			}), change_list.end()
	);
}

void SorterSolver::_apply_changes(std::vector<changePair> changes, SortingProblemSolution & base_solution)
{
	for (auto change : changes) {
		//copy state
		SortingProblemSolution new_state = base_solution;
		new_state.apply_change_fast(change);
		_state_list.push(std::move(new_state));
		q_additions++;
	}
}

void SorterSolver::_process_queue()
{
	if (_state_list.empty()) {
		_broadcast_stop();
		return;
	}
	auto state = _state_list.back();
	_state_list.pop();
	inspected_solutions++;
	//check if state is fully homogenous
	auto finisher = _check_fully_homogen(state.current);
	if (finisher) {
		solved = true;
		_solution = std::make_unique<SortingProblemSolution>(state);
		_broadcast_stop();
		return;
	}

	auto decayed = state.is_decaying();
	if (decayed) {
		decayed_solutions++;
		return;
	}

	// generate possible followups
	auto changes = _create_possible_changes(state.current);
	_remove_oscillations(changes, state);
	if (changes.empty()) {
		// dead solution
		dead_solutions++;
		return;
	}
	_apply_changes(changes, state);
}

bool SorterSolver::_check_fully_homogen(SortingProblemState state)
{
	for (const auto& glass : state.glasses) {
		if (!glass.is_homogen())
			return false;
	}
	return true;
}

void SorterSolver::_broadcast_stop()
{
	_stop = true;
}
