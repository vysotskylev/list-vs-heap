# Comparison of list and heap based LRU caches

The somewhat strange result is illustrated below. Note that despite
theoretical complexity of list-based solution vs the heap-based one is O(1) vs O(log n)
the latter performs better for larger hit-rates.

![GitHub Logo](/plots/list-vs-heap.png)
