#include <iostream>
#include <chrono>
#include <random>
#include <vector>

#include "cache.h"

const size_t HEAP_ARITY = 8;

std::vector<bool> CreateRandomBitVector(size_t size, double probOfOne)
{
    std::mt19937 randGen(42);
    std::vector<bool> res;
    res.reserve(size);
    std::bernoulli_distribution distr(probOfOne);
    for (size_t i = 0; i < size; ++i) {
        res.push_back(distr(randGen));
    }
    return res;
}

template <typename CacheT>
std::chrono::nanoseconds Measure(
    CacheT& cache,
    size_t isHitSize, 
    size_t numRequests,
    double hitRate)
{
    auto requests = cache.CreateRequests(cache.Size());
    auto isHit = CreateRandomBitVector(isHitSize, hitRate);
    auto startTime = std::chrono::high_resolution_clock::now();
    size_t reqIdx = 0, isHitIdx = 0;
    for (size_t i = 0; i < numRequests; ++i) {
        if (reqIdx >= requests.size()) {
            reqIdx = 0;
        }
        if (isHitIdx >= isHit.size()) {
            isHitIdx = 0;
        }
        if (isHit[isHitIdx]) {
            cache.Hit(requests[reqIdx]);
        } else {
            cache.Miss();
        }
        ++reqIdx;
        ++isHitIdx;
    }
    auto finishTime = std::chrono::high_resolution_clock::now();
    return finishTime - startTime;
}

int main(int argc, char** argv)
{
    if (argc < 6) {
        std::cerr << "Usage: " << argv[0] << " type cache_size is_hit_size num_requests hit_rate" << std::endl;
        return 1;
    }

    size_t cacheSize = std::stoull(argv[2]);
    size_t isHitSize = std::stoull(argv[3]);
    size_t numRequests = std::stoull(argv[4]);
    double hitRate = std::stod(argv[5]);

    std::chrono::nanoseconds time;
    if (argv[1] == std::string("list")) {
        ListCache<uint64_t> cache(cacheSize);
        time = Measure(cache, isHitSize, numRequests, hitRate);
    } else if (argv[1] == std::string("heap")) {
        HeapCache<uint64_t> cache(HEAP_ARITY, cacheSize);
        time = Measure(cache, isHitSize, numRequests, hitRate);
    } else {
        std::cerr << "type must be either 'list' or 'heap'" << std::endl;
        return 1;
    }

    std::cout << time.count() / 1e9 << " seconds" << std::endl;
    
    return 0;
}
