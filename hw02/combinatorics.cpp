#include "combinatorics.h"


uint64_t factorial(uint64_t val) {
    // TODO
    uint64_t result{1};
    for (uint64_t i=val; i>0; i--) {
        result*=i;
    }
    return result;
}


uint64_t permutation(uint64_t val, uint64_t val2) {
    // TODO
    uint64_t denominator{factorial(val-val2)};
    uint64_t numerator{factorial(val)};
    return numerator/denominator;
}


uint64_t combination(uint64_t val, uint64_t val2) {
    // TODO
    uint64_t denominator{factorial(val2)*factorial(val-val2)};
    uint64_t numerator{factorial(val)};
    return numerator/denominator;
}
