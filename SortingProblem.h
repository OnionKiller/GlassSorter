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
	SortingProblemSolution(uint16_t max_depth,SortingProblemState initial) {
		current = initial;
		changes = std::vector<changePair>();
		_max_depth = max_depth;
	}
	bool is_decaying() {
		return changes.size() >= _max_depth;
	}
	bool apply_change(changePair change) {
		changes.push_back(change);
		auto& to = current.glasses[change.first];
		auto& from = current.glasses[change.second];
		return to.combine_from(from);
	}
	void apply_change_fast(changePair change) {
		changes.push_back(change);
		auto& to = current.glasses[change.first];
		auto& from = current.glasses[change.second];
		to.combine_from_fast(from);
	}
	std::vector<changePair> changes;
	SortingProblemState current;
protected:
	uint16_t _max_depth = -1;
};

