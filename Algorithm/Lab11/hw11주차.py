# 부분집합의 합
def promising(i, weight, total):
    return (weight + total >= W) and (weight == W or weight + w[i + 1] <= W)

def s_s(i, weight, total, include):
    global node
    if promising(i, weight, total):
        node += 2
        if weight == W:
            node -= 2
            print("sol", include)
        else:
            include[i + 1] = 1
            s_s(i + 1, weight + w[i + 1], total - w[i + 1], include)
            include[i + 1] = 0
            s_s(i + 1, weight, total - w[i + 1], include)


n = 5
w = [1, 4, 5, 6, 8]
W = 10
node = 1
print("items = ", w, "W = ", W)
include = n * [0]
total = 0
for k in w:
    total += k
s_s(-1, 0, total, include)
print("node", node)
print()

# m-coloring
def color(i, vcolor):
    global node_c
    if promising_c(i, vcolor):
        node_c += m
        if i == n - 1:
            node_c -= m
            print(vcolor)
        else:
            for colors in range(1, m + 1):
                vcolor[i + 1] = colors
                color(i + 1, vcolor)

def promising_c(i, vcolor):
    switch = True
    j = 0

    while j < i and switch:
        if W[i][j] and vcolor[i] == vcolor[j]:
            switch = False
        j += 1

    return switch

n = 7
W = [[0, 1, 1, 0, 0, 0, 1],
     [1, 0, 1, 1, 0, 0, 0],
     [1, 1, 0, 1, 1, 1, 1],
     [0, 1, 1, 0, 1, 0, 0],
     [0, 0, 1, 1, 0, 1, 0],
     [0, 0, 1, 0, 1, 0, 1],
     [1, 0, 1, 0, 0, 1, 0]]
vcolor = n * [0]
m = 3
node_c = 1
color(-1, vcolor)
print("node", node_c)