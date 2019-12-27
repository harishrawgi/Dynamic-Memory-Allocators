#ifndef POLYNOMIALS_H
#define POLYNOMIALS_H

#define INT_MAX 999999

typedef struct term{

  //represents the power which the term has
  int power;

  //the coefficient of the term
  float coefficient;

  //pointers for the doubly linked list
  struct term* next;
  struct term* prev;

} *Term;

typedef struct polynomial{

  //pointers to the first and last terms of the polynomial
  Term first;
  Term last;

  //the degree of the polynomial
  int degree;

} *Polynomial;

//function to create a new term of polynomial with supplied power and coefficient
Term createTerm(int power, float coefficient);

//function to create an empty polynomial
Polynomial createPolynomial();

//function to free the polynomial
void freePolynomial(Polynomial inputPolynomial);

//function to copy a polynomial to another empty polynomial
int copyPolynomial(Polynomial inputPolynomial, Polynomial outputPolynomial);

//function to add a term to the polynomial
int addTermToPolynomial(Polynomial inputPolynomial, Term inputTerm);

//function to input a polynomial
Polynomial takeInputPolynomial();

//function to print a polynomial
void printPolynomial(Polynomial poly);

//function to delete a term from the polynomial
int deleteTermFromPolynomial(Polynomial inputPolynomial, Term inputTerm);

//function to divide term a by term b
Term divideTerms(Term a, Term b);

//function to subtract polynomial b from polynomial a
//the result is stored in polynomial a itslef
int subtractPolynomials(Polynomial a, Polynomial b);

//function to multiply a polynomial with a single term
int multiplyPolnomialWithTerm(Polynomial inputPolynomial, Term inputTerm, Polynomial resPolynomial);

//function for polynomial division
int dividePolynomials(Polynomial dividend, Polynomial divisor, Polynomial quotient, Polynomial remainder);

// getter function for mem_mgmt_time
double getTime();

#endif
