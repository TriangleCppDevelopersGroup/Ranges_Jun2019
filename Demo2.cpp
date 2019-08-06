#include <iostream>
#include <vector>

#include "range/v3/all.hpp" // Brute force; slows down compile. Should use individuals.


/*
https://ericniebler.github.io/range-v3/index.html#tutorial-quick-start
*/


using std::cout;
using std::vector;


// Simple utility generic lambda (lambda template - tipoff is auto in args)
// for printing vector, list, etc
// Note: this is one case where we need to include the trailing return type
// (Can you see why?)
auto print_container = [](
    auto container_arg,
    char delim = ' ',
    std::ostream& os = std::cout ) ->std::ostream&
{
	for (const auto& item : container_arg)
		os << item << delim;
	os << '\n';
	return os;
};


int main()
{
    // Now, let's see how simple our code can look with ranges
    using ranges::view::filter;
    using ranges::view::transform;
    using ranges::action::shuffle;

    // Here are the filter function definitions.
    // They don't need to be lambdas. Just demoing different declarations.
    // The 'auto' here is because lambdas are object types, and you & I
    // would have a real hassle figuring them out  (probably impossible in
    // cross-platform sense)
    auto EvenFilter  = []( int n ) -> int { return n % 2 == 0; };
    auto MultX2      = []( int n )        { return n * 2;      };
    auto Add_3       = []( int n )        { return n + 3;      };

    // Fill a vector with iota of numbers. Do a few tricks
    vector< int > vecNumbers = ranges::view::ints(0, 10);

    // Let's make two cycles of ints so when we later count a value we get more than one
    vecNumbers.reserve(2 * vecNumbers.size());
    std::copy(vecNumbers.begin(), vecNumbers.end(), std::back_inserter( vecNumbers ));
    cout << "Vector of numbers to manipulate\n\t";
    print_container(vecNumbers);
	cout << '\n';

    // count algorithm: count occurences of 6
    auto c = ranges::count(vecNumbers, 6);
    cout << "Counting occurences of the value 6: " << c << '\n';;

    // 1. Pass a vector to a filtering function. NO ITERATORS, etc
    auto vecEvenNumbers = vecNumbers | filter(EvenFilter);
    cout << "Filtered out the odds\n\t";
    print_container(vecEvenNumbers);
	cout << '\n';

    // 2a. Cascaded function calls on vector. NO ITERATORS, etc.
    auto vecEvenDoubled = vecNumbers | filter(EvenFilter) | transform(MultX2);
    cout << "Filtered out odds, Multiplied by two\n\t";
    print_container(vecEvenDoubled);
	cout << '\n';

   // 2b. Cascaded function calls on vector. NO ITERATORS, etc.
	std:: vector< int > vecNumbersPipe { 1, 3, 5, 7, 8, 9, 10 };
	auto vecPipedTransforms = vecNumbersPipe
		| transform(Add_3)
		| filter(EvenFilter)
        | transform(MultX2);


    cout << "Piped thru: add 3, filter out the odds, multiply by two\n\t";
    print_container(vecPipedTransforms);
    cout << "sizeof vecPipedTransforms\n\t";
    cout << "bytes " << sizeof(vecPipedTransforms) << '\n';
    cout << '\n';

    int xxx = 0;
}


