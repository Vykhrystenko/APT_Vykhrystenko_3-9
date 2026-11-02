
#pragma once

#ifdef LINEARSYSTEMS_EXPORTS
#define LINEARSYSTEMS_API __declspec(dllexport)
#else
#define LINEARSYSTEMS_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

    LINEARSYSTEMS_API int solve(
        const double* A,
        const double* b,
        int n,
        double* x
    );

    LINEARSYSTEMS_API double residual(
        const double* A,
        const double* b,
        const double* x,
        int n
    );

    LINEARSYSTEMS_API int checkSolution(
        const double* A,
        const double* b,
        const double* x,
        int n,
        double tolerance
    );

#ifdef __cplusplus
}
#endif
