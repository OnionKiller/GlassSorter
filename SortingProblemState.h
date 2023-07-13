#pragma once
#include <vector>
#include <unordered_set>

#include "Glass.h"

typedef std::pair<uint16_t, uint16_t> changePair;


class SortingProblemState
{
public:
	SortingProblemState(std::vector<Glass> glasses);
	SortingProblemState create_new_state(changePair change);
	virtual ~SortingProblemState() = default;
	bool operator==(const SortingProblemState& other) const;
	friend std::ostream& operator<<(std::ostream& os, const SortingProblemState& obj)
	{
		for (const auto& glass : obj._glasses) {
			os << glass << std::endl;
		}
		return os;
	}
	uint64_t hash() const noexcept { return _hash; }
	size_t size() const noexcept { return _glasses.size(); }
	bool accepts_change(const uint16_t to, const uint16_t from) const;
	bool is_homogen() const noexcept;
protected:
	std::vector<Glass> _glasses;
	void _update_hash();
	uint64_t _hash = 1;

	friend class TestMutableSortingProblemState;
};

//use very carefully
class TestMutableSortingProblemState{
public:
	TestMutableSortingProblemState(std::shared_ptr<SortingProblemState> base) {
		_base = base;
		_mutated = false;
		_glasses_test = std::vector<Glass>(2);
	}
	//use very carefully: change represents in the form of {to,from}
	void mutate_for_test(changePair change) {
		_mutated = true;
		_test_change = change;
		_glasses_test[0] = _base->_glasses[change.first];
		_glasses_test[1] = _base->_glasses[change.second];
		_base->_glasses[change.first].combine_from_fast(_base->_glasses[change.second]);
		_base->_update_hash();
	}

	//use very carefully
	void reset_test_mutation() {
		_mutated = false;
		_base->_glasses[_test_change.first] = _glasses_test[0];
		_base->_glasses[_test_change.second] = _glasses_test[1];
		_base->_update_hash();
	}
	~TestMutableSortingProblemState()
	{
		if (_mutated) {
			reset_test_mutation();
			std::cout << "Implicit reversal of mutation. It is an error on your behalf, fix your code!";
			//throw std::exception("Temporarily mutated immutable object is not reversed!");
		}
	}
private:
	bool  _mutated;
	std::shared_ptr<SortingProblemState> _base;
	std::vector<Glass> _glasses_test;
	changePair _test_change;
};

inline SortingProblemState::SortingProblemState(std::vector<Glass> glasses)
{
	//this should grant minimal size
	_glasses = glasses;
	_update_hash();
}

inline SortingProblemState SortingProblemState::create_new_state(changePair change)
{
	SortingProblemState r_ = SortingProblemState(*this);
	r_._glasses[change.first].combine_from(r_._glasses[change.second]);
	r_._update_hash();
	return r_;
}

inline bool SortingProblemState::operator==(const SortingProblemState& other) const
{
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

inline bool SortingProblemState::accepts_change(const uint16_t to, const uint16_t from) const
{
	if (to >= size() || from >= size())
		return false;
	return _glasses[to].accepts_combine(_glasses[from]);
}

inline bool SortingProblemState::is_homogen() const noexcept
{
	for (const auto& glass : _glasses) {
		if (!glass.is_homogen())
			return false;
	}
	return true;
}

inline void SortingProblemState::_update_hash()
{
	uint64_t hash = 1;
	for (auto& I : _glasses) {
		// this in theory gives a hash independent from the order of the glasses
		hash *= I.hash() + 1;
	}
	_hash = hash;
}


namespace std {
	template<> struct hash<SortingProblemState> {
		std::size_t operator()(const SortingProblemState& state)const noexcept {
			return state.hash();
		}
	};
}
