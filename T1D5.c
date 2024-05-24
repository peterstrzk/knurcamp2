#include <stdio.h>
#include <stdlib.h>

/*
 T1D5 - Pamiętaj bo pamięć to rzecz święta xD (pzdr bonusrpk)
 Zarządzanie pamięcią - utrwalenie
 22.05.2025


 Postępuj zgodnie z instrukcjami w komentarzach, i dopisz brakujący kod.
 Zwróć uwagę na użycie funkcji calloc i free.
 Zastanów się, dlaczego używamy calloc zamiast malloc.
 */
// tak tak moge normalne nazwy zmiennych pisac typu n itd itd
int main(void) {
    int* elements;
    int numberOfElements, i;

    // Zadanie 1: Poproś użytkownika o podanie liczby elementów

    printf("%s", "dAwaj liczbe elementów!\n");
    scanf("%d", &numberOfElements);

    // Zadanie 2: Przydziel pamięć dynamicznie dla 'n' elementów używając calloc

    elements = (int*)calloc(numberOfElements, sizeof(int));

    // Zadanie 3: Sprawdź, czy pamięć została pomyślnie przydzielona

    if (elements == NULL ) {

        printf("%s", "Memory is not allocated!\n");

    } else {

        printf("%s", "Memory allocated!\n");

    // Zadanie 4: Wprowadź elementy do przydzielonej pamięci

        for (i = 0; i < numberOfElements; ++i) {

            elements[i] = i + 1;
        }
        // Zadanie 5: Wyświetl te elementy
        printf("%s","The elements of the array are: \n");
        for (i = 0; i < numberOfElements; ++i) {
            //tu użyłem specifier character dla "spacji" xD
            printf("%3d", elements[i]);
        }
    }

    // Zadanie 6: Zwolnij przydzieloną pamięć
    free(elements);
    return 0;
}