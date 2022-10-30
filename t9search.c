#include <stdio.h>

int equals_char(char c, char num) { // porovna znak s cislici, pokud cislici nenalezne, porovná znak se znaky přiřazenými cislici
    switch (num) { // prirazeni cislici znaku
        case '0':
            return c == '+' || c == '0';
        case '1':
            return c == '1';
        case '2':
            return c == 'a' || c == 'b' || c == 'c' || c == '2';
        case '3':
            return c == 'd' || c == 'e' || c == 'f' || c == '3';
        case '4':
            return c == 'g' || c == 'h' || c == 'i' || c == '4';
        case '5':
            return c == 'j' || c == 'k' || c == 'l' || c == '5';
        case '6':
            return c == 'm' || c == 'n' || c == 'o' || c == '6';
        case '7':
            return c == 'p' || c == 'q' || c == 'r' || c == 's' || c == '7';
        case '8':
            return c == 't' || c == 'u' || c == 'v' || c == '8';
        case '9':
            return c == 'w' || c == 'x' || c == 'y' || c == 'z' || c == '9';
        default:
            return 0;
    }
}

int is_substr_in_str(char* str, char* substr) { // zjisti, zda je podretezec v retezci
    char *p1, *p2, *p3; // pomocne ukazatele

    p1 = str;  // p1 ukazuje na prvni znak str
    p2 = substr;  // p2 ukazuje na prvni znak v substr(podretezec)

    int flag = 0; // flag je promenna, ktera nam rekne, zda jsme nasli podřetězec v retezci

    for (int i = 0; str[i] != '\0'; i++) { // projdeme retezec str
        if (equals_char(*p1, *p2)) {
            p3 = p1; // p3 ukazuje na prvni znak retezce str, ktery odpovida prvnimu znaku podretezce substr
            for (int j = 0; substr[j] != '\0'; j++) { // projdeme podretezec substr
                if (equals_char(*p3, *p2)) { // pokud znak retezce str odpovida znaku podretezce substr
                    p3++; // posuneme se o jeden znak v retezci str
                    p2++; // posuneme se o jeden znak v podřetězci substr
                } else {
                    break; // pokud se znaky nerovnaji, prerusime cyklus
                }
            }
            if (*p2 == '\0') { // pokud jsme projeli cely podretezec substr
                if (str[0] == '+' || (str[0] >= '0' && str[0] <= '9')) {
                    return 1; // pokud je prvni znak retezce str cislo nebo +, vracime 1
                } else {
                    // Uvedte vyskyt velkymi pismeny
                    for (int j = 0; substr[j] != '\0'; j++) { // projdeme podretezec substr
                        str[i + j] -= 32;
                    }
                    flag = 1;
                }
            }
            p2 = substr; // vratime se na zacatek podretezce substr
        }
        p1++;
    }
    return flag;
}

int search_for_broken_sequences(char* str, char* substr) { // funkce, ktera hleda prerusenou sekvenci
    int flag; // flag je promenna, ktera nam rekne, zda jsme nasli prerusenou sekvenci

    char* rest_of_str = str; // rest_of_str je ukazatel na zacatek retezce str

    for (char* p_in_substr = substr; *p_in_substr != '\0'; p_in_substr++) { // projdeme podretezec substr
        flag = 0;
        for (char *p_in_str = rest_of_str; *p_in_str != '\0'; p_in_str++) { // projdeme retezec str
            rest_of_str++;
            if (equals_char(*p_in_str, *p_in_substr)) {
                flag = 1; // pokud jsme nasli znak, ktery odpovida znaku v podretezci substr, nastavime flag na 1
                break;
            }
        }
        if (!flag) { // pokud jsme nenasli znak, ktery odpovida znaku v podretezci substr, vracime 0
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    char* substr = argv[1]; // podretezec
    int found = 0;  // 0 znamena nenalezeno, 1 znamena, ze v telefonu nebo ve jmene byl alespon jeden vyskyt
    int search_in_names = 0;  // 0 znamena hledani v čislech, 1 znamena hledani ve jmenech


    char buffer[101]; // buffer pro nacitani radku ze souboru
    int buffer_len = 0; // delka radku
    char c; // promenna pro nacitani znaku ze souboru

    char name[100];

    for (int i = 0;;) { // nacitame znaky ze souboru
        c = getc(stdin); // nacteme znak ze souboru
        if (c == EOF) { // pokud jsme na konci souboru
            if (!search_in_names && !found) { // pokud jsme hledali v cislech a nebyl nalezen zadny vyskyt
                i = 0;
                buffer_len = 0;
                fseek(stdin, 0, SEEK_SET);  // pretocit soubor na zacatek
                search_in_names = 1; // prepneme se do hledani ve jmenech
                continue;
            }
            break;
        }
        if (c == '\n') { // pokud jsme na konci radku
            if (i % 2 == 0) {
                for (int j = 0; j <= buffer_len; j++) { // projdeme radek
                    name[j] = buffer[j]; // ulozime jmeno do promenne name
                }
            } else { // pokud jsme na radku s cislem
                if (argc == 1) { // pokud nebyl zadan zadny podretezec
                    printf("%s, %s\n", name, buffer); // vypiseme jmeno a cislo
                    found = 1;
                }
                else if (!search_in_names && is_substr_in_str(buffer, substr)) { // pokud jsme hledali v cislech a byl nalezen vyskyt
                    printf("%s, %s\n", name, buffer); // vypiseme jmeno a cislo
                    found = 1;
                }
                else if (search_in_names && is_substr_in_str(name, substr)) { // pokud jsme hledali ve jmenech a byl nalezen vyskyt
                    printf("%s, %s\n", name, buffer);
                    found = 1;
                }
                else if (search_for_broken_sequences(name, substr) || search_for_broken_sequences(buffer, substr)) { // pokud jsme nasli prerusenou sekvenci
                    printf("%s, %s\n", name, buffer);
                    found = 1;
                }
            }
            buffer_len = 0;

            i++;
            continue;
        }
        if (buffer_len > 100) { // pokud je radek delsi nez 100 znaku
            printf("Error: buffer overflow\n"); // vypiseme chybu
            fprintf(stderr, "Error: buffer overflow\n"); // vypiseme chybu na stderr
            return 1;
        }
        buffer[buffer_len++] = c >= 'A' && c <= 'Z' ? c + 32 : c; // ulozime znak do bufferu
        buffer[buffer_len] = '\0'; // ukoncime retezec
    }

    if (!found) { // pokud nebyl nalezen zadny vyskyt
        printf("Not found\n"); // vypiseme, ze nebyl nalezen zadny vyskyt
        fprintf(stderr,"Not found\n"); // vypiseme, že nebyl nalezen zadny vyskyt na stderr
        return 1;
    }

    return 0;
}
