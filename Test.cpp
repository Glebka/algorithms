// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <cassert>
#include <functional>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

/*
���� ��� �������� ����������:
1. ������������ ���������:
	1.1 b - the first element
	1.2 e - points to the 'element' after the end of sequence
	1.3 empty if e=b
	1.4 size = e - b
	1.5 for any m: b<= m < e -> [b,e) = [b,m) + [m, e)
2. �������� - ���� ��������, ����������� �� ����, �������
3. ��������� = ������� �� ����������, ���-�� �� ��� �� ����� ����������. ��������� - ��� ��������, ������� ����������� � ������ ��������, 
������� �� �������� ��� �� �������������.
��������� - ��� �������, �� ������� �� ����� ���������� �� ����� ���������� ���������.
��������� ����� - �������, ������� ����������� �� �������� � ����� �������� �����. ��������� - ��� �������, �� �������� ����� ����� ���� ��� ���������.
4. �������� � ��������:
	4.1 ���������� ���� �������� �� ������������ ���������.
	4.2 ��������� ���� (��������� �� ����������� ������).
	4.3 ���������� ���������.
	4.4 �������� ���� �������� ��������� ����������� �������� ������������ ����������.
	4.5 ������� ���������.
	4.6 ����������� ����.
	4.7 ����������
5. ��������
6. ���������� - ������ ��������� ������ ����������. �� ������ ��������� ���.

������� � - ������ ������
��������� ��������� ��������� ������
*/

/*
// ����� ������������� ��������� ����� �������� end - begin
// ������������ ���������� ���������, ������� ����� ������� - ��� ����� ������������� ���������
// ����� ������� ������ ��������
// ������������ �������� ����� ������ �������� ������� �� 2 ������������ ���������

// ��������� ��������
def bin_search(A, k):
	assert is.sorted(A)
	b, e = 0, len(A)
	if e == 0:
		return e
	if e == 1:
		return 0 if A[0] == k else e
	assert b < e ''' assert b > e
	m = (b + e) / 2 ''' = b + ( e - b ) / 2
	if k < A[m]:
		return binary_search(A[b:m), k)
	else:
		return binary_search(A[m:e), k)
	''' [b, m) [m, e)
*/


template <class TIter, class T>
TIter binary_search_1(TIter begin, TIter end, T key)
{
	assert(std::is_sorted(begin, end));
	size_t size = end - begin;
	if (size == 0)
		return end;
	if (size == 1)
		return (*begin) == key ? begin : end;
	auto m = begin + (end - begin) / 2;
	if (key < *m)
	{
		auto r = binary_search_1(begin, m, key);
		return m == r ? end : r;
	}
	else
		return binary_search_1(m, end, key);
}

template <class TIter, class T>
TIter binary_search_2(TIter begin, TIter end, T key)
{
	assert(std::is_sorted(begin, end));
	size_t size = end - begin;
	if (size == 0)
		return end;

	assert(size > 0);

	auto m = begin + (end - begin) / 2;

	//[begin, m) [m] (m end )

	//if (*m == key)
	//	return m;
	if (key < *m) // [begin, m)
	{
		auto r = binary_search_2(begin, m, key);
		return m == r ? end : r;
	}
	else if (*m < key)
		return binary_search_2(m+1, end, key);
	else
		return m;
}

template <class TIter, class T>
TIter upper_bound_1(TIter begin, TIter end, T key)
{
	assert(std::is_sorted(begin, end));
	while ( begin < end )
	{
		auto m = begin + (end - begin) / 2;
		if (key < *m) 
		{
			end = m; // [begin, m)
		}
		else
		{
			begin = m + 1; // [m+1, end)
		}
	}
	return begin;
}

template <class TIter, class T>
TIter binary_search_3(TIter begin, TIter end, T key)
{
	auto res = upper_bound_1(begin, end, key);
	return *res == key ? res : end;
}

// implement lower-bound
// implement tests for upper / lower bound
// count 7 in sequence

ostream& operator<<( ostream& o, const vector<int>& v )
{
	o << "{";
	for (auto x : v)
	{
		o << x << ", ";
	}
	o << "}";
	return o;
}

template<class T, class TFunc, class P1, class P2>
void test(T expected, TFunc f, P1 p1, P2 p2)
{
	auto result = f(p1, p2);
	cout << "---------------------------------------------------------------------" << endl;
	cout << "f(" << p1 << ", " << p2 << ") = " << result << endl;

	if (expected != result)
	{
		cout << "Expected: " << expected << "; Result: " << result << endl;
		system("pause");
	}
	else
	{
		cout << "OK!" << endl;
	}
}

typedef vector<int> Vec;

template<class TFunc>
void test_binary_search(TFunc bin_search)
{
	
	int key = 42;
	auto adaptor = [bin_search](Vec& v, int key)
	{
		auto result = bin_search(v.begin(), v.end(), key);
		return (result == v.end()) ? -1 : (result - v.begin());
	};

	test(-1, adaptor, Vec({}), key);
	test(0, adaptor, Vec({ 42 }), key);
	test(-1, adaptor, Vec({ 43 }), key);
	test(0, adaptor, Vec({ 42, 43 }), key);
	test(1, adaptor, Vec({ 41, 42 }), key);
	test(-1, adaptor, Vec({ 41, 43 }), key);
	test(-1, adaptor, Vec({1,2,3,4}), key);

	test(3, adaptor, Vec({ 1, 2, 5, 42 }), key);
	test(2, adaptor, Vec({ 3, 5, 42, 45, 67 }), key);
	test(3, adaptor, Vec({ 3, 5, 41, 42, 45, 67 }), key);
	test(3, adaptor, Vec({ 3, 5, 41, 42 }), key);
	test(3, adaptor, Vec({ 3, 5, 41, 42, 43, 44, 45 }), key);

	// more than one key
	test(1, adaptor, Vec({ key, key }), key);
	//test(3, adaptor, Vec({ 1,2,key, key }), key);
	test(1, adaptor, Vec({ key, key, key+1, key+10 }), key);
	test(4, adaptor, Vec({ 1,2,3, key, key }), key);
}

int main()
{
	//test_binary_search(binary_search_1<Vec::iterator, int>);
	test_binary_search(binary_search_3<Vec::iterator, int>);
	system("pause");
    return 0;
}
// ����� ���. ��������
// => inference
// <=> 
// less wrong ru 
// ��������� �����
// �������� �������� ����������
// Monte Carlo algo
// RANSAC