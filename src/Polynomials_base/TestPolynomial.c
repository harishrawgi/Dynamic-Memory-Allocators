#include <stdio.h>
#include <stdlib.h>
#include "Polynomials.h"

int main(){

  //input the divident polynomial
  printf("\nEnter the dividend polynomial.");
  Polynomial dividendPolynomial = takeInputPolynomial();
  if(!dividendPolynomial){
    printf("\nFailed to take input.\n");
    return 0;
  }

  //input the divisor polynomial
  printf("\nEnter the divisor polynomial.");
  Polynomial divisorPolynomial = takeInputPolynomial();
  if(!divisorPolynomial){
    printf("\nFailed to take input.\n");
    return 0;
  }

  int n=1000;

  while(n){

    n--;

    //create empty polynomials to store the result
    Polynomial quotientPolynomial = createPolynomial();
    if(!quotientPolynomial){
      printf("\nFailed to take input.\n");
      return 0;
    }
    Polynomial remainderPolynomial = createPolynomial();
    if(!remainderPolynomial){
      printf("\nFailed to take input.\n");
      return 0;
    }

    //perform the division
    if(!dividePolynomials(dividendPolynomial, divisorPolynomial, quotientPolynomial, remainderPolynomial)){
      printf("\nFailed to divide polynomials.\n");
      return 0;
    }

    //free all the polynomials we used
    freePolynomial(remainderPolynomial);
    freePolynomial(quotientPolynomial);

  }

  double timeTaken = getTime();
  printf("\nTime taken for memory management in the program by standard allocator: %lf\n", timeTaken);

  printf("\nEnding the program....\n");


  return 0;
}
