/* NIE DOTYKAJ */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void weryfikacja1(void* ss, int sz)
{
    char* st = (char*)ss;
    int wiek = *st;
    if(wiek != 105)
    {
        printf("Błędne rozwiązanie\n");
        return;
    }
    st += sz / 3;
    uint64_t imiePtr = (*(uint64_t*)st);
    char* imie = (char*)imiePtr;
    st += sz / 3;

    if(strcmp(imie, "Jan") != 0)
    {
        printf("Błędne rozwiązanie\n");
        return;
    }
    float wzrost = *(float*)st;
    if(wzrost < 2.138 && wzrost > 2.136)
    {
        printf("Brawo poprawne rozwiązanie\n");
    }
    else
    {
        printf("Ups, błędne rozwiązanie\n");
    }
}
/* KONIEC SEKCJI NIE DOTYKAJ */

/*
 Treść zadania:
 
 Stwórz strukturę o nazwie `czlowiek`.
 Zrób żeby zawierała następujące pola:
 - `wiek` <- liczba całkowita (rozmiar standardowy)
 - `imie` <- no jego imie a co xD (ptr do stringa)
 - `wzrost` <- liczba zmiennoprzecinkowa w metrach amerykańskich np 1.234
 
 Stwórz człowieka o imieniu `Jan`, wieku 105 lat, wzroście 2 m 137 cm (w metrach amerykańskich - 1 metr = 1000 cm - 2m 137cm = 2.137)
 Pola wiek, imie, i wzrost muszą mieć taką samą koleność.
 
 Uruchom funckje `weryfikacja1` ze swojej funkcji main podając wskaźnik do człowieka oraz rozmiar struktury człowiek jako drugi parametr.
 
 ***** Zadanie na 6 albo gwiazdke jak zwał tak zwał *****
 Opisz co robi po kolei funckja `void weryfikacja1(void* ss, int sz)`
*/

struct Czlowiek {
    int wiek;
    char *imie;
    float wzrost;
};

int main() {

    struct Czlowiek czlowiek;

    czlowiek.wiek = 105;
    czlowiek.imie = "Jan";
    czlowiek.wzrost = 2.137;

    weryfikacja1(&czlowiek, sizeof(czlowiek));

    return 0;
}
/*
void weryfikacja1(void* ss, int sz) <-- fankszyn testujące przyjmujące argumenty jak struktura i ten sizof podający ilość pamięci w bajtach w integer
{
    char* st = (char*)ss; <--wskaznik st do struktury ss
    int wiek = *st; ,-- przypisuje wartosc wskaznika do zmiennej
    if(wiek != 105) <-- sprawdza ifem wiek, jesli wiek nie jest =105 to wywala błąd poniżej poprzez wyswietlenie "bledne rozwiązanie" i konczy funkcje
    {
        printf("Błędne rozwiązanie\n"); <--wyswietla bład jak tez nie spełnia powyzszego warunku
        return;
    }
    st += sz / 3; <-- cos przesuwa, chyba wskaznik(?)
    uint64_t imiePtr = (*(uint64_t*)st); <-- po kolei uint64 to typ (cos tam 64 bity) zmienna imiePtr castowana do wskaznika uint64 na st
    char* imie = (char*)imiePtr; <-- casting imie do imieptr (konwertowanie jednego typu danych do innego)
    st += sz / 3; <-- to samo co wyzej

    if(strcmp(imie, "Jan") != 0) <-- sprawdza czy czlowiek ze struktury ma na imie Jan
    {
        printf("Błędne rozwiązanie\n"); <--wyswietla bład jak tez nie spełnia powyzszego warunku
        return;
    }
    float wzrost = *(float*)st;
    if(wzrost < 2.138 && wzrost > 2.136) <-- sprawdza wzrost czy znajduje sie pomiedzy wypisanymi wartosciami
    {
        printf("Brawo poprawne rozwiązanie\n"); <-- wyswietla gdy wszystkie warunki są spełnione
    }
    else
    {
        printf("Ups, błędne rozwiązanie\n"); <-- jesli nie spelnia warunku wyskakuje to
    }
}*/