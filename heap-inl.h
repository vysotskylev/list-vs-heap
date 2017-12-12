#include <algorithm>

template <typename KeyT, typename DataT>
template <typename Iter>
Heap<KeyT, DataT>::Heap(size_t arity, Iter begin, Iter end)
    : Els(begin, end)
    , Arity(arity)
{ }

template <typename KeyT, typename DataT>
Heap<KeyT, DataT>::Heap(size_t arity)
    : Arity(arity)
{ }

template <typename KeyT, typename DataT>
template <typename Iter>
void Heap<KeyT, DataT>::Assign(Iter begin, Iter end)
{
    Els.assign(begin, end);
}

template <typename KeyT, typename DataT>
size_t Heap<KeyT, DataT>::Size() const
{
    return Els.size();
}

template <typename KeyT, typename DataT>
void Heap<KeyT, DataT>::IncreaseKey(uint64_t idx, KeyT newKey)
{
    Els[idx].Key = std::move(newKey);
    BubbleDown(idx);
}

template <typename KeyT, typename DataT>
void Heap<KeyT, DataT>::DeleteMinAndInsert(KeyT key, DataT data)
{
    Els[0].Key = std::move(key);
    Els[0].Data = std::move(data);
    BubbleDown(0);
}

template <typename KeyT, typename DataT>
uint64_t Heap<KeyT, DataT>::GetMinChild(uint64_t idx) const
{
    auto firstChild = idx * Arity + 1;
    if (firstChild >= Els.size()) {
        return INVALID_IDX;
    }
    auto afterLastChild = std::min(firstChild + Arity, Els.size());
    auto minIt = std::min_element(&Els[firstChild], &Els[afterLastChild],
        [] (const Element& a, const Element& b) {
            return a.Key < b.Key;
        });
    return minIt - &Els[0];
}

template <typename KeyT, typename DataT>
void Heap<KeyT, DataT>::BubbleDown(uint64_t idx)
{
    while (true) {
        auto minChild = this->GetMinChild(idx);
        if (minChild == INVALID_IDX || Els[idx].Key < Els[minChild].Key) {
            break;
        }
        std::swap(Els[idx], Els[minChild]);
        idx = minChild;
    }
}
