#pragma once
#include <vector>
#include <unordered_set>

#include "Glass.h"

typedef std::pair<uint16_t, uint16_t> changePair;


class SortingProblemState
{
public:
	SortingProblemState(std::vector<Glass> glasses)
	{
		//this should grant minimal size
		_glasses = glasses;
	}
	SortingProblemState create_new_state(changePair change){
		auto r_ = SortingProblemState(*this);
		r_._glasses[change.first].combine_from(r_._glasses[change.second]);
		return r_;
	}
	bool operator==(const SortingProblemState& other) const {
		if (hash() != other.hash())
			return false;
		std::unordered_set<Glass> t, o;
		for (auto& I : _glasses)
			t.insert(I);
		for (auto& I : other._glasses)
			o.insert(I);
		return t == o;
		//return glasses == other.glasses;
	}
	friend std::ostream& operator<<(std::ostream& os, const SortingProblemState& obj) {
		for (const auto& glass : obj._glasses) {
			os << glass << std::endl;
		}
		return os;
	}

	uint64_t hash() const noexcept {
		return _hash;
	}
	uint16_t size() {
		return _glasses.size();
	}
	bool accepts_change(uint16_t to, uint16_t from) {
		if (to >= size() || from >= size())
			return false;
		return _glasses[to].accepts_combine(_glasses[from]);
	}

	bool is_homogen() {
		for (const auto& glass : _glasses) {
			if (!glass.is_homogen())
				return false;
		}
		return true;
	}
protected:
	std::vector<Glass> _glasses;
	void _update_hash() {
		uint64_t hash = 1;
		for (auto& I : _glasses) {
			// this in theory gives a hash independent from the order of the glasses
			hash *= I.hash() + 1;
		}
		_hash = hash;
	}
	uint16_t _hash = 1;
};

//use very carefully
class TestMutableSortingProblemState : public SortingProblemState {
public:
	TestMutableSortingProblemState (std::vector<Glass> init) :SortingProblemState(init){
		_glasses_test = std::vector<Glass>(2);
	}
	//use very carefully
	void mutate_for_test(changePair change) {
		_test_change = change;
		_glasses_test[0] = _glasses[change.first];
		_glasses_test[1] = _glasses[change.second];
		_glasses[change.first].combine_from_fast(_glasses[change.second]);
	}

	//use very carefully
	void reset_test_mutation() {
		_glasses[_test_change.first] = _glasses_test[0];
		_glasses[_test_change.second] = _glasses_test[1];
	}
private:
	std::vector<Glass> _glasses_test;
	changePair _test_change;
};


namespace std {
	template<> struct hash<SortingProblemState> {
		std::size_t operator()(const SortingProblemState& state)const noexcept {
			return state.hash();
		}
	};
}
