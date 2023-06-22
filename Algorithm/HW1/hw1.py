# (1) O(n²) 알고리즘인 bubble sort(알고리즘 A)와 평균적으로 O(nlog₂n) 알고리즘 quick sort(알고리즘 B)를 python으로 구현한다.

# 알고리즘 A
def BubbleSort(ary):
    for i in range(len(ary) - 1, 0, -1):
        for j in range(i):
            if (ary[j] > ary[j + 1]):
                temp = ary[j]
                ary[j] = ary[j + 1]
                ary[j + 1] = temp

# 알고리즘 B
def QuickSort(ary, first, last):
    if first >= last:
        return
    
    start = first + 1
    end = last
    split = first

    while start <= end:
        while (start <= last and ary[start] <= ary[split]):
            start += 1
        while (end > first and ary[end] >= ary[split]):
            end -= 1
        if start > end:
            temp = ary[end]
            ary[end] = ary[split]
            ary[split] = temp
        else:
            temp = ary[start]
            ary[start] = ary[end]
            ary[end] = temp
    
    QuickSort(ary, first, end - 1)
    QuickSort(ary, end + 1, last)
            

import random
b = []
q = []
for i in range (1, 5000):
    b.append(random.randint(1, 1001))
for j in range (1, 10000):
    q.append(random.randint(1, 1001))

print(b)
print("BubbleSort")
BubbleSort(b)
print(b)

print(q)
print("QuickSort")
QuickSort(q, 0, len(q) - 1)
print(q)

import time
N = [5000, 10000, 15000, 20000, 30000, 40000]
for n in N:
    A = []
    for i in range(1, n):
        A.append(random.randint(1, 1001))
    start = time.time()
    BubbleSort(A)
    end = time.time()
    print(f"{end - start:.6f} sec")

M = [5000, 10000, 15000, 20000, 30000, 40000, 80000]
for m in M:
    B = []
    for i in range(1, m):
        B.append(random.randint(1, 1001))
    start = time.time()
    QuickSort(B, 0, len(B) - 1)
    end = time.time()
    print(f"{end - start:.6f} sec")
