#include <stdio.h>
#include "set.hpp"

using namespace misc;

int main() {
    using set = type_set<int, int, float, double>;
    printf(set::in<int> ? "in\n" : "not in\n");
}