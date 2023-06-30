#pragma once
#include <vector>
#include <memory>
#include "Glass.h"

typedef std::pair<uint16_t, uint16_t> changePair;


class SortingProblemState
{
public:
	SortingProblemState() = default;
	SortingProblemState(int glass_count) {
		glasses = std::vector<Glass>(glass_count);
	}
	std::vector<Glass> glasses;
	bool operator==(const SortingProblemState& other) {
		return glasses == other.glasses;
	}
};

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

