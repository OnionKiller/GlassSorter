#pragma once
#include "pch.h"

#include "../Sorter-Solver/StateProvider.h"
#include <vector>

class TestStateProv : public StateProvider {
public:
	std::unordered_set<SortingProblemState> get_states_set() {
		return _reached_states;
	}
};


class TestSPS : public SortingProblemState {
public:
	std::vector<Glass> glasses() {
		return _glasses;
	}
};


TEST(TestProvider, init) {
	SortingProblemState initial(
		{
			Glass(1,2),
			Glass(1,1),
			Glass()
		}
	);
	EXPECT_NO_FATAL_FAILURE(
		auto provider = StateProvider(initial);
	);
}

TEST(TestProvider, get_new_state_runs) {
	SortingProblemState initial(
		{
			Glass(1),
			Glass(1,1),
			Glass()
		});
	// test new state for running
	EXPECT_NO_THROW(
			auto ptr = std::make_shared<SortingProblemState>(initial);
			auto provider = StateProvider(initial);
			auto nxt = provider.get_new_state(ptr, { 0,1 });
	);
	// test non new state for running
	EXPECT_NO_THROW(
			auto ptr = std::make_shared<SortingProblemState>(initial);
	auto provider = StateProvider(initial);
	auto nxt = provider.get_new_state(ptr, { 0,2 });
	);
}

TEST(TestProvider, get_new_unseen_state) {
	SortingProblemState initial(
		{
			Glass(2,1),
			Glass(1,1),
			Glass()
		}
	);
	auto res1 = std::vector<Glass>{
		Glass(2,1,1,1),
		Glass(),
		Glass()
	};
	auto res2 = std::vector<Glass>{
		Glass(2),
		Glass(1,1,1),
		Glass()
	};

	auto ptr = std::make_shared<SortingProblemState>(initial);
	auto provider = StateProvider(initial);

	auto nxt = provider.get_new_state(ptr, { 0,1 });
	EXPECT_FALSE(nxt.get() == nullptr);
	auto state = std::static_pointer_cast<TestSPS>( nxt);
	EXPECT_EQ(state->glasses(),res1);

	nxt = provider.get_new_state(ptr, { 1,0 });
	EXPECT_FALSE(nxt.get() == nullptr);
	state = std::static_pointer_cast<TestSPS>(nxt);
	EXPECT_EQ(state->glasses(), res2);
	

	//check if internal state is correct
	auto provider_ptr = std::make_shared<StateProvider>(provider);
	auto provider_inspector_ptr = std::static_pointer_cast<TestStateProv>(provider_ptr);
	auto internal_set = provider_inspector_ptr->get_states_set();

	EXPECT_EQ(internal_set.size() ,3);
	EXPECT_TRUE(internal_set.find(initial) != internal_set.end());
	EXPECT_TRUE(internal_set.find(res1) != internal_set.end());
	EXPECT_TRUE(internal_set.find(res2) != internal_set.end());

}

TEST(TestProvider, get_new_redundant_state) {
	SortingProblemState initial(
		{
			Glass(2,1),
			Glass(1,1),
			Glass()
		}
	);
	auto res1 = std::vector<Glass>{
		Glass(2,1,1,1),
		Glass(),
		Glass()
	};
	auto res2 = std::vector<Glass>{
		Glass(2),
		Glass(1,1,1),
		Glass()
	};

	auto ptr = std::make_shared<SortingProblemState>(initial);
	auto provider = StateProvider(initial);
	auto nxt = provider.get_new_state(ptr, { 0,1 });
	nxt = provider.get_new_state(ptr, { 1,0 });
	nxt = provider.get_new_state(ptr, { 1,2 });
	EXPECT_TRUE(nxt.get() == nullptr);
	nxt = provider.get_new_state(ptr, { 2,1 });
	EXPECT_TRUE(nxt.get() == nullptr);
	nxt = provider.get_new_state(ptr, { 0,1 });
	EXPECT_TRUE(nxt.get() == nullptr);
	nxt = provider.get_new_state(ptr, { 1,0 });
	EXPECT_TRUE(nxt.get() == nullptr);

	//check if internal state is correct
	auto provider_ptr = std::make_shared<StateProvider>(provider);
	auto provider_inspector_ptr = std::static_pointer_cast<TestStateProv>(provider_ptr);
	auto internal_set = provider_inspector_ptr->get_states_set();

	EXPECT_EQ(internal_set.size(), 3);
	EXPECT_TRUE(internal_set.find(initial) != internal_set.end());
	EXPECT_TRUE(internal_set.find(res1) != internal_set.end());
	EXPECT_TRUE(internal_set.find(res2) != internal_set.end());

}

TEST(TestProvider, get_states) {
	SortingProblemState initial(
		{
			Glass(1,2),
			Glass(1,1,2),
			Glass()
		}
	);

	auto ptr = std::make_shared<SortingProblemState>(initial);
	auto provider = StateProvider(initial);
	auto nxt = provider.get_new_state(ptr, { 0,1 });
	auto nxt2 = provider.get_new_state(ptr, { 0,2 });
	provider.get_new_state(nxt, { 0, 2 });

	auto states = provider.get_states();

	auto expected_states = std::vector<SortingProblemState>{
		initial,
		SortingProblemState(
			{
				Glass(1,2,2),
				Glass(1,1),
				Glass()
			})
	};

	EXPECT_EQ(states.size(), expected_states.size());
	EXPECT_EQ(states, expected_states);
}

