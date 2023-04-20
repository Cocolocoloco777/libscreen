#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>

#define BACKGROUND(r,g,b) "\x1B[48;2;" #r ";" #g ";" #b "m"
#define RESET "\x1B[0m"

int wbstrlen(char *str){
  int i, count = 0;
  for (i = 0; str[i] != '\0'; i++){
    if ((str[i] & 0x80) == 0){
      count ++;
    } else if ((str[i] & 0xE0) == 0xA0){
      count ++;
      i++;
    } else if ((str[i] & 0xF0) == 0xE0){
      count ++;
      i+= 2;
    } else {
      count ++;
      i+= 3;
    }
  }

  return count;
}

int main(){

  char a[20] = "👃á👃";
  char input[5][20];
  FILE *f;

  printf("%d\n",wbstrlen(a));

  printf("%s\n", a);   
  
  return 0;
}