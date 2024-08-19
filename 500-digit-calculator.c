/* Program to perform multi-digit integer arithmetic.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2023, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
   All included code is (c) Copyright University of Melbourne, 2023

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Lachlan Chue 1463779
   Dated:     11/09/2023

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>

/* All necessary #defines provided as part of the initial skeleton */

#define INTSIZE	500	/* max number of digits per integer value */
#define LINELEN	999	/* maximum length of any input line */
#define NVARS	26	/* number of different variables */

#define CH_A     'a'    /* character 'a', first variable name */

#define ERROR	(-1)	/* error return value from some functions */
#define PROMPT	"> "	/* the prompt string for interactive input */

#define PRINT	'?'	/* the print operator */
#define ASSIGN	'='	/* the assignment operator */
#define PLUS	'+'	/* the addition operator */
#define SUBT    '-' /* the subtraction operator */
#define MULT	'*'	/* the multiplication operator */
#define POWR	'^'	/* the power-of operator */
#define DIVS	'/'	/* the division operator */
#define ALLOPS  "?=+*^/-"

#define CH_ZERO  '0'    /* character zero */
#define CH_ONE   '1'    /* character one */
#define CH_NINE  '9'    /* character nine */

#define CH_COM   ','    /* character ',' */
#define PUT_COMMAS 3    /* interval between commas in output values */

#define INT_ZERO 0	/* integer 0 */
#define INT_TEN  10	/* integer 10 */

#define TRUE 1  /* boolean to return true */
#define FALSE 0 /* boolean to return false */

/* Placeholder typedef for skeleton code
*/
typedef int longint_t[INTSIZE+1];
#define LONG_ZERO 0
#define LONG_ONE  1


/****************************************************************/

/* A "magic" additional function needing explicit declaration */
int fileno(FILE *);

/* Skeleton program function prototypes */

void print_prompt(void);
void print_tadaa();
void print_error(char *message);
int  read_line(char *line, int maxlen);
void process_line(longint_t vars[], char *line);
int  get_second_value(longint_t vars[], char *rhsarg,
	longint_t *second_value);
int  to_varnum(char ident);
void do_print(int varnum, longint_t *var);
void do_assign(longint_t *var1, longint_t *var2);
void do_plus(longint_t *var1, longint_t *var2);
void do_mult(longint_t *var1, longint_t *var2);
void do_powr(longint_t *var1, longint_t *var2);
int do_subt(longint_t *var1, longint_t *var2, longint_t *new_arg);
void do_divs(longint_t *var1, longint_t *var2);
void zero_vars(longint_t vars[]);
void parse_num(char *rhs, longint_t *second_value);
int overflow_check(int n);



/****************************************************************/

/* Main program controls all the action
*/
int
main(int argc, char *argv[]) {
	char line[LINELEN+1] = {0};
	longint_t vars[NVARS];
    
	zero_vars(vars);
    
	print_prompt();
	while (read_line(line, LINELEN)) {
		if (strlen(line) > 0) {
			/* non empty line, so process it */
			process_line(vars, line);
		}
		print_prompt();
	}

	print_tadaa();
	return 0;
}

/****************************************************************/

/* Prints the prompt indicating ready for input, but only if it
   can be confirmed that the input is coming from a terminal.
   Plus, output might be going to a file, that's why the prompt,
   if required, is written to stderr and not stdout
*/
void
print_prompt(void) {
	if (isatty(fileno(stdin))) {
		fprintf(stderr, "> ");
		fflush(stderr);
	}
}

void
print_tadaa() {
	/* all done, so pack up bat and ball and head home,
	   getting the exact final lines right is a bit tedious,
	   because input might be coming from a file and output
	   might be going to a file */
	if (isatty(fileno(stdin)) && isatty(fileno(stdout))) {
		printf("\n");
	}
	printf("ta daa!!!\n");
	if (isatty(fileno(stdin)) && !isatty(fileno(stdout))) {
		fprintf(stderr, "\n");
	}
}

void
print_error(char *message) {
	/* need to write an error message to the right place(s)
	*/
	if (isatty(fileno(stdin)) || isatty(fileno(stdout))) {
		fprintf(stderr, "%s\n", message);
		fflush(stderr);
	}
	if (!isatty(fileno(stdout))) {
		printf("%s\n", message);
	}
}

/****************************************************************/

/* Reads a line of input into the array passed as argument,
   returns false if there is no input available.
   All whitespace characters are removed on the way through.
*/
int
read_line(char *line, int maxlen) {
	int i=0, c;
	while (((c=getchar())!=EOF) && (c!='\n')) {
		if (i<maxlen && !isspace(c)) {
			line[i++] = c;
		}
	}
	line[i] = '\0';
	/* then, if the input is coming from a file or the output
	   is going to a file, it is helpful to echo the input line
	   and record what the command was */
	if (!isatty(fileno(stdin)) || !isatty(fileno(stdout))) {
		printf("%s%s\n", PROMPT, line);
	}
	return ((i>0) || (c!=EOF));
}

/****************************************************************/

/* Process a command by parsing the input line into parts
*/
void
process_line(longint_t vars[], char *line) {
	int varnum, optype;
	longint_t second_value;

	/* determine the LHS variable, it
	   must be first character in compacted line
	*/
	varnum = to_varnum(line[0]);
	if (varnum==ERROR) {
		print_error("invalid LHS variable");
		return;
	}

	/* more testing for validity 
	*/
	if (strlen(line)<2) {
		print_error("no operator supplied");
		return;
	}

	/* determine the operation to be performed, it
	   must be second character of compacted line
	*/
	optype = line[1];
	if (strchr(ALLOPS, optype) == NULL) {
		print_error("unknown operator\n");
		return;
	}

	/* determine the RHS argument (if one is required),
	   it must start in the third character of compacted line
	*/
	if (optype != PRINT) {
		if (strlen(line)<3) {
			print_error("no RHS supplied");
			return;
		}

        zero_vars(&second_value);
		if (get_second_value(vars, line+2, &second_value)==ERROR) {
			print_error("RHS argument is invalid");
			return;
		}
	}
	/* finally, do the actual operation
	*/
    /* if statement determines the relevant operation that 
    links to the function */
	if (line[1] == PRINT) {
		do_print(varnum, vars+varnum);
	} else if (line[1] == ASSIGN) {
        longint_t *second_value_ptr = &second_value;
		do_assign(vars+varnum, second_value_ptr);
	} else if (line[1] == PLUS) {
		do_plus(vars+varnum, &second_value);
	} else if (line[1] == MULT) {
        do_mult(vars+varnum, &second_value);
    } else if (line[1] == POWR) {
        do_powr(vars+varnum, &second_value);
    } else if (line[1] == SUBT) {
        do_subt(vars+varnum, &second_value, vars+varnum);
    } else if (line[1] == DIVS) {
        do_divs(vars+varnum, &second_value);
    } else {
		print_error("operation not available yet");
		return;
	}
    
    /* reset the integer array holding the second value for the next operation 
    */
    for (int i=0; i<INTSIZE+1; i++) {
        (second_value)[i] = INT_ZERO;
    }
	return;
}

/****************************************************************/

/* Convert a character variable identifier to a variable number
*/
int
to_varnum(char ident) {
	int varnum;
	varnum = ident - CH_A;
	if (0<=varnum && varnum<NVARS) {
		return varnum;
	} else {
		return ERROR;
	}
}

/****************************************************************/

/* Process the input line to extract the RHS argument, which
   should start at the pointer that is passed
*/
int
get_second_value(longint_t vars[], char *rhsarg,
			longint_t *second_value) {
	char *p;
	int varnum2;
	if (isdigit(*rhsarg)) {
		/* first character is a digit, so RHS is a number
		   now check the rest of RHS for validity */
		for (p=rhsarg+1; *p; p++) {
			if (!isdigit(*p)) {
				/* nope, found an illegal character */
				return ERROR;
			}
		}
		/* nothing wrong, ok to convert */
		parse_num(rhsarg, second_value);
        
		return !ERROR;
	} else {
		/* argument is not a number, so should be a variable */
		varnum2 = to_varnum(*rhsarg);
		if (varnum2==ERROR || strlen(rhsarg)!=1) {
			/* nope, not a variable either */
			return ERROR;
		}
		/* and finally, get that variable's value */
		do_assign(second_value, vars+varnum2);
		return !ERROR;
	}
	return ERROR;
}

/* Set the vars array to all zero values
*/
void
zero_vars(longint_t vars[]) {
    /* changes all the values in the integer array to 0 */
	int i;
	longint_t zero = {LONG_ZERO};
	for (i=0; i<NVARS; i++) {
		do_assign(vars+i, &zero);
	}
	return;
}

/*****************************************************************
******************************************************************

Your answer to the assignment should start here, using your new
typedef defined at the top of the program. The next few functions
will require modifications because of the change of structure
used for a long_int, and then you'll need to start adding whole
new functions after you get these first ones working properly.
Try and make the new functions fit the style and naming pattern
of the existing ones, ok?

******************************************************************
*****************************************************************/

/* Create an internal-format number out of a string
*/
void
parse_num(char *rhs, longint_t *second_value) {
    int n=strlen(rhs), invalid_char=0;
    overflow_check(n); /* checks if the number of digits in input exceeds limit */
    for (int i=0; i<n; i++) {
        if (rhs[i] < 49 || rhs[i] > 57) { /* using ASCII, determines if the value is not numeric */
            invalid_char++;
        } else {
            break; 
        }
    }
    
    for (int i=n; i>=invalid_char; i--) {
        char temp=rhs[i];
        ((*second_value)[n-i]) = atoi(&temp); /* changes all character values into integers */
    }
    ((*second_value)[0]) = n-invalid_char; /* takes the length of the input minus any values that were not numeric */
}

/****************************************************************/

/* Print out a longint value
*/
void
do_print(int varnum, longint_t *var) {
	printf("register %c: ", varnum+CH_A);
    int n=((*var)[0]);
    for (int i=n; i>0; i--) {
        printf("%d", (*var)[i]);
        if ((((i-1)-3)%3 == 0) && (i != 1)) { 
            printf(",");
        }
    }
    printf("\n");
}

/****************************************************************/

/* Assign a longint value, could do this with just an assignment
   statement, because structs can be assigned, but this is more
   elegant, and only copies over the array elements (digits) that
   are currently in use: var1 = var2

*/
void
do_assign(longint_t *var1, longint_t *var2) {
    for (int i=0; i<=((*var2)[0]); i++) {
        (*var1)[i] = (*var2)[i];
    }
}

/****************************************************************/

/* Update the indicated variable var1 by doing an addition
   using var2 to compute var1 = var1 + var2
*/
void
do_plus(longint_t *var1, longint_t *var2) {
	int n1 = (*var1)[0], n2 = (*var2)[0], n_smll=0, n_big=0, count=0;
    /* determines which of the variables are longer 
    */
    if (n2>n1) {
        n_smll = n1; 
        n_big = n2; 
    } else {
        n_smll = n2;
        n_big = n1;
    }
    
    /* initialises count to be the longest length as addition can only yield a longer value
    */
    count = n_big;
    
    /* iterating up to the smallest length, adding the values together 
    */
    for (int i=1; i<n_smll+1; i++) { 
        (*var1)[i] += (*var2)[i]; 
        
        if ((*var1)[i] >= 10) { 
            (*var1)[i] -= 10; 
            (*var1)[i+1] += 1; 
            if ((i+1) >= n_big+1) { 
                count++;
            }
        }
    }

    for (int i = n_smll+1; i < n_big+1; i++) {
        (*var1)[i] += (*var2)[i];
        if ((*var1)[i] >= 10) {
            (*var1)[i] -= 10; 
            (*var1)[i+1] += 1; 
            if ((i+1) >= n_big+1) { 
                count++;
            }
        }
    }
    overflow_check(count);
    (*var1)[0] = count;
}

/*****************************************************************
******************************************************************

Put your new functions below this line. Make sure you add suitable
prototypes at the top of the program.

******************************************************************
*****************************************************************/
/* a multiplication function that takes two long integer values and returns its product
*/
void 
do_mult(longint_t *var1, longint_t *var2) {
    longint_t subtotal, addition;
    int n1=(*var1)[0], n2=(*var2)[0], value=0, count=1, remainder=0, carry=0, indent=0, length=0;
    longint_t *subtotal_ptr = &subtotal;
    longint_t *add_ptr = &addition;
    
    for (int k=0; k<INTSIZE+1; k++) {
        subtotal[k] = INT_ZERO;
        addition[k] = INT_ZERO;
    }
    
    for (int i=1; i<n2+1; i++) {
        for (int j=1; j<n1+1; j++) {
            value = (*var2)[i] * (*var1)[j];
            length++;
            if (value >= 10) {
                carry = (addition[count+indent]+value)/10;
                remainder = (addition[count+indent]+value)%10;
                addition[count+indent] = remainder;
                addition[count+1+indent] += carry;
                carry = 0;
                remainder = 0;

                if (j == n1) {
                    length++;
                }
            } else {
                addition[count+indent] += value;
            }
            count++;
        }
        length += indent;
        overflow_check(length);
        addition[0] = length;
        length=0;

        do_plus(subtotal_ptr, add_ptr);

        indent++;
        count=1;
        
        for (int l=0; l<INTSIZE+1; l++) {
            addition[l] = 0;
        }
    }
    
    for (int g=0; g<subtotal[0]+1; g++) {
        (*var1)[g] = subtotal[g];
    }
}

/* a power function that takes a value and a power value and returns an expontential value
*/
void
do_powr(longint_t *var1, longint_t *var2) {
    int power_index=0;
    longint_t subtotal;
    longint_t *subtotal_ptr = &subtotal;
    
    for (int k=0; k<INTSIZE+1; k++) {
        subtotal[k] = INT_ZERO;
    }
    
    for (int i=0; i<=(*var1)[0]; i++) {
        subtotal[i] = (*var1)[i];
    }
    
    for (int i=(*var2)[0]; i>=0; i--) {
        power_index = power_index + ((*var2)[i]) * pow(10, (i-1));
    }

    if (power_index == INT_ZERO) {
        subtotal[0] = 1;
        subtotal[1] = 1;
    } else {
        for (int i=1; i<power_index; i++) {
            do_mult(subtotal_ptr, var1);
        }
    }
    for (int i=0; i<subtotal[0]+1; i++) {
        (*var1)[i] = subtotal[i];
    }
}

/* a subtraction function that takes two long integer values and subtracts one from the other
*/
int
do_subt(longint_t *var1, longint_t *var2, longint_t *new_arg) {
    int n1 = (*var1)[0], n2 = (*var2)[0], n_smll=0, n_big=0, count=0;
    longint_t result;
    
    if (n1 < n2) {
        return 0;
    } else if (n2 == n1) {
        if ((*var1)[n1] < (*var2)[n2]) {
            return 0;
        }
    }
    
    for (int i=0; i<INTSIZE+1; i++) {
        result[i] = INT_ZERO;
    }
    
    for (int i=0; i<n1+1; i++) {
        result[i] = (*var1)[i];
    }
    
    if (n2>n1) {
        n_smll = n1; 
        n_big = n2; 
    } else {
        n_smll = n2;
        n_big = n1;
    }
    
    count = n_big; 
   
    for (int i=1; i<n_smll+1; i++) {
        result[i] = (*var1)[i] - (*var2)[i]; 
        
        if (result[i] < 0) { 
            result[i] = ((*var1)[i]+10) - (*var2)[i]; 
            (*var1)[i+1] -= 1; 
        }
        if (result[n_big] <= 0) { 
            n_smll--;
        }
    }

    for (int i = n_smll+1; i < n_big+1; i++) {
        result[i] = (*var1)[i] - (*var2)[i];
        
        if (result[i] < 0) { 
            result[i] += 10; 
            result[i+1] -= 1;
        }
        if (result[n_big] <= 0) { 
            count--;
        }
    }

    result[0] = count;
    
    for (int i=0; i<count+1; i++) {
        (*new_arg)[i] = result[i];
    }
    return 1;
}

void 
do_divs(longint_t *var1, longint_t *var2) {
    longint_t remainder;
    longint_t temp;
    longint_t *temp_ptr = &temp;
    int n1 = (*var1)[0];

    for (int i=0; i<=n1; i++) {
        remainder[i] = (*var1)[i];
    }
    remainder[0]++;
    for (int i=0; i<n1+1; i++) {
        (*var1)[0] = i;
        if (do_subt(var1, var2, temp_ptr) == 1) {
            remainder[n1+1-i] = temp[1];
            remainder[0] -= i;
        }
    }
}

int
overflow_check(int n) {
    if (n>INTSIZE) {
        printf("Integer Overflow Detected - Value Cannot Exceed 500 Digits\n");
        exit(EXIT_FAILURE);
        return 0;
    } else {
        return 1;
    }
}

/* algorithms are fun */