#이분검색 구현
def bs(data, item, low, high):
    if low > high:
        return -1
    
    mid = (low + high) // 2
    if data[mid] == item:
        return mid
    elif data[mid] > item:
        return bs(data, item, low, mid - 1)
    else:
        return bs(data, item, mid + 1, high)
    
data = [1, 3, 5, 6, 7, 9, 10, 14, 17, 19]
n = 10
location = bs(data, 17, 0, n - 1)
print(location)

#이분검색 객체지향방법
class Data:
    def __init__(self, data):
        self.data = data
        self.low = 0
        self.high = len(self.data) - 1

    def binsearch(self, item):
        location = -1
        while self.low <= self.high:
            mid = (self.low + self.high) // 2
            
            if self.data[mid] == item and location == -1:
                location = mid
            elif self.data[mid] > item:
                self.high = mid - 1
            else:
                self.low = mid + 1
        
        return location

data = [1, 3, 5, 6, 7, 9, 10, 14, 17, 19]
n = 17
result = Data(data)
location = result.binsearch(n)
print(location)

#합병정렬 구현
def mergeSort(n, s):
    h = n //2
    m = n - h

    if n > 1:
        u = s[:h]
        v = s[h:]
        mergeSort(h, u)
        mergeSort(m, v)
        merge(h, m, u, v, s)

def merge(h, m, u, v, s):
    i = 0
    j = 0
    k = 0

    while (i < h and j < m):
        if u[i] < v[j]:
            s[k] = u[i]
            i += 1
        else:
            s[k] = v[j]
            j += 1
        k += 1

    if i >= h:
        s[k:h+m] = v[j:m]
    else:
        s[k:h+m] = u[i:h]


s = [3, 5, 2, 9, 10, 14, 4, 8]
mergeSort(8, s)
print(s)

#합병정렬2 구현
def mergeSort2(s, low, high):
    mid = (low + high) // 2
    if low < high:
        mergeSort2(s, low, mid)
        mergeSort2(s, mid + 1, high)
        merge2(s, low, mid, high)

def merge2(s, low, mid, high):
    i = low
    j = mid + 1
    k = low
    u = [0 for i in range(0, high + 1)]

    while (i <= mid and j <= high):
        if s[i] < s[j]:
            u[k] = s[i]
            i += 1
        else:
            u[k] = s[j]
            j += 1
        k += 1

    if i > mid:
        u[k:high + 1] = s[j:high + 1]
    else:
        u[k:high + 1] = s[i:mid + 1]
    s[low:high + 1] = u[low:high + 1]

s = [3, 5, 2, 9, 10, 14, 4, 8]
mergeSort2(s, 0, 7)
print(s)