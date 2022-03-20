#include "tests.h"
#include "single_linked_list.h"
#include <vector>
#include <cassert>

template <typename T>
void ReverseArray(T* start, size_t size) {
    T* end = start + size;
    std::reverse(start, end);
}

template<typename Type>
bool equal(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

using namespace std;

// ������� ���������� �������������� ������,
// ����������� ������� � ��������� ������
template <class Type>
auto MakeInsertingFunction(vector<SingleLinkedList<Type>>& lists, int&& x) {
    return [&](const Type& value) {
        lists[x].PushFront(value);
    };
}

template <class T>
void InsertRange(int from, int to, T push_function) {
    for (int i = from; i < to; ++i) {
        push_function(i);
    }
}

int main() {
    // main ��������� ������, � �� ��� ������.
    // �� ������� ��� ���� �������
    // ������ ���� ������, ������ ������ ��������� ��� ������� ����� ������.
    // ����� ������ � ���� ������ �
    // ������ �������� ����� �����

    vector<SingleLinkedList<int>> lists_a(10);

    auto push_to_2a = MakeInsertingFunction(lists_a, 2);
    auto push_to_5a = MakeInsertingFunction(lists_a, 5);
    auto push_to_7a = MakeInsertingFunction(lists_a, 7);

    InsertRange(10, 12, push_to_2a);
    InsertRange(12, 14, push_to_5a);
    InsertRange(14, 16, push_to_7a);

    assert(lists_a[2] == SingleLinkedList<int>({ 11, 10 }));
    assert(lists_a[5] == SingleLinkedList<int>({ 13, 12 }));
    assert(lists_a[7] == SingleLinkedList<int>({ 15, 14 }));

    vector<SingleLinkedList<int>> lists_b = lists_a;

    auto push_to_2b = MakeInsertingFunction(lists_b, 2);
    auto push_to_5b = MakeInsertingFunction(lists_b, 5);
    auto push_to_7b = MakeInsertingFunction(lists_b, 7);

    InsertRange(20, 22, push_to_2b);
    InsertRange(22, 24, push_to_5b);
    InsertRange(24, 26, push_to_7b);

    assert(lists_b[2] == SingleLinkedList<int>({ 21, 20, 11, 10 }));
    assert(lists_b[5] == SingleLinkedList<int>({ 23, 22, 13, 12 }));
    assert(lists_b[7] == SingleLinkedList<int>({ 25, 24, 15, 14 }));

    lists_a[2].PopFront();
    lists_a[5].InsertAfter(lists_a[5].begin(), 100);
    lists_b[5].EraseAfter(next(lists_b[5].begin()));
    lists_b[7].Clear();

    assert(lists_a[2] == SingleLinkedList<int>({ 10 }));
    assert(lists_a[5] == SingleLinkedList<int>({ 13, 100, 12 }));
    assert(lists_b[5] == SingleLinkedList<int>({ 23, 22, 12 }));
    assert(lists_b[7] == SingleLinkedList<int>());

    Test0();
    Test1();
    Test2();
    Test3();
    Test4();

    const SingleLinkedList<int> lhs = { 1,3,2,3,23,2,3 };
    const SingleLinkedList<int> rhs = { 1,3,2,3,23,2,3 };

    bool flag = equal(lhs, rhs);
    std::cout << flag;
}

