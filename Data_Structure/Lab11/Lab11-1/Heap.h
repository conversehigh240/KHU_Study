template <class ItemType>
void swap(ItemType& one, ItemType& two);

template<class ItemType>
// Assumes ItemType is either a built-in simple type or a class
// with overloaded relational operators.
struct HeapType
{
    void ReheapDown(int root, int bottom);
    void ReheapUp(int root, int bottom);
    ItemType* elements;   // Array to be allocated dynamically
    int numElements;
};

template <class ItemType>
void Swap(ItemType& one, ItemType& two)
{
    ItemType temp;
    temp = one;
    one = two;
    two = temp;
}
template<class ItemType>
void HeapType<ItemType>::ReheapUp(int root, int bottom) // 1-A
// Post: Heap property is restored.
{
    int parent;
    bool reheaped = false;

    while (bottom > root && !reheaped)
    {
        parent = (bottom - 1) / 2;

        if (elements[parent] < elements[bottom])
        {
            Swap(elements[parent], elements[bottom]);
            bottom = parent;
        }
        else
            reheaped = true;
    }
}
template<class ItemType>
void HeapType<ItemType>::ReheapDown(int root, int bottom) // 1-B
// Post: Heap property is restored.
{
    int maxChild;
    int rightChild;
    int leftChild;
    bool reheaped = false;

    leftChild = root * 2 + 1;
    rightChild = root * 2 + 2;

    while (leftChild <= bottom && !reheaped)
    {
        if (leftChild == bottom)
            maxChild = leftChild;
        else
        {
            maxChild = (elements[leftChild] <= elements[rightChild]) ? rightChild : leftChild;
        }
        if (elements[root] < elements[maxChild])
        {
            Swap(elements[root], elements[maxChild]);
            root = maxChild;
            leftChild = root * 2 + 1;
        }
        else
            reheaped = true;
    }
}

/* 1-C
재귀를 사용하는 버전과 반복문을 이용하는 버전 두 경우 모두 함수가 행하는 작업은 동일하다.
따라서 시간 복잡도 역시 동일하다.
두 함수의 시간 복잡도는 log2(N)이다.
*/
