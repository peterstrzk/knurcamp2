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
    int numberOfElements;
    // Zadanie 1: Poproś użytkownika o podanie liczby elementów
    printf("%s", "dAwaj liczbe elementów!\n");
    scanf("%d", numberOfElements);

    // Zadanie 2: Przydziel pamięć dynamicznie dla 'n' elementów używając calloc
    elements = (int*)calloc(numberOfElements, sizeof(numberOfElements));
    // Zadanie 3: Sprawdź, czy pamięć została pomyślnie przydzielona
    if (elements != NULL ) {
        printf("%s", "Memory allocated!");
    }

    // Zadanie 4: Wprowadź elementy do przydzielonej pamięci

    // Zadanie 5: Wyświetl te elementy

    // Zadanie 6: Zwolnij przydzieloną pamięć

    return 0;
}