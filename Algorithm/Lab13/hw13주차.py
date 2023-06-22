import math
class Heap(object):
    n = 0

    def __init__(self, data):
        self.data = data
        self.n = len(self.data) - 1

    def addElt(self, elt):
        self.data.append(elt)
        self.n += 1
        self.siftUp(self.n)

    def siftUp(self, i):
        while i >= 2:
            if self.data[i] > self.data[i // 2]:
                temp = self.data[i]
                self.data[i] = self.data[i // 2]
                self.data[i // 2] = temp
            i = i // 2

    def siftDown(self, i):
        siftkey = self.data[i]
        parent = i
        spotfound = False

        while 2 * parent <= self.n and not spotfound:
            if 2 * parent < self.n and self.data[2 * parent] < self.data[2 * parent + 1]:
                largechild = 2 * parent + 1
            else:
                largechild = 2 * parent
            if siftkey < self.data[largechild]:
                self.data[parent] = self.data[largechild]
                parent = largechild
            else:
                spotfound = True
        
        self.data[parent] = siftkey

# 방법 1을 이용
    def makeHeap1(self):
        for i in range(2, self.n + 1):
            self.siftUp(i)

# 방법 2를 이용
    def makeHeap2(self):
        for i in range(self.n // 2, 0, -1):
            self.siftDown(i)

    def root(self):
        keyout = self.data[1]
        self.data[1] = self.data[self.n]
        self.n = self.n - 1
        self.siftDown(1)
        return keyout

    def removeKeys(self):
        S = []
        for i in range(self.n, 0, -1):
            S.append(self.root())

        return S

def heapSort1(a):
    a = Heap(a)
    a.makeHeap1()
    result = a.removeKeys()
    return result

def heapSort2(a):
    a = Heap(a)
    a.makeHeap2()
    result = a.removeKeys()
    return result

print("방법 1")
a = [0, 11, 14, 2, 7, 6, 3, 9, 5]
b = Heap(a)
b.makeHeap1()
print(b.data)
s = heapSort1(a)
print(s)

print()

print("방법 2")
a = [0, 11, 14, 2, 7, 6, 3, 9, 5]
b = Heap(a)
b.makeHeap2()
print(b.data)
b.addElt(50)
print(b.data)
s = heapSort2(a)
print(s)