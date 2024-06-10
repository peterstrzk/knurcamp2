#include <stdio.h>



int fibonaczi(int liczba1) {
    int N = liczba1;
    int first = 0, second = 1, next;


    if (N == 0) {
        printf("%d ", first);
    }
    else if(N == 1) {
        printf("%d %d", first, second);
    } else if(N == 2) {
        printf("%d %d ", first, second);
    } else if(N >= 3) {
        printf("%d %d ", first, second);
        next = first + second;
        while(next <= N) {
            printf("%d ", next);
            first = second;

            second = next;
            next = first + second;
        }
    }

    return 0;
};

//int main() {
//    int N, first = 0, second = 1, next;
//
//    printf("Podaj liczbę całkowitą N: ");
//    scanf("%d", &N);
//    if (N == 0) {
//        printf("%d ", first);
//    }
//    else if(N == 1) {
//        printf("%d %d", first, second);
//    } else if(N == 2) {
//        printf("%d %d ", first, second);
//    } else if(N >= 3) {
//        printf("%d %d ", first, second);
//        next = first + second;
//        while(next <= N) {
//            printf("%d ", next);
//            first = second;
//
//            second = next;
//            next = first + second;
//        }
//    }
//
//    return 0;
//}



//int fankszyn1(int num1, char operator, int num2) {
//    if (operator == '+') {
//        return num1 + num2;
//    } else if (operator == '-') {
//        return num1 - num2;
//    } else if (operator == '*') {
//        return num1 * num2;
//    } else if (operator == '/') {
//        if(num2 != 0) {
//            return num1 / num2;
//        } else {
//            printf("Błąd: Dzielenie przez zero jest niewłaściwe.\n");
//            return 0;
//        }
//    } else {
//        printf("Brak znaku operacji matematycznej!.\n");
//        return 0;
//    }
//}

int fankszyn1 (int num1, char operator, int num2) {
    return (num1, operator, num2;
}

int main() {
    int num1, num2;
    int liczba1;
    char operator;
    int wynik;

    printf("Podaj pierwszą liczbę: ");
    scanf("%d", &num1);

    printf("Podaj operator (+, -, *, /): ");
    scanf(" %c", &operator);

    printf("Podaj drugą liczbę: ");
    scanf("%d", &num2);

    wynik = fankszyn1(num1, operator, num2);
    printf("Wynik: %d\n", wynik);

//    int N;
//
//    printf("%s","Podaj liczbe calkowita N: ");
//    scanf("%d", &N);
//    fibonaczi(N);
//


    return 0;
}
