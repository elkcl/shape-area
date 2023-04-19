#include <stdio.h>
#include <stdbool.h>
#include <math.h>

typedef struct {
    double l, r;
    bool fin;
} segment;

segment iter_secant(double (*f)(double), double (*fd)(double), 
                    double (*g)(double), double (*gd)(double), 
                    double a, double b, double eps) {
    #define F(x) (f(x) - g(x))
    #define Fd(x) (fd(x) - gd(x))
    #define Fdd (Fd(b) - Fd(a))
    
    double k = (F(b) - F(a)) / (b - a);
    double c = a - F(a) / k;
    
    if (Fd(a) * Fdd > 0) {
        if (F(c) * F(c + eps) < 0) {
            return (segment){c, c, true};
        } else {
            return (segment){c, b, false};
        }
    } else {
        if (F(c) * F(c - eps) < 0) {
            return (segment){c, c, true};
        } else {
            return (segment){a, c, false};
        }
    }
    
    #undef Fdd
    #undef Fd
    #undef F
}

segment iter_newton(double (*f)(double), double (*fd)(double), 
                    double (*g)(double), double (*gd)(double), 
                    double a, double b, double eps) {
    #define F(x) (f(x) - g(x))
    #define Fd(x) (fd(x) - gd(x))
    #define Fdd (Fd(b) - Fd(a))
    
    if (Fd(a) * Fdd > 0) {
        double c = b - F(b) / Fd(b);
        if (F(c) * F(c - eps) < 0) {
            return (segment){c, c, true};
        } else {
            return (segment){a, c, false};
        }
    } else {
        double c = a - F(a) / Fd(a);
        if (F(c) * F(c + eps) < 0) {
            return (segment){c, c, true};
        } else {
            return (segment){c, b, false};
        }
    }
    
    #undef Fdd
    #undef Fd
    #undef F
}

double root(double (*f)(double), double (*fd)(double),
            double (*g)(double), double (*gd)(double),
            double a, double b, double eps) {
    bool newton = true;
    while (true) {
        segment res;
        if (newton)
            res = iter_newton(f, fd, g, gd, a, b, eps);
        else
            res = iter_secant(f, fd, g, gd, a, b, eps);
        if (res.fin)
            return res.l;
        a = res.l;
        b = res.r;
        newton = !newton;
    }
}

double integral(double (*f)(double), double a, double b, double eps) {
    int n = 2;
    double h = (b - a) / n;
    double curr = h/3 * (f(a) + 4*f((a+b)/2) + f(b));
    double diff = h/3 * f((a+b)/2)
    const double p = 1/15;
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

int main(int argc, char* argv[]) {
    
    return 0;
}
