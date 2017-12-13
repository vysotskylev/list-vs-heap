import  matplotlib.pyplot as plt
import subprocess
import numpy as np

EXE = "../measure"

def get_human_size(size):
    power_to_letter = [(10**9, "G"), (10**6, "M"), (10**3, 'K')]
    for power, letter in power_to_letter:
        if size % power == 0:
            return str(size // power) + letter

    
cache_size = 10**6
num_requests = 10**7
hit_rates = np.linspace(0, 1, num=30)

plt.figure(figsize = (10, 5))
lines = {}
for typ in ["list", "heap"]:
    times = []
    for hit_rate in hit_rates:
        args = map(str, [EXE, typ, cache_size, num_requests, hit_rate])
        time = float(subprocess.check_output(args).split()[0]) / num_requests * 10**9
        times.append(time)
    lines[typ], = plt.plot(hit_rates, times)

plt.title("Cache size = {}".format(get_human_size(cache_size)))
plt.xlabel("Hit rate")
plt.ylabel("Mean access time, ns")
plt.legend(lines.values(), lines.keys())
plt.savefig("list-vs-heap.png")
