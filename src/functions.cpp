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

#include "functions.h"
#include <chrono>
#include <map>
#include <quadmath.h>
#include <iostream>
#include "examples/example_sdl3_sdlrenderer3/imgui.h"
#include "misc/cpp/imgui_stdlib.h"

using namespace std;

globals global;

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
    if (global.globalInput[0]!='('||global.globalInput[global.globalInput.size()-1]!=')')
    {
        global.globalInput.insert(global.globalInput.begin(),'(');
        global.globalInput.push_back(')');
    }
    global.rawInput=global.globalInput;
};

/**
 * @brief Removes whitespace from input expression
 *
 * Strips space, horizontal tab, newline, vertical tab, form feed, and carriage return with "isspace"
 * from the raw input string for easier parsing.
 */
void getCleanInput()
{
    for (size_t i=0;i<global.rawInput.size();i++)
    {
        if (isspace(global.rawInput[i]))
        {
            global.rawInput.erase(global.rawInput.begin()+i);
            i--;
        }
    }
    global.cleanInput={global.rawInput};
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
        char c{global.cleanInput[0]};
        if(invalidFirst.find(c)!=string::npos)
        {
            cerr<<"Error: String cannot start with: "<<invalidFirst;
            return false;
        }

        if(c=='-'&&!isdigit(global.cleanInput[1])&&global.cleanInput[1]!='.'&&global.cleanInput[1]!='(')
        {
            cerr<<"Error: '-' Must be followed by a digit, decimal, or open parentheses";
            return false;
        }
        if(c=='('&&global.cleanInput[1]==')')
        {
            cerr<<"Error: Empty Parentheses()";
            return false;
        }
        if(c=='.'&&!isdigit(global.cleanInput[1]))
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

        for (unsigned long long i{0}; i<global.cleanInput.size(); i++)
        {

            char currentChar=global.cleanInput[i];

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
            if (i==global.cleanInput.size()-1&&openCount-closeCount==0)
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

    for(unsigned long long i{0};i<global.cleanInput.size();++i)
    {
      char characterIndex{global.cleanInput[i]};

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
      for(char currentCharacter:global.cleanInput)
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

  for (unsigned long long index{0};index<=global.cleanInput.length();index++)
  {
    char currentCharacter{global.cleanInput[index]};
    if (isdigit(currentCharacter))
    {
      if (decimalCount>1)
      {
        cerr<< "Error: Too Many Decimals";
      }
      if (global.cleanInput[index-1]==')')
      {
        global.rawTokens.emplace_back("*");
      }
      substring.push_back(currentCharacter);
      continue;
    }
    if (currentCharacter=='.')
    {
      if (!isdigit(global.cleanInput[index+1]))
      {
        cerr<< "Error: Invalid Decimal";
      }
      if (global.cleanInput[index-1]==')')
      {
        global.rawTokens.emplace_back("*");
      }
      decimalCount++;
      substring.push_back(currentCharacter);
      continue;
    }
    if (operatorList.find(currentCharacter)!=string::npos)
    {
      if (!empty(substring))
      {
        global.rawTokens.emplace_back(substring);
        substring.clear();
      }
      decimalCount=0;
      if (currentCharacter=='(')
      {
        if (index==0)
        {
          global.rawTokens.emplace_back("(");
          continue;
        }
        if (index>=1&&global.cleanInput[index-1]=='-')
        {
          if (index>=2&&global.cleanInput[index-2]!=')'&&!isdigit(global.cleanInput[index-2]))
          {
            global.rawTokens.emplace_back("-1");
            global.rawTokens.emplace_back("*");
            global.rawTokens.emplace_back("(");
            continue;
          }
          if (index>=2&&global.cleanInput[index-2]==')')
          {
            global.rawTokens.emplace_back("(");
            continue;
          }
        }
        if (global.cleanInput[index-1]==')')
        {
          global.rawTokens.emplace_back("*");
          global.rawTokens.emplace_back("(");
          continue;
        }
        if (isdigit(global.cleanInput[index-1]))
        {
          global.rawTokens.emplace_back("*");
          global.rawTokens.emplace_back("(");
          continue;
        }
        if (isalpha(global.cleanInput[index-1]))
        {
          global.rawTokens.emplace_back("*");
          global.rawTokens.emplace_back("(");
          continue;
        }
        if (index!=0)
        {
          global.rawTokens.emplace_back("(");
          continue;
        }
      }
      else if (currentCharacter==')')
      {
        global.rawTokens.emplace_back(")");
        continue;
      }
      else if (currentCharacter=='^')
      {
        global.rawTokens.emplace_back("^");
        continue;
      }
      else if (currentCharacter=='*')
      {
        global.rawTokens.emplace_back("*");
        continue;
      }
      else if (currentCharacter=='/')
      {
        global.rawTokens.emplace_back("/");
        continue;
      }
      else if (currentCharacter=='+')
      {
        global.rawTokens.emplace_back("+");
        continue;
      }
      else if (currentCharacter=='-')
      {
        if (index==0)
        {
          if (global.cleanInput[index+1]=='(')
          {
            continue;
          }
          substring.push_back('-');
          continue;
        }
        if (isblank(global.cleanInput[index+1])||global.cleanInput[index+1]==')')
        {
          cerr<<"Error: Invalid End\n";
          continue;
        }
        if ((isdigit(global.cleanInput[index-1]) || isalpha(global.cleanInput[index-1])||global.cleanInput[index-1]==')')
          &&!isdigit(global.cleanInput[index+1])&&!isalpha(global.cleanInput[index+1])&&global.cleanInput[index+1]!='.')
        {
          global.rawTokens.emplace_back("-");
          continue;
        }
        if (isdigit(global.cleanInput[index+1]) || isalpha(global.cleanInput[index+1])
          ||global.cleanInput[index+1]=='.'||global.cleanInput[index-1]=='(')
        {
          if (global.cleanInput[index-1]==')')
          {
            global.rawTokens.emplace_back("+");
          }
          else if (isalpha(global.cleanInput[index-1])||isdigit(global.cleanInput[index-1]))
          {
            global.rawTokens.emplace_back("+");
          }
          substring.push_back('-');
          continue;
        }
      }
    }
    if (!empty(substring))
    {
      global.rawTokens.emplace_back(substring);
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

    global.rawTokens.emplace_back(" ");

    BEGIN:

    for (size_t index{0};index<global.rawTokens.size();++index)
    {
      if (global.rawTokens[index]==")")
      {

        for (size_t firstClosedPar{0};firstClosedPar<index;++firstClosedPar) //double check it's the first closed par
        {
          if (global.rawTokens[firstClosedPar]==")"&&index>firstClosedPar)
          {
            index=firstClosedPar;
          }
        }
        for (size_t firstOpenPar{index};firstOpenPar>=0;--firstOpenPar)
        {
          if (global.rawTokens[firstOpenPar]=="(")
          {

            for (size_t subIndex=index;subIndex>firstOpenPar;--subIndex)
            {
              if (global.rawTokens[subIndex]=="^")
              {
                __float128 base=strtoflt128(global.rawTokens[subIndex-1].c_str(),nullptr);
                __float128 exponent=strtoflt128(global.rawTokens[subIndex+1].c_str(),nullptr);
                __float128 localResult=powq(base,exponent);

                global.rawTokens[subIndex-1]=to_string128(localResult);

                global.rawTokens.erase(global.rawTokens.begin()+subIndex,global.rawTokens.begin()+subIndex+2);

                goto BEGIN;
              }
            }
            for (size_t subIndex=firstOpenPar;subIndex<index;++subIndex)
            {
              if (global.rawTokens[subIndex]=="*"||global.rawTokens[subIndex]=="/")
              {
                __float128 leftOperand=strtoflt128(global.rawTokens[subIndex-1].c_str(),nullptr);
                __float128 rightOperand=strtoflt128(global.rawTokens[subIndex+1].c_str(),nullptr);
                __float128 localResult;
                if (global.rawTokens[subIndex]=="*")
                {
                  localResult=leftOperand*rightOperand;
                  global.rawTokens[subIndex-1]=to_string128(localResult);
                }
                else
                {
                  if (rightOperand==0)
                  {
                    dividedZero();
                  }
                  {
                    localResult=leftOperand/rightOperand;
                    global.rawTokens[subIndex-1]=to_string128(localResult);
                  }
                }

                global.rawTokens.erase(global.rawTokens.begin()+subIndex,global.rawTokens.begin()+subIndex+2);

                goto BEGIN;
              }

            }
            for (size_t subIndex=firstOpenPar;subIndex<index;++subIndex)
            {
              if (global.rawTokens[subIndex]=="+"||global.rawTokens[subIndex]=="-")
              {
                __float128 leftOperand=strtoflt128(global.rawTokens[subIndex-1].c_str(),nullptr);
                __float128 rightOperand=strtoflt128(global.rawTokens[subIndex+1].c_str(),nullptr);
                __float128 localResult;
                if (global.rawTokens[subIndex]=="+")
                {
                  localResult=leftOperand+rightOperand;
                  global.rawTokens[subIndex-1]=to_string128(localResult);
                }
                else
                {
                  localResult=leftOperand-rightOperand;
                  global.rawTokens[subIndex-1]=to_string128(localResult);
                }

                global.rawTokens.erase(global.rawTokens.begin()+subIndex,global.rawTokens.begin()+subIndex+2);

                goto BEGIN;
              }
            }
            if (global.rawTokens[firstOpenPar]=="("&&global.rawTokens[firstOpenPar+2]==")")
            {

              global.rawTokens.erase(global.rawTokens.begin()+firstOpenPar);
              global.rawTokens.erase(global.rawTokens.begin()+firstOpenPar+1);

              goto BEGIN;
            }
          }
        }
      }
    }
  global.float128_Result=strtoflt128(global.rawTokens[0].c_str(),nullptr);
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
    switch (global.currentPhase)
    {
       case phase::verifying:
            ImGui::Text("Verifying input...");
           break;
        case phase::validating:
            ImGui::Text("Parsing tokens...");
           break;
       case phase::calculating:
       {
           float progress={1.0f - global.currentTokens / global.totalTokens};
           ImGui::Text("Calculating...");
           ImGui::ProgressBar(progress);
           ImGui::Text("Operations remaining: %zu", global.currentTokens);
           break;
       }
       case phase::complete:
       {
           ImGui::Text("Completed in: ", global.timeTaken);
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

        long double doubleResult{static_cast<double>(global.float128_Result)};

        roundedResult << fixed << setprecision(global.userPrecision);
        roundedResult << doubleResult << endl;

        global.stringResult = roundedResult.str();
        global.globalResult=global.stringResult;
        global.calculationTime=calculationTimeFormat(duration);
    }
    else
    {
        global.stringResult = "UNDEFINED ERROR(calculationResult)";
        global.globalResult=global.stringResult;
        global.calculationTime.clear();
    }
    global.rawInput.clear();
    global.cleanInput.clear();
    global.rawTokens.clear();
    global.float128_Result={0};
    global.stringResult.clear();
    global.calculationTime.clear();
}

void renderGUI ()
    {
      constexpr ImGuiWindowFlags guiFlags = ImGuiWindowFlags_NoResize |
                                   ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoCollapse |
                                   ImGuiWindowFlags_NoTitleBar |
                                   ImGuiWindowFlags_NoBringToFrontOnFocus;
      ImGui::Begin("Main",nullptr,guiFlags);
      ImGui::InputTextMultiline("boo", &global.globalInput, ImVec2(windowWidth/2, windowHeight/2));
      ImGui::NewLine();
      if (ImGui::SmallButton("click here"))
      {
        calculationResult();
      }
      ImGui::TextWrapped(global.globalResult.c_str());
      ImGui::End();
    }