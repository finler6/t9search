#include <stdio.h>

int equals_char(char c, char num) { // porovná znak s číslicí, pokud číslici nenalezne, porovná znak se znaky přiřazenými číslici
    switch (num) { // přiřazení číslici znaků
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

int is_substr_in_str(char* str, char* substr) { // zjistí, zda je podřetězec v řetězci
    char *p1, *p2, *p3; // pomocné ukazatele

    p1 = str;  // p1 ukazuje na první znak str
    p2 = substr;  // p2 ukazuje na první znak v substr(podřetězec)

    int flag = 0; // flag je proměnná, která nám řekne, zda jsme našli podřetězec v řetězci

    for (int i = 0; str[i] != '\0'; i++) { // projdeme řetězec str
        if (equals_char(*p1, *p2)) {
            p3 = p1; // p3 ukazuje na první znak řetězce str, který odpovídá prvnímu znaku podřetězce substr
            for (int j = 0; substr[j] != '\0'; j++) { // projdeme podřetězec substr
                if (equals_char(*p3, *p2)) { // pokud znak řetězce str odpovídá znaku podřetězce substr
                    p3++; // posuneme se o jeden znak v řetězci str
                    p2++; // posuneme se o jeden znak v podřetězci substr
                } else {
                    break; // pokud se znaky nerovnají, přerušíme cyklus
                }
            }
            if (*p2 == '\0') { // pokud jsme projeli celý podřetězec substr
                if (str[0] == '+' || (str[0] >= '0' && str[0] <= '9')) {
                    return 1; // pokud je první znak řetězce str číslo nebo +, vracíme 1
                } else {
                    // Uveďte výskyt velkými písmeny
                    for (int j = 0; substr[j] != '\0'; j++) { // projdeme podřetězec substr
                        str[i + j] -= 32;
                    }
                    flag = 1;
                }
            }
            p2 = substr; // vrátíme se na začátek podřetězce substr
        }
        p1++;
    }
    return flag;
}

int search_for_broken_sequences(char* str, char* substr) { // funkce, která hledá přerušenou sekvenci
    int flag; // flag je proměnná, která nám řekne, zda jsme našli přerušenou sekvenci

    char* rest_of_str = str; // rest_of_str je ukazatel na začátek řetězce str

    for (char* p_in_substr = substr; *p_in_substr != '\0'; p_in_substr++) { // projdeme podřetězec substr
        flag = 0;
        for (char *p_in_str = rest_of_str; *p_in_str != '\0'; p_in_str++) { // projdeme řetězec str
            rest_of_str++;
            if (equals_char(*p_in_str, *p_in_substr)) {
                flag = 1; // pokud jsme našli znak, který odpovídá znaku v podřetězci substr, nastavíme flag na 1
                break;
            }
        }
        if (!flag) { // pokud jsme nenašli znak, který odpovídá znaku v podřetězci substr, vracíme 0
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    char* substr = argv[1]; // podřetězec
    int found = 0;  // 0 znamená nenalezeno, 1 znamená, že v telefonu nebo ve jméně byl alespoň jeden výskyt
    int search_in_names = 0;  // 0 znamená hledání v číslech, 1 znamená hledání ve jménech


    char buffer[101]; // buffer pro načítání řádků ze souboru
    int buffer_len = 0; // délka řádku
    char c; // proměnná pro načítání znaků ze souboru

    char name[100];

    for (int i = 0;;) { // načítáme znaky ze souboru
        c = getc(stdin); // načteme znak ze souboru
        if (c == EOF) { // pokud jsme na konci souboru
            if (!search_in_names && !found) { // pokud jsme hledali v číslech a nebyl nalezen žádný výskyt
                i = 0;
                buffer_len = 0;
                fseek(stdin, 0, SEEK_SET);  // přetočit soubor na začátek
                search_in_names = 1; // přepneme se do hledání ve jménech
                continue;
            }
            break;
        }
        if (c == '\n') { // pokud jsme na konci řádku
            if (i % 2 == 0) {
                for (int j = 0; j <= buffer_len; j++) { // projdeme řádek
                    name[j] = buffer[j]; // uložíme jméno do proměnné name
                }
            } else { // pokud jsme na řádku s číslem
                if (argc == 1) { // pokud nebyl zadán žádný podřetězec
                    printf("%s, %s\n", name, buffer); // vypíšeme jméno a číslo
                    found = 1;
                }
                else if (!search_in_names && is_substr_in_str(buffer, substr)) { // pokud jsme hledali v číslech a byl nalezen výskyt
                    printf("%s, %s\n", name, buffer); // vypíšeme jméno a číslo
                    found = 1;
                }
                else if (search_in_names && is_substr_in_str(name, substr)) { // pokud jsme hledali ve jménech a byl nalezen výskyt
                    printf("%s, %s\n", name, buffer);
                    found = 1;
                }
                else if (search_for_broken_sequences(name, substr) || search_for_broken_sequences(buffer, substr)) { // pokud jsme našli přerušenou sekvenci
                    printf("%s, %s\n", name, buffer);
                    found = 1;
                }
            }
            buffer_len = 0;

            i++;
            continue;
        }
        if (buffer_len > 100) { // pokud je řádek delší než 100 znaků
            printf("Error: buffer overflow\n"); // vypíšeme chybu
            fprintf(stderr, "Error: buffer overflow\n"); // vypíšeme chybu na stderr
            return 1;
        }
        buffer[buffer_len++] = c >= 'A' && c <= 'Z' ? c + 32 : c; // uložíme znak do bufferu
        buffer[buffer_len] = '\0'; // ukončíme řetězec
    }

    if (!found) { // pokud nebyl nalezen žádný výskyt
        printf("Not found\n"); // vypíšeme, že nebyl nalezen žádný výskyt
        fprintf(stderr,"Not found\n"); // vypíšeme, že nebyl nalezen žádný výskyt na stderr
        return 1;
    }

    return 0;
}
