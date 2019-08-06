#include <iostream>
#include <vector>
#include "range/v3/all.hpp" // Brute force; slows down compile. Should use individuals.

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
    // These examples are from Eric Niebler's blog, maybe modified
    // https://ericniebler.github.io/range-v3/index.html#tutorial-quick-start

    // Let's do a vector and a view adaptor
    std::vector<int> vi{ 1,2,3,4,5,6,7,8,9,10 };
    cout << "vi     ";
    print_container(vi) << " (simple std vector)\n";

    auto rng_adapted = vi |
         ranges::view::remove_if([](int i) {return i % 2 == 1; }) |
         ranges::view::transform([](int i) {return std::to_string(i); });

    cout << "rng_adapted     ";
    print_container(rng_adapted)
         << " : rng_adapted == " << R"({"2","4","6","8","10"})" << "\n";   // rng == {"2","4","6","8","10"};

    cout << "vi     ";
    print_container(vi) << ": vi  == {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}  (Unchanged)\n";

    // Now let's look at the view after its source has been modified

    vi.push_back(3);
    cout << "vi     ";

    print_container(vi);

    cout << "rng_adapted after vi mod    ";
    print_container(rng_adapted);  // No Good. Why? (Hint: what is the source of rng_adapted?)


    // Let's try a range action.
    // Sort a vector and make it unique,
    // Cool note: The vector's end is gone, not like std::unique which needs erase()
    vi = { 6,2,9,6,5,7,1,3,1,5 };

    auto r_action = std::move(vi) | ranges::action::sort | ranges::action::unique;

    cout << "raction    ";
    print_container(r_action);  // vi2 == {1,2,3,5,6,7,9};
    cout << "vi    ";
    print_container(vi );   // vi is gone (moved from). Lucky we don't crash
    cout << "------\n";

    // Same thing in-place using the 'pipe equals' operator
    vi = { 6,2,9,6,5,7,1,3,1,5 };
    vi |= ranges::action::sort | ranges::action::unique;
    print_container(vi);  // vi == {1,2,3,5,6,7,9};

    int xxx = 0;
}


