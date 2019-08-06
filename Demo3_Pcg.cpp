#include <iostream>
#include <vector>
#include <list>
#include <random>

#include "range/v3/all.hpp" // Brute force; slows down compile. Should use individuals.


/*
https://ericniebler.github.io/range-v3/index.html#tutorial-quick-start
*/

#ifdef USE_PCG
    #include "pcg_random.hpp"
#endif

using std::cout;
using std::vector;
using std::list;


// Really just shuffle() - but choosing the random engine
void scramble(vector< int >& vec)
{
#ifdef USE_PCG
    // PCG Random is a header C++ library for by Melissa O'Neill.
    // https://github.com/imneme/pcg-cpp
    // Make a random engine for use with shuffle, etc
    // Use PCG Random Generator. I've maybe fallen for O'Neill's kool-aid
    // but I like this library and the simple wrapper / example code
    // Seed with a real random value, if available
    // Code is from O'Neill's PCG examples, based on cppreference
    pcg_extras::seed_seq_from<std::random_device> seed_source;
    pcg32 rngen(seed_source);

#else   // Just use C++ built-in generator
    // Make a random engine for use with shuffle, etc
    // Seed with a real random value, if available
    // From cppreference https://en.cppreference.com/w/cpp/numeric/random
    std::random_device r;
    std::seed_seq seed{ r(), r(), r(), r(), r(), r(), r(), r() };
    std::mt19937 rngen(seed);
#endif
    ranges::shuffle(vec, rngen);
}


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


    // Create a vector of ints to shuffle, etc
    std::vector<int> vecUnshuffled = ranges::view::ints(100, 115);
    cout << "\n\nA set of ints in a vector\n\t";
    print_container(vecUnshuffled);
	cout << '\n';

    // Copy with the ranges::copy algorithm
    auto vecCopy = ranges::copy(vecUnshuffled);
    cout << "Copied without iterators\n\t";
    print_container(vecCopy);
	cout << "Tick\n";

    // Shuffle the vector with ranges::shuffle
    // - now the variable name is not indicative (sorry!)
    scramble(vecUnshuffled); // My little shuffle function to use PCG or MT19337
    cout << "Randomly permuted\n\t";
    print_container(vecUnshuffled);  // now the name is not good, eh?



//*
    // Sort algorithm
    // Real limitation here.... GCC version will fail for non-ints
    // TODO: Which (or both?) are standards compliant?
#if _MSC_VER
    ranges::sort(vecUnshuffled, std::greater());
#else  // GCC
    ranges::sort(vecUnshuffled, std::greater<int>());
#endif
    cout << "Sorted back into order, but reversed just for fun\n\t";
    print_container(vecUnshuffled);
	cout << '\n';

    // A few tricks with lists, just so we see things don't depend on random access
    std::list<int> list1 = ranges::view::ints(200, 215);
    cout << "\n\nInts in a list (list1):\n\t";
    print_container(list1);
	cout << '\n';

    // Cascaded function calls on list. NO ITERATORS, etc.
    auto listPipedTransforms = list1
        | transform(Add_3)
        | filter(EvenFilter)
        | transform(MultX2);

    cout << "list1 Piped thru: "
            "[add 3, filter out the odds, multiply by two]\n\t";
    print_container(listPipedTransforms);

    // Let's print out list and vector view sizes.
    // Quiz: Why don't we just call size()?
    cout << "List size information\n";
    cout << "  listPipedTransforms # elements  "
         << distance( listPipedTransforms.begin(), listPipedTransforms.end()) << '\n';
    cout << "  sizeof listPipedTransforms   bytes     "
         << "bytes " << sizeof(listPipedTransforms) << '\n';
	cout << '\n';


	auto vecPipedTransforms = vecUnshuffled
        | transform(Add_3)
        | filter(EvenFilter)
        | transform(MultX2);

	cout << "Compare to vector size information\n";
    cout << "  vecPipedTransforms # elements  "
         << distance( vecPipedTransforms.begin(), vecPipedTransforms.end()) << '\n';
    cout << "  sizeof vecPipedTransforms   bytes     "
         << "bytes " << sizeof(vecPipedTransforms) << '\n';
	cout << '\n';
/**/

	int xxx = 0;
}


