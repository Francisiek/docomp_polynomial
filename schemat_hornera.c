#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>

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

typedef struct Fraction {
    int up;
    unsigned int down;
} Fraction;

Fraction default_fraction = {};

bool is_default_polynomial(Polynomial x) {
    return (x.coefficient == default_polynomial.coefficient) &&
        (x.power == default_polynomial.power);
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

// TODO: obecnie funkcja zwraca po kolei po dzielniku JEDNEJ liczby
// dodać możliwość zwracania dzielników dla wielu liczb z zachowaniem
// kolejności ich zwracania -> dodać mapę [liczba, ostatni zwrócony dzielnik]
int divisors(int number) {
// zwraca po jednym następnym naturalnym dzielniku 'number'
    number = abs(number);
    static int last_number = 0;
    static int last_div = 1;
    
    if (number != last_number) {
        last_div = 1;
    }

    last_number = number;

    while (last_div <= number) {
        if (number % (last_div++) == 0) 
            return last_div - 1;
    }

    return 0;
}


int max_power = 0, min_power = INT_MAX;
int coefficients[256];

int coefficients[256];
int first_product_power;
int rest;

Fraction get_next_divisor(void) {
    // zwraca wymierny dzielnik p/q taki, że p dzieli wyraz wolny,
    // a q wyraz największej potęgi

    static int free_word = 0;
    static int maxp_word = 0;

    if (free_word != coefficients[0]) {
        free_word = coefficients[0];
        maxp_word = coefficients[max_power];
    }

    Fraction f = default_fraction;
    while (f.down = divisors(coefficients[maxp_word])) { // q
        while (f.up = divisors(coefficients[free_word])) { // p
            return f;
        }
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

int calculate_scheme_with_x(int x) {
    int result = 0;
    for (int i = 0; i < 256; i++) {
        if (coefficients[i])
            result += pow(x, i) * coefficients[i]; 
    }

    return result;
}


int* calculate_scheme(int divisor) {
    int* new_coefficients = calloc(sizeof(int), 256);
    new_coefficients[max_power - 1] = coefficients[max_power]; 


    for (int i = max_power - 2; i >= 0; i--) {
        new_coefficients[i] = new_coefficients[i + 1] * divisor + coefficients[i + 1];
    } 

    rest = new_coefficients[0] * divisor + coefficients[0];

    for (int i = 0; i <= max_power; i++)
        coefficients[i] = new_coefficients[i];
    
    max_power--;
    min_power--;
    
    return new_coefficients;
}


/*
int horner_scheme(FILE* output, char polynomial[]) {
    fill_coefficients(polynomial);
    
    int div, best_div = INT_MAX;
    while (div = divisors(coefficients[0])) {
         if (calc_coefficients(div) == 0) {
            best_div = div;
            break;
        }
        else if (calc_coefficients(div) < calc_coefficients(best_div))
            best_div = div;
    }

    rest += calc_scheme(best_div);
    coefficient[pindex++] = best_div;
}
*/

int main(int argc, char** argv) {
    const size_t max_input_size = 256;
    char inputString[MAX_INPUT];
    char* b = inputString;
    Polynomial p;

    int line_size = getline(&b, &max_input_size, stdin);
    inputString[line_size - 1] = '\0';
    
    while (!is_default_polynomial(p = get_next_polynomial(inputString))) {
        printf("w %dx^%d\n", p.coefficient, p.power);
    }
    
    fill_coefficients(inputString);

    printf("coefficient table -------\n");
    printf("first x product power = %d\n", first_product_power);
    for (int i = 0; i < 256; i++) {
        if (coefficients[i])
            printf("power %d, coefficient %d\n", i, coefficients[i]);
    }

    printf("maxp = %d, minp = %d\n", max_power, min_power);

    while (max_power > 0) {
        int div, best_div = 0;
        while (div = divisors(coefficients[0])) {
            if (calculate_scheme_with_x(div) == 0) {
                best_div = div;
                break;
            }

            if (calculate_scheme_with_x(-div) == 0) {
                best_div = -div;
                break;
            }
        }

        if (best_div != 0) {
            calculate_scheme(best_div);
            printf("------\n");
            printf("divisor = %d\n", best_div);
            printf("rest = %d\n", rest);
            for (int i = 0; i < 256; i++) {
                if (coefficients[i])
                    printf("power %d, coefficient %d\n", i, coefficients[i]);
            }
        } else
            break;
    }

    exit(EXIT_SUCCESS);
}