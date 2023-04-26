#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "functions.h"

typedef long double ld;

typedef struct {
    ld l, r;
} segment;

typedef struct {
    ld ans;
    int cnt;
} root_ans;

segment iter_secant(ld (*f)(ld), ld (*fd)(ld), 
                    ld (*g)(ld), ld (*gd)(ld), 
                    ld a, ld b) {
    #define F(x) (f(x) - g(x))
    #define Fd(x) (fd(x) - gd(x))
    #define Fdd (Fd(b) - Fd(a))
    
    ld k = (F(b) - F(a)) / (b - a);
    ld c = a - F(a) / k;
    
    if (Fd(a) * Fdd > 0) {
        return (segment){c, b};
    } else {
        return (segment){a, c};
    }
    
    #undef Fdd
    #undef Fd
    #undef F
}

segment iter_newton(ld (*f)(ld), ld (*fd)(ld), 
                    ld (*g)(ld), ld (*gd)(ld), 
                    ld a, ld b) {
    #define F(x) (f(x) - g(x))
    #define Fd(x) (fd(x) - gd(x))
    #define Fdd (Fd(b) - Fd(a))
    
    if (Fd(a) * Fdd > 0) {
        ld c = b - F(b) / Fd(b);
        return (segment){a, c};
    } else {
        ld c = a - F(a) / Fd(a);
        return (segment){c, b};
    }
    
    #undef Fdd
    #undef Fd
    #undef F
}

root_ans root(ld (*f)(ld), ld (*fd)(ld),
              ld (*g)(ld), ld (*gd)(ld),
              ld a, ld b, ld eps) {
    int cnt = 0;
    bool newton = true;
    while (b - a >= eps) {
        segment res;
        if (newton)
            res = iter_newton(f, fd, g, gd, a, b);
        else
            res = iter_secant(f, fd, g, gd, a, b);
        a = res.l;
        b = res.r;
        newton = !newton;
        ++cnt;
    }
    return (root_ans){a, cnt};
}

ld integral(ld (*f)(ld), ld a, ld b, ld eps) {
    int n = 2;
    ld h = (b - a) / n;
    ld curr = h/3 * (f(a) + 4*f((a+b)/2) + f(b));
    ld diff = h/3 * f((a+b)/2);
    const ld p = 15e-1;
    while (true) {
        ld curr2 = curr;
        curr2 -= 2*diff;
        n *= 2;
        h /= 2;
        curr2 /= 2;
        diff = 0;
        for (int i = 1; i < n; i += 2)
            diff += f(a + i*h);
        diff *= h/3;
        curr2 += 4*diff;
        if (p * fabs(curr2 - curr) < eps) {
            curr = curr2;
            break;
        }
        curr = curr2;
    }
    return curr;
}

ld ft1(ld x) {
    return 40 * pow(x, 4) - 820 * pow(x, 3) + 5280 * pow(x, 2) - 11500 * x + 7000;
}

ld ft1d(ld x) {
    return 160 * pow(x, 3) - 2460 * pow(x, 2) + 10560 * x - 11500;
}

ld ft1i(ld x) {
    return 8*pow(x, 5) - 205*pow(x, 4) + 1760*pow(x, 3) - 5750*pow(x, 2) + 7000*x;
}

ld ft2(ld x) {
    return 0;
}

void test() {
    puts("Testing...");
    puts("Roots of the test function:");
    const ld eps1 = 0.001 / (4*500);
    const ld eps2 = 0.001 / 2;
    ld p1 = root(ft1, ft1d, ft2, ft2, 0.5, 1.5, eps1).ans;
    ld p2 = root(ft1, ft1d, ft2, ft2, 2.0, 3.0, eps1).ans;
    ld p3 = root(ft1, ft1d, ft2, ft2, 6.5, 7.5, eps1).ans;
    ld p4 = root(ft1, ft1d, ft2, ft2, 9.5, 10.5, eps1).ans;
    printf("%.7Lf %.7Lf %.7Lf %.7Lf\n", p1, p2, p3, p4);

    puts("Area under the curve between the roots:");
    ld i12 = integral(ft1, p1, p2, eps2);
    ld ic12 = -977.0625;
    ld i23 = integral(ft1, p2, p3, eps2);
    ld ic23 = 11345.0625;
    ld i34 = integral(ft1, p3, p4, eps2);
    ld ic34 = -8181.0;
    printf("Area 1-2, approximate: %.7Lf\n"
           "Area 1-2, analytic:    %.7Lf\n"
           "Area 1-2, error:       %.7Lf\n"
           "Area 2-3, approximate: %.7Lf\n"
           "Area 2-3, analytic:    %.7Lf\n"
           "Area 2-3, error:       %.7Lf\n"
           "Area 3-4, approximate: %.7Lf\n"
           "Area 3-4, analytic:    %.7Lf\n"
           "Area 3-4, error:       %.7Lf\n",
           i12, ic12, fabsl(i12 - ic12),
           i23, ic23, fabsl(i23 - ic23),
           i34, ic34, fabsl(i34 - ic34));

    //printf("%.5Lf %.5Lf %.5Lf %.5Lf %.5Lf %.5Lf\n",
    //      f1(1), f1d(1), f2(1), f2d(1), f3(1), f3d(1));
}

int main(int argc, char* argv[]) {
    const ld EPS = 0.001;
    const ld M = 7.0;
    const ld EPS1 = EPS / (4*M);
    const ld EPS2 = EPS / 2;

    const ld L12 = 3.0;
    const ld R12 = 4.0;
    const ld L23 = 3.0;
    const ld R23 = 4.0;
    const ld L31 = 0.5;
    const ld R31 = 1.0;

    const char HELP[] =
        "Calculate the area between three predetermined curves.\n"
        "   --help: print this help message\n"
        "   --root: print the intersection points\n"
        "   --iters: print the number of iterations performed\n"
        "   --test: run tests\n";

    root_ans p12 = root(f1, f1d, f2, f2d, L12, R12, EPS1);
    root_ans p23 = root(f2, f2d, f3, f3d, L23, R23, EPS1);
    root_ans p31 = root(f3, f3d, f1, f1d, L31, R31, EPS1);

    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0) {
            puts(HELP);
        } else if (strcmp(argv[i], "--root") == 0) {
            printf("The intersection points are: %.5Lf, %.5Lf, %.5Lf\n", p31.ans, p23.ans, p12.ans);
        } else if (strcmp(argv[i], "--iters") == 0) {
            printf("The iteration counts for the roots are: %d, %d, %d\n", p31.cnt, p23.cnt, p12.cnt);
        } else if (strcmp(argv[i], "--test") == 0) {
            test();
        }
    }

    ld ans = 0;
    ans += fabsl(integral(f1, p31.ans, p23.ans, EPS2) - integral(f3, p31.ans, p23.ans, EPS2));
    ans += fabsl(integral(f1, p23.ans, p12.ans, EPS2) - integral(f2, p23.ans, p12.ans, EPS2));
    printf("The area between the curves is: %.5Lf\n", ans);

    return 0;
}
