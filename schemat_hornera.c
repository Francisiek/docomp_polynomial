#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>

// #define DEBUG_MODE

int printe(char format[], ...) {
    va_list arg;
    vfprintf(stderr, format, arg);
    exit(EXIT_FAILURE);
}

typedef struct Polynomial {
    unsigned int power;
    int coefficient;
} Polynomial;

Polynomial default_polynomial = {};

bool is_default_polynomial(Polynomial x) {
    return (x.coefficient == default_polynomial.coefficient) &&
        (x.power == default_polynomial.power);
}

typedef struct Fraction {
    int up;
    unsigned int down;
} Fraction;

Fraction default_fraction = {};

bool is_default_fraction(Fraction x) {
    return (x.up == default_fraction.up) && 
        (x.down == default_fraction.down);
}

Fraction get_fraction(int up, int down) {
    Fraction x = {up, down};
    return x;
}

Polynomial get_next_polynomial(char polynomial[]) {
    // returns one polynomial in form [-/+][n]x[^n]
    static char* w = NULL;
    static size_t i = 0;

    if (polynomial != w) 
        i = 0;

    w = polynomial;
 
    while (polynomial[i]) {
        Polynomial x = {0, 1};
        int sign = 1;
        bool was_digit = false;
        bool was_x = false;

        while (isspace(polynomial[i]))
            i++;

        if (polynomial[i] == '\0')
            return default_polynomial;

        if (polynomial[i] == '-' || polynomial[i] == '+') {
            if (polynomial[i] == '-') // append only sign
                sign = -1;
            x.coefficient *= sign;
            i++;
        } /*else if (i > 0) {
            printe("sign required or there's some shit\n");
        } */

        while (isspace(polynomial[i]))
            i++;

        if (isdigit(polynomial[i])) {
            was_digit = true;
            x.coefficient = sign * atoi(&polynomial[i]);
            while (isdigit(polynomial[++i]))
                ;
        }
 
        if (polynomial[i] == 'x') {
            was_x = true;
            x.power = 1;
            i++;
            bool was_space = false;

            while (isspace(polynomial[i])) {
                was_space = true;
                i++;
            }

            if (polynomial[i] == '^' && !was_space) {
                i++;
                if (isdigit(polynomial[i])) { 
                    x.power = atoi(&polynomial[i]);
                    while(isdigit(polynomial[++i]))
                        ;
                }
                else
                    printe("missing or wrong power\n");
            } else if ((polynomial[i] != '+') && (polynomial[i] != '-') && (polynomial[i] != '\0'))
                printe("wrong char after x\n"); 

        } else {
            while (isspace(polynomial[i]))
                i++;
            if (was_digit && (polynomial[i] == '+' || polynomial[i] == '-') || 
                (polynomial[i] == '\0' && was_digit)) {
                // only digit occoured - no x
                return x;
            } else {
                // no digit and sth other than x
                // error we accept only x or there was wrong char
                printe("no digit or sth other than x\n");
            }
        }
 
        return x;
    }
    i = 0;
    return default_polynomial;
}

// table for storing last divisor for a number
int last_divisors[2];

void clear_last_div(void) {
    last_divisors[0] = 0;
    last_divisors[1] = 1;
}

static int last_number = 0;
static int last_div = 1;

int divisors(int number) {
// zwraca po jednym następnym naturalnym dzielniku 'number'
// jednocześnie pamięta po ostatnim dzielniku dwóch liczb 'last_divisors'
    number = abs(number);
    static int last_number = 0;
    static int last_div = 1;
    static bool use_saved_div = false;

    if (number != last_number || last_div == 0) {
        if (number == last_divisors[0]) {
            last_div = last_divisors[1];
            use_saved_div = true;
        } else {
            last_div = 1;
            use_saved_div = false;
        }
    }

    last_number = number;

    while (last_div <= number) {
        if (number % (last_div++) == 0) {
            if (use_saved_div)
                last_divisors[1] = last_div;
            return last_div - 1;
        }
    }

    if (use_saved_div)
        last_divisors[1] = 1;
    return last_div = 0;
}

// sets only number, not it's divisor, better use before 'clear_last_div'
void set_last_div(int x) {
    last_divisors[0] = abs(x);
    last_number = 0;
    last_div = 1;
}


int max_power = 0, min_power = INT_MAX;
int coefficients[256];
int first_product_power;
double rest;

Fraction get_next_divisor(void) {
// zwraca wymierny dzielnik p/q taki, że p dzieli wyraz wolny,
// a q wyraz największej potęgi
// przed wywołaniem, wywołaj clear_last_div

    static int free_word = 0;
    static int maxp_word = 0;
    static bool was_q_set = false;
    static Fraction f = {};

    if (free_word != coefficients[0]) {
        free_word = coefficients[0];
        maxp_word = coefficients[max_power];
        was_q_set = false;
    }

    set_last_div(maxp_word);

    if (was_q_set == false) {
        f.down = divisors(maxp_word);
        was_q_set = true;
    }

    while (f.down) { // q
        while (f.up = divisors(free_word)) { // p
            return f;
        }
        f.down = divisors(maxp_word);
    } 

    return default_fraction;
}

int fill_coefficients(char w[]) {
// wypełnia tablicę współczynników

    Polynomial element;

    while (!is_default_polynomial(element = get_next_polynomial(w))) {
        if (element.power > 255)
            printe("power bigger than 255\n");

        if (element.power > max_power)
            max_power = element.power;
        else if (element.power < min_power)
            min_power = element.power;

        coefficients[element.power] += element.coefficient;
    }

    if (min_power > 0) {
        first_product_power = min_power;
        for (int i = 0; i < 256 - min_power; i++)
            coefficients[i] = coefficients[i + min_power];
    }

    return 0;
}

int calculate_scheme_with_x(Fraction x) {
    int result = 0;
    for (int i = 0; i < 256; i++) {
        if (coefficients[i])
            result += (pow(x.up, i) / pow(x.down, i)) * coefficients[i]; 
    }

    return result;
}


int* calculate_scheme(Fraction divisor) {
    int* new_coefficients = calloc(sizeof(int), 256);
    new_coefficients[max_power - 1] = coefficients[max_power]; 


    for (int i = max_power - 2; i >= 0; i--) {
        new_coefficients[i] = 
            (new_coefficients[i + 1] * divisor.up) / divisor.down + coefficients[i + 1];
    } 

    rest = 
        (new_coefficients[0] * divisor.up) / divisor.down + coefficients[0];

    // change old table to new one
    for (int i = 0; i <= max_power; i++)
        coefficients[i] = new_coefficients[i];
    
    max_power--;
    min_power--;
    
    return new_coefficients;
}

int main(void) {
    const size_t max_input_size = 256;
    char inputString[MAX_INPUT];
    char* b = inputString;
    Polynomial p;
    

    int line_size = getline(&b, &max_input_size, stdin);
    inputString[line_size - 1] = '\0';
    
    #ifdef DEBUG_MODE
    // print each word of the given polynomial
    while (!is_default_polynomial(p = get_next_polynomial(inputString))) {
        printf("w %dx^%d\n", p.coefficient, p.power);
    }
    #endif
    
    // parse it to the table
    fill_coefficients(inputString);

    #ifdef DEBUG_MODE
    // debug info: show what habe been just saved
    printf("coefficient table -------\n");
    printf("first x product power = %d\n", first_product_power);
    for (int i = 0; i < 256; i++) {
        if (coefficients[i])
            printf("power %d, coefficient %d\n", i, coefficients[i]);
    }
    printf("maxp = %d, minp = %d\n", max_power, min_power);
    #endif

    // find the divisors and calculate products
    while (max_power > 0) {
        int div, best_div = 0;
        Fraction fdiv, best_fdiv = default_fraction;
        
        // search for fractional divisor
        clear_last_div();
        while (!is_default_fraction(fdiv = get_next_divisor())) {
            
            if (calculate_scheme_with_x(fdiv) == 0) {
                best_fdiv = fdiv;
                break;
            }

            fdiv.up *= -1;
            if (calculate_scheme_with_x(fdiv) == 0) {
                best_fdiv = fdiv;
                break;
            }
        }

        // now search for intigere divisor
        clear_last_div();
        while (div = divisors(coefficients[0])) {
            if (calculate_scheme_with_x(get_fraction(div, 1)) == 0) {
                best_div = div;
                break;
            }

            if (calculate_scheme_with_x(get_fraction(-div, 1)) == 0) {
                best_div = -div;
                break;
            }
        }
        
        if (best_div != 0)
            best_fdiv = get_fraction(best_div, 1);
        
        if (max_power == 1) {
            printf("last zero place = %d/%d\n", best_fdiv.up, best_fdiv.down);
            break;
        } else if (!is_default_fraction(best_fdiv)) {
            // print what has been found - divisor, rest, and product
            calculate_scheme(best_fdiv);
            printf("divisor = (x - %d/%d)\n", best_fdiv.up, best_fdiv.down);
            printf("rest = %f\n", rest);
            for (int i = 0; i < 256; i++) {
                if (coefficients[i])
                    printf("\tpower %d, coefficient %d\n", i, coefficients[i]);
            }
        } else
            break;
    }

    exit(EXIT_SUCCESS);
}