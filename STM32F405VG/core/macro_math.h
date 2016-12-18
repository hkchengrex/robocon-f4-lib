#ifndef _MACRO_MATH
#define _MACRO_MATH

//Find max/min of 2~4 arguments
#define MAX_2(a, b) (a>b?a:b)
#define MAX_3(a, b, c) (a>b?(a>c?a:c):(b>c?b:c))
#define MAX_4(a, b, c, d) (MAX_2(MAX_2(a, b), MAX_2(c, d)))

#define GET_MAX_MACRO (_1, _2, _3, _4, MACRO, ...) (MACRO)
#define MAX(...) GET_MAX_MACRO(__VA_ARGS__, MAX_4, MAX_3, MAX_2)(__VA_ARGS__)

#define MIN_2(a, b) (a<b?a:b)
#define MIN_3(a, b, c) (a<b?(a<c?a:c):(b<c?b:c))
#define MIN_4(a, b, c, d) (MIN_2(MIN_2(a, b), MIN_2(c, d)))

#define GET_MIN_MACRO (_1, _2, _3, _4, MACRO, ...) (MACRO)
#define MIN(...) GET_MIN_MACRO(__VA_ARGS__, MIN_4, MIN_3, MIN_2)(__VA_ARGS__)

//Find median of a, b and c
#define MEDIAN(a, b, c) (MAX(MIN(a, b), MIN(MAX(a, b), c)))

//Find the value a, after lower bounded by b and upper bounded by c
#define CAP(a, b, c) (a<b?b:(a>c?c:a))

//Scale value a from a range from 0~b to 0~c
#define SCALE(a, b, c) (a*c/b)

#endif
