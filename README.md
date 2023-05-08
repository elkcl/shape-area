# Вычисление корней уравнений и определенных интегралов
## Денисьев Илья, вариант 6/4/3

Программа для вычисления площади между заданными кривыми.

Кривые:
1) f1(x) = 0.6x + 3
2) f2(x) = (x - 2)^3 - 1
3) f3(x) = 3/x

Доп. кривые для тестирования:
1) f0(x) = 0
2) f4(x) = 20(x - 1)(x - 7)(2x - 5)(x - 10)
3) f5(x) = sin(x)

Требуемая точность: 0.001

## Использование программы

```
Usage:
    --help: print this help message
    --roots: print the intersection points
    --iters: print the number of iterations performed
    --test-root: test the root function with the parameters given in the format "--test-root <func1_id> <func2_id> <from> <to> <eps>"
    --test-integral: test the integral function with the parameters given in the format "--test-integral <func_id> <from> <to> <eps>"
    --full-test: perform a full test as in the report
```
