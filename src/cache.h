#include <list>
#include <random>
#include <unordered_map>

#include "heap.h"

template <typename CacheIter>
struct SwapFun
{
    void operator() (CacheIter lhs, CacheIter rhs)
    {
        std::swap(lhs->second, rhs->second);
    }
};

template <typename DataT>
class HeapCache
{
private:
    using CacheT = std::unordered_map<DataT, uint64_t>;
    using CacheIter = typename CacheT::iterator;
    using HeapT = Heap<uint64_t, CacheIter, SwapFun<CacheIter>>;

public:
    template <typename Iter>
    HeapCache(size_t arity, Iter begin, Iter end)
        : EvictionHeap(arity)
        , CurrentTs(0)
    {
        std::vector<CacheIter> els;
        uint64_t idx = 0;
        size_t size = 0;
        for (auto it = begin; it != end; ++it) {
            auto res = Cache.emplace(*it, idx++);
            els.push_back(res.first);
            size++;
        }
        std::vector<uint64_t> keys(size);
        std::iota(keys.begin(), keys.end(), uint64_t(0));
        EvictionHeap.Assign(keys.begin(), keys.end(), els.begin(), els.end());
        CurrentTs = size;
    }

    size_t Size() const
    {
        return Cache.size();
    }

    bool Access(DataT x)
    {
        auto it = Cache.find(x);
        if (it == Cache.end()) {
            Miss(std::move(x));
            return false;
        } else {
            Hit(it);
            return true;
        }
    }

    DataT GetLastEvicted() const
    {
        return LastEvicted;
    }

private:
    HeapT EvictionHeap;
    CacheT Cache;
    uint64_t CurrentTs;
    DataT LastEvicted;

    void Hit(CacheIter it)
    {
        EvictionHeap.IncreaseKey(it->second, CurrentTs++);
    }

    void Miss(DataT x)
    {
        auto it = EvictionHeap.GetMin();
        LastEvicted = it->first;
        Cache.erase(it);
        auto res = Cache.emplace(std::move(x), 0);
        EvictionHeap.DeleteMinAndInsert(CurrentTs++, res.first);
    }
};

//////////////////////////////////////////////////

template <typename DataT>
class ListCache {
private:
    struct CacheIterWrapper;
    using ListT = std::list<CacheIterWrapper>;
    using CacheT = std::unordered_map<DataT, typename ListT::iterator>;
    using CacheIter = typename CacheT::iterator;
    struct CacheIterWrapper
        : public CacheIter
    {
        CacheIterWrapper(CacheIter it) : CacheIter(it) { }
    };

public:
    template <typename Iter>
    ListCache(Iter begin, Iter end)
    {
        for (auto it = begin; it != end; ++it) {
            EvictionList.push_back(Cache.end());
            auto res = Cache.emplace(*it, std::prev(EvictionList.end()));
            *EvictionList.rbegin() = res.first;
        }
    }

    size_t Size() const
    {
        return Cache.size();
    }

    bool Access(DataT x)
    {
        auto it = Cache.find(x);
        if (it == Cache.end()) {
            Miss(x);
            return false;
        } else {
            Hit(it);
            return true;
        }
    }

    DataT GetLastEvicted() const
    {
        return LastEvicted;
    }
private:
    CacheT Cache;
    ListT EvictionList;
    DataT LastEvicted;

    void Hit(CacheIterWrapper it)
    {
        EvictionList.splice(EvictionList.end(), EvictionList, it->second);
    }

    void Miss(DataT x)
    {
        auto lru = EvictionList.begin();
        LastEvicted = (*lru)->first;
        auto res = Cache.emplace(std::move(x), lru);
        Cache.erase(*lru);
        *lru = res.first;
        EvictionList.splice(EvictionList.end(), EvictionList, lru);
    }
};
