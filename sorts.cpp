#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

template<typename TIter>
void my_swap( TIter first, TIter second )
{
	auto tmp = *first;
	*first = *second;
	*second = tmp;
}

template<typename TIter>
void naive_sort( TIter b, TIter e )
{
	// [b,e)
	for ( auto i = b; i != e; ++i )
	{
		for ( auto j = i + 1; j != e; ++j  )
		{
			if ( *j < *i )
			{
				my_swap( i, j );
			}
		}
	}
}


// { |10, 9, 1, 5, 8, 3, 7, 4, 6 }
// { 1, |9, 10, 5, 8, 3, 7, 4, 6 }
template<typename TIter>
void selection_sort( TIter b, TIter e )
{
	// [b,e) = [sorted)[unsorted)
	// [b,e) = [b,pivot)[pivot, e)
	for ( auto pivot = b; pivot != e; ++pivot )
		my_swap( pivot, std::min_element(pivot, e) );
}

template<typename TIter>
void bubble_sort(TIter b, TIter e)
{
	for (auto pivot = e; pivot != b; --pivot)
	{
		for (auto i = b; i != pivot-1; ++i)
		{
			auto next_i = i + 1;
			if (*next_i < *i)
				my_swap(i, next_i);
		}
	}
}

template<typename TIter>
void insertion_sort(TIter b, TIter e)
{
	// [b,e) = [sorted)[unsorted)
	// [b,e) = [b, pivot)[pivot, e)
	for (auto pivot = b; pivot != e; ++pivot)
	{
		for (auto i = pivot; i != b; --i)
		{
			auto prev_i = i - 1;
			if (*i < *prev_i )
			{
				my_swap(i, prev_i);
			}
			else
			{
				break;
			}
		}
	}
}

typedef std::vector<int> Vec;

template<typename T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& vector)
{
	for (auto el : vector)
	{
		o << el << ", ";
	}
	return o;
}


template<typename T, typename TFunc, typename P1>
void test(T expected, TFunc f, P1 p1)
{
	auto result = f(p1);
	std::cout << "---------------------------------------------------------------------" << std::endl;
	std::cout << "f(" << p1  << ") = " << result << std::endl;

	if (!std::equal( p1.begin(), p1.end(), expected.begin() ))
	{
		std::cout << "Expected: " << expected << "; Result: " << result << std::endl;
		system("pause");
	}
	else
	{
		std::cout << "OK!" << std::endl;
	}
}


template<typename T, typename TFunc, typename P1>
void test_merge(T expected, TFunc f, P1 p1, P1 p2)
{
	auto result = f(p1, p2);
	std::cout << "---------------------------------------------------------------------" << std::endl;
	std::cout << "f(" << p1 << ") = " << result << std::endl;

	if (!std::equal(result.begin(), result.end(), expected.begin()))
	{
		std::cout << "Expected: " << expected << "; Result: " << result << std::endl;
		system("pause");
	}
	else
	{
		std::cout << "OK!" << std::endl;
	}
}

template<class TFunc>
void test_sort(TFunc sort)
{

	auto adaptor = [sort](Vec input)
	{
		sort( input.begin(), input.end() );
		return input;
	};

	test(Vec({}), adaptor, Vec({}));
	test(Vec({1}), adaptor, Vec({1}));
	test(Vec({ 1, 2 }), adaptor, Vec({ 1, 2 }));
	test(Vec({ 2, 1 }), adaptor, Vec({ 2, 1 }));
	test(Vec({ 2, 2 }), adaptor, Vec({ 2, 2 }));
	test(Vec({ 3, 2, 1 }), adaptor, Vec({ 3, 2, 1 }));
	test(Vec({ 1,2,3,4,5,6,7,8,9,10 }), adaptor, Vec({ 7,1,9,8,6,2,3,4,10,5 }));
}

/*
Merge - объединяет 2 отсорт последоват.
void Merge( b1, e1, b2, e2 )
не пересекаются
*/

template<class TFunc>
void test_merge(TFunc merge)
{

	auto adaptor = [merge](Vec in1, Vec in2)
	{
		return merge(in1.begin(), in1.end(), in2.begin(), in2.end());
	};

	test_merge(Vec({}), adaptor, Vec({}), Vec({}));
	test_merge(Vec({1,1}), adaptor, Vec({1}), Vec({1}));
	test_merge(Vec({1,2,3,4,6,7,8}), adaptor, Vec({ 2,4,6,8 }), Vec({ 1,3,7 }));
}

template<typename TIter>
void merge(TIter b1, TIter e1, TIter b2, TIter e2, TIter out)
{

	auto p1 = b1;
	auto p2 = b2;

	while (p1 != e1 && p2 != e2)
	{
		if (*p1 < *p2)
		{
			*out = *p1;
			++out;
			++p1;
		}
		else
		{
			*out = *p2;
			++out;
			++p2;
		}
	}
	for (p1; p1 != e1; ++p1)
	{
		*out = *p1;
		++out;
	}
		
	for (p2; p2 != e2; ++p2)
	{
		*out = *p2;
		++out;
	}
}

template<typename TIter>
void merge_sort(TIter b, TIter e, TIter buff)
{
	/*
	Любая рекурсия должна начинаться с проверки выхода из рекурсии
	*/
	if (e - b > 1)
	{
		TIter m = b + (e - b) / 2; // O(1)
		merge_sort(b, m, buff);
		merge_sort(m, e, buff);
		merge(b, m, m, e, buff); // O(n)
		std::copy(buff, buff + (e - b), b); // O(n)
	}
	assert(std::is_sorted(b, e));
}

template<typename TIter>
void outer_merge_sort(TIter src_b, TIter src_e, TIter dst_b)
{
	if (e - b <= 1)
	{
		std::copy(src_b, src_e, dst_b);
		return;
	}
	size_t size = src_e - src_b;
	size_t m = size / 2;
	auto src_m = src_b + m;
	auto dst_m = dst_b + m;
	auto dst_e = dst_b + size;

	outer_merge_sort(src_b, src_m, dst_b);
	outer_merge_sort(src_m, src_e, dst_m);
	merge(dst_b, dst_m, dst_m, dst_e, src_b);
	assert(std::is_sorted(b, e));
}

//quick sort
/*
[ )[x][  )
 <     =<
 partition
*/
template<typename TIter>
void move_element(TIter from, TIter to)
{
	assert(from <= to);
	if (from == to)
		return;

	auto current = from;
	auto next = from + 1;
	if ( next <= to )
	{ }
}

template<typename TIter>
TIter my_partition(TIter b, TIter p, TIter e)
{
	assert(b <= p && p <=e);

	auto x = *p;
	auto m = b;
	while ()
	{
		// [b, p)[p][p, bu)[bu, e)
		// [b, p)[p][p+1, bu) [bu] [bu +1, e)
		if (*bu > *p)
		{
			// [b, p)[p, bu] [bu+1, e)
			bu++;
		}
		else
		{
			// [b, p)[p][p+1, bu) [bu] [bu+1, e)
			// [b, bu][m][m+1,bu)[m+1, bu)[m+1][bu+1,e)
			std::swap(*m, *bu);
		}
	}

	/*if (e - b <= 1)
		return p;

	auto newPivot = p;
	for (auto i = p - 1; i != b; --i)
	{
		if (*i >= *p)
		{
			std::iter_swap(i, p);
			newPivot = i;
		}
	}

	for (auto j = newPivot + 1; j < e; ++j)
	{
		if (*j < *newPivot)
		{
			std::iter_swap(j, newPivot);
			newPivot = j;
		}
	}

	/*assert(*std::max(b, pivot) < *pivot);
	assert(*std::min(pivot + 1) >= *pivot);*/
	return newPivot;
}

template<typename TIter>
void quick_sort(TIter b, TIter e)
{
	if (e - b <= 1)
		return;

	//TIter pivot = b + ( e - b) / 2;
	pivot = partition(b, e, *b);
	quick_sort(b, pivot);
	quick_sort(pivot + 1, e);
}

int main()
{
	//test_sort( naive_sort<Vec::iterator> );
	//test_merge(merge<Vec::iterator>);
	Vec in1 = { 3, 9, 7, 4, 5, 10, 1, 2 };

	auto pivot = in1.begin() + 4;
	auto res = my_partition(in1.begin(), pivot, in1.end());
	
	std::cout << in1 << std::endl;
	/*Vec out( in1.size() );
	merge_sort(in1.begin(), in1.end(), out.begin() );
	std::cout << out << std::endl;*/
	/*Vec in1 = { 1, 3, 5, 7 };
	Vec in2 = { 2, 4, 6, 8 };

	std::cout << merge(in1.begin(), in1.end(), in2.begin(), in2.end()) << std::endl;*/
	system( "pause" );

	return 0;
}