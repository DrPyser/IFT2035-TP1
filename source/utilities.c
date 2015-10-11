#include <stdio.h>
#include <stdlib.h>
#include "headers/utilities.h"

boolean isDigit(char c){
  return (c >= 48 && c <= 57);
}

boolean isLetter(char c){
  return (c >= 97 && c <= 122);
}
