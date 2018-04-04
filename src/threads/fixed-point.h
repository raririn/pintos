#ifndef THREAD_FIXED_POINT_H
#define THREAD_FIXED_POINT_H

/* <inttypes.h> should be included in .c files, so it should be safe to call int64_t() here. */
/* x and y are fixed-point numbers, n is an integer. Fixed-point numbers are in signed p.q format where p + q = 31, and f = 1<<q */

/* The constants will be used in caculation */
#define FP_p 15
#define FP_q 16
#define FP_f 2**q

/* Convert n to fixed point */
#define FP_INT2FP(n) (n * FP_f)
/* Convert x to integer (rounding toward zero) */
#define FP_FP2INT_ROUNDZERO(x) (x / FP_f)
/* Convert x to integer (rounding to nearest) */
#define FP_FP2INT_ROUNDNEAR(x) ((x) >= 0 ? ((x) + (FP_f) / 2) / (FP_f) : ((x) - (FP-f) / 2) / (FP_f))
/* Add x and y */
#define FP_ADD(x, y) (x + y)
/* Subtract y from x */
#define FP_SUB(x, y) (x - y)
/* Add x and n */
#define FP_FP_ADD_INT(x, n) (x + n * FP_f )
/* Subtract n from x */
#define FP_FP_SUB_INT(x, n) (x - n * FP_f)
/* Multiply x by y */
#define FP_MUL(x, y) (((int64_t)x) * y / FP_f)
/* Multiply x by n */
#define FP_INT_MUL_FP(x, n) 
/* Divide x by y */
#define FP_DIV(x, y) (((int64_t)x) *FP_f / y)
/* Divide x by n */
#define FP_FP_DIV_INT(x, n) (x / n)
#endif /* threads/fixed-point.h */