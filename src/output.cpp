//
// Created by war on 5/28/25.
//

#include <iostream>
#include <iomanip>
#include <string>

#include <input.h>
#include <validate.h>
#include <tokenize.h>
#include <calculate.h>
#include <output.h>
#include <quadmath.h>

int userSetPrecision=40;

inline string to_string128(__float128 value, int precision = userSetPrecision) {
    char buf[256];
    char fmt[32];
    snprintf(fmt, sizeof(fmt), "%%.%dQg", precision);
    quadmath_snprintf(buf, sizeof(buf), fmt, value);
    return {buf};
}

void calculationResult(Context& reference)
{
    getRawInput(reference);
    getCleanInput(reference);
    if (isValidInput(reference))
    {
        tokenize(reference);
        calculate(reference);
        __float128 floatResult={reference.roundedResult};
        string stringResult=to_string128(floatResult,userSetPrecision);
        std::cout << stringResult << std::endl;
    }
}
