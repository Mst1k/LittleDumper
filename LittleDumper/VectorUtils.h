#pragma once

#include <vector>
#include <algorithm>

template<typename T>
void RemoveDuplicates(std::vector<T>& arr)
{
	std::sort(arr.begin(), arr.end());
	arr.erase(std::unique(arr.begin(), arr.end()), arr.end());
}

