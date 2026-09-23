#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <limits>

using namespace std;

struct TrajectoryResult {
    unsigned long long start;

    int block;
    unsigned long long lowerBound;
    unsigned long long upperBound;

    unsigned long long exitValue;

    unsigned long long oddSteps;
    unsigned long long halvingSteps;
    double halvingRatio;
    vector<int> halvingRuns;

    int maxHalvingRun;
    unsigned long long maxValue;
};

int v2(unsigned long long n) {
    int count = 0;
    
    while (n % 2 == 0) {
        n /= 2;
        count++;
    }
    return count;
}

TrajectoryResult analyzeTrajectory (unsigned long long start) {
    if (start <= 1 || start % 2 == 0) {
        throw invalid_argument("Starting value must be an odd integer greater than 1");
    }

    int k = 0;
    unsigned long long temp = start;

    while (temp > 1) {
        temp /=2;
        k++;
    }

    unsigned long long lowerBound = 1ULL << k;
    unsigned long long upperBound = 1ULL << (k+1);

    unsigned long long n = start;

    unsigned long long oddSteps = 0;
    unsigned long long halvingSteps = 0;

    unsigned long long maxValue = start;

    vector<int> halvingRuns;

    while (n >= lowerBound) {
        n = 3 * n + 1;
        oddSteps++;

        maxValue = max(maxValue, n);

        int run = v2(n);

        halvingRuns.push_back(run);
        halvingSteps += run;

        for (int i = 0; i < run; i++) {
            n /= 2;
        }
        maxValue = max(maxValue, n);
    }

    double ratio = static_cast<double>(halvingSteps) / static_cast<double>(oddSteps);

    int maxHalvingRun = *max_element(halvingRuns.begin(), halvingRuns.end());

    return {
        start, k, lowerBound, upperBound, n, oddSteps, halvingSteps, ratio, halvingRuns, maxHalvingRun, maxValue
    };
}

void printResult(const TrajectoryResult& result) {
    cout << "Start: " << result.start << endl;
    cout << "Block: [" << result.lowerBound << ", " << result.upperBound << ")" << endl;

    cout << "Exit value: " << result.exitValue << endl;

    cout << "3n + 1 steps: " << result.oddSteps << endl;
    cout << "n/2 steps: " << result.halvingSteps << endl;
    cout << "Halving ratio: " << result.halvingRatio << endl;
    cout << "Maximum value: " << result.maxValue << endl;
    cout << "Largest halving run: " << result.maxHalvingRun << endl;
    cout << "Halving runs: ";

    for (int run : result.halvingRuns) {
        cout << run << ' ';
    }
    cout << endl;
}


void analyzeBlock(int k) {
    unsigned long long lowerBound = 1ULL << k;
    unsigned long long upperBound = 1ULL << (k+1);
    
    double lowestRatio = numeric_limits<double>::infinity();
    unsigned long lowestRatioStart = 0;
    
    unsigned long long largestMaxValue = 0;
    unsigned long long largestMaxStart = 0;

    int largestHalvingRun = 0;
    unsigned long long largestRunStart = 0;

    unsigned long long mostOddSteps = 0;
    unsigned long long mostOddStepsStart = 0;

    unsigned long long count = 0;
    unsigned long long n = lowerBound + 1;

    for (; n < upperBound; n += 2) {
        TrajectoryResult result = analyzeTrajectory(n);
        count++;

        if (result.halvingRatio < lowestRatio) {
            lowestRatio = result.halvingRatio;
            lowestRatioStart = n;
        }

        if (result.maxValue > largestMaxValue) {
            largestMaxValue = result.maxValue;
            largestMaxStart = n;
        }

        if (result.maxHalvingRun > largestHalvingRun) {
            largestHalvingRun = result.maxHalvingRun;
            largestRunStart = n;
        }

        if (result.oddSteps > mostOddSteps) {
            mostOddSteps = result.oddSteps;
            mostOddStepsStart = n;
        }
    }

    cout << "\nBlock [" << lowerBound << ", " << upperBound << ")" << endl;
    cout << "Odd starting values analyzed: " << count << endl;
    cout << "Lowest halving ratio: " << lowestRatio << "(start = " << lowestRatioStart << ")" << endl;
    cout << "Largest excursion: " << largestMaxValue << " (start = " << largestMaxStart << ")" << endl;
    cout << "Largest halving run: " << largestHalvingRun << " (start = " << largestRunStart << ")" << endl;
    cout << "Most 3n + 1 steps before descent: " << mostOddSteps << " (start = " << mostOddStepsStart << ")" << endl; 
}

int main() {
    for (int k = 3; k <= 10; k++) {
        analyzeBlock(k);
    }

    return 0;
}
