#include <stdio.h>

int main(){
    int test = getscancode();
    puts("What do you want to do?(-, +, *, /):");
    char *choice_calculator = gets();
    puts("\nFirst number:");
    char *first_number = gets();
    puts("\nSecond number:");
    char *second_number = gets();
    if(!str_is_digit(first_number) && !str_is_digit(second_number)){
        puts("\nSomewhere found not a number");
    }
    return 0;
}


int str_is_digit(char *str){
    for(int i=0; str[i]!='\0'; i++)
        if(str[i] != '1' && str[i] != '2' && str[i] != '3' && str[i] != '4' && str[i] != '5' && 
        str[i] != '6' && str[i] != '7' && str[i] != '8' && str[i] != '9') return 0;
    return 1;
}