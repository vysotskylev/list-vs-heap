#include <list>
#include <random>

#include "heap.h"

template <typename DataT>
class HeapCache
{
private:
    using HeapT = Heap<uint64_t, DataT>;

public:
    using Iterator = uint64_t;

    HeapCache(size_t arity, size_t size)
        : Impl(arity)
        , CurrentTs(0)
    {
        std::vector<typename HeapT::Element> elements;
        elements.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            elements.emplace_back(CurrentTs++, DataT());
        }
        Impl.Assign(elements.begin(), elements.end());
    }

    size_t Size() const
    {
        return Impl.Size();
    }

    void Hit(uint64_t idx)
    {
        Impl.IncreaseKey(idx, CurrentTs);
        ++CurrentTs;
    }

    void Miss()
    {
        Impl.DeleteMinAndInsert(CurrentTs, DataT());
        ++CurrentTs;
    }

    std::vector<uint64_t> CreateRequests(size_t size) const
    {
        std::vector<uint64_t> allInds(Size());
        std::iota(allInds.begin(), allInds.end(), uint64_t(0));
        std::mt19937 randGen(43);
        std::shuffle(allInds.begin(), allInds.end(), randGen);
        return {allInds.begin(), allInds.begin() + size};
    }

private:
    HeapT Impl;
    uint64_t CurrentTs;
};

//////////////////////////////////////////////////

template <typename DataT>
class ListCache {
private:
    using ListT = std::list<DataT>;
public:
    using Iterator = typename ListT::const_iterator;

    ListCache(size_t size)
        : Impl(size, DataT())
    { }

    size_t Size() const
    {
        return Impl.size();
    }

    void Hit(Iterator it)
    {
        if (it == Impl.begin()) {
            return;
        }
        Impl.splice(Impl.begin(), Impl, it);
    }

    void Miss()
    {
        auto last = std::prev(Impl.end());
        *last = DataT();
        Hit(last);
    }

    std::vector<Iterator> CreateRequests(size_t size) const
    {
        std::vector<Iterator> allIters(Size());
        std::iota(allIters.begin(), allIters.end(), Impl.begin());
        std::mt19937 randGen(44);
        std::shuffle(allIters.begin(), allIters.end(), randGen);
        return {allIters.begin(), allIters.begin() + size};
    }

private:
    ListT Impl;
};
