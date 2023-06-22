def promising(i, col):
    global node
    node += 1
    k = 0
    switch = True
    
    while k < i and switch:
        if col[i] == col[k] or abs(col[i] - col[k]) == i - k:
            switch = False
        k += 1

    return switch

def queens(n, i, col):
    global count
    if promising(i, col):
        if i == n - 1:
            print(col)
            count += 1
        else:
            for j in range(n):
                col[i + 1] = j
                queens(n, i + 1, col)

n = 8
col = n * [0]
count = 0
node = 0
queens(n, -1, col)
print(count)
print(node)