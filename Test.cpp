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
База для создания алгоритмов:
1. Полуоткрытые интервалы:
	1.1 b - the first element
	1.2 e - points to the 'element' after the end of sequence
	1.3 empty if e=b
	1.4 size = e - b
	1.5 for any m: b<= m < e -> [b,e) = [b,m) + [m, e)
2. Контракт - база индукции, ограничения на типы, ассерты
3. Инвариант = никогда не изменяется, что-то на что мы можем положиться. Инвариант - это свойство, которое сохраняется у класса объектов, 
которое не меняется при их трансформации.
Инвариант - это условие, на которое мы можем положиться во время выполнения программы.
Инвариант цикла - условие, которое сохраняется до итерации и полсе итерации цикла. Инвариант - это правило, по которому будет вести себя ваш алогоритм.
4. Разделяй и властвуй:
	4.1 Определить совю проблему на полуоткрытом интервале.
	4.2 Проверьте базу (проверить на тривиальных кейсах).
	4.3 Определить инвариант.
	4.4 Разбейте вашу проблему используя разделяющее свойство полуоткрытых интервалов.
	4.5 Условие остановки.
	4.6 Используйте цикл.
	4.7 Сходимость
5. Рекурсия
6. Сходимость - дальше алгоритму некуда выполнятся. Мы должны проверять это.

Нотация О - символ Ландау
посчитать сложность бинарного поиска
*/

/*
// длина полуоткрытого интервала равна разности end - begin
// единственным невалидным елементом, который можно вернуть - это конец полуоткрытого интервала
// можно указать пустой интервал
// полуоткрытый интервал можно всегда поделить пополам на 2 полуоткрытых интервала

// исправить алгоритм
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
TIter lower_bound_1(TIter begin, TIter end, T key)
{
	//....
}

template <class TIter, class T>
TIter binary_search_3(TIter begin, TIter end, T key)
{
	auto res = lower_bound_1(begin, end, key);
	return *res == key ? res : end;
}

template <class T>
void my_swap(T& first, T& second)
{
	T temp = first;
	first = second;
	second = temp;
}

template <class TIter>
void naive_sort(TIter begin, TIter end)
{
	// [begin, end) = [sorted) [unsorted)
	//[b,e) = [b,pivot)[pivot,e)

	for (TIter i = begin; i < end; ++i)
	{
		for (TIter j = i+1 ; j < end; ++j)
		{
			if (*j < *i)
			{
				my_swap(*i, *j);
			}
		}
	}
}

template < class TIter >
TIter my_min_element(TIter b, TIter e)
{
	TIter result = b;
	while (b < e)
	{
		if (*b < *result)
			result = b;
		++b;
	}
	return result;
}

//Savonia
// selection sort - сортировка выбором
template <class TIter>
void selection_sort( TIter b, TIter e )
{
	// [begin, end) = [sorted) [unsorted)
	//[b,e) = [b,pivot)[pivot,e) - devide and conquer
	// 
	for (TIter pivot = b; pivot < e; ++pivot)
		my_swap(*pivot, *my_min_element(pivot, e)); // n, n-1, n-2, n-3, ..., 1
	// O(n^2)
}

template <class TIter>
void buble_sort(TIter b, TIter e)
{
	// [b,e) = [unsorted)[sorted)
	for (TIter pivot = e; pivot != b; --pivot)
	{
		//[b,pivot) = [b, i)[i,pivot)
		assert( std::is_sorted(pivot, e) );
		for (TIter next_i = b + 1; next_i < pivot; ++next_i)
		{
			auto i = next_i - 1;
			assert(i == my_min_element(b, next_i)); //[b,i]

			
			if (*next_i < *i)
				my_swap(*i, *next_i);

			assert(i == my_min_element(b, next_i)); //[b,i]
		}
		assert(std::is_sorted(pivot, e));
	}
}

template <class TIter>
void insertion_sort(TIter b, TIter e)
{
	// p = pivot
	// [b,e) = [sorted)[unsorted)
	// [b,e) = [b, pivot)[pivot,e)
	for ()
	{
		assert(std::is_sorted(b, pivot));
		// insert *pivot to [b, pivot)
		// [b, pivot)[*pivot] = [b, pivot)[v]
		// [b, pivot)[v] = [b, i)[v](i, pivot]
		// [b, pivot)[v] = [b, i)[i][i+1, pivot]
		for (TIter i = pivot; i != b; --i)
		{
			assert(std::is_sorted(b, i) && std::is_sorted(i, pivot + 1));
			auto next_i = i + 1;
			
			if (*next_i < *i)
				swap(*i, *next_i);
			else
				break;

			assert(std::is_sorted(b, i) && std::is_sorted(i, pivot + 1));
		}
		assert(std::is_sorted(b, pivot));
	}
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
	//test_binary_search(binary_search_2<Vec::iterator, int>);

	Vec test = { 10, 9 , 12, 23, 0, 7, 8, 6, 1 };
	naive_sort(test.begin(), test.end());
	system("pause");
    return 0;
}
// метод мат. индукции
// => inference
// <=> 
// less wrong ru 
// множества чисел
// доказать свойства логарифмов
// Monte Carlo algo
// RANSAC