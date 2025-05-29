//
// Created by war on 5/28/25.
//
#pragma once

#include <string>
#include <cstdint>
#include <context.h>

using namespace std;

const string validList{" ()^*/+-0123456789."};
const string invalidFirst{"*/+=)^"};

bool isValidFirst(Context&);
bool isValidPar(Context&);
bool isValidSyntax(Context&);
bool isValidInput(Context&);

enum class state : uint8_t
{
    start,

    //0-9
    number,

    //currently not implemented in final calculation
    //tests isalpha
    variable,

    //*/+-^
    operator_,

    //minus sign pre-assessment
    //BASE VALUE for unary and binary minus
    minus,

    //negative number
    unary_minus,

    //subtraction
    binary_minus,

    //only for testing syntax
    decimal,

    //if decimal comes before a number
    fractional,

    open_par,

    close_par,
  };