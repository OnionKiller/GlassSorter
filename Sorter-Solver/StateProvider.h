#pragma once
#include <memory>
#include <unordered_set>
#include "SortingProblemState.h"

class StateProvider
{
public:
	StateProvider(SortingProblemState& initial);
	// return nullptr if not a new unique
	std::shared_ptr<SortingProblemState> get_new_state(std::shared_ptr<SortingProblemState> init_state, changePair change);
	std::vector<SortingProblemState> get_states();
protected:
	std::unordered_set<SortingProblemState> _reached_states;
};

class SafeStateProvider : public StateProvider
{
public:
	SafeStateProvider(SortingProblemState& initial) :StateProvider(initial) {};
	size_t caught_unsafe_combines = 0;
	// return nullptr if not a new unique
	std::shared_ptr<SortingProblemState> get_new_state(std::shared_ptr<SortingProblemState> init_state, changePair change);
};

inline StateProvider::StateProvider(SortingProblemState& initial)
{
	_reached_states.insert(std::move(initial));
}


inline std::shared_ptr<SortingProblemState> StateProvider::get_new_state(std::shared_ptr<SortingProblemState> init_state, changePair change)
{
	auto init_test = std::make_unique<TestMutableSortingProblemState>(init_state);
	init_test->mutate_for_test(change);
	if (_reached_states.find(*init_state) == _reached_states.end())
	{
		// it isn't redundant
		init_test->reset_test_mutation();
		auto new_state =  init_state->create_new_state(change);
		auto inserted = _reached_states.insert(std::move(new_state));
		return std::make_shared<SortingProblemState>(*inserted.first);
	}
	//it's redundant
	init_test->reset_test_mutation();
	return std::shared_ptr<SortingProblemState>();
}

inline std::vector<SortingProblemState> StateProvider::get_states()
{
	auto r_ = std::vector<SortingProblemState>();
	for (auto& I : _reached_states) {
		r_.push_back(I);
	}
		return r_;
}

inline std::shared_ptr<SortingProblemState> SafeStateProvider::get_new_state(std::shared_ptr<SortingProblemState> init_state, changePair change)
{
	if (!init_state->accepts_change(change.first, change.second)) {
		caught_unsafe_combines++;
		return nullptr;
	}
	return StateProvider::get_new_state(init_state, change);
}