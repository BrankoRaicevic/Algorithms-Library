#include "MiscAlgorithms.h"
#include <cmath>
#include <limits>

template<typename T>
int BinarySearch(const std::vector<T>& array, const T item) {
	int left = 0;
	int right = array.size() - 1;
	while (left <= right) {
		int middle = (right + left) / 2;
		if (array[middle] == item) {
			return middle;
		}
		else if (array[middle] > item) {
			right = middle - 1;
		}
		else {
			left = middle + 1;
		}
	}
	return -1;
}

template<typename T>
void InsertionSort(std::vector<T>& array) {
	int j, key;
	for (int i = 0; i < array.size(); i++) {
		j = i - 1;
		key = array[i];
		while (j >= 0 && key < array[j]) {
			array[j + 1] = array[j];
			j--;
		}
		array[j + 1] = key;
	}
}

double Kadanes(const std::vector<double>& inputNumbers) {
	double bestSum = 0;
	double currentSum = 0;
	for (auto n : inputNumbers) {
		currentSum = fmax(0, currentSum + n);
		bestSum = fmax(currentSum, bestSum);
	}
	return bestSum;
}

double KadanesNegative(const std::vector<double>& inputNumbers) {
	double bestSum = -std::numeric_limits<double>::infinity();
	double currentSum = 0;
	for (auto n : inputNumbers) {
		currentSum = fmax(n, currentSum + n);
		bestSum = fmax(currentSum, bestSum);
	}
	return bestSum;
}