#include "MiscAlgorithms.h"
#include <cmath>
#include <limits>


int BinarySearch(const std::vector<int>& array, const int num) {
	int left = 0;
	int right = array.size() - 1;
	while (left <= right) {
		int middle = (right + left) / 2;
		if (array[middle] == num) {
			return middle;
		}
		else if (array[middle] > num) {
			right = middle - 1;
		}
		else {
			left = middle + 1;
		}
	}
	return -1;
}

double KadanesAlgorithm(const std::vector<double>& inputNumbers) {
	double bestSum = 0;
	double currentSum = 0;
	for (auto n : inputNumbers) {
		currentSum = fmax(0, currentSum + n);
		bestSum = fmax(currentSum, bestSum);
	}
	return bestSum;
}

double KadanesAlgorithmNegative(const std::vector<double>& inputNumbers) {
	double bestSum = -std::numeric_limits<double>::infinity();
	double currentSum = 0;
	for (auto n : inputNumbers) {
		currentSum = fmax(n, currentSum + n);
		bestSum = fmax(currentSum, bestSum);
	}
	return bestSum;
}