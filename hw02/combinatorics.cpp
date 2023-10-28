#include "combinatorics.h"


uint64_t factorial(uint64_t val) {
    // TODO
    return fact_tail(val, 1);
}

uint64_t fact_tail(uint64_t val, uint64_t acc){
    if (val == 0) return acc;
    return fact_tail(val-1, acc*val);
}

uint64_t permutation(uint64_t val, uint64_t val2) {
    // TODO
    if(val2 > val) return 0;
    return factorial(val)/factorial(val-val2);
}


uint64_t combination(uint64_t val, uint64_t val2) {
    // TODO
    
    return permutation(val,val2)/factorial(val2);
}
