/**
* @file context.h
 * @brief Mathematical expression parser and calculator context definitions
 * @author jwarbax
 * @date 5/28/25
 *
 * This header defines the Context structure and supporting enums/constants
 * for a high-precision mathematical expression calculator using 128-bit
 * floating point arithmetic.
 */
#pragma once
#include <cstdint>
#include <vector>
#include <string>

using namespace std;

//╔══════════════════════════════════════════════════════════════════════════════╗
//║▓▓▓▓▓▒▒▒▒▒░░░░░                    ❖ ◦ ❖ ◦ ❖                   ░░░░░▒▒▒▒▒▓▓▓▓▓║
//║▓▓▓▓▒▒▒▒░░░░                      ENUMERATIONS                    ░░░░▒▒▒▒▓▓▓▓║
//║▓▓▓▓▓▒▒▒▒▒░░░░░                     ❖ ◦ ◦ ❖                    ░░░░░▒▒▒▒▒▓▓▓▓▓║
//╚══════════════════════════════════════════════════════════════════════════════╝
/**
 * @brief States for syntax validation and phases for progress reporting
 *
 * "state" Defines all possible states during mathematical expression parsing
 * and validation. Used by the syntax checker to ensure proper grammar.
 * "phase" Defines what the calculation function is doing so that the user
 * can have a progress report displayed to them
 */

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

//╔══════════════════════════════════════════════════════════════════════════════╗
//║▓▓▓▓▓▒▒▒▒▒░░░░░                    ❖ ◦ ❖ ◦ ❖                   ░░░░░▒▒▒▒▒▓▓▓▓▓║
//║▓▓▓▓▒▒▒▒░░░░                       CONSTANTS                      ░░░░▒▒▒▒▓▓▓▓║
//║▓▓▓▓▓▒▒▒▒▒░░░░░                     ❖ ◦ ◦ ❖                    ░░░░░▒▒▒▒▒▓▓▓▓▓║
//╚══════════════════════════════════════════════════════════════════════════════╝
/** @brief Allowed characters, and data needed for parsing, validating, and calculating
 *
 * Tracks the complete workflow from raw
 * input through tokenization to final high-precision results.
 */
const string validList{" ()^*/+-0123456789."};
const string invalidFirst{"*/+=)^"};
const string operatorList{"^*/+-()"};

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
  string globalResult=" ";

  //time taken to complete calculation
  string calculationTime;

  //stores result so that GUI can continue displaying after calculationTime is cleared
  //MUST be initialized non-empty! (I think)
  string timeTaken=" ";

  //the user can disable the elapsed time being displayed if they wish
  bool displayElapsedTime;

  //user can set desired precision,
  //the number represents how many digits are shown after the decimal
  int userPrecision{40};

  //records how many times the user attempts to divide by zero
  //so that we can display increasingly witty failure responses, of course
  int zeroCount;

  //for rendering the progress bar for longer calculations
  float totalTokens{0.0f};
  float currentTokens{static_cast<float>(rawTokens.size())};

  auto currentPhase{phase::idle};


//╔══════════════════════════════════════════════════════════════════════════════╗
//║▓▓▓▓▓▒▒▒▒▒░░░░░                    ❖ ◦ ❖ ◦ ❖                   ░░░░░▒▒▒▒▒▓▓▓▓▓║
//║▓▓▓▓▒▒▒▒░░░░                  FUNCTION DECLARATIONS               ░░░░▒▒▒▒▓▓▓▓║
//║▓▓▓▓▓▒▒▒▒▒░░░░░                     ❖ ◦ ◦ ❖                    ░░░░░▒▒▒▒▒▓▓▓▓▓║
//╚══════════════════════════════════════════════════════════════════════════════╝

/**
 * @brief Validates the first character of input expression
 * @return true if first character is valid, false otherwise
 */
bool isValidFirst();

/**
 * @brief Validates parentheses matching in expression
 * @return true if all parentheses are properly matched, false otherwise
 */
bool isValidPar();

/**
 * @brief Validates complete syntax of mathematical expression
 * @return true if syntax is valid, false otherwise
 */
bool isValidSyntax();

/**
 * @brief Master validation function combining all input checks
 *
 * This function first checks entire string against validList to ensure it includes no illegal characters
 * @return true if input passes all validation tests, false otherwise
 */
bool isValidInput();

/**
 * @brief Processes mathematical expression and generates result
 */
void calculationResult();

/**
 * @brief Renders calculation progress in ImGui interface
 */
void renderCalculationProgress();

