import sys
inf = 1000
w = [[0, 7, 4, 6, 1],
     [inf, 0, inf, inf, inf],
     [inf, 2, 0, 5, inf],
     [inf, 3, inf, 0, inf],
     [inf, inf, inf, 1, 0]]

n = 5
f = set()
touch = n * [0]
length = n * [0]
save_length = n * [0]

for i in range(1, n):
    length[i] = w[0][i]


for j in range(1, n):
    min = sys.maxsize

    for k in range(1, n):
        if 0 <= length[k] < min:
            min = length[k]
            vnear = k

    e = (touch[vnear], vnear)
    f.add(e)

    for l in range(1, n):
        if length[vnear] + w[vnear][l] < length[l]:
            length[l] = length[vnear] + w[vnear][l]
            touch[l] = vnear
    
        save_length[vnear] = length[vnear]
    length[vnear] = -1

print(f)
print(save_length)