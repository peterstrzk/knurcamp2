#include "stdio.h"

#include "stdbool.h"
 /*T1D6
 BOMBELKOWE SORTOWANIE NA WSKAZNIKACH HEHE
 Powodzenia bombelku, napisz ciało funkcji sortujPrzezWskazniki.
 Aby spełniała swoje zadanie i posortowała liczby rosnąco.
 */

void sortujPrzezWskazniki(int *tab, int rozmiar) {

    //czeking czy następna liczba jest wieksza od pierwszej jakos takos idąc po indeksie 01, 12, 23, 34, 45
//    for (int i = 0; i < rozmiar; i++) {
//        if (tab[i] > tab[i + 1]) {
//            printf("%s", "pierwsza jest większa od drugiej\n");
//        } else {
//            printf("%s", "nie jest\n");
//        }
//    }
     printf("%d\n", *tab);
     int i, j, temp;
     for(i = 0; i < rozmiar - 1; i++) {
         for(j = 0; j < rozmiar - i - 1; j++) {
             if(*(tab + j) > *(tab + j + 1)) {
                 temp = *(tab + j);
                 *(tab + j) = *(tab + j + 1);
                 *(tab + j + 1) = temp;
             }
         }
     }
}

int main() {
    int liczby[] = {34, 67, 23, 28, 98, 15};
    int rozmiar = sizeof(liczby) / sizeof(liczby[0]);

    printf("Tablica przed sortowaniem: \n");
    for (int i = 0; i < rozmiar; i++) {
        printf("%d ", liczby[i]);
    }
    printf("\n");

    sortujPrzezWskazniki(liczby, rozmiar);

    printf("Tablica po sortowaniu: \n");
    for (int i = 0; i < rozmiar; i++) {
        printf("%d ", liczby[i]);
    }
    printf("\n");

    return 0;
}