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
    if (val2 > val) {
        return 0;
    }
    uint64_t result{1};
    for (uint64_t i=val; i>val-val2; i--) {
        result*=i;
    }
    return result;
}


uint64_t combination(uint64_t val, uint64_t val2) {
    // TODO
    if (val2 > val) {
        return 0;
    }
    if (val2 < val-val2) { // val2 always be the smaller one
        val2 = val-val2;
    }
    uint64_t numerator{1};
    for (uint64_t i=val; i>val-val2; i--) {
        numerator*=i;
    }
    uint64_t denominator{factorial(val2)};
    return numerator/denominator;
}
