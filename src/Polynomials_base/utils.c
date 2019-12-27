#include <stdio.h>
#include "utils.h"

//function to check valid string character
int checkValidStringInt(char *str){
  for(int i=0; str[i] != '\0'; i++){
    if(str[i] < '0' || str[i] > '9'){
      printf("\nInvalid character found.\n");
      return 0;
    }
  }
  return 1;
}
