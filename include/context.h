//
// Created by war on 5/28/25.
//
#pragma once

#include <atomic>
#include <vector>
#include <string>

using namespace std;

struct Context
{
  //unaltered string taken directly out of the GUI input field
  string input;

  //string has had parentheses added to the ends (if needed)
  string rawInput;

  //string has had all unnecessary characters removed with 'isspace'
  //space (' '),
  //horizontal tab ('\t'),
  //newline ('\n'),
  //vertical tab ('\v'),
  //form feed ('\f'),
  //and carriage return ('\r')
  string cleanInput;

  //string is parsed into tokens contained in an array
  //the minus sign '-' prefers to create negative numbers and add '+' in front of them
  //multiplication is inferred with )( or any parenthesis next to a number
  //factorial '!' not currently handled
  //variables "x, y, z" not currently handled
  vector<string> rawTokens;

  //numerical result in 128bit floating point
  __float128 float128_Result;

  //numerical result converted to std::string
  string stringResult;

  //stores result so that GUI can continue displaying after stringResult is cleared
  //MUST be initialized non-empty! (I think)
  string result=" ";

  //time taken to complete calculation
  string calculationTime;

  //the user can disable the elapsed time being displayed if they wish
  bool displayElapsedTime;

  //user can set desired precision,
  //the number represents how many digits are shown after the decimal
  int userPrecision{40};

  //records how many times the user attempts to divide by zero
  //so that we can display increasingly witty failure responses, of course
  int zeroCount;

  //for rendering the progress bar for longer calculations
  size_t totalTokens{0};
  size_t currentTokens{rawTokens.size()};

  //state holding for progress system
  enum class phase
  {
    idle,
    verifying,
    validating,
    calculating,
    complete,
    error
  };
atomic<phase> currentPhase{phase::idle};
};

//to display the calculation progress in GUI
void renderCalculationProgress();

