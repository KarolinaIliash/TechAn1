#pragma once

#include <vector>
#include <algorithm>

size_t findMaxIndex(std::vector<double> const& data, size_t from, size_t to)
{
	return std::distance(data.begin(), std::max_element(data.begin() + from, data.begin() + to));

	//try this, i actually don't remember but 
	// return std::distance(std::max_element(data.begin() + from, data.begin() + to), data.begin() + to);
}

size_t findMinIndex(std::vector<double> const& data, size_t from, size_t to)
{
	return std::distance(data.begin(), std::min_element(data.begin() + from, data.begin() + to));

	//try this, i actually don't remember but 
	// return std::distance(std::min_element(data.begin() + from, data.begin() + to), data.begin() + to);
}

template<typename T>
T findMax(std::vector<T> const& data, size_t from, size_t to)
{
	return *std::max_element(data.begin() + from, data.begin() + to);
}


template<typename T>
T findMin(std::vector<T> const& data, size_t from, size_t to)
{
	return *std::min_element(data.begin() + from, data.begin() + to);
}