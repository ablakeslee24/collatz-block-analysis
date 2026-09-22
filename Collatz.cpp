#include <iostream>
#include <vector>
#include <cstdint>
#include <limits>
#include <fstream>
#include <string>

using namespace std;

struct Result {
    uint64_t start;
    uint64_t steps;
    uint64_t maxReached;
};

Result crossingTime(uint64_t start, uint64_t lowerBound) {
    uint64_t x = start;
    uint64_t steps = 0;
    uint64_t maxReached = x;
    
    while (x >= lowerBound) {
        if (x % 2 == 0) {
            x /= 2;
        }
        else {
            if (x > (numeric_limits<uint64_t>::max()-1) / 3) {
                cerr << "Overflow detected for value " << start << endl;
                exit(1); 
            }
            x = 3 * x + 1;
        }
        steps++;

        if (x > maxReached) {
            maxReached = x;
        }
    }
    return {start, steps, maxReached};
}

int main() {
    
    for (int k = 4; k <= 20; k++) {
        uint64_t lowerBound = 1ULL << k;
        uint64_t upperBound = 1ULL << (k + 1);

        string filename = "collatz_k" + to_string(k) + ".csv";
        ofstream file(filename);
        file << "start,normalizedPosition, steps,maxReached" << endl;
    
        vector<Result> results;

        for(uint64_t x = lowerBound + 1; x < upperBound; x++) {
            Result r = crossingTime(x, lowerBound);
            double normalizedPosition = static_cast<double>(x - lowerBound) / lowerBound;
            results.push_back(r);

            file << r.start << "," << normalizedPosition << "," << r.steps << "," << r.maxReached << endl;
        }

        Result worst = results[0];
        for (const Result& r : results) {
            if (r.steps > worst.steps) {
                worst = r;
            }
        }

        file.close();

        cout << "k: " << k << " | Range: " << lowerBound << "-" << upperBound << " | Worst x: " << worst.start << " | Steps: " << worst.steps << " | Saved: " << filename << endl;
    }
    

    return 0;
}