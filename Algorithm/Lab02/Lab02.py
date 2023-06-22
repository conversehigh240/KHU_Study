#1.1 순차검색 알고리즘

def seqsearch(s, x):
    for i in range(0, len(s)):
        if s[i] == x:
            return i
    return -1

s = [3, 5, 2, 1, 7, 9]
loc = seqsearch(s, 4)
print(loc)

#1.2 배열의 수 더하기

def sum1(s):
    result = 0
    for a in s:
        result = result + a
    return result

s = [3, 5, 2, 1, 7, 9]
answer1 = sum1(s)
print(answer1)

def sum2(s):
    result = 0
    for i in range(len(s)):
        result = result + s[i]
    return result

s = [3, 5, 2, 1, 7, 9]
answer2 = sum2(s)
print(answer2)

