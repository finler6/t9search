#include <stdio.h>

int equals_char(char c, char num) {
    switch (num) {
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

int is_substr_in_str(char* str, char* substr) {
    char *p1, *p2, *p3;

    p1 = str;  // p1 points to the first character of str
    p2 = substr;  // p2 points to the first character of substr

    int flag = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (equals_char(*p1, *p2)) {
            p3 = p1;  // p3 points to the first character of str
            for (int j = 0; substr[j] != '\0'; j++) {
                if (equals_char(*p3, *p2)) {
                    p3++;
                    p2++;
                } else {
                    break;
                }
            }
            if (*p2 == '\0') {
                if (str[0] == '+' || (str[0] >= '0' && str[0] <= '9')) {
                    return 1;
                } else {
                    // Capitalize the occurrence
                    for (int j = 0; substr[j] != '\0'; j++) {
                        str[i + j] -= 32;
                    }
                    flag = 1;
                }
            }
            p2 = substr;
        }
        p1++;
    }
    return flag;
}

int search_for_broken_sequences(char* str, char* substr) {
    int flag;

    char* rest_of_str = str;

    for (char* p_in_substr = substr; *p_in_substr != '\0'; p_in_substr++) {
        flag = 0;
        for (char *p_in_str = rest_of_str; *p_in_str != '\0'; p_in_str++) {
            rest_of_str++;
            if (equals_char(*p_in_str, *p_in_substr)) {
                flag = 1;
                break;
            }
        }
        if (!flag) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    char* substr = argv[1];
    int found = 0;  // 0 means not found, 1 means there was at least one occurrence in the phone or in the name
    int search_in_names = 0;  // 0 means search in numbers, 1 means search in names


    char buffer[100];
    int buffer_len = 0;
    char c;

    char name[100];

    for (int i = 0;;) {
        c = getc(stdin);
        if (c == EOF) {
            if (!search_in_names && !found) {
                i = 0;
                buffer_len = 0;
                fseek(stdin, 0, SEEK_SET);  // rewind
                search_in_names = 1;
                continue;
            }
            break;
        }
        if (c == '\n') {
            if (i % 2 == 0) {
                for (int j = 0; j <= buffer_len; j++) {
                    name[j] = buffer[j];
                }
            } else {
                if (argc == 1) {
                    printf("%s, %s\n", name, buffer);
                    found = 1;
                }
                else if (!search_in_names && is_substr_in_str(buffer, substr)) {
                    printf("%s, %s\n", name, buffer);
                    found = 1;
                }
                else if (search_in_names && is_substr_in_str(name, substr)) {
                    printf("%s, %s\n", name, buffer);
                    found = 1;
                }
                else if (search_for_broken_sequences(name, substr) || search_for_broken_sequences(buffer, substr)) {
                    printf("%s, %s\n", name, buffer);
                    found = 1;
                }
            }
            buffer_len = 0;

            i++;
            continue;
        }
        if (buffer_len > 100) {
            printf("Error: buffer overflow\n");
            fprintf(stderr, "Error: buffer overflow\n");
            return 1;
        }
        buffer[buffer_len++] = c >= 'A' && c <= 'Z' ? c + 32 : c;
        buffer[buffer_len] = '\0';
    }

    if (!found) {
        printf("Not found\n");
        fprintf(stderr,"Not found\n");
        return 1;
    }

    return 0;
}
