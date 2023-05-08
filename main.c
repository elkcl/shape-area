#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"


/* Структуры и typedef'ы */

typedef long double ld;

typedef struct {
    ld l, r;
} segment;

// Результат функции root
typedef struct {
    ld ans;
    int cnt;
} root_ans;

// Функция, её производная и максмиальное приращение при маленьком приращении аргумента (для eps1)
typedef struct {
    ld (*f)(ld);
    ld (*fd)(ld);
    ld maxv;
} func_meta;

// Описание теста (номер функции, промежутки поиска двух корней, точность, аналитически посчитанный ответ
typedef struct {
    int func_id;
    ld l1, r1, l2, r2;
    ld eps;
    ld ans;
} test_def;


/* Функции для поиска корней и вычисления интегралов */

// Метод хорд
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

// Метод касательных
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

// Комбинированный метод
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

// Численное интегрирование методом Симпсона
ld integral(ld (*f)(ld), ld a, ld b, ld eps) {
    int n = 2;
    ld h = (b - a) / n;
    ld curr = 0;
    ld curr2 = h/3 * (f(a) + 4*f((a+b)/2) + f(b));
    // Сумма членов с коэфф-ми 4 в curr2, поддерживается с целью избежания пересчёта значений
    ld diff = h/3 * f((a+b)/2);
    // Коэффициент 1/15 в правиле Рунге для метода Симпсона
    const ld p = 1.0/15;
    do {
        curr = curr2;
        curr2 -= 2*diff;
        n *= 2;
        h /= 2;
        curr2 /= 2;
        diff = 0;
        for (int i = 1; i < n; i += 2) {
            diff += f(a + i * h);
        }
        diff *= h/3;
        curr2 += 4*diff;
    } while (p * fabsl(curr2 - curr) >= eps);
    return curr2;
}


/* Дополнительные кривые для тестирования */

// Тестовая функция 4
ld f4(ld x) {
    return 40 * powl(x, 4) - 820 * powl(x, 3) + 5280 * powl(x, 2) - 11500 * x + 7000;
}

// Производная тестовой функции 4
ld f4d(ld x) {
    return 160 * powl(x, 3) - 2460 * powl(x, 2) + 10560 * x - 11500;
}

// Тестовая функция 5
ld f5(ld x) {
    return sinl(x);
}

// Производная тестовой функции 5
ld f5d(ld x) {
    return cosl(x);
}

// Прямая y = 0
ld f0(ld x) {
    return 0;
}


/* Описания кривых и тестов */

func_meta funcs[] = {
     //  f,  fd,  maxv
        {f0, f0,  0.00001},
        {f1, f1d, 1.0},
        {f2, f2d, 1.0},
        {f3, f3d, 1.0},
        {f4, f4d, 500.0},
        {f5, f5d, 0.5}
};

test_def tests[] = {
     //  id, l1,   r1,   l2,   r2,   eps,    ans
        {4,  0.8,  1.1,  2.3,  2.8,  0.0001, 977.0625},
        {4,  2.3,  2.8,  6.67, 7.1,  0.0001, 11345.0625},
        {4,  6.42, 7.12, 9.91, 11.5, 0.0001, 8181.0},
        {5,  2.95, 3.5,  6.1,  7.2,  0.0001, 2.0}
};


/* Полное тестирование */

bool full_test(void) {
    puts("====Testing====");
    for (int i = 0; i < sizeof(tests) / sizeof(tests[0]); ++i) {
        printf("---Test %d---\n", i + 1);
        ld eps1 = tests[i].eps / (4 * funcs[tests[i].func_id].maxv);
        ld eps2 = tests[i].eps / 2;

        root_ans p1 = root(funcs[tests[i].func_id].f,
                           funcs[tests[i].func_id].fd,
                           f0, f0, tests[i].l1, tests[i].r1, eps1);
        printf("Root 1: %.7Lf (%d iterations performed)\n", p1.ans, p1.cnt);
        root_ans p2 = root(funcs[tests[i].func_id].f,
                           funcs[tests[i].func_id].fd,
                           f0, f0, tests[i].l2, tests[i].r2, eps1);
        printf("Root 2: %.7Lf (%d iterations performed)\n", p2.ans, p2.cnt);

        ld area = fabsl(integral(funcs[tests[i].func_id].f, p1.ans, p2.ans, eps2));
        printf("Calculated area: %.7Lf\n", area);
        printf("Expected area: %.7Lf\n", tests[i].ans);
        printf("Error: %.10Lf\n", fabsl(area - tests[i].ans));
        printf("Expected error: %.7Lf\n", tests[i].eps);
        if (fabsl(area - tests[i].ans) < tests[i].eps) {
            puts("---Test passed!---\n");
        } else {
            puts("---Test failed!---\n");
            return false;
        }
    }
    putchar('\n');
    return true;
}

int main(int argc, char* argv[]) {
    // Вычилсение eps1 и eps2
    const ld EPS = 0.001;
    const ld M = funcs[1].maxv;
    const ld EPS1 = EPS / (4*M);
    const ld EPS2 = EPS / 2;

    // Промежутки поиска корней
    const ld L12 = 3.0;
    const ld R12 = 4.0;
    const ld L23 = 3.0;
    const ld R23 = 4.0;
    const ld L31 = 0.5;
    const ld R31 = 1.0;

    // Текст справки
    const char HELP[] =
        "Usage:\n"
        "   --help: print this help message\n"
        "   --roots: print the intersection points\n"
        "   --iters: print the number of iterations performed\n"
        "   --test-root: test the root function with the parameters given in the format \"--test-root <func1_id> <func2_id> <from> <to> <eps>\"\n"
        "   --test-integral: test the integral function with the parameters given in the format \"--test-integral <func_id> <from> <to> <eps>\"\n"
        "   --full-test: perform a full test as in the report\n";


    // Поиск корней
    root_ans p12 = root(f1, f1d, f2, f2d, L12, R12, EPS1);
    root_ans p23 = root(f2, f2d, f3, f3d, L23, R23, EPS1);
    root_ans p31 = root(f3, f3d, f1, f1d, L31, R31, EPS1);

    // Обработка ключей
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0) {
            puts(HELP);
        } else if (strcmp(argv[i], "--roots") == 0) {
            printf("The intersection points are: %.5Lf, %.5Lf, %.5Lf\n", p31.ans, p23.ans, p12.ans);
        } else if (strcmp(argv[i], "--iters") == 0) {
            printf("The iteration counts for the roots are: %d, %d, %d\n", p31.cnt, p23.cnt, p12.cnt);
        } else if (strcmp(argv[i], "--test-root") == 0) {
            if (i + 5 >= argc) {
                puts("Syntax error!");
                puts(HELP);
                return 1;
            }
            int func1_id = (int) strtol(argv[i + 1], NULL, 10);
            int func2_id = (int) strtol(argv[i + 2], NULL, 10);
            ld l = strtold(argv[i + 3], NULL);
            ld r = strtold(argv[i + 4], NULL);
            ld eps = strtold(argv[i + 5], NULL);
            root_ans rans = root(funcs[func1_id].f, funcs[func1_id].fd, funcs[func2_id].f, funcs[func2_id].fd, l, r, eps);
            printf("Root: %.7Lf (%d iterations performed)\n", rans.ans, rans.cnt);
            i += 5;
        } else if (strcmp(argv[i], "--test-integral") == 0) {
            if (i + 4 >= argc) {
                puts("Syntax error!");
                puts(HELP);
                return 1;
            }
            int func_id = (int) strtol(argv[i + 1], NULL, 10);
            ld l = strtold(argv[i + 2], NULL);
            ld r = strtold(argv[i + 3], NULL);
            ld eps = strtold(argv[i + 4], NULL);
            ld ians = integral(funcs[func_id].f, l, r, eps);
            printf("Integral: %.7Lf\n", ians);
            i += 4;
        } else if (strcmp(argv[i], "--full-test") == 0) {
            if (!full_test())
                return 1;
        }
    }

    // Вычисление площади
    ld ans = 0;
    ans += fabsl(integral(f1, p31.ans, p23.ans, EPS2) - integral(f3, p31.ans, p23.ans, EPS2));
    ans += fabsl(integral(f1, p23.ans, p12.ans, EPS2) - integral(f2, p23.ans, p12.ans, EPS2));
    printf("The area between the curves is: %.5Lf\n", ans);

    return 0;
}
