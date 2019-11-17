#include <iostream>

#include "set.hpp"

using namespace misc;

struct A {};
struct B {};
struct C {};

using set1 = type_set<A, type_set<A>>;
using set2 = type_set<type_set<A>>;
using set3 = set1::add_element<B>;

int main() {
    std::cout << (set1::contains<set2> ? "contains" : "not contains") << std::endl;				// C2131
	std::cout << (type_set<>::contains<type_set<>> ? "contains" : "not contains") << std::endl;	// C1001
}