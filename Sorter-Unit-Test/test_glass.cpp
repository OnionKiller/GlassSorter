
#include "pch.h"
#include "../Sorter-Solver/Glass.h"
#include <vector>


class seeGlass :public Glass {
public:
	using Glass::Glass;
	uint8_t get_top_index() { return _top_index; };
	uint16_t* get_data() { return  _data; };
};


template <typename T, size_t N>
bool arraysEqual(const T(&arr1)[N], const T(&arr2)[N]) {
	return std::equal(arr1, arr1 + N, arr2);
}


//https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string
template <typename Out>
void split(const std::string& s, char delim, Out result) {
	std::istringstream iss(s);
	std::string item;
	while (std::getline(iss, item, delim)) {
		*result++ = item;
	}
}

std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}
TEST(TestGlass, EmptyInit) {
	auto g = Glass();
	EXPECT_TRUE(g.is_empty());
}

TEST(TestGlass, EmptyInit_NotFull) {
	auto g = Glass();
	EXPECT_TRUE(!g.is_full());
}


TEST(TestGlass, Init1) {
	auto g = Glass(1);
	EXPECT_TRUE(!g.is_empty());
	EXPECT_TRUE(!g.is_full());
	ASSERT_EQ(g.top(), 1);
	seeGlass* gg = static_cast<seeGlass*>(&g);
	ASSERT_EQ(gg->get_top_index(), 0);
	ASSERT_EQ(gg->get_data()[0], 1);
}

TEST(TestGlass, Init2) {
	auto g = Glass(1, 2);
	EXPECT_TRUE(!g.is_empty());
	EXPECT_TRUE(!g.is_full());
	ASSERT_EQ(g.top(), 2);
	seeGlass* gg = static_cast<seeGlass*>(&g);
	ASSERT_EQ(gg->get_top_index(), 1);
	ASSERT_EQ(gg->get_data()[0], 1);
	ASSERT_EQ(gg->get_data()[1], 2);
}

TEST(TestGlass, Init3) {
	auto g = Glass(1, 2, 3);
	EXPECT_TRUE(!g.is_empty());
	EXPECT_TRUE(!g.is_full());
	ASSERT_EQ(g.top(), 3);
	seeGlass* gg = static_cast<seeGlass*>(&g);
	ASSERT_EQ(gg->get_top_index(), 2);
	ASSERT_EQ(gg->get_data()[0], 1);
	ASSERT_EQ(gg->get_data()[1], 2);
	ASSERT_EQ(gg->get_data()[2], 3);
}

TEST(TestGlass, CheckFull) {
	auto g = Glass(1, 2, 3, 4);
	EXPECT_TRUE(g.is_full());
}

TEST(TestGlass, Init4) {
	auto g = Glass(7, 6, 5, 3);
	EXPECT_TRUE(!g.is_empty());
	EXPECT_TRUE(g.is_full());
	ASSERT_EQ(g.top(), 3);
	seeGlass* gg = static_cast<seeGlass*>(&g);
	ASSERT_EQ(gg->get_top_index(), 3);
	ASSERT_EQ(gg->get_data()[0], 7);
	ASSERT_EQ(gg->get_data()[1], 6);
	ASSERT_EQ(gg->get_data()[2], 5);
	ASSERT_EQ(gg->get_data()[3], 3);
}

TEST(TestGlass, InitVector) {
	std::vector<Glass> vec{
		Glass(4, 3, 2, 1),
		Glass(7, 6, 5, 3)
	};
	auto g = vec[0];
	EXPECT_TRUE(g.is_full());
	seeGlass* gg = static_cast<seeGlass*>(&g);
	ASSERT_EQ(gg->get_top_index(), 3);
	ASSERT_EQ(gg->get_data()[0], 4);
	ASSERT_EQ(gg->get_data()[1], 3);
	ASSERT_EQ(gg->get_data()[2], 2);
	ASSERT_EQ(gg->get_data()[3], 1);
	g = vec[1];
	EXPECT_TRUE(g.is_full());
	gg = static_cast<seeGlass*>(&g);
	ASSERT_EQ(gg->get_top_index(), 3);
	ASSERT_EQ(gg->get_data()[0], 7);
	ASSERT_EQ(gg->get_data()[1], 6);
	ASSERT_EQ(gg->get_data()[2], 5);
	ASSERT_EQ(gg->get_data()[3], 3);
}

TEST(TestGlass, VariableSizeInit) {
	for (auto i = 1; i <= glass_size; i++) {
		std::vector<uint16_t> data(i);
		for (auto& I : data)
			I = i;
		auto g = Glass(data.data(), i);
		EXPECT_TRUE(!g.is_empty());
		ASSERT_EQ(g.top(), i);
		auto gg = static_cast<seeGlass*>(&g);
		EXPECT_TRUE(
			std::equal(gg->get_data(), gg->get_data() + i, data.data())
		);
		if (i == glass_size)
			EXPECT_TRUE(g.is_full());
	}
}

TEST(TestGlass, VariableSizeInitEmpty) {
	ASSERT_NO_FATAL_FAILURE(
		uint16_t * data = NULL;
	auto g = Glass(data, 0);
	EXPECT_TRUE(g.is_empty());
	EXPECT_TRUE(!g.is_full());
	);
}

TEST(TestGlass, VariableSizeInitOversize) {
	ASSERT_NO_FATAL_FAILURE(
		auto i = 6;
	std::vector<uint16_t> data(i);
	for (auto& I : data)
		I = i;
	auto g = Glass(data.data(), i);
	EXPECT_TRUE(!g.is_empty());
	ASSERT_EQ(g.top(), i);
	auto gg = static_cast<seeGlass*>(&g);
	EXPECT_TRUE(
		std::equal(gg->get_data(), gg->get_data() + glass_size, data.data())
	);
	EXPECT_TRUE(g.is_full());
	);
}

TEST(TestGlass, AcceptsCombineEmptyFull) {
	const uint16_t full_data[] = { 1,2,3,4 };
	const uint16_t same_data[] = { 1,2 };
	const uint16_t diff_data[] = { 1,2,3 };

	Glass emptyGlass = Glass();
	Glass fullGlass = Glass((uint16_t*)full_data, 4);
	Glass otherEmptyGlass = Glass();
	Glass sameTopGlass = Glass((uint16_t*)same_data, 2);
	Glass differentTopGlass = Glass((uint16_t*)diff_data, 3);

	// Test when the current glass is empty
	EXPECT_TRUE(emptyGlass.accepts_combine(fullGlass));
	EXPECT_FALSE(emptyGlass.accepts_combine(otherEmptyGlass));
	EXPECT_TRUE(emptyGlass.accepts_combine(sameTopGlass));
	EXPECT_TRUE(emptyGlass.accepts_combine(differentTopGlass));

	// Test when the current glass is full
	EXPECT_FALSE(fullGlass.accepts_combine(emptyGlass));
	EXPECT_FALSE(fullGlass.accepts_combine(otherEmptyGlass));
	EXPECT_FALSE(fullGlass.accepts_combine(sameTopGlass));
	EXPECT_FALSE(fullGlass.accepts_combine(differentTopGlass));

}


TEST(TestGlass, AcceptsCombineComplex) {
	const uint16_t full_data[] = { 1,2,3,4 };
	const uint16_t same_data[] = { 1,2 };
	const uint16_t diff_data[] = { 1,2,3 };

	Glass emptyGlass = Glass();
	Glass fullGlass = Glass((uint16_t*)full_data, 4);
	Glass otherEmptyGlass = Glass();
	Glass sameTopGlass = Glass((uint16_t*)same_data, 2);
	Glass differentTopGlass = Glass((uint16_t*)diff_data, 3);

	// Test when the current glass is not empty or full
	EXPECT_FALSE(sameTopGlass.accepts_combine(emptyGlass));
	EXPECT_FALSE(sameTopGlass.accepts_combine(fullGlass));
	EXPECT_FALSE(sameTopGlass.accepts_combine(otherEmptyGlass));
	EXPECT_TRUE(sameTopGlass.accepts_combine(sameTopGlass));
	EXPECT_FALSE(sameTopGlass.accepts_combine(differentTopGlass));

	// Test when the current glass is not empty or full, and the other glass is empty
	EXPECT_FALSE(differentTopGlass.accepts_combine(emptyGlass));
	EXPECT_FALSE(differentTopGlass.accepts_combine(fullGlass));
	EXPECT_FALSE(differentTopGlass.accepts_combine(otherEmptyGlass));
	EXPECT_FALSE(differentTopGlass.accepts_combine(sameTopGlass));
}


TEST(TestGlass, CombineTestsEmpyOut) {
	const uint16_t test_case_1[] = { 1,2, };
	const uint16_t test_case_2[] = { 2,2, };
	const uint16_t result[] = { 1,2,2,2, };

	auto glassEmpty = Glass();
	auto glassRecive = Glass((uint16_t*)test_case_1, 2);
	auto glassPour = Glass((uint16_t*)test_case_2, 2);

	glassEmpty.combine_from_fast(glassPour);

	EXPECT_TRUE(glassPour.is_empty());
	auto gg = static_cast<seeGlass*>(&glassPour);
	ASSERT_TRUE(
		std::equal(gg->get_data(), gg->get_data() + 2, test_case_2)
	);
	gg = static_cast<seeGlass*>(&glassEmpty);
	ASSERT_EQ(gg->get_top_index(), 1);
	glassPour.combine_from_fast(glassEmpty);

	glassRecive.combine_from_fast(glassPour);

	EXPECT_TRUE(glassPour.is_empty());
	EXPECT_TRUE(glassRecive.is_full());

	gg = static_cast<seeGlass*>(&glassRecive);

	ASSERT_EQ(gg->get_top_index(), 3);
	EXPECT_TRUE(
		std::equal(gg->get_data(), gg->get_data() + 4, result)
	);
}

TEST(TestGlass, CombineTestInterrupted) {
	const uint16_t test_case_1[] = { 1 };
	const uint16_t test_case_2[] = { 2,2,1 };
	const uint16_t recive_result[] = { 1,1 };
	const uint16_t pour_result[] = { 2,2 };

	auto glassRecive = Glass((uint16_t*)test_case_1, 1);
	auto glassPour = Glass((uint16_t*)test_case_2, 3);

	glassRecive.combine_from_fast(glassPour);

	EXPECT_TRUE(!glassPour.is_empty());
	EXPECT_TRUE(!glassRecive.is_empty());

	auto gg = static_cast<seeGlass*>(&glassRecive);

	ASSERT_EQ(gg->get_top_index(), 1);
	EXPECT_TRUE(
		std::equal(gg->get_data(), gg->get_data() + 2, recive_result)
	);

	gg = static_cast<seeGlass*>(&glassPour);

	ASSERT_EQ(gg->get_top_index(), 1);
	EXPECT_TRUE(
		std::equal(gg->get_data(), gg->get_data() + 2, pour_result)
	);
}

TEST(TestGlass, allCombineTest) {
	std::vector<Glass> glist = {
		Glass(1,2,2,4),
		Glass(1,1,4,4),
		Glass(4,2,2,1),
		Glass(),
		Glass(),
	};
	glist[3].combine_from(glist[2]);
	EXPECT_EQ(glist[3], Glass(1));
	EXPECT_EQ(glist[2], Glass(4,2,2));
	glist[4].combine_from(glist[2]);
	EXPECT_EQ(glist[4], Glass(2,2));
	EXPECT_EQ(glist[2], Glass(4));
	glist[2].combine_from(glist[0]);
	EXPECT_EQ(glist[2], Glass(4, 4));
	EXPECT_EQ(glist[0], Glass(1,2,2));
	glist[2].combine_from(glist[1]);
	EXPECT_EQ(glist[2], Glass(4, 4, 4, 4));
	EXPECT_EQ(glist[1], Glass(1,1));
	glist[4].combine_from(glist[0]);
	EXPECT_EQ(glist[4], Glass(2, 2, 2, 2));
	EXPECT_EQ(glist[0], Glass(1));
	glist[3].combine_from(glist[0]);
	EXPECT_EQ(glist[3], Glass(1, 1));
	EXPECT_EQ(glist[0], Glass());
	glist[1].combine_from(glist[3]);
	EXPECT_EQ(glist[1], Glass(1, 1, 1, 1));
	EXPECT_EQ(glist[3], Glass());
}

TEST(TestGlass, TestHomogenous) {
	const uint16_t test_case_1[] = { 2,2,2,2 };
	const uint16_t test_case_2[] = { 2,2,1 };
	const uint16_t test_case_3[] = { 2,2,1,2 };

	auto g1 = Glass((uint16_t*)test_case_1, 4);
	auto g2 = Glass((uint16_t*)test_case_2, 3);
	auto g3 = Glass((uint16_t*)test_case_3, 4);
	auto g4 = Glass();

	EXPECT_TRUE(g1.is_homogen());
	EXPECT_FALSE(g2.is_homogen());
	EXPECT_FALSE(g3.is_homogen());
	EXPECT_TRUE(g4.is_homogen());
}

TEST(TestGlass, TestCout) {

	auto emptyGlass = Glass();
	auto halfGlass = Glass(1, 1);
	auto fullGlass = Glass(1, 1, 1, 1);


	std::stringstream buffer;
	std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

	std::cout << emptyGlass << std::endl;
	std::cout << halfGlass << std::endl;
	std::cout << fullGlass;

	std::string resultstring = buffer.str();
	auto results = split(resultstring, '\n');
	ASSERT_EQ(results[0], "{_,_,_,_}");
	ASSERT_EQ(results[1], "{1,1,_,_}");
	ASSERT_EQ(results[2], "{1,1,1,1}");
}

TEST(TestGlass, TestEqual) {
	auto emptyGlass = Glass();
	auto otherGlass = Glass();
	auto halfGlass = Glass(1, 1);
	auto otherHalsfGlass = Glass(1, 1);
	auto fullGlass = Glass(1, 1, 1, 1);
	auto otherfullGlass = Glass(1, 1, 1, 2);

	EXPECT_TRUE(emptyGlass == otherGlass);
	EXPECT_FALSE(emptyGlass == halfGlass);
	EXPECT_FALSE(emptyGlass == otherHalsfGlass);

	//check different length
	EXPECT_FALSE(emptyGlass == fullGlass);

	//check different length
	EXPECT_FALSE(halfGlass == fullGlass);
	//check same length true
	EXPECT_TRUE(halfGlass == otherHalsfGlass);
	//check same length different
	EXPECT_FALSE(fullGlass == otherfullGlass);

}

TEST(TestGlass, TestEqualVecotrs) {
	std::vector<Glass> first = {
		Glass(),
		Glass(1,1),
		Glass(1,2,3)
	};
	std::vector<Glass> second = {
		Glass(),
		Glass(1,1),
		Glass(1,2,3)
	};
	std::vector<Glass> third = {
		Glass(),
		Glass(1,1),
		Glass(1,2,4)
	};

	EXPECT_TRUE(first == second);
	EXPECT_FALSE(second == third);

}

TEST(TestGlass, TestHash) {
	auto emptyGlass = Glass();
	auto fullGlass = Glass(1, 2, 3, 4);
	auto x1Glass = Glass(1);
	auto x2Glass = Glass(1, 2);
	auto x3Glass = Glass(1, 2, 2);


	auto hash0 = emptyGlass.hash();
	auto hash1 = fullGlass.hash();
	auto hash11 = x1Glass.hash();
	auto hash12 = x2Glass.hash();
	auto hash13 = x3Glass.hash();

	emptyGlass.combine_from_fast(fullGlass);

	auto hash2 = emptyGlass.hash();
	
	x3Glass.combine_from_fast(x2Glass);
	auto hash3 = x3Glass.hash();
	auto hash4 = x2Glass.hash();


	EXPECT_EQ(hash0, 0);
	EXPECT_EQ(hash1, 0x0001000200030004);
	EXPECT_EQ(hash11, 0x0000000000000001);
	EXPECT_EQ(hash12, 0x0000000000010002);
	EXPECT_EQ(hash13, 0x0000000100020002);

	EXPECT_FALSE(hash2 == hash0);
	EXPECT_EQ(hash2, 0x0000000000000004);
	EXPECT_EQ(hash3, 0x0001000200020002);
	EXPECT_EQ(hash4, 0x0000000000000001);

}