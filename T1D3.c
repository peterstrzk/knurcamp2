#include <stdio.h>
#include <stdbool.h>
/*
 Zaimplementuj ciało funkcji isPrime która zwróci true jeśli podany int §num§ jest liczba pierwszą.
 Zwróć false w przeciwnym wypadku.
 */



bool isPrime(int num) {
    int i = 2;
    int n = num;
    if (n == 0 || n == 1) {
        return false;
    }

    // Checking Prime
    if (n == i)
        return true;

    // base cases
    if (n % i == 0) {
        return false;
    }
    i++;
// UPDATE: poniżej chciałem skorzystać z pierwiastka kwadratowego, ale nie wiedziałem, czy można skorzystać z odpowiedniej libki gdzie jest sqrt, wiec spróbowałem potworzyć cos swojego, ale w teście czy
// z palca 99 nie chce wyjść z petli, byc moze to z powodu przyblizenia bo to int, wiec 49,5 sprowadza do 49 i jest pętla nieskonczona maybe.
    //poniższe nie działają przy 99
//    int temp, sqrt;
//
//    sqrt = num / 2;
//    printf("%d",sqrt);
//    temp = 0;
//    while(sqrt != temp)
//    {
//        temp = sqrt;
//        sqrt = (num/temp + temp)/2;
//
//
//    }

//    to wogóle garbage
//    printf("P  of ‘%d’ is ‘%d’", num, sqrt);
//    if (sqrt % 2 != 0) {
//        return true;
//    } else {
//        return false;
//        };
//    int liczba, dzielnik, iterator;
//    liczba = num;
//    dzielnik = 1;
//    iterator = 1;
//    while(dzielnik <= liczba) {
//
//        liczba = liczba / dzielnik;
//        ++dzielnik;
//       if(dzielnik <= liczba) {
//           ++iterator;
//
//       }
//       if(iterator == 2) {
//           return true;
//       }
//    }
//    printf("%d", iterator);
//
//    return 0;
}


void selfTest(void) {
    printf("Self-checking code:\n");
    printf("Test 1: %s\n", isPrime(2) ? "POPRAWNY" : "NIEPRAWIDŁOWY");
    printf("Test 2: %s\n", isPrime(11) ? "POPRAWNY" : "NIEPRAWIDŁOWY");
    printf("Test 3: %s\n", isPrime(99) ?  "NIEPRAWIDŁOWY" : "POPRAWNY");
    printf("Test 4: %s\n", isPrime(97) ? "POPRAWNY" : "NIEPRAWIDŁOWY");
}

int main(void) {
    int num;
    printf("Podaj liczbe: ");
    scanf("%d", &num);

    if (isPrime(num)) {
        printf("%d jest liczba pierwsza.\n", num);
    } else {
        printf("%d nie jest liczba pierwsza.\n", num);
    }

    // Run the self-checking code
    selfTest();

    return 0;
}