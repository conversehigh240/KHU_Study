# (1) 깊이우선검색
def kp(i, profit, weight):
    global bestset
    global maxp
    global node

    node += 1

    if weight <= W and profit > maxp:
        maxp = profit
        bestset = include[:]

    if promising(i, weight, profit):
        include[i + 1] = 1
        kp(i + 1, profit + p[i + 1], weight + w[i + 1])
        include[i + 1] = 0
        kp(i + 1, profit, weight)

def promising(i, weight, profit):
    global maxp

    if weight >= W:
        return False
    
    else:
        j = i + 1
        bound = profit
        totweight = weight

        while j < n and totweight + w[j] <= W:
            totweight = totweight + w[j]
            bound = bound + p[j]
            j += 1

        k = j
        if k < n:
            bound = bound + (W - totweight) * p[k] / w[k]
        
        return bound > maxp

n = 4
W = 7
p = [14, 5, 20, 4]
w = [2, 1, 5, 2]
maxp = 0
include = [0, 0, 0, 0]
bestset = [0, 0, 0, 0]
node = 0
kp(-1, 0, 0)
print("이익", maxp)
print("해", bestset)
print("노드", node)

# (2) 최고우선검색
import queue

class Node:
    def __init__(self, level, weight, profit, bound, include):
        self.level = level
        self.weight = weight
        self.profit = profit
        self.bound = bound
        self.include = include
    
    def __lt__(self, other):
        return self.bound < other.bound
    
def kp_Best_FS():
    global maxProfit
    global bestset_bfs
    global node_bfs
    global q_data_count
    temp = n * [0]
    v = Node(-1, 0, 0, 0, temp)
    q = queue.PriorityQueue()

    v.bound = compBound(v)
    q.put(v)

    while not q.empty():
        v = q.get()

        if v.bound < -maxProfit:
            u = Node(-1, 0, 0, 0, temp)
            u.level = v.level + 1
            u.weight = v.weight + w[u.level]
            u.profit = v.profit + p[u.level]
            u.include = v.include[:]
            u.include[u.level] = 1
            node_bfs += 1
            
            if u.weight <= W and u.profit > maxProfit:
                maxProfit = u.profit
                bestset_bfs = u.include[:]
            
            u.bound = compBound(u)

            if u.bound < -maxProfit:
                q.put(u)
                node_bfs += 1

            s = Node(-1, 0, 0, 0, temp)
            s.level = v.level + 1
            s.weight = v.weight
            s.profit = v.profit
            s.bound = compBound(s)
            s.include = v.include[:]

            if s.bound < -maxProfit:
                q.put(s)
                q_data_count = q.qsize()
                node_bfs += 1

    maxProfit *= -1
    
def compBound(u):
    if u.weight >= W:
        return 0
    else:
        result = u.profit
        j = u.level + 1
        totweight = u.weight

        while j < n and totweight + w[j] <= W:
            totweight = totweight + w[j]
            result = result + p[j]
            j += 1

        k = j
        if k < n:
            result = result + (W - totweight) * p[k] / w[k]

        return -result
        
n = 4
W = 7
p = [14, 5, 20, 4]
w = [2, 1, 5, 2]
include = [0] * n
maxProfit = 0
bestset_bfs = n * [0]
node_bfs = 0
q_data_count = 0
kp_Best_FS()
print("이익", maxProfit)
print("해", bestset_bfs)
print("노드", node_bfs)
print("PQ데이터 존재개수", q_data_count)