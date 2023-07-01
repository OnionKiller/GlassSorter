#pragma once
#include <iostream>
#include <cstdint>

constexpr int glass_size = 4;

class Glass
{
public:
	Glass();
	//filling from bottom up, this api should not exists
	Glass(uint16_t i0, uint16_t i1 = -1, uint16_t i2 = -1, uint16_t i3 = -1);
	Glass(uint16_t* data, uint8_t length);
	bool is_empty() const;
	bool is_full() const;
	bool is_homogen() const;
	bool accepts_combine(const Glass& other) const;
	bool combine_from(Glass& other);
	void combine_from_fast(Glass& other);
	const uint16_t top() const;
	const uint64_t hash() const;
	friend std::ostream& operator<<(std::ostream& os, const Glass& obj);
	bool operator==(const Glass& other) const;
protected:
	void _update_hash();
	// assumes there are only 2^16 different colours
	uint16_t _data[glass_size];
	uint8_t _top_index;
	uint64_t _hash = 0;

};

inline Glass::Glass()
{
	_top_index = -1;
	memset(_data, 0, glass_size);
}

inline Glass::Glass(uint16_t i0, uint16_t i1, uint16_t i2, uint16_t i3)
{
	_data[0] = i0;
	if (i1 == uint16_t(-1)) {
		_top_index = 0;
		return;
	}
	_data[1] = i1;
	if (i2 == uint16_t(-1)) {
		_top_index = 1;
		return;
	}
	_data[2] = i2;
	if (i3 == uint16_t(-1)) {
		_top_index = 2;
		return;
	}
	_data[3] = i3;
	_top_index = 3;
	_update_hash();
}

inline Glass::Glass(uint16_t* data, uint8_t length)
{
	if (length > glass_size)
		length = glass_size;
	if (length == 0) {
		Glass();
		return;
	}
	memcpy_s(_data, glass_size * sizeof(uint16_t), data, length * sizeof(uint16_t));
	_top_index = length - 1;
	_update_hash();
}

inline bool Glass::is_empty() const {
	return _top_index > glass_size;
}

inline bool Glass::is_full() const {
	return _top_index == glass_size - 1;
}

inline bool Glass::is_homogen() const {
	if (is_empty())
		return true;
	if (!is_full())
		return false;
	auto example = _data[0];
	for (auto i = 1; i < glass_size; i++) {
		if (_data[i] != example) 
			return false;
	}
	return true;
}

inline const uint16_t Glass::top() const {
	if (is_empty())
		// overflow on empty by design, to cause as low harm as possible
		// maybe thow an exception
		return -1;
	return _data[_top_index];
}

inline const uint64_t Glass::hash() const
{
	return _hash;
}

inline bool Glass::accepts_combine(const Glass& other) const {
	if (is_empty())
		return !other.is_empty();
	if (is_full()) //it's full
		return false;
	if (other.is_empty())
		return false;
	if (other.top() == top())
		return true;
	return false;
}

inline bool Glass::combine_from(Glass& other)
{
	if (!accepts_combine(other))
		return false;
	combine_from_fast(other);
	return true;
}

inline void Glass::combine_from_fast(Glass& other)
{
	if (is_empty()) {
		_top_index++;
		_data[_top_index] = other._data[other._top_index];
		other._top_index--;
	}
	for (int i = 0; i < glass_size; i++) {
		if (is_full())
			break;
		if (other.is_empty())
			return;
		if (this->_data[_top_index] != other._data[other._top_index])
			break;
		//in theory this handles the emptyness with overflow, altough it is unclear if this is a good idea
		this->_data[_top_index + 1] = other._data[other._top_index];
		other._top_index--;
		this->_top_index++;
	}
	_update_hash();
}

inline 	std::ostream& operator<<(std::ostream& os, const Glass& obj) {
	os << '{';
	for (auto i = 0; i < glass_size-1; i++) {
		if (i <= obj._top_index && !obj.is_empty())
			os << obj._data[i] << ',';
		else
			os << "_,";
	}
	if (obj.is_full())
		os << obj._data[obj._top_index] << '}';
	else
		os << "_}";
	return os;
}

inline bool Glass::operator==(const Glass& other) const
{
	return _hash == other._hash;
}

inline void Glass::_update_hash()
{
	_hash = 0;
	if (is_empty())
		return;
	for (auto i = 0; i <= _top_index; i++) {
		//hope for little endian
		_hash = _hash << 16;
		_hash += _data[i];
	}
}


namespace std {
	template<> struct hash<Glass> {
		std::size_t operator()(const Glass& state)const noexcept {
			return state.hash();
		}
	};
}