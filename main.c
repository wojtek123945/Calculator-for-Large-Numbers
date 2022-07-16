#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ctype.h"

int multiply(const char* number1, const char* number2, char** result);
int subtract(const char* number1, const char* number2, char** result);
int sub(const char* number1, const char* number2, char** result);
int add(const char* number1, const char* number2, char** result);
int remove_zero(char* number);
int abs_number(const char* n1, const char* n2);
int add_minus(char* result);

int validate(const char *number);
int validate_expression(const char *expr);
int calculate(const char* expr, char **result);

int main() {
    char* result, *number;
    number = calloc(501, sizeof(char));
    if(!number){
        printf("Failed to allocate memory");
        return 8;
    }
    printf("Podaj wyrazenie: ");
    scanf("%500[^\n]s", number);
    int error = validate_expression(number);
    if(error == 1){
        printf("Incorrect input");
        free(number);
        return 1;
    }
    error = calculate(number, &result);
    if(error == 3){
        printf("Failed to allocate memory");
        free(number);
        return 8;
    }
    printf("%s", result);
    free(number);
    free(result);
    return 0;
}
//ADD działa dobrze pierdoli sie substruct !!!
///DZIAŁA KURWAAAAA
int add(const char* number1, const char* number2, char** result){
    if(!number1 || !number2 || !result){
        return 1;
    }
    int error=0;
    error = validate(number1);
    if(error == 2){
        return 2;
    }
    error = validate(number2);
    if(error == 2){
        return 2;
    }
    int which_larger = abs_number(number1, number2);
    int if_minus=0;
    if(*number1 == '-' && *number2=='-'){
        if_minus=1;
    }else if(*number1 == '-' && *number2 != '-'){
        int error_1=0;
        if(which_larger == 1){
            error_1 = subtract(number1+1, number2, result);
        }else if(which_larger == -1){
            error_1 = subtract(number2, number1+1, result);
        }else{
            error_1 = subtract(number1+1, number2, result);
        }
        if(error_1 == 3){
            return 3;
        }
        if(which_larger == 1){
            //Musze dodać minus do result
            int size = strlen(*result);
            char *temp;
            temp = realloc(*result, sizeof(char)*(size+2));
            if(!temp){
                return 3;
            }
            for (int i = size-1; i >= 0; --i) {
                *(temp+i+1) = *(temp+i);
            }
            *temp='-';
            *result = temp;
            return 0;
        }
        return 0;
    }else if(*number2=='-' && *number1 !='-'){
        int error_1=0;
        if(which_larger == 1){
            error_1 = subtract(number1, number2+1, result);
        }else if(which_larger == -1){
            error_1 = subtract(number2+1, number1, result);
        }else{
            error_1 = subtract(number1, number2+1, result);
        }
        if(error_1 == 3){
            return 3;
        }
        if(which_larger == -1){
            //Musze dodać minus do result
            int size = strlen(*result);
            char *temp;
            temp = realloc(*result, sizeof(char)*(size+2));
            if(!temp){
                return 3;
            }
            for (int i = size-1; i >= 0; --i) {
                *(temp+i+1) = *(temp+i);
            }
            *temp='-';
            *result = temp;
            return 0;
        }
        return 0;
    }
    //Sprawdzam która liczba jest większa
    int size_1 = (int)strlen(number1);
    int size_2 = (int)strlen(number2);
    int size;

    //Alokuje większa wartość liczbową do wartości result
    if(size_1 > size_2){
        *result = calloc((size_1+3), sizeof(char));
        if(!*result){
            return 3;
        }
        size = size_1+2;
    }else if(size_1 < size_2){
        *result = calloc((size_2+3), sizeof(char));
        if(!*result){
            return 3;
        }
        size = size_2+2;
    }else{
        *result = calloc((size_2+3), sizeof(char));
        if(!*result){
            return 3;
        }
        size = size_2+2;
    }
    for (int i = 0; i < size; ++i) {
        *(*result + i) = '0';
    }
    int bool=0;
    //mam pewność że jade do większej z liczb
    for (int i = 0; i < size; ++i) {
        int temp;
        int pierwsza = ((int)*(number1 + size_1-i-1) - '0');
        int druga = ((int)*(number2 + size_2-i-1) - '0');
        if(i > size_1 || pierwsza<0){
            pierwsza = 0;
        }
        if(i > size_2 || druga<0){
            druga=0;
        }
        temp = pierwsza + druga + bool;
        if(bool == 1){
            bool = 0;
        }

        if(temp >= 10){
            bool = 1;
            temp = temp%10;
        }
        *(*result + size-i-1) = (temp + '0');
    }
    int i;
    for (i = 0; i < size; ++i) {
        if(*(*result + i) != '0'){
            break;
        }
    }
    if(if_minus == 1){
        *(*result + i-1) = '-';
    }
    if(*(*result) == '0'){
        int j;
        for(j=0;*(*result+j + 1) != '\0';j++)
            *(*result+j)=*(*result+j+1);
        *(*result+j) = '\0';
    }
    remove_zero(*result);
    return 0;
}
int sub(const char* number1, const char* number2, char** result){
    int size_1 = (int)strlen(number1);
    int size_2 = (int)strlen(number2);
    int size;
    //Alokuje większa wartość liczbową do wartości result
    if(size_1 > size_2){
        *result = calloc((size_1+2), sizeof(char));
        if(!*result){
            return 3;
        }
        size = size_1+1;
    }else{
        *result = calloc((size_2+2), sizeof(char));
        if(!*result){
            return 3;
        }
        size = size_2+1;
    }
    for (int i = 0; i < size; ++i) {
        *(*result+ i) = '0';
    }
    int bool=0, usuniete=0;
        for (int i = 0; i < size - 1; ++i) {
            int temp = 0;
            int pierwsza = ((int) *(number1 + size_1 - i - 1) - '0');
            int druga = ((int) *(number2 + size_2 - i - 1) - '0');
            if (i > size_1 || pierwsza < 0) {
                pierwsza = 0;
            }
            if (i > size_2 || druga < 0) {
                druga = 0;
            }
            if (druga == 0) {
                if(usuniete == 1){
                    *(*result + size - i - 1) = *(number1 + size_1 - i - 1)-1;
                    usuniete=0;
                }else{
                    *(*result + size - i - 1) = *(number1 + size_1 - i - 1);
                }
                continue;
            }
            int j = 0;
            if (pierwsza - druga < 0) {
                int bool_2 = 0, if_done = 0;
                //LICZBA SIE SKONCZYLA
                while (*(number1 + size_1 - i - 1 - j) <= *(number2 + size_2 - i - 1 - j)) {
                    temp = 0;
                    temp = *(number1 + size_1 - i - 1 - j) - '0' + 10 + bool_2;
                    if (bool_2 > 0) {
                        bool_2--;
                        temp--;
                    }
                    temp -= *(number2 + size_2 - i - 1 - j) - '0';
                    if (if_done == 1) {
                        temp--;
                    }
                    if (temp >= 10) {
                        bool_2++;
                        temp = temp % 10;
                    }
                    *(*result + size - i - 1 - j) = (char) (temp + '0');

                    j++;
                    if_done = 1;
                }
                i += j;
                bool = 1;
            }
            if (bool == 1) {
                pierwsza = ((int) *(number1 + size_1 - i - 1) - '0');
                druga = ((int) *(number2 + size_2 - i - 1) - '0');
                if (i > size_1 || pierwsza < 0) {
                    pierwsza = 0;
                }
                if (i > size_2 || druga < 0) {
                    druga = 0;
                }
                temp = pierwsza - 1 - druga;
                if(temp<0){
                    temp+=10;
                    usuniete = 1;
                }
                *(*result + size - i - 1) = (char) (temp + '0');
                bool = 0;

                //usunięte -1 !!!
                if(*(number1 + size_1 - i - 1) == '0'){
                    i++;
                    while (*(number1 + size_1 - i - 1) == '0'){
                        temp = *(number1 + size_1 - i - 1) - '0';
                        temp--;
                        if(temp<0){
                            temp+=10;
                        }
                        *(*result + size - i - 1) = (char) (temp + '0');
                        i++;
                    }
                    usuniete=1;
                    i--;
                }
            } else {
                temp = pierwsza - druga;
                *(*result + size - i - 1) = (char) (temp + '0');
            }
        }

        return 0;
}
int subtract(const char* number1, const char* number2, char** result){
    if(!number1 || !number2 || !result){
        return 1;
    }
    int error=0;
    error = validate(number1);
    if(error == 2){
        return 2;
    }
    error = validate(number2);
    if(error == 2){
        return 2;
    }

    int which_larger = abs_number(number1, number2);

    if(*number1 == '-' && *number2=='-'){
        error = add(number1, number2+1, result);
        if(error == 3){
            return 3;
        }

        if(which_larger == 1 && **result!='-'){
            int error_1 = add_minus(*result);
            if(error_1 == 1){
                return 3;
            }
            return 0;
        }
        return 0;
    }else if(*number1=='-' && *number2!='-'){
        error = add(number1+1, number2, result);
        if(error == 3){
            return 3;
        }
        error = add_minus(*result);
        if(error == 1){
            return 3;
        }
        return 0;
    }else if(*number1!='-' && *number2=='-'){
        error = add(number1, number2+1, result);
        if(error == 3){
            return 3;
        }
        return 0;
    }
    if(which_larger>=0) {
        error = sub(number1, number2, result);
        if(error == 3){
            return 3;
        }
    }else{
        error = sub(number2, number1, result);
        if(error == 3){
            return 3;
        }
    }
    remove_zero(*result);
    if(**result == '\0'){
        **result = '0';
        return 0;
    }
    if(which_larger==-1){
        error = add_minus(*result);
        if(error == 1){
            return 3;
        }
    }
    return 0;
}
int multiply(const char* number1, const char* number2, char** result){
    if(!number1 || !number2 || !result){
        return 1;
    }
    int error=0;
    error = validate(number1);
    if(error == 2){
        return 2;
    }
    error = validate(number2);
    if(error == 2){
        return 2;
    }
    if(*number1 =='-' && *number2 == '-'){
        int error_multi = multiply(number1+1, number2+1, result);
        if(error_multi == 3){
            return 3;
        }
        return 0;
    }else if(*number1 =='-' && *number2 != '-'){
        //Musze zwiększyć i dodac minus
        int error_multi = multiply(number1+1, number2, result);
        if(error_multi == 3){
            return 3;
        }
        int size = strlen(*result);

        if(size == 1){
            if(**result == '0'){
                return 0;
            }
        }

        char *temp;
        temp = realloc(*result, sizeof(char)*(size+2));
        if(!temp){
            return 3;
        }
        for (int i = size-1; i >= 0; --i) {
            *(temp+i+1) = *(temp+i);
        }
        *temp='-';
        *result = temp;
        return 0;
    }else if(*number1 != '-' && *number2 == '-'){
        //Musze zwiększyć i dodac minus
        int error_multi = multiply(number1, number2+1, result);
        if(error_multi == 3){
            return 3;
        }
        int size = strlen(*result);
        if(size == 1){
            if(**result == '0'){
                return 0;
            }
        }
        char *temp;
        temp = realloc(*result, sizeof(char)*(size+2));
        if(!temp){
            return 3;
        }
        for (int i = size-1; i >= 0; --i) {
            *(temp+i+1) = *(temp+i);
        }
        *temp='-';
        *result = temp;
        return 0;
    }


    int size_1 = (int)strlen(number1);
    int size_2 = (int)strlen(number2);
    int which_larger = abs_number(number1, number2);
    if(which_larger >= 0){
        *result = calloc(size_1+size_2+2, sizeof(char));
        if(!*result){
            return 3;
        }
        for (int i = 0; i < size_1+size_2+1; ++i) {
            *(*result + i) = '0';
        }
    }else{
        int error_1 = multiply(number2, number1, result);
        if(error_1 == 3){
            return 3;
        }
        return 0;
    }
    char *temp = calloc(size_1 + size_2 + 2, sizeof(char));
    if(!temp){
        free(*result);
        return 3;
    }
    for (int i = 0; i < size_1+size_2+1; ++i) {
        *(temp + i) = '0';
    }

    for (int i = 0; i < size_2; ++i) {
        ///Wyznaczanie pierwszej liczby do dodania
        int bool=0, place;
        for (int j = 0; j < size_1; ++j) {
            int temp_number=0;
            int pierwsza = ((int)*(number1 + size_1-j-1) - '0');
            int druga = ((int)*(number2 + size_2-i-1) - '0');
            temp_number = pierwsza*druga + bool%10;
            bool/=10;
            *(temp + strlen(temp) - 1 - j) = temp_number%10 + '0';
            if(temp_number > 9){
                bool += temp_number/10;
            }
            place = j;
        }
        if(bool > 0){
            *(temp + strlen(temp) - 1 - place - 1) = bool%10 + '0';
        }
        remove_zero(temp);
        if(strlen(temp) <= 1){
            for (int k = 0; k < size_1+size_2+1; ++k) {
                *(temp + k) = '0';
            }
            continue;
        }
        char *copy_result = strdup(*result);
        if(!copy_result){
            return 3;
        }
        //Zwiększyć temp o zera tyle zer ile wynosi i
        int len_temp = strlen(temp);
        char *temp_temp = realloc(temp, strlen(temp)+2+i);
        if(!temp_temp){
            return 3;
        }
        temp = temp_temp;
        for (int j = 0; j < i; ++j) {
            *(temp + len_temp + j) = '0';
        }
        free(*result);
        remove_zero(copy_result);
        if(strlen(copy_result)<1){
            *(copy_result)='0';
        }
        add(copy_result, temp, result);
        free(copy_result);
        free(temp);

        temp = calloc(size_1 + size_2 + 2, sizeof(char));
        if(!temp){
            free(*result);
            return 3;
        }
        for (int k = 0; k < size_1+size_2+1; ++k) {
            *(temp + k) = '0';
        }
    }
    free(temp);
    remove_zero(*result);
    if(strlen(*result)<1){
        *(*result)='0';
    }
    return 0;
}
int calculate(const char* expr, char **result){
    if(!expr || !result){
        return 1;
    }
    if(*expr == '\0'){
        return 2;
    }
    int only_number=0, o=0;
    if(*expr == '-') {
        o++;
    }
    for (; *(expr + o) != '\0'; ++o) {
        if(isdigit(*(expr + o))){
            continue;
        }else{
            only_number=1;
            break;
        }
    }
    if(only_number == 0){
        *result = calloc(strlen(expr) + 1, sizeof(char));
        if(!*result){
            return 2;
        }
        strcpy(*result, expr);
        return 0;
    }
    int error = validate_expression(expr);
    if(error == 1){
        return 2;
    }
    int i=0, position=0;
    char *n1, *n2;
    if(*expr == '-') {
        i++;
    }
    int if_done=0;
    while (*(expr + i) != '\0'){
        if(isdigit(*(expr + i))){
            i++;
            continue;
        }else if(*(expr + i) == '+' && if_done == 0){
            n1 = calloc(i - position + 1, sizeof(char));
            if(!n1){
                return 3;
            }
            int j=0;
            i++;
            if(*(expr + i + j) == '-'){
                j++;
            }
            while(isdigit(*(expr + i + j))){j++;}

            n2 = calloc(j+1, sizeof(char));
            if(!n2){
                free(n1);
                return 3;
            }

            strncpy(n1, expr + position, i-1-position);
            strncpy(n2, expr + i, j);
            error = add(n1, n2, result);
            if(error == 3){
                free(n1);
                free(n2);
                return 3;
            }
            i+=j-1;
            position=i+2;
            free(n1);
            free(n2);
            if_done=1;
        }else if(*(expr + i) == '-' && if_done == 0){
            n1 = calloc(i - position + 1, sizeof(char));
            if(!n1){
                return 3;
            }
            int j=0;
            i++;
            if(*(expr + i + j) == '-'){
                j++;
            }
            while(isdigit(*(expr + i + j))){j++;}

            n2 = calloc(j+1, sizeof(char));
            if(!n2){
                free(n1);
                return 3;
            }

            strncpy(n1, expr + position, i-1-position);
            strncpy(n2, expr + i, j);
            error = subtract(n1, n2, result);
            if(error == 3){
                free(n1);
                free(n2);
                return 3;
            }
            i+=j-1;
            position=i+2;
            free(n1);
            free(n2);
            if_done=1;
        }else if(*(expr + i) == '*' && if_done == 0){
            n1 = calloc(i - position + 1, sizeof(char));
            if(!n1){
                return 3;
            }
            int j=0;
            i++;
            if(*(expr + i + j) == '-'){
                j++;
            }
            while(isdigit(*(expr + i + j))){j++;}

            n2 = calloc(j+1, sizeof(char));
            if(!n2){
                free(n1);
                return 3;
            }

            strncpy(n1, expr + position, i-1-position);
            strncpy(n2, expr + i, j);
            error = multiply(n1, n2, result);
            if(error == 3){
                free(n1);
                free(n2);
                return 3;
            }
            i+=j-1;
            position=i+2;
            free(n1);
            free(n2);
            if_done=1;
        }else if(*(expr + i) == '+' && if_done == 1){
            n1 = strdup(*result);
            if(!n1){
                free(*result);
                return 3;
            }
            int j=0;
            i++;
            if(*(expr + i + j) == '-'){
                j++;
            }
            while(isdigit(*(expr + i + j))){j++;}

            n2 = calloc(j+1, sizeof(char));
            if(!n2){
                free(n1);
                return 3;
            }

            strncpy(n2, expr + position, j);
            free(*result);
            error = add(n1, n2, result);
            if(error == 3){
                free(n1);
                free(n2);
                return 3;
            }
            i+=j-1;
            position=i+2;
            free(n1);
            free(n2);
            if_done=1;
        }else if(*(expr + i) == '-' && if_done == 1){
            n1 = strdup(*result);
            if(!n1){
                return 3;
            }
            int j=0;
            i++;
            if(*(expr + i + j) == '-'){
                j++;
            }
            while(isdigit(*(expr + i + j))){j++;}

            n2 = calloc(j+1, sizeof(char));
            if(!n2){
                free(n1);
                return 3;
            }

            strncpy(n2, expr + i, j);
            free(*result);
            error = subtract(n1, n2, result);
            if(error == 3){
                free(n1);
                free(n2);
                return 3;
            }
            i+=j-1;
            position=i+2;
            free(n1);
            free(n2);
            if_done=1;
        }else if(*(expr + i) == '*' && if_done == 1){
            n1 = strdup(*result);
            if(!n1){
                return 3;
            }
            int j=0;
            i++;
            if(*(expr + i + j) == '-'){
                j++;
            }
            while(isdigit(*(expr + i + j))){j++;}

            n2 = calloc(j+1, sizeof(char));
            if(!n2){
                free(n1);
                return 3;
            }

            strncpy(n2, expr + i, j);
            free(*result);
            error = multiply(n1, n2, result);
            if(error == 3){
                free(n1);
                free(n2);
                return 3;
            }
            i+=j-1;
            position=i+2;
            free(n1);
            free(n2);
            if_done=1;
        }
        i++;
    }
    return 0;
}
int remove_zero(char* number){
    for (int i = 0; i < (int)strlen(number); ++i) {
        if(*(number + i) == '0'){
            for(int j=0;j<(int)strlen(number);j++)
            *(number+j)=*(number+j+1);
            i--;
        }else{
            break;
        }
    }
    return 0;
}
int validate(const char *number){
    if(!number){
        return -1;
    }
    int i=0;
    if(*number == '0' && strlen(number) != 1){return 2;}
    if(isdigit(*(number))){}
    else if(*(number) == '+'){return 2;}
    else if(*(number) == '-'){
        if(!isdigit(*(number + 1))){
            return 2;
        }
        i++;
    }
    else{return 2;}
    while (*(number + i) != 0){
        if(!isdigit(*(number + i))){
            return 2;
        }
        i++;
    }
    return 0;
}
int validate_expression(const char *expr){
    if(!expr){
        return 2;
    }
    int i=0;
    if(*expr == '-'){
        i++;
    }else if(!isdigit(*expr)){
        return 1;
    }
    while(*(expr + i) != 0){
        if(isdigit(*(expr + i))){
            i++;
            continue;
        }else if(*(expr + i) == '+'){
            if(isdigit(*(expr + i + 1))){
                i++;
                continue;
            }
            if(*(expr + i + 1) == '-'){
                if(!isdigit(*(expr + i + 2))){
                    return 1;
                }
                i+=2;
            } else{
                return 1;
            }
        }else if(*(expr + i) == '-'){
            if(isdigit(*(expr + i + 1))){
                i++;
                continue;
            }
            if(*(expr + i + 1) == '-'){
                if(!isdigit(*(expr + i + 2))){
                    return 1;
                }
                i+=2;
            } else{
                return 1;
            }
        }else if(*(expr + i) == '*'){
            if(isdigit(*(expr + i + 1))){
                i++;
                continue;
            }
            if(*(expr + i + 1) == '-'){
                if(!isdigit(*(expr + i + 2))){
                    return 1;
                }
                i+=2;
            } else{
                return 1;
            }
        }else{
            return 1;
        }
    }
    return 0;
}
int abs_number(const char* n1, const char* n2){
    int size_n1 = strlen(n1);
    int size_n2 = strlen(n2);
    int n1_minus=0, n2_minus=0;
    if(*n1 == '-') {
        size_n1--;
        n1_minus=1;
    }
    if(*n2 == '-'){
        size_n2--;
        n2_minus=1;
    }
    //która liczba większa: oczywiste że ta która dłuższa
    if(size_n1 > size_n2){
        return 1;
    }else if(size_n2>size_n1){
        return -1;
    }
    //jeśli takie same to 3 przypadki gdy n1 ma minus gdy n2 ma minus gdy obaj są na minusie i gdy obaj na plusie
    for (int i = 0; i + n1_minus < size_n1 || i + n2_minus < size_n1; n1_minus++, n2_minus++) {
        if((int)*(n1 + n1_minus) > (int)*(n2 + n2_minus)){
            return 1;
        }else if((int)*(n1 + n1_minus) < (int)*(n2 + n2_minus)){
            return -1;
        }
    }
    return 0;
}
int add_minus(char* result){
    int size = strlen(result);
    char *temp;
    temp = realloc(result, sizeof(char)*(size+2));
    if(!temp){
        return 1;
    }
    for (int i = size-1; i >= 0; --i) {
        *(temp+i+1) = *(temp+i);
    }
    *temp='-';
    result = temp;
    return 0;
}
