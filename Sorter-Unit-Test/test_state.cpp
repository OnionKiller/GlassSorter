#pragma once
#include "pch.h"

#include "../SortingProblemState.h"
#include <vector>

class TestSPS : public SortingProblemState {
public:
	std::vector<Glass> glasses() {
		return _glasses;
	}
};

TEST(TestState, init) {
	std::vector<Glass> init = {
	Glass(1,2,3,4),
	Glass(3,5,6,7),
	Glass(8,9,4),
	Glass(1,2,3),
	Glass()
	};

	auto state = SortingProblemState(init);
	auto tester = static_cast<TestSPS*>(&state);
	uint64_t hash = 1;
	for (auto& I : init) {
		hash *= I.hash() + 1;
	}
	EXPECT_EQ(tester->glasses(), init);
	EXPECT_EQ(state.hash(), hash);
}


TEST(TestState, hash) {
	std::vector<Glass> init = {
	Glass(1,2,3,4),
	Glass()
	};
	std::vector<Glass> sh_init = {
	Glass(),
	Glass(1,2,3,4),
	};
	std::vector<Glass> dh_init = {
	Glass(1,2,3),
	Glass(4)
	};
	auto s1 = SortingProblemState(init);
	auto s2 = SortingProblemState(sh_init);
	auto s3 = SortingProblemState(dh_init);

	uint64_t hash1 = 1;
	for (auto& I : init)
		hash1 *= I.hash() + 1;
	uint64_t hash2 = 1;
	for (auto& I : sh_init)
		hash2 *= I.hash() + 1;
	uint64_t hash3 = 1;
	for (auto& I : dh_init)
		hash3 *= I.hash() + 1;

	EXPECT_EQ(s1, s2);
	EXPECT_FALSE(s2 == s3);

	EXPECT_EQ(s1.hash(), hash1);
	EXPECT_EQ(s2.hash(), hash2);
	EXPECT_EQ(s3.hash(), hash3);

	auto change = s1.create_new_state({ 1,0 });

	EXPECT_EQ(change.hash(), hash3);

}

TEST(TestState, newState) {
	std::vector<Glass> init = {
	Glass(1,2,3,4),
	Glass()
	};
	auto s1 = SortingProblemState(init);
	auto nwst = s1.create_new_state({ 1,0 });
	auto cntrl = SortingProblemState({
		Glass(1,2,3),
		Glass(4)
		});

	const auto immut_cntrl = SortingProblemState(init);

	EXPECT_EQ(s1, immut_cntrl);
	EXPECT_EQ(nwst, cntrl);
	EXPECT_FALSE(&s1 == &nwst);
}

TEST(TestState, size) {
	std::vector<Glass> init = {
		Glass(1,2,3,4),
		Glass()
	};
	std::vector<Glass> sh_init = {
		Glass(),
		Glass(1,2,3,4),
	};

	auto s1 = SortingProblemState(init);
	auto s2 = SortingProblemState({});

	EXPECT_EQ(s1.size(), 2);
	EXPECT_EQ(s2.size(), 0);

	auto nwst = s1.create_new_state({ 1, 0 });


	EXPECT_EQ(s1.size(), 2);
	EXPECT_EQ(nwst.size(), 2);
}


TEST(TestState, equals) {
	std::vector<Glass> init = {
	Glass(1,2,3,4),
	Glass()
	};
	std::vector<Glass> sh_init = {
	Glass(),
	Glass(1,2,3,4),
	};
	std::vector<Glass> dh_init = {
	Glass(1,2,3),
	Glass()
	};
	auto s1 = SortingProblemState(init);
	auto s2 = SortingProblemState(sh_init);
	auto s3 = SortingProblemState(dh_init);
	auto change_state = SortingProblemState({
		Glass(1,2,3),
		Glass(4)
		});

	EXPECT_TRUE(s1 == s2);
	EXPECT_EQ(s1, s2);
	EXPECT_FALSE(s1 == s3);
	EXPECT_FALSE(s2 == s3);

	auto change = s1.create_new_state({ 1,0 });
	EXPECT_EQ(change, change_state);
}
TEST(TestState, AcceptsChange) {
	auto s1 = SortingProblemState({
		Glass(1,2,3,4),
		Glass()
		});
	auto s2 = SortingProblemState({
		Glass(1,2,3),
		Glass(4)
		});
	auto s3 = SortingProblemState({
		Glass(2,2),
		Glass(2,2)
		});
	EXPECT_FALSE(s2.accepts_change(0, 1));
	EXPECT_FALSE(s2.accepts_change(1, 0));
	EXPECT_TRUE(s1.accepts_change(1, 0));
	EXPECT_FALSE(s1.accepts_change(0, 1));
	EXPECT_FALSE(s1.accepts_change(3, 5));
	EXPECT_TRUE(s3.accepts_change(1, 0));
	EXPECT_TRUE(s3.accepts_change(0, 1));
}

TEST(TestState, IsHomogen) {
	auto s1 = SortingProblemState({
		Glass(1,1,1,1),
		Glass(2,2,2,2),
		Glass()
		});
	auto s2 = SortingProblemState({
		Glass(1,2,3),
		Glass(4)
		});
	auto s3 = SortingProblemState({
		Glass(),
		Glass(),
		Glass(),
		Glass(),
		Glass(),
		Glass(),
		Glass(),
		Glass(3,3,3,3),
		});

	EXPECT_TRUE(s1.is_homogen());
	EXPECT_FALSE(s2.is_homogen());
	EXPECT_TRUE(s3.is_homogen());
}


TEST(TestMutation, init) {
	EXPECT_NO_THROW(
		auto init = SortingProblemState({
			Glass(1,2),
			Glass()
			});
		std::shared_ptr<SortingProblemState> ptr = std::make_shared<SortingProblemState>(init);
		{
			auto init_test = std::make_unique<TestMutableSortingProblemState>(ptr);
			init_test->mutate_for_test({ 0,1 });
			init_test->reset_test_mutation();
		}
	);
}

TEST(TestMutation, reset_implicit) {
	auto init = SortingProblemState({
			Glass(1,2),
			Glass()
		});
	std::shared_ptr<SortingProblemState> ptr = std::make_shared<SortingProblemState>(init);
	auto init_test = std::make_unique<TestMutableSortingProblemState>(ptr);
	init_test->mutate_for_test({ 1,0 });
	init_test.release();
	auto checkstate = SortingProblemState({
			Glass(1,2),
			Glass()
		});
	EXPECT_EQ(init, checkstate);
}

TEST(TestMutation, reset_explicit) {
	auto init = SortingProblemState({
			Glass(1,2),
			Glass()
		});
	std::shared_ptr<SortingProblemState> ptr = std::make_shared<SortingProblemState>(init);
	auto init_test = std::make_unique<TestMutableSortingProblemState>(ptr);
	init_test->mutate_for_test({ 1,0 });
	init_test->reset_test_mutation();
	auto checkstate = SortingProblemState({
			Glass(1,2),
			Glass()
		});
	EXPECT_EQ(init, checkstate);
}

TEST(TestMutation, mutate) {
	auto init = SortingProblemState({
			Glass(1,2),
			Glass()
		});
	std::shared_ptr<SortingProblemState> ptr = std::make_shared<SortingProblemState>(init);
	auto init_test = std::make_unique<TestMutableSortingProblemState>(ptr);
	init_test->mutate_for_test({ 1,0 });
	auto checkstate = SortingProblemState({
			Glass(1),
			Glass(2)
		});
	EXPECT_EQ(*ptr, checkstate);
	init_test->reset_test_mutation();
}