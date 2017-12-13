import  matplotlib.pyplot as plt
import subprocess
import numpy as np

EXE = "../measure"

cache_size = 10**6
is_hit_size = 194867
num_requests = 10**7
hit_rates = np.linspace(0, 1, num=30)

plt.figure(figsize = (10, 5))
lines = {}
for typ in ["list", "heap"]:
    times = []
    for hit_rate in hit_rates:
        args = map(str, [EXE, typ, cache_size, is_hit_size, num_requests, hit_rate])
        time = float(subprocess.check_output(args).split()[0]) / num_requests * 10**9
        times.append(time)
    lines[typ], = plt.plot(hit_rates, times)

plt.xlabel("Hit rate")
plt.ylabel("Mean access time, ns")
plt.legend(lines.values(), lines.keys())
plt.savefig("list-vs-heap.png")
