#include <algorithm>

template <typename KeyT, typename DataT, typename SwapFunT>
template <typename KeyIter, typename DataIter>
Heap<KeyT, DataT, SwapFunT>::Heap(size_t arity, KeyIter keyBegin, KeyIter keyEnd, DataIter dataBegin, DataIter dataEnd, SwapFunT fun)
    : Keys(keyBegin, keyEnd)
    , Els(dataBegin, dataEnd)
    , Arity(arity)
    , SwapFun(fun)
{ }

template <typename KeyT, typename DataT, typename SwapFunT>
Heap<KeyT, DataT, SwapFunT>::Heap(size_t arity, SwapFunT fun)
    : Arity(arity)
    , SwapFun(fun)
{ }

template <typename KeyT, typename DataT, typename SwapFunT>
template <typename KeyIter, typename DataIter>
void Heap<KeyT, DataT, SwapFunT>::Assign(KeyIter keyBegin, KeyIter keyEnd, DataIter dataBegin, DataIter dataEnd)
{
    Keys.assign(keyBegin, keyEnd);
    Els.assign(dataBegin, dataEnd);
}

template <typename KeyT, typename DataT, typename SwapFunT>
size_t Heap<KeyT, DataT, SwapFunT>::Size() const
{
    return Keys.size();
}

template <typename KeyT, typename DataT, typename SwapFunT>
DataT Heap<KeyT, DataT, SwapFunT>::GetMin() const
{
    return Els[0];
}

template <typename KeyT, typename DataT, typename SwapFunT>
void Heap<KeyT, DataT, SwapFunT>::IncreaseKey(uint64_t idx, KeyT newKey)
{
    Keys[idx] = std::move(newKey);
    BubbleDown(idx);
}

template <typename KeyT, typename DataT, typename SwapFunT>
void Heap<KeyT, DataT, SwapFunT>::DeleteMinAndInsert(KeyT key, DataT data)
{
    Keys[0] = std::move(key);
    Els[0] = std::move(data);
    BubbleDown(0);
}

template <typename KeyT, typename DataT, typename SwapFunT>
void Heap<KeyT, DataT, SwapFunT>::Swap(uint64_t first, uint64_t second)
{
    std::swap(Keys[first], Keys[second]);
    std::swap(Els[first], Els[second]);
    SwapFun(Els[first], Els[second]);
}

template <typename KeyT, typename DataT, typename SwapFunT>
uint64_t Heap<KeyT, DataT, SwapFunT>::GetMinChild(uint64_t idx) const
{
    auto firstChild = idx * Arity + 1;
    if (firstChild >= Keys.size()) {
        return INVALID_IDX;
    }
    auto afterLastChild = std::min(firstChild + Arity, Keys.size());
    auto minIt = std::min_element(&Keys[firstChild], &Keys[afterLastChild]);
    return minIt - &Keys[0];
}

template <typename KeyT, typename DataT, typename SwapFunT>
void Heap<KeyT, DataT, SwapFunT>::BubbleDown(uint64_t idx)
{
    while (true) {
        auto minChild = this->GetMinChild(idx);
        if (minChild == INVALID_IDX) {
            break;
        }
        Swap(idx, minChild);
        idx = minChild;
    }
}
