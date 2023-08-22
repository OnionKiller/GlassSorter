#pragma once
#include <vector>
#include <memory>
#include <ostream>
#include <unordered_set>
#include "Glass.h"
#include "SortingProblemState.h"


class SortingProblemSolution
{
public:
	SortingProblemSolution() = delete;
	SortingProblemSolution(uint16_t max_depth,std::shared_ptr<SortingProblemState> initial) {
		current = initial;
		changes = std::vector<changePair>();
		_max_depth = max_depth;
	}
	SortingProblemSolution(const SortingProblemSolution& base, changePair change, std::shared_ptr<SortingProblemState> new_state) {
		current = new_state;
		changes = base.changes;
		changes.push_back(change);
		_max_depth = base._max_depth;
	}
	bool is_decaying() {
		return changes.size() >= _max_depth;
	}
	std::vector<changePair> changes;
	std::shared_ptr<SortingProblemState> current;
protected:
	uint16_t _max_depth = -1;
};

