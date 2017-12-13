#pragma once

#include <vector>

template <typename KeyT, typename DataT>
class Heap
{
public:
    struct Element
    {
        KeyT Key;
        DataT Data;
        Element(KeyT key, DataT data)
            : Key(std::move(key))
            , Data(std::move(data))
        { }
    };

    template <typename Iter>
    Heap(size_t arity, Iter begin, Iter end);

    Heap(size_t arity);

    template <typename Iter>
    void Assign(Iter begin, Iter end);

    size_t Size() const;

    void IncreaseKey(uint64_t idx, KeyT newKey);
    void DeleteMinAndInsert(KeyT key, DataT data);

private:
    std::vector<Element> Els;
    size_t Arity;
    
    const uint64_t INVALID_IDX = std::numeric_limits<uint64_t>::max();

    uint64_t GetMinChild(uint64_t idx) const;
    void BubbleDown(uint64_t idx);
};

#include "heap-inl.h"
