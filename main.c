#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "functions.h"

typedef struct {
    double l, r;
} segment;

segment iter_secant(double (*f)(double), double (*fd)(double), 
                    double (*g)(double), double (*gd)(double), 
                    double a, double b) {
    #define F(x) (f(x) - g(x))
    #define Fd(x) (fd(x) - gd(x))
    #define Fdd (Fd(b) - Fd(a))
    
    double k = (F(b) - F(a)) / (b - a);
    double c = a - F(a) / k;
    
    if (Fd(a) * Fdd > 0) {
        return (segment){c, b};
    } else {
        return (segment){a, c};
    }
    
    #undef Fdd
    #undef Fd
    #undef F
}

segment iter_newton(double (*f)(double), double (*fd)(double), 
                    double (*g)(double), double (*gd)(double), 
                    double a, double b) {
    #define F(x) (f(x) - g(x))
    #define Fd(x) (fd(x) - gd(x))
    #define Fdd (Fd(b) - Fd(a))
    
    if (Fd(a) * Fdd > 0) {
        double c = b - F(b) / Fd(b);
        return (segment){a, c};
    } else {
        double c = a - F(a) / Fd(a);
        return (segment){c, b};
    }
    
    #undef Fdd
    #undef Fd
    #undef F
}

double root(double (*f)(double), double (*fd)(double),
            double (*g)(double), double (*gd)(double),
            double a, double b, double eps) {
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
    }
    return a;
}

double integral(double (*f)(double), double a, double b, double eps) {
    int n = 2;
    double h = (b - a) / n;
    double curr = h/3 * (f(a) + 4*f((a+b)/2) + f(b));
    double diff = h/3 * f((a+b)/2);
    const double p = 15e-1;
    while (true) {
        double curr2 = curr;
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

double ft1(double x) {
    return 0.1 * pow(x, 4) - 2.05 * pow(x, 3) + 13.2 * pow(x, 2) - 28.75 * x + 17.5;
}

double ft1d(double x) {
    return 0.4 * pow(x, 3) - 6.15 * pow(x, 2) + 26.4 * x - 28.75;
}

double ft2(double x) {
    return 0;
}

void test() {
    double p1 = root(ft1, ft1d, ft2, ft2, 0.5, 1.5, 0.001);
    double p2 = root(ft1, ft1d, ft2, ft2, 2.0, 3.0, 0.001);
    double p3 = root(ft1, ft1d, ft2, ft2, 6.5, 7.5, 0.001);
    double p4 = root(ft1, ft1d, ft2, ft2, 9.5, 10.5, 0.001);
    printf("%.4f %.4f %.4f %.4f\n", p1, p2, p3, p4);

    double i12 = integral(ft1, p1, p2, 0.001);
    double i23 = integral(ft1, p2, p3, 0.001);
    double i34 = integral(ft1, p3, p4, 0.001);
    printf("%.4f %.4f %.4f\n", i12, i23, i34);

    printf("%.4f %.4f %.4f %.4f %.4f %.4f\n",
           f1(1), f1d(1), f2(1), f2d(1), f3(1), f3d(1));
}

int main(int argc, char* argv[]) {
    test();
    return 0;
}
