/**
* @file context.cpp
 * @brief Implementation of mathematical expression calculator
 * @author jwarbax
 * @date 5/28/25
 *
 * High-precision mathematical expression calculator supporting 128-bit
 * floating point arithmetic. Handles parsing, validation, tokenization,
 * and calculation of mathematical expressions with proper operator
 * precedence and parentheses handling.
 */

#include "context.h"
#include <chrono>
#include <map>
#include <quadmath.h>
#include <iostream>
#include "examples/example_sdl3_sdlrenderer3/imgui.h"

using namespace std;

//╔══════════════════════════════════════════════════════════════════════════════╗
//║▓▓▓▓▓▒▒▒▒▒░░░░░                    ❖ ◦ ❖ ◦ ❖                   ░░░░░▒▒▒▒▒▓▓▓▓▓║
//║▓▓▓▓▒▒▒▒░░░░                    HELPER FUNCTIONS                  ░░░░▒▒▒▒▓▓▓▓║
//║▓▓▓▓▓▒▒▒▒▒░░░░░                     ❖ ◦ ◦ ❖                    ░░░░░▒▒▒▒▒▓▓▓▓▓║
//╚══════════════════════════════════════════════════════════════════════════════╝

/**
 * @brief Converts 128-bit float to string with specified precision
 * @param value The 128-bit floating point value to convert
 * @param precision Number of decimal places (default: 40)
 * @return String representation of the value
 */
string to_string128(__float128 value, int precision = 40) {
  char buf[1024];
  char fmt[40];
  snprintf(fmt, sizeof(fmt), "%%.%dQg", precision);
  quadmath_snprintf(buf, sizeof(buf), fmt, value);
  return {buf};
}

/**
 * @brief Handles division by zero error reporting
 *
 * Creates error table and outputs division by zero message.
 * Is intended to be used to track error counts for witty responses.
 */
inline void dividedZero()
{
  map<string,int>zero_table;

  zero_table["Error: Division by Zero"]=0;
  zero_table["Error: Division by Zero"]=1;

  cerr<<"Big Error:\n";
}

//╔══════════════════════════════════════════════════════════════════════════════╗
//║▓▓▓▓▓▒▒▒▒▒░░░░░                    ❖ ◦ ❖ ◦ ❖                   ░░░░░▒▒▒▒▒▓▓▓▓▓║
//║▓▓▓▓▒▒▒▒░░░░                    INPUT PROCESSING                  ░░░░▒▒▒▒▓▓▓▓║
//║▓▓▓▓▓▒▒▒▒▒░░░░░                     ❖ ◦ ◦ ❖                    ░░░░░▒▒▒▒▒▓▓▓▓▓║
//╚══════════════════════════════════════════════════════════════════════════════╝

/**
 * @brief Ensures input has surrounding parentheses
 *
 * Adds outer parentheses to input if not already present.
 * This simplifies parsing by guaranteeing parenthetical structure.
 */
void getRawInput()
{
    if (input[0]!='('||input[input.size()-1]!=')')
    {
        input.insert(input.begin(),'(');
        input.push_back(')');
    }
    rawInput=input;
};

/**
 * @brief Removes whitespace from input expression
 *
 * Strips space, horizontal tab, newline, vertical tab, form feed, and carriage return with "isspace"
 * from the raw input string for easier parsing.
 */
void getCleanInput()
{
    for (size_t i=0;i<rawInput.size();i++)
    {
        if (isspace(rawInput[i]))
        {
            rawInput.erase(rawInput.begin()+i);
            i--;
        }
    }
    cleanInput={rawInput};
};

//╔══════════════════════════════════════════════════════════════════════════════╗
//║▓▓▓▓▓▒▒▒▒▒░░░░░                    ❖ ◦ ❖ ◦ ❖                   ░░░░░▒▒▒▒▒▓▓▓▓▓║
//║▓▓▓▓▒▒▒▒░░░░                  VALIDATION FUNCTIONS                ░░░░▒▒▒▒▓▓▓▓║
//║▓▓▓▓▓▒▒▒▒▒░░░░░                     ❖ ◦ ◦ ❖                    ░░░░░▒▒▒▒▒▓▓▓▓▓║
//╚══════════════════════════════════════════════════════════════════════════════╝

/**
 * @brief Validates the first character of cleaned input
 * @return true if first character is valid, false otherwise
 *
 * Checks for invalid starting characters and validates context-specific
 * rules for minus signs, decimals, and parentheses.
 */
    bool isValidFirst()
    {
        char c{cleanInput[0]};
        if(invalidFirst.find(c)!=string::npos)
        {
            cerr<<"Error: String cannot start with: "<<invalidFirst;
            return false;
        }

        if(c=='-'&&!isdigit(cleanInput[1])&&cleanInput[1]!='.'&&cleanInput[1]!='(')
        {
            cerr<<"Error: '-' Must be followed by a digit, decimal, or open parentheses";
            return false;
        }
        if(c=='('&&cleanInput[1]==')')
        {
            cerr<<"Error: Empty Parentheses()";
            return false;
        }
        if(c=='.'&&!isdigit(cleanInput[1]))
        {
            cerr<<"Error: '.' Must be followed by a digit";
            return false;
        }
        return true;
    }

/**
 * @brief Validates parentheses matching and nesting
 * @return true if parentheses are properly matched, false otherwise
 *
 * Ensures all opening parentheses have matching closing parentheses
 * and that nesting is valid throughout the expression.
 */
    bool isValidPar()
    {
        int openCount{0};
        int closeCount{0};

        for (unsigned long long i{0}; i<cleanInput.size(); i++)
        {

            char currentChar=cleanInput[i];

            if (currentChar=='(')
            {
                openCount++;
            }
            else if (currentChar==')')
            {
                closeCount++;
            }
            if (openCount-closeCount < 0||closeCount-openCount > 0)
            {
                cerr<<"Error: Invalid Parenthesis";
                return false;
            }
            if (i==cleanInput.size()-1&&openCount-closeCount==0)
            {
                return true;
            }
        }
        cerr<<"Error: Invalid Parenthesis";
        return false;
    }

/**
 * @brief Validates complete mathematical expression syntax
 * @return true if syntax is valid, false otherwise
 *
 * Uses state machine to validate proper sequence of numbers, operators,
 * variables, decimals, and parentheses according to mathematical grammar.
 */
    bool isValidSyntax()
  {

    auto currentState{state::start};

    for(unsigned long long i{0};i<cleanInput.size();++i)
    {
      char characterIndex{cleanInput[i]};

      state previousState=currentState;

      //counter for currentState
      if(isdigit(characterIndex))
      {
        currentState=state::number;
      }
      else if(isalpha(characterIndex))
      {
        currentState=state::variable;
      }
      else if(characterIndex=='*'||characterIndex=='/'||characterIndex=='+'||characterIndex=='^')
      {
        currentState=state::operator_;
      }
      else if(characterIndex=='-')
      {
        currentState=state::minus;
      }
      else if(characterIndex=='.')
      {
        currentState=state::decimal;
      }
      else if(characterIndex=='(')
      {
        currentState=state::open_par;
      }
      else if(characterIndex==')')
      {
        currentState=state::close_par;
      }
      else
      {
        cerr<<"Error: Undefined valid_syntax";
      }

      switch(currentState)
      {
      case state::start:
        cerr<<"Error: Undefined //syntax_switch_test";
        return false;
      case state::number:
          currentState=state::number;
          break;
      case state::variable:
        if (previousState==state::decimal)
        {
          cerr<<"Error: Decimal cannot come before variable";
          return false;
        }
        currentState=state::variable;
        break;
      case state::operator_:
        if (previousState==state::decimal)
        {
          cerr<<"Error: Decimal cannot come before operator";
          return false;
        }
        if (previousState==state::operator_)
        {
          cerr<<"Error: Operator cannot come before operator";
          return false;
        }
        if (previousState==state::start)
        {
          cerr<<"Error: Cannot start string with an operator";
          return false;
        }
        currentState=state::operator_;
        break;
      case state::minus:
      case state::unary_minus:
      case state::binary_minus:
          if (previousState==state::operator_||previousState==state::open_par
            ||previousState==state::start||previousState==state::binary_minus)
          {
            currentState=state::unary_minus;
          }
          if (previousState==state::number||previousState==state::close_par
            ||previousState==state::variable)
          {
            currentState=state::binary_minus;
          }
          if (currentState==state::unary_minus)
          {
            if (previousState==state::unary_minus||previousState==state::decimal
              ||previousState==state::close_par||previousState==state::number
              ||previousState==state::variable)
            {
              cerr<<"Error: Double Operator";
              return false;
            }
            break;
          }
          if (currentState==state::binary_minus)
          {
            if (previousState==state::decimal||previousState==state::unary_minus
           ||previousState==state::binary_minus||previousState==state::operator_)
            {
              cerr<<"Error: Double Operator";
              return false;
            }
            break;
          }
        break;
      case state::decimal:
      case state::fractional:
        if (previousState==state::decimal||previousState==state::fractional)
        {
          cerr<<"Error: Double Decimal";
          return false;
        }
        if (previousState==state::start||previousState==state::close_par
          ||previousState==state::variable||previousState==state::operator_
          ||previousState==state::open_par||previousState==state::unary_minus
          ||previousState==state::binary_minus)
        {
          currentState=state::fractional;
        }
        if (previousState==state::number)
        {
          currentState=state::decimal;
        }
        break;
      case state::open_par:
        if (previousState==state::decimal||previousState==state::fractional)
        {
          cerr<<"Error: Invalid Open Parenthesis/Unary Minus must only precede a digit or decimal";
          return false;
        }
        currentState=state::open_par;
        break;
      case state::close_par:
        if (previousState==state::decimal||previousState==state::fractional
          ||previousState==state::unary_minus||previousState==state::binary_minus
          ||previousState==state::operator_||previousState==state::open_par)
        {
          cerr<<"Error: Invalid Close Parenthesis";
        }
        currentState=state::close_par;
        break;
      }
    }
    return true;
  }

/**
 * @brief Master validation function for input expressions
 * @return true if input passes all validation tests, false otherwise
 *
 * Combines character validation, first character check, parentheses
 * validation, and complete syntax validation into single test.
 */
    bool isValidInput()
    {
      for(char currentCharacter:cleanInput)
      {
        if(validList.find(currentCharacter)==string::npos)
        {
          cerr<<"Error: Invalid Input\n"<<"valid_list = "<<validList;
          return false;
        }
      }
        if(!isValidFirst())
      {
          return false;
      }
        if(!isValidPar())
      {
          return false;
      }
        if (!isValidSyntax())
      {
          return false;
      }
          return true;
    }

//╔══════════════════════════════════════════════════════════════════════════════╗
//║▓▓▓▓▓▒▒▒▒▒░░░░░                    ❖ ◦ ❖ ◦ ❖                   ░░░░░▒▒▒▒▒▓▓▓▓▓║
//║▓▓▓▓▒▒▒▒░░░░                     TOKEN PARSING                    ░░░░▒▒▒▒▓▓▓▓║
//║▓▓▓▓▓▒▒▒▒▒░░░░░                     ❖ ◦ ◦ ❖                    ░░░░░▒▒▒▒▒▓▓▓▓▓║
//╚══════════════════════════════════════════════════════════════════════════════╝

/**
 * @brief Converts validated input into mathematical tokens
 *
 * Parses mathematical expression into array of tokens (numbers, operators,
 * parentheses). Handles implicit multiplication, unary minus conversion,
 * and decimal number recognition. Does not handle factorial calculation or variables.
 */
void tokenize()
{
  int decimalCount{0};
  string substring;

  for (unsigned long long index{0};index<=cleanInput.length();index++)
  {
    char currentCharacter{cleanInput[index]};
    if (isdigit(currentCharacter))
    {
      if (decimalCount>1)
      {
        cerr<< "Error: Too Many Decimals";
      }
      if (cleanInput[index-1]==')')
      {
        rawTokens.emplace_back("*");
      }
      substring.push_back(currentCharacter);
      continue;
    }
    if (currentCharacter=='.')
    {
      if (!isdigit(cleanInput[index+1]))
      {
        cerr<< "Error: Invalid Decimal";
      }
      if (cleanInput[index-1]==')')
      {
        rawTokens.emplace_back("*");
      }
      decimalCount++;
      substring.push_back(currentCharacter);
      continue;
    }
    if (operatorList.find(currentCharacter)!=string::npos)
    {
      if (!empty(substring))
      {
        rawTokens.emplace_back(substring);
        substring.clear();
      }
      decimalCount=0;
      if (currentCharacter=='(')
      {
        if (index==0)
        {
          rawTokens.emplace_back("(");
          continue;
        }
        if (index>=1&&cleanInput[index-1]=='-')
        {
          if (index>=2&&cleanInput[index-2]!=')'&&!isdigit(cleanInput[index-2]))
          {
            rawTokens.emplace_back("-1");
            rawTokens.emplace_back("*");
            rawTokens.emplace_back("(");
            continue;
          }
          if (index>=2&&cleanInput[index-2]==')')
          {
            rawTokens.emplace_back("(");
            continue;
          }
        }
        if (cleanInput[index-1]==')')
        {
          rawTokens.emplace_back("*");
          rawTokens.emplace_back("(");
          continue;
        }
        if (isdigit(cleanInput[index-1]))
        {
          rawTokens.emplace_back("*");
          rawTokens.emplace_back("(");
          continue;
        }
        if (isalpha(cleanInput[index-1]))
        {
          rawTokens.emplace_back("*");
          rawTokens.emplace_back("(");
          continue;
        }
        if (index!=0)
        {
          rawTokens.emplace_back("(");
          continue;
        }
      }
      else if (currentCharacter==')')
      {
        rawTokens.emplace_back(")");
        continue;
      }
      else if (currentCharacter=='^')
      {
        rawTokens.emplace_back("^");
        continue;
      }
      else if (currentCharacter=='*')
      {
        rawTokens.emplace_back("*");
        continue;
      }
      else if (currentCharacter=='/')
      {
        rawTokens.emplace_back("/");
        continue;
      }
      else if (currentCharacter=='+')
      {
        rawTokens.emplace_back("+");
        continue;
      }
      else if (currentCharacter=='-')
      {
        if (index==0)
        {
          if (cleanInput[index+1]=='(')
          {
            continue;
          }
          substring.push_back('-');
          continue;
        }
        if (isblank(cleanInput[index+1])||cleanInput[index+1]==')')
        {
          cerr<<"Error: Invalid End\n";
          continue;
        }
        if ((isdigit(cleanInput[index-1]) || isalpha(cleanInput[index-1])||cleanInput[index-1]==')')
          &&!isdigit(cleanInput[index+1])&&!isalpha(cleanInput[index+1])&&cleanInput[index+1]!='.')
        {
          rawTokens.emplace_back("-");
          continue;
        }
        if (isdigit(cleanInput[index+1]) || isalpha(cleanInput[index+1])
          ||cleanInput[index+1]=='.'||cleanInput[index-1]=='(')
        {
          if (cleanInput[index-1]==')')
          {
            rawTokens.emplace_back("+");
          }
          else if (isalpha(cleanInput[index-1])||isdigit(cleanInput[index-1]))
          {
            rawTokens.emplace_back("+");
          }
          substring.push_back('-');
          continue;
        }
      }
    }
    if (!empty(substring))
    {
      rawTokens.emplace_back(substring);
    }
  }
}

//╔══════════════════════════════════════════════════════════════════════════════╗
//║▓▓▓▓▓▒▒▒▒▒░░░░░                    ❖ ◦ ❖ ◦ ❖                   ░░░░░▒▒▒▒▒▓▓▓▓▓║
//║▓▓▓▓▒▒▒▒░░░░                   CALCULATION ENGINE                 ░░░░▒▒▒▒▓▓▓▓║
//║▓▓▓▓▓▒▒▒▒▒░░░░░                     ❖ ◦ ◦ ❖                    ░░░░░▒▒▒▒▒▓▓▓▓▓║
//╚══════════════════════════════════════════════════════════════════════════════╝

/**
 * @brief Evaluates tokenized mathematical expression
 *
 * Processes tokens using operator precedence rules:
 * 1. Parentheses (innermost first)
 * 2. Exponentiation (^)
 * 3. Multiplication and Division (*, /)
 * 4. Addition and Subtraction (+, -)
 *
 * Uses 128-bit floating point for high precision calculations.
 */
  void calculate()
  {
    const string power{"^"};
    const string multiply{"*"};
    const string divide{"/"};
    const string add{"+"};
    const string subtract{"-"};

    rawTokens.emplace_back(" ");

    BEGIN:

    for (size_t index{0};index<rawTokens.size();++index)
    {
      if (rawTokens[index]==")")
      {

        for (size_t firstClosedPar{0};firstClosedPar<index;++firstClosedPar) //double check it's the first closed par
        {
          if (rawTokens[firstClosedPar]==")"&&index>firstClosedPar)
          {
            index=firstClosedPar;
          }
        }
        for (size_t firstOpenPar{index};firstOpenPar>=0;--firstOpenPar)
        {
          if (rawTokens[firstOpenPar]=="(")
          {

            for (size_t subIndex=index;subIndex>firstOpenPar;--subIndex)
            {
              if (rawTokens[subIndex]=="^")
              {
                __float128 base=strtoflt128(rawTokens[subIndex-1].c_str(),nullptr);
                __float128 exponent=strtoflt128(rawTokens[subIndex+1].c_str(),nullptr);
                __float128 result=powq(base,exponent);

                rawTokens[subIndex-1]=to_string128(result);

                rawTokens.erase(rawTokens.begin()+subIndex,rawTokens.begin()+subIndex+2);

                goto BEGIN;
              }
            }
            for (size_t subIndex=firstOpenPar;subIndex<index;++subIndex)
            {
              if (rawTokens[subIndex]=="*"||rawTokens[subIndex]=="/")
              {
                __float128 leftOperand=strtoflt128(rawTokens[subIndex-1].c_str(),nullptr);
                __float128 rightOperand=strtoflt128(rawTokens[subIndex+1].c_str(),nullptr);
                __float128 result;
                if (rawTokens[subIndex]=="*")
                {
                  result=leftOperand*rightOperand;
                  rawTokens[subIndex-1]=to_string128(result);
                }
                else
                {
                  if (rightOperand==0)
                  {
                    dividedZero();
                  }
                  {
                    result=leftOperand/rightOperand;
                    rawTokens[subIndex-1]=to_string128(result);
                  }
                }

                rawTokens.erase(rawTokens.begin()+subIndex,rawTokens.begin()+subIndex+2);

                goto BEGIN;
              }

            }
            for (size_t subIndex=firstOpenPar;subIndex<index;++subIndex)
            {
              if (rawTokens[subIndex]=="+"||rawTokens[subIndex]=="-")
              {
                __float128 leftOperand=strtoflt128(rawTokens[subIndex-1].c_str(),nullptr);
                __float128 rightOperand=strtoflt128(rawTokens[subIndex+1].c_str(),nullptr);
                __float128 localResult;
                if (rawTokens[subIndex]=="+")
                {
                  localResult=leftOperand+rightOperand;
                  rawTokens[subIndex-1]=to_string128(localResult);
                }
                else
                {
                  localResult=leftOperand-rightOperand;
                  rawTokens[subIndex-1]=to_string128(localResult);
                }

                rawTokens.erase(rawTokens.begin()+subIndex,rawTokens.begin()+subIndex+2);

                goto BEGIN;
              }
            }
            if (rawTokens[firstOpenPar]=="("&&rawTokens[firstOpenPar+2]==")")
            {

              rawTokens.erase(rawTokens.begin()+firstOpenPar);
              rawTokens.erase(rawTokens.begin()+firstOpenPar+1);

              goto BEGIN;
            }
          }
        }
      }
    }
  float128_Result=strtoflt128(rawTokens[0].c_str(),nullptr);
  }

//╔══════════════════════════════════════════════════════════════════════════════╗
//║▓▓▓▓▓▒▒▒▒▒░░░░░                    ❖ ◦ ❖ ◦ ❖                   ░░░░░▒▒▒▒▒▓▓▓▓▓║
//║▓▓▓▓▒▒▒▒░░░░                   OUTPUT AND DISPLAY                 ░░░░▒▒▒▒▓▓▓▓║
//║▓▓▓▓▓▒▒▒▒▒░░░░░                     ❖ ◦ ◦ ❖                    ░░░░░▒▒▒▒▒▓▓▓▓▓║
//╚══════════════════════════════════════════════════════════════════════════════╝

/**
 * @brief Renders calculation progress in ImGui interface
 *
 * Displays appropriate progress information based on current calculation
 * phase: verifying, validating, calculating, complete, or error state.
 */
void renderCalculationProgress()
{
    switch (currentPhase)
    {
       case phase::verifying:
            ImGui::Text("Verifying input...");
           break;
        case phase::validating:
            ImGui::Text("Parsing tokens...");
           break;
       case phase::calculating:
       {
           float progress={1.0f - currentTokens / totalTokens};
           ImGui::Text("Calculating...");
           ImGui::ProgressBar(progress);
           ImGui::Text("Operations remaining: %zu", currentTokens);
           break;
       }
       case phase::complete:
       {
           ImGui::Text("Completed in: ", timeTaken);
           break;
       }
       case phase::error:
            ImGui::Text("UNDEFINED ERROR (renderCalculationProgress)");
           break;
    }
}

/**
 * @brief Formats calculation duration for display
 * @param duration Microseconds elapsed during calculation
 * @return Formatted time string with appropriate units (µs, ms, s)
 *
 * Automatically selects appropriate time units based on duration length.
 */
string calculationTimeFormat(chrono::microseconds& duration)
{
    using namespace std::chrono;
    ostringstream durationStream;
    if (duration.count()<1000)
    {
        durationStream << "Elapsed Time: " << duration.count() << " µs";
    }
    else if (duration.count()<1000000)
    {
        auto ms = duration_cast<milliseconds>(duration);
        durationStream << "Elapsed Time: " << ms.count() << " ms";
    }
    else
    {
        double seconds = duration.count()/1000000.0;
        durationStream << fixed << setprecision(2);
        durationStream << "Elapsed Time: " << seconds << " s";
    }
    return durationStream.str();
}

/**
 * @brief Main calculation orchestrator function
 *
 * Coordinates the complete calculation workflow:
 * 1. Input processing and cleaning
 * 2. Validation of mathematical expression
 * 3. Tokenization and parsing
 * 4. High-precision calculation
 * 5. Result formatting and timing
 *
 * Cleans up context after processing.
 */
void calculationResult()
{
    getRawInput();
    getCleanInput();

    if (isValidInput())
    {
        auto start{chrono::high_resolution_clock::now()};

        tokenize();
        calculate();

        auto end{chrono::high_resolution_clock::now()};
        auto duration{std::chrono::duration_cast<std::chrono::microseconds>(end - start)};

        ostringstream roundedResult;

        long double doubleResult{static_cast<double>(float128_Result)};

        roundedResult << fixed << setprecision(userPrecision);
        roundedResult << doubleResult << endl;

        stringResult = roundedResult.str();
        globalResult=stringResult;
        calculationTime=calculationTimeFormat(duration);
    }
    else
    {
        stringResult = "UNDEFINED ERROR(calculationResult)";
        globalResult=stringResult;
        calculationTime.clear();
    }
    rawInput.clear();
    cleanInput.clear();
    rawTokens.clear();
    float128_Result={0};
    stringResult.clear();
    calculationTime.clear();
}