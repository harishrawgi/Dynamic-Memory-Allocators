#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Polynomials.h"
#include "utils.h"
#include "OneBin.h"

double mem_mgmt_time = 0;

// getter function for mem_mgmt_time
double getTime(){
  return mem_mgmt_time;
}

//function to create a new term of polynomial with supplied power and coefficient
Term createTerm(int power, float coefficient){

  //check that the power and coefficient aree valid
  if(power < 0 || coefficient == 0){
    printf("\nInvalid input (%d, %f) for creation of a term.\n", power, coefficient);
    return NULL;
  }

  // clock_t variables for storing timestamps to calculate execution times
  clock_t start_time, end_time;
  // variables to store the time taken
  double time_taken;

  //allocate memory for a new term

  // start time for malloc call
  start_time = clock();
  Term newTerm = (Term) ob_malloc();
  end_time = clock();
  // end time for malloc call

  if(!newTerm){
    printf("\nUnable to allocate memory for a new term.\n");
    return NULL;
  }

  //calculate the time taken for malloc call and add it to mem mgmt time
  time_taken = ((double) (end_time-start_time)) / CLOCKS_PER_SEC;
  mem_mgmt_time += time_taken;

  //everything went well, initialize newTerm and return
  newTerm->power = power;
  newTerm->coefficient = coefficient;
  newTerm->next = NULL;
  newTerm->prev = NULL;

  return newTerm;
}

//function to create an empty polynomial
Polynomial createPolynomial(){

  // clock_t variables for storing timestamps to calculate execution times
  clock_t start_time, end_time;
  // variables to store the time taken
  double time_taken;

  //allocate memory for a new term

  // start time for malloc call
  start_time = clock();
  Polynomial newPolynomial = (Polynomial) ob_malloc();
  end_time = clock();
  // end time for malloc call

  if(!newPolynomial){
    printf("\nUnable to create an empty polynomial.\n");
    return NULL;
  }

  //calculate the time taken for malloc call and add it to mem mgmt time
  time_taken = ((double) (end_time-start_time)) / CLOCKS_PER_SEC;
  mem_mgmt_time += time_taken;

  newPolynomial->first = NULL;
  newPolynomial->last = NULL;
  newPolynomial->degree = -1;

  return newPolynomial;

}

//function to free the polynomial
void freePolynomial(Polynomial inputPolynomial){

  //there are no terms, so we just have to free the polynomial itself
  if(inputPolynomial->degree == -1){

    // clock_t variables for storing timestamps to calculate execution times
    clock_t start_time, end_time;
    // variables to store the time taken
    double time_taken;

    // start time for free call
    start_time = clock();
    ob_free(inputPolynomial);
    end_time = clock();
    // end time for free call

    //calculate the time taken for malloc call and add it to mem mgmt time
    time_taken = ((double) (end_time-start_time)) / CLOCKS_PER_SEC;
    mem_mgmt_time += time_taken;

    return;
  }


  Term curTerm = inputPolynomial->first;
  Term nextTerm = curTerm->next;

  //we iterate the list of Terms and free one Term at a time
  while(nextTerm){
    // clock_t variables for storing timestamps to calculate execution times
    clock_t start_time, end_time;
    // variables to store the time taken
    double time_taken;

    // start time for free call
    start_time = clock();
    ob_free(curTerm);
    end_time = clock();
    // end time for free call

    //calculate the time taken for malloc call and add it to mem mgmt time
    time_taken = ((double) (end_time-start_time)) / CLOCKS_PER_SEC;
    mem_mgmt_time += time_taken;

    curTerm = nextTerm;
    nextTerm = curTerm->next;
  }

  // clock_t variables for storing timestamps to calculate execution times
  clock_t start_time, end_time;
  // variables to store the time taken
  double time_taken;

  // start time for free call
  start_time = clock();
  ob_free(curTerm);
  end_time = clock();
  // end time for free call

  //calculate the time taken for malloc call and add it to mem mgmt time
  time_taken = ((double) (end_time-start_time)) / CLOCKS_PER_SEC;
  mem_mgmt_time += time_taken;

  // start time for free call
  start_time = clock();
  ob_free(inputPolynomial);
  end_time = clock();
  // end time for free call

  //calculate the time taken for malloc call and add it to mem mgmt time
  time_taken = ((double) (end_time-start_time)) / CLOCKS_PER_SEC;
  mem_mgmt_time += time_taken;


}

//function to copy a polynomial to another empty polynomial
int copyPolynomial(Polynomial inputPolynomial, Polynomial outputPolynomial){

  if(outputPolynomial->degree != -1){
    printf("\nCannot copy to a non-empty polynomial.\n");
    return 0;
  }

  //term for iterating through the polynomial
  Term curTerm = inputPolynomial->first;

  while(curTerm){
    Term newTerm = createTerm(curTerm->power, curTerm->coefficient);
    if(!newTerm){
      printf("\nFailed to create a new term (%d, %f).\n", curTerm->power, curTerm->coefficient);
      return 0;
    }
    addTermToPolynomial(outputPolynomial, newTerm);
    curTerm = curTerm->next;
  }

  return 1;
}

//function to add a term to the polynomial
int addTermToPolynomial(Polynomial inputPolynomial, Term inputTerm){

  //if first term being added
  if(inputPolynomial->degree == -1){
    inputPolynomial->first = inputTerm;
    inputPolynomial->last = inputTerm;
    inputPolynomial->degree = inputTerm->power;
    return 1;
  }

  //term for iterating through the polynomial
  Term curTerm = inputPolynomial->first;

  //if the power of the new term is higher than the degree, we make it the new fisrt term
  if(curTerm->power < inputTerm->power){
    inputTerm->next = curTerm;
    curTerm->prev = inputTerm;
    inputPolynomial->first = inputTerm;
    inputPolynomial->degree = inputTerm->power;
    return 1;
  }

  //iterate to the first term in polynomial with power less than the input term or the last term of polynomial
  while(curTerm->next && (curTerm->power >= inputTerm->power)){
    if(curTerm->power == inputTerm->power){
      /*
      curTerm->coefficient += inputTerm->coefficient;
      //free the input term as we already have a term with same power
      free(inputTerm);
      //if the coefficient becomes zero, delete this term from polynomial
      if(curTerm->coefficient == 0){
        deleteTermFromPolynomial(inputPolynomial, curTerm);
      }
      return 1;
      */
      printf("\nIt should never reach here.\n");
      return 0;
    }
    curTerm = curTerm->next;
  }

  //input term is of power less than any term in the polynomial
  if(!curTerm->next && (curTerm->power >= inputTerm->power)){
    curTerm->next = inputTerm;
    inputTerm->prev = curTerm;
    inputPolynomial->last = inputTerm;
    return 1;
  }
  //input term has an intermediate power
  else{
    Term prevTerm = curTerm->prev;
    prevTerm->next = inputTerm;
    inputTerm->prev = prevTerm;
    inputTerm->next = curTerm;
    curTerm->prev = inputTerm;
    return 1;
  }

}

//function to input a polynomial
Polynomial takeInputPolynomial(){

  //creating an empty polynomial
  Polynomial resPolynomial = createPolynomial();
  if(!resPolynomial){
    printf("\nUnable to create an empty polynomial for taking input.\n");
    return NULL;
  }

  printf("\nEnter the polynomial in the form of comma seperated values (i,coefficient) in decreasing order of i. (coefficient can't be zero)\n");

  int curPower, prevPower = INT_MAX;
  float coefficient;
  int ch;
  char powerStr[10];
  char coefficientStr[10];

  while(1){
    ch = fgetc(stdin);

    //ch indicating start of a new term
    if(ch == '('){
      scanf("%[^,]", powerStr);
      if(!checkValidStringInt(powerStr)){
        printf("\nInvalid power input: %s\n", powerStr);
        return NULL;
      }
      curPower = atoi(powerStr);
      if(curPower >= prevPower){
        printf("\nPowers should be in decreasing order.\n");
        return NULL;
      }
      ch=fgetc(stdin);
      if(ch != ','){
        printf("\nExpected ',' found %c.\n", ch);
        return NULL;
      }
      scanf("%[^)]", coefficientStr);
      coefficient = atof(coefficientStr);
      if(!coefficient){
        printf("\nInvalid coefficient input: %s\n", coefficientStr);
        return NULL;
      }
      ch=fgetc(stdin);
      if(ch != ')'){
        printf("\nExpected ')' found %c.\n", ch);
        return NULL;
      }

      Term newTerm = createTerm(curPower, coefficient);
      if(!newTerm){
        printf("\nUnable to create a new term (%d, %f).\n", curPower, coefficient);
        return NULL;
      }

      if(!addTermToPolynomial(resPolynomial, newTerm)){
        printf("\nUnable to add a new term (%d, %f).\n", curPower, coefficient);
        return NULL;
      }

      prevPower = curPower;
    }

    //ch indicating end of polynomial
    else if('\n' == ch){
      return resPolynomial;
    }

    //ch is invalid
    else{
      printf("\nInvalid input: %c\n", ch);
      return NULL;
    }

  }


}

//function to print a polynomial
void printPolynomial(Polynomial poly){

  //empty polynomial
  if(poly->degree == -1){
    printf("Polynomial is empty.\n");
    return;
  }

  //term to iterate through polynomial
  Term curTerm = poly->first;

  //printing the terms one by one
  while(curTerm){
    printf("(%d, %f)", curTerm->power, curTerm->coefficient);
    curTerm = curTerm->next;
  }

}

//function to delete a term from the polynomial
int deleteTermFromPolynomial(Polynomial inputPolynomial, Term inputTerm){

  //invalid deletion
  if(inputPolynomial->degree == -1){
    printf("\nCan't delete from an empty polynomial.\n");
    return 0;
  }

  //term to iterate through the polynomial
  Term curTerm = inputPolynomial->first;

  //if we are deleting the first term
  if(curTerm->power == inputTerm->power){
    Term nextTerm = curTerm->next;

    //if this is the only term
    if(!nextTerm){
      // clock_t variables for storing timestamps to calculate execution times
      clock_t start_time, end_time;
      // variables to store the time taken
      double time_taken;

      // start time for free call
      start_time = clock();
      ob_free(curTerm);
      end_time = clock();
      // end time for free call

      //calculate the time taken for malloc call and add it to mem mgmt time
      time_taken = ((double) (end_time-start_time)) / CLOCKS_PER_SEC;
      mem_mgmt_time += time_taken;

      inputPolynomial->first = NULL;
      inputPolynomial->last = NULL;
      inputPolynomial->degree = -1;
      return 1;
    }

    //there is atleast one more term
    inputPolynomial->first = nextTerm;
    inputPolynomial->degree = nextTerm->power;
    nextTerm->prev = NULL;
    // clock_t variables for storing timestamps to calculate execution times
    clock_t start_time, end_time;
    // variables to store the time taken
    double time_taken;

    // start time for free call
    start_time = clock();
    ob_free(curTerm);
    end_time = clock();
    // end time for free call

    //calculate the time taken for malloc call and add it to mem mgmt time
    time_taken = ((double) (end_time-start_time)) / CLOCKS_PER_SEC;
    mem_mgmt_time += time_taken;
    return 1;
  }

  //iterating through the polynomial till we delete the term or reach the last term
  while(curTerm->next){
    if(curTerm->power == inputTerm->power){
      Term nextTerm = curTerm->next;
      Term prevTerm = curTerm->prev;
      nextTerm->prev = prevTerm;
      // clock_t variables for storing timestamps to calculate execution times
      clock_t start_time, end_time;
      // variables to store the time taken
      double time_taken;

      // start time for free call
      start_time = clock();
      ob_free(curTerm);
      end_time = clock();
      // end time for free call

      //calculate the time taken for malloc call and add it to mem mgmt time
      time_taken = ((double) (end_time-start_time)) / CLOCKS_PER_SEC;
      mem_mgmt_time += time_taken;
      return 1;
    }
    curTerm = curTerm->next;
  }

  //we are at the last term
  if(!curTerm->next){
    if(curTerm->power == inputTerm->power){
      Term prevTerm = curTerm->prev;
      prevTerm->next = NULL;
      inputPolynomial->last = prevTerm;
      // clock_t variables for storing timestamps to calculate execution times
      clock_t start_time, end_time;
      // variables to store the time taken
      double time_taken;

      // start time for free call
      start_time = clock();
      ob_free(curTerm);
      end_time = clock();
      // end time for free call

      //calculate the time taken for malloc call and add it to mem mgmt time
      time_taken = ((double) (end_time-start_time)) / CLOCKS_PER_SEC;
      mem_mgmt_time += time_taken;
      return 1;
    }
  }

  //unable to find the term to delete
  return 0;

}

//function to divide term a by term b
Term divideTerms(Term a, Term b){

  //check if the division is valid
  if(a->power < b->power){
    printf("\nInvalid division of terms.\n");
    return NULL;
  }

  //find the power and coefficient of the new resultant term
  int newPower = a->power - b->power;
  float newCoefficient = (a->coefficient)/(b->coefficient);

  //printf("\nnew power: %d", newPower);
  //printf("\nnew coefficient: %f", newCoefficient);

  //create a resultant term
  Term resTerm = createTerm(newPower, newCoefficient);
  if(!resTerm){
    printf("\nUnable to create a resultant term for divison of terms.\n");
    return NULL;
  }

  return resTerm;

}

//function to subtract polynomial b from polynomial a
//the result is stored in polynomial a itslef
int subtractPolynomials(Polynomial a, Polynomial b){

   //terms which represent the terms of a and b currently in iteration
   Term curTermA = a->first;
   Term curTermB = b->first;

   //iterating over both ploynomials till atleast one ends
   while(curTermA && curTermB){

     //if the current term of a is of higher power, we don't have to do anything
     if(curTermA->power > curTermB->power){
       curTermA = curTermA->next;
     }
     //if the current power of b is higher than a, we need to add this term with sign reversed
     else if(curTermA->power < curTermB->power){
       //create a new term with inverted sign
       Term newTerm = createTerm(curTermB->power, (-1)*(curTermB->coefficient));
       //add the term to the polynomial a
       if(!addTermToPolynomial(a, newTerm)){
         printf("\nUnable to add a new term (%d, %f) to the polynomial.\n", newTerm->power, newTerm->coefficient);
         return 0;
       }
       curTermB = curTermB->next;
     }
     //if the powers are equal, we subtract the coefficients
     else{
       curTermA->coefficient -= curTermB->coefficient;
       if(curTermA->coefficient == 0){
         //delete the term if coefficient becomes zero
         if(!deleteTermFromPolynomial(a, curTermA)){
           printf("\nUnable to delete the term (%d, %f).\n", curTermA->power, curTermA->coefficient);
           return 0;
         }
       }
       curTermA = curTermA->next;
       curTermB = curTermB->next;
     }
   }

   //if some terms are left in polynomial b, we must add them to the the polynomial a
   while(curTermB){
     //create a new term with inverted sign
     Term newTerm = createTerm(curTermB->power, (-1)*(curTermB->coefficient));
     //add the term to the polynomial a
     if(!addTermToPolynomial(a, newTerm)){
       printf("\nUnable to add a new term (%d, %f) to the polynomial.\n", newTerm->power, newTerm->coefficient);
       return 0;
     }
     curTermB = curTermB->next;
   }

   return 1;

}

//function to multiply a polynomial with a single term
int multiplyPolnomialWithTerm(Polynomial inputPolynomial, Term inputTerm, Polynomial resPolynomial){

  //creating a new polynomial for storing the result
  if(resPolynomial->degree != -1){
    printf("\nCannot store result in a non-empty polynomial.\n");
    return 0;
  }

  //term for iterating through the polynomial
  Term curTerm = inputPolynomial->first;

  //iterating and multiplying the input term with each term of polynomial
  while(curTerm){

    //calculate the new power and coefficient as a result of multiplying current term with the input term
    int newPower = (curTerm->power) + (inputTerm->power);
    float newCoefficient = (curTerm->coefficient) * (inputTerm->coefficient);

    //create a new term with calculated power and coefficient
    Term newTerm = createTerm(newPower, newCoefficient);
    if(!newTerm){
      printf("\nUnable to create a new term (%d, %f) while multiplying polynomials.\n", newPower, newCoefficient);
      return 0;
    }

    //add the new term to the resultant polynomial
    if(!addTermToPolynomial(resPolynomial, newTerm)){
      printf("\nUnable to add term (%d, %f) to resultant polynomial.\n", newTerm->power, newTerm->coefficient);
      return 0;
    }

    curTerm = curTerm->next;
  }

  //return successfully
  return 1;

}

//function for polynomial division
int dividePolynomials(Polynomial dividend, Polynomial divisor, Polynomial quotient, Polynomial remainder){

  //check if divsion is valid
  if(dividend->degree < divisor->degree){
    printf("\nInvalid divsion as degree of divisor is greater than degree of dividend.\n");
    return 0;
  }

  //copy the dividend polynomial into the empty remainder polynomial
  if(!copyPolynomial(dividend, remainder)){
    printf("\nUnable to copy dividend polynomial to remainder polynomial.\n");
    return 0;
  }

  //highest power terms of remainder and divisor
  Term curTermRemainder = remainder->first;
  Term curTermDivisor = divisor->first;

  //keep dividing till the degree of remainder is less than the degree of divisor
  while(curTermRemainder->power >= curTermDivisor->power){

    //find the quotient term by dividing the highest degree remainder term with the divisor term
    Term quotientTerm = divideTerms(curTermRemainder, curTermDivisor);
    if(!quotientTerm){
      printf("\nFailed to divide highest degree remainder term with divisor term.\n");
      return 0;
    }

    //printf("\nQuotient term is: (%d, %f)\n", quotientTerm->power, quotientTerm->coefficient);

    //add this term to the quotient polynomial
    if(!addTermToPolynomial(quotient, quotientTerm)){
      printf("\nFailed to add quotient term (%d, %f) to the quotient polynomial.\n", quotientTerm->power, quotientTerm->coefficient);
      return 0;
    }

    //printf("\nAdded Quotient term (%d, %f)\n", quotientTerm->power, quotientTerm->coefficient);

    //create an empty polynomial to store intermediate polynomial
    Polynomial toSubtract = createPolynomial();
    if(!toSubtract){
      printf("\nFailed to create an empty polynomial.\n");
      return 0;
    }

    //calculate the polynomial which will be subtracted from the current remainder
    if(!multiplyPolnomialWithTerm(divisor, quotientTerm, toSubtract)){
      printf("\nFailed to calculate the polynomial to be subtracted from the current remainder.\n");
      return 0;
    }

    //printf("\nPolynomial to be subtracted is: ");
    //printPolynomial(toSubtract);

    //calculate the new remainder
    if(!subtractPolynomials(remainder, toSubtract)){
      printf("\nFailed to the polynomial from the current remainder.\n");
      return 0;
    }

    //printf("\nNew remainder is: ");
    //printPolynomial(remainder);

    //free the intermediate polynomial
    freePolynomial(toSubtract);

    //update the current remainder term to point to new remainder's highest degree term
    if(remainder->first){
      curTermRemainder = remainder->first;
    }else{
      //remainder has become 0
      return 1;
    }


  }

  //divison successfully done
  return 1;

}
