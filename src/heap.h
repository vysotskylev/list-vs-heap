#pragma once

#include <vector>

template <typename KeyT, typename DataT, typename SwapFunT>
class Heap
{
public:
    template <typename KeyIter, typename DataIter>
    Heap(size_t arity, KeyIter keyBegin, KeyIter keyEnd, DataIter dataBegin, DataIter dataEnd, SwapFunT = SwapFunT());

    Heap(size_t arity, SwapFunT = SwapFunT());

    template <typename KeyIter, typename DataIter>
    void Assign(KeyIter keyBegin, KeyIter keyEnd, DataIter dataBegin, DataIter dataEnd);

    size_t Size() const;

    DataT GetMin() const;
    void IncreaseKey(uint64_t idx, KeyT newKey);
    void DeleteMinAndInsert(KeyT key, DataT data);

private:
    std::vector<KeyT> Keys;
    std::vector<DataT> Els;
    size_t Arity;
    SwapFunT SwapFun;
    
    const uint64_t INVALID_IDX = std::numeric_limits<uint64_t>::max();

    void Swap(uint64_t first, uint64_t second);
    uint64_t GetMinChild(uint64_t idx) const;
    void BubbleDown(uint64_t idx);
};

#include "heap-inl.h"
