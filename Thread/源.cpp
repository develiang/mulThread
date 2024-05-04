#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> countNumberOfRetailers(const std::vector<std::vector<int>>& retailers, const std::vector<std::vector<int>>& requests) {
    // Find the maximum x and y coordinates that we need to consider
    int max_x = 0, max_y = 0;
    for (const auto& r : retailers) {
        max_x = std::max(max_x, r[0]);
        max_y = std::max(max_y, r[1]);
    }

    // Create a 2D prefix sum matrix
    std::vector<std::vector<int>> prefix(max_x + 1, std::vector<int>(max_y + 1, 0));

    // Populate the prefix sum matrix
    for (const auto& r : retailers) {
        prefix[r[0]][r[1]]++;
    }
    for (int i = 1; i <= max_x; ++i) {
        for (int j = 1; j <= max_y; ++j) {
            prefix[i][j] += prefix[i - 1][j] + prefix[i][j - 1] - prefix[i - 1][j - 1];
        }
    }

    // Answer the requests using the prefix sum matrix
    std::vector<int> results;
    for (const auto& req : requests) {
        int x = req[0], y = req[1];
        x = std::min(x, max_x);
        y = std::min(y, max_y);

        // Retrieve the number of retailers that can deliver to (x, y)
        results.push_back(prefix[x][y]);
    }

    return results;
}

int main() {
    // Example use case
    std::vector<std::vector<int>> retailers = { {1, 4}, {2, 4}, {1, 5} };
    std::vector<std::vector<int>> requests = { {2, 6}, {1, 4} };
    std::vector<int> results = countNumberOfRetailers(retailers, requests);

    // Print results
    for (int result : results) {
        std::cout << result << std::endl;
    }

    return 0;
}


//#include <iostream>
//#include <vector>
//#include <algorithm>
//#include <numeric>
//
//using namespace std;
//
//std::vector<std::vector<long long>> processExecution(const std::vector<int>& power, 
//                                                     const std::vector<int>& minPower, 
//                                                     const std::vector<int>& maxPower) {
//    std::vector<std::vector<long long>> result;
//    std::vector<int> sortedPower = power;
//
//    // Sort the power requirements of processes
//    std::sort(sortedPower.begin(), sortedPower.end());
//
//    // For each processor, find the range of process powers it can handle using binary search
//    for (size_t i = 0; i < minPower.size(); ++i) {
//        // Lower and upper bounds for the range of powers this processor can handle
//        auto lower = std::lower_bound(sortedPower.begin(), sortedPower.end(), minPower[i]);
//        auto upper = std::upper_bound(sortedPower.begin(), sortedPower.end(), maxPower[i]);
//
//        // Calculate count and sum of powers within the range
//        long long count = std::distance(lower, upper);
//        long long powerSum = std::accumulate(lower, upper, 0LL);
//
//        // Add to the result
//        result.push_back({count, powerSum});
//    }
//
//    return result;
//}
//
//int main() {
//    // Example input
//    vector<int> power = {11, 11, 11};
//    vector<int> minPower = {8, 13};
//    vector<int> maxPower = {11,100};
//
//    // Call the function and get the result
//    vector<vector<long long>> results = processExecution(power, minPower, maxPower);
//
//    // Output the results
//    for (const auto &pair : results) {
//        cout << "[" << pair[0] << ", " << pair[1] << "]" << endl;
//    }
//
//    return 0;
//}
