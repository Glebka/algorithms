#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

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
Merge - ���������� 2 ������ ����������.
void Merge( b1, e1, b2, e2 )
�� ������������
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
Vec merge(TIter b1, TIter e1, TIter b2, TIter e2)
{
	Vec result;

	auto p1 = b1;
	auto p2 = b2;

	while (p1 != e1 && p2 != e2)
	{
		if (*p1 < *p2)
		{
			result.push_back(*p1);
			++p1;
		}
		else
		{
			result.push_back(*p2);
			++p2;
		}
	}
	for (p1; p1 != e1; ++p1)
		result.push_back(*p1);
	for (p2; p2 != e2; ++p2)
		result.push_back(*p2);
	return result;
}

int main()
{
	//test_sort( naive_sort<Vec::iterator> );
	test_merge(merge<Vec::iterator>);
	/*Vec in1 = { 1, 3, 5, 7 };
	Vec in2 = { 2, 4, 6, 8 };

	std::cout << merge(in1.begin(), in1.end(), in2.begin(), in2.end()) << std::endl;*/
	system( "pause" );

	return 0;
}