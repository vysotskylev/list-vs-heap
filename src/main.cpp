#include <iostream>
#include <chrono>
#include <random>
#include <vector>

#include "cache.h"

const size_t HEAP_ARITY = 8;

std::vector<uint64_t> CreateRandomLruRequests(size_t cacheSize, size_t numRequests, double hitRate)
{
    std::vector<uint64_t> els(cacheSize);
    std::iota(els.begin(), els.end(), uint64_t(0));
    ListCache<uint64_t> cache(els.begin(), els.end());

    std::mt19937 randGen(42);
    std::bernoulli_distribution hitDistr(hitRate);
    std::uniform_int_distribution<uint64_t> elDistr(0, cacheSize);
    std::vector<uint64_t> result;
    result.reserve(numRequests);
    size_t evicted = cacheSize;
    for (size_t i = 0; i < numRequests; ++i) {
        if (hitDistr(randGen)) {
            uint64_t el;
            do {
                el = elDistr(randGen);
            } while (el == evicted);
            result.push_back(el);
            cache.Access(el);
        } else {
            cache.Access(evicted);
            result.push_back(evicted);
            evicted = cache.GetLastEvicted();
        }
    }
    return result;
}

template <typename CacheT>
std::chrono::nanoseconds Measure(
    CacheT& cache,
    size_t numRequests,
    double hitRate)
{
    auto requests = CreateRandomLruRequests(cache.Size(), numRequests, hitRate);
    auto startTime = std::chrono::high_resolution_clock::now();
    size_t hitCount = 0;
    for (size_t i = 0; i < numRequests; ++i) {
        hitCount += cache.Access(requests[i]);
    }
    auto finishTime = std::chrono::high_resolution_clock::now();
    return finishTime - startTime;
}

int main(int argc, char** argv)
{
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " type cache_size num_requests hit_rate" << std::endl;
        return 1;
    }

    size_t cacheSize = std::stoull(argv[2]);
    size_t numRequests = std::stoull(argv[3]);
    double hitRate = std::stod(argv[4]);

    std::chrono::nanoseconds time;
    std::vector<uint64_t> els(cacheSize);
    std::iota(els.begin(), els.end(), uint64_t(0));
    if (argv[1] == std::string("list")) {
        ListCache<uint64_t> cache(els.begin(), els.end());
        time = Measure(cache, numRequests, hitRate);
    } else if (argv[1] == std::string("heap")) {
        HeapCache<uint64_t> cache(HEAP_ARITY, els.begin(), els.end());
        time = Measure(cache, numRequests, hitRate);
    } else {
        std::cerr << "type must be either 'list' or 'heap'" << std::endl;
        return 1;
    }

    std::cout << time.count() / 1e9 << " seconds" << std::endl;
    return 0;
}
