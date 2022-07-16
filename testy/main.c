#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ctype.h"
#include "tested_declarations.h"
#include "rdebug.h"

int multiply(const char* number1, const char* number2, char** result);
int subtract(const char* number1, const char* number2, char** result);
int add(const char* number1, const char* number2, char** result);
int* add_2(const char* n1, const char* n2);
char* remove_zero(char* number);

int validate(const char *number);
int validate_expression(const char *expr);
int calculate(const char* expr, char **result);

int main() {
    char *wynik;
////    int error = subtract("27482648", "7376385", &wynik);
////    printf("%s\n", wynik);
////    free(wynik);
     add("374", "-140", &wynik);
//    int error = subtract("7376385", "27482648", &wynik);
    printf("%s", wynik);
//    int error = validate_expression("-1+2+-3--1");
//
    //printf("error: %d\nwynik: %s", error, wynik);
    //printf("%d", strlen(wynik));
    return 0;
}
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
    if(*number1 == '-' && *number2=='-'){
        int error_1 = add(number1+1, number2+1, result);
        if(error_1==3){
            return 3;
        }
        //Musze dodać minus do result
        char *temp;
        temp = realloc(result, sizeof(strlen(*result) + 1));
        for (int i = strlen(*result); i > 0; ++i) {
            *(result + i) = (temp + i-1);
        }
        *result = (char *)'-';
        return 0;
    }else if(*number1 == '-' && *number2 != '-'){
        int error_1 = subtract(number1, number2, result);
        if(error_1 == 3){
            return 3;
        }
        return 0;
    }else if(*number2=='-' && *number1 !='-'){
        int error_1 = subtract(number1, number2+1, result);
        if(error_1 == 3){
            return 3;
        }
        return 0;
    }
    //Sprawdzam która liczba jest większa
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
    }else if(size_1 < size_2){
        *result = calloc((size_2+2), sizeof(char));
        if(!*result){
            return 3;
        }
        size = size_2+1;
    }else{
        *result = calloc((size_2+3), sizeof(char));
        if(!*result){
            return 3;
        }
        size = size_2+2;
    }
    int bool=0;
    //mam pewność że jade do większej z liczb
    for (int i = 0; i < size; ++i) {
        int temp=0;
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
        *(*result + size-i-1) = (char)(temp + '0');
    }
    *result = remove_zero(*result);
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
    int bool=0;
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
                *(*result + size - i - 1) = *(number1 + size_1 - i - 1);
                continue;
            }
            int j = 0;
            if (pierwsza - druga < 0) {
                int bool_2 = 0, if_done = 0;
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
                *(*result + size - i - 1) = (char) (temp + '0');
                bool = 0;
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

    //Sprawdzam która liczba jest większa
    int size_1 = (int)strlen(number1);
    int size_2 = (int)strlen(number2);

    /*
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
    int bool=0;
     */
    if(size_1>=size_2) {
        /*
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
                *(*result + size - i - 1) = *(number1 + size_1 - i - 1);
                continue;
            }
            int j = 0;
            if (pierwsza - druga < 0) {
                int bool_2 = 0, if_done = 0;
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
                *(*result + size - i - 1) = (char) (temp + '0');
                bool = 0;
            } else {
                temp = pierwsza - druga;
                *(*result + size - i - 1) = (char) (temp + '0');
            }
        }
         */
        int error = sub(number1, number2, result);
        if(error == 3){
            return 3;
        }
    }else{
        int error = sub(number2, number1, result);
        if(error == 3){
            return 3;
        }
        int cos=0;
        for (int i = 0; i < (int)strlen(*result); ++i) {
            if(*(*result + i) == '0'){
                cos++;
            }else{
                break;
            }
        }
        *(*result + cos-1) = '-';
    }
    *result = remove_zero(*result);
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

    int size_1 = (int)strlen(number1);
    int size_2 = (int)strlen(number2);

    if(size_1>=size_2){
        *result = calloc((size_1 + 2), sizeof(char));

        if(!*result){
            return 3;
        }
        for (int i = 0; i < size_1+1; ++i) {
            *(*result + i) = '0';
        }
        int i=0;
        while (i<size_2){
            if(*(*result + strlen(*result)-1) != '0'){
                char *p;
                int old_length = strlen(*result);
                p = realloc(*result, strlen(*result) + 2);
                if(!p){
                    free(*result);
                    return 3;
                }
                for (int j = old_length; j < (int)strlen(*result); ++j) {
                    *(*result + j) = '0';
                }
            }
            for (int j = 0; j < size_1; ++j) {
                int temp=0;
                int pierwsza = (*(number1 + j)-'0');
                int druga = (*(number2 + i)-'0');
                //printf("%d\n", strlen(*result)-i-j-1);
                int size = strlen(*result)+i-j-1;
                temp = ((*(*result + strlen(*result)-i-j-1))) - '0' + (pierwsza * druga);

                *(*result + size) = temp%10 + '0';
                *(*result + size + 1) = temp/10 + '0';
            }
            i++;
        }
        printf("%s", *result);
        return 0;
    }
    return 10;
}
int calculate(const char* expr, char **result){
    if(!expr || !result){
        return -1;
    }
    return 0;
}
char* remove_zero(char* number){
    for (int i = 0; i < (int)strlen(number); ++i) {
        if(*(number + i) == '0'){
            number++;
            i--;
        }else{
            break;
        }
    }
    return number;
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
        return 1;
    }
    int i=0;
    if(*expr == '-'){
        i++;
    }else if(!isdigit(*expr)){
        return 2;
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
                    return 2;
                }
                i+=2;
            } else{
                return 2;
            }
        }else if(*(expr + i) == '-'){
            if(isdigit(*(expr + i + 1))){
                i++;
                continue;
            }
            if(*(expr + i + 1) == '-'){
                if(!isdigit(*(expr + i + 2))){
                    return 2;
                }
                i+=2;
            } else{
                return 2;
            }
        }else if(*(expr + i) == '*'){
            if(isdigit(*(expr + i))){
                i++;
                continue;
            }
            if(*(expr + i + 1) == '-'){
                if(!isdigit(*(expr + i + 2))){
                    return 2;
                }
                i+=2;
            } else{
                return 2;
            }
        }else{
            return 2;
        }
    }
    return 0;
}

