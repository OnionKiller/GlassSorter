#pragma once
#include <vector>
#include "Glass.h"
#include <unordered_set>

typedef std::pair<uint16_t, uint16_t> changePair;


class SortingProblemState
{
public:
	std::vector<Glass> glasses;
	bool operator==(const SortingProblemState& other) const {
		if (hash() != other.hash())
			return false;
		std::unordered_set<Glass> t, o;
		for (auto& I : glasses)
			t.insert(I);
		for (auto& I : other.glasses)
			o.insert(I);
		return t == o;
		//return glasses == other.glasses;
	}
	friend std::ostream& operator<<(std::ostream& os, const SortingProblemState& obj) {
		for (const auto& glass : obj.glasses) {
			os << glass << std::endl;
		}
		return os;
	}

	uint64_t hash() const noexcept {
		uint64_t hash = 1;
		for (auto& I : glasses) {
			// this in theory gives a hash independent from the order of the glasses
			hash *= I.hash() + 1;
		}
		return hash;
	}
};

namespace std {
	template<> struct hash<SortingProblemState> {
		std::size_t operator()(const SortingProblemState& state)const noexcept {
			return state.hash();
		}
	};
}