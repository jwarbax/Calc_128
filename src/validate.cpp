//
// Created by war on 5/28/25.
//
#include <validate.h>
#include <iostream>
#include <string>
#include <cctype>

    bool isValidFirst(Context& reference)
    {
        char c{reference.cleanInput[0]};
        if(invalidFirst.find(c)!=string::npos)
        {
            cerr<<"Error: String cannot start with: "<<invalidFirst;
            return false;
        }

        if(c=='-'&&!isdigit(reference.cleanInput[1])&&reference.cleanInput[1]!='.'&&reference.cleanInput[1]!='(')
        {
            cerr<<"Error: '-' Must be followed by a digit, decimal, or open parentheses";
            return false;
        }
        if(c=='('&&reference.cleanInput[1]==')')
        {
            cerr<<"Error: Empty Parentheses()";
            return false;
        }
        if(c=='.'&&!isdigit(reference.cleanInput[1]))
        {
            cerr<<"Error: '.' Must be followed by a digit";
            return false;
        }
        return true;
    }
    bool isValidPar(Context& reference)
    {
        int open_count{0};
        int close_count{0};

        for (unsigned long long i{0}; i<reference.cleanInput.size(); i++)
        {

            char currentChar=reference.cleanInput[i];

            if (currentChar=='(')
            {
                open_count++;
            }
            else if (currentChar==')')
            {
                close_count++;
            }
            if (open_count-close_count < 0||close_count-open_count > 0)
            {
                cerr<<"Error: Invalid Parenthesis";
                return false;
            }
            if (i==reference.cleanInput.size()-1&&open_count-close_count==0)
            {
                return true;
            }
        }
        cerr<<"Error: Invalid Parenthesis";
        return false;
    }
    bool isValidSyntax(Context& reference)
  {

    auto current_state{state::start};

    for(unsigned long long i{0};i<reference.cleanInput.size();++i)
    {
      //character index from user_input
      char chi{reference.cleanInput[i]};

      state previous_state=current_state;

      //dumb counter for currentState
      if(isdigit(chi))
      {
        current_state=state::number;
      }
      else if(isalpha(chi))
      {
        current_state=state::variable;
      }
      else if(chi=='*'||chi=='/'||chi=='+'||chi=='^')
      {
        current_state=state::operator_;
      }
      else if(chi=='-')
      {
        current_state=state::minus;
      }
      else if(chi=='.')
      {
        current_state=state::decimal;
      }
      else if(chi=='(')
      {
        current_state=state::open_par;
      }
      else if(chi==')')
      {
        current_state=state::close_par;
      }
      else
      {
        cerr<<"Error: Undefined valid_syntax";
      }

      //syntax_switch_test
      switch(current_state)
      {
      case state::start:
        cerr<<"Error: Undefined //syntax_switch_test";
        return false;
      case state::number:
          current_state=state::number;
          break;
      case state::variable:
        if (previous_state==state::decimal)
        {
          cerr<<"Error: Decimal cannot come before variable";
          return false;
        }
        current_state=state::variable;
        break;
      case state::operator_:
        if (previous_state==state::decimal)
        {
          cerr<<"Error: Decimal cannot come before operator";
          return false;
        }
        if (previous_state==state::operator_)
        {
          cerr<<"Error: Operator cannot come before operator";
          return false;
        }
        if (previous_state==state::start)
        {
          cerr<<"Error: Cannot start string with an operator";
          return false;
        }
        current_state=state::operator_;
        break;
      case state::minus:
      case state::unary_minus:
      case state::binary_minus:
          if (previous_state==state::operator_||previous_state==state::open_par
            ||previous_state==state::start||previous_state==state::binary_minus)
          {
            current_state=state::unary_minus;
          }
          if (previous_state==state::number||previous_state==state::close_par
            ||previous_state==state::variable)
          {
            current_state=state::binary_minus;
          }
          if (current_state==state::unary_minus)
          {
            if (previous_state==state::unary_minus||previous_state==state::decimal
              ||previous_state==state::close_par||previous_state==state::number
              ||previous_state==state::variable)
            {
              cerr<<"Error: Double Operator";
              return false;
            }
            break;
          }
          if (current_state==state::binary_minus)
          {
            if (previous_state==state::decimal||previous_state==state::unary_minus
           ||previous_state==state::binary_minus||previous_state==state::operator_)
            {
              cerr<<"Error: Double Operator";
              return false;
            }
            break;
          }
        break;
      case state::decimal:
      case state::fractional:
        if (previous_state==state::decimal||previous_state==state::fractional)
        {
          cerr<<"Error: Double Decimal";
          return false;
        }
        if (previous_state==state::start||previous_state==state::close_par
          ||previous_state==state::variable||previous_state==state::operator_
          ||previous_state==state::open_par||previous_state==state::unary_minus
          ||previous_state==state::binary_minus)
        {
          current_state=state::fractional;
        }
        if (previous_state==state::number)
        {
          current_state=state::decimal;
        }
        break;
      case state::open_par:
        if (previous_state==state::decimal||previous_state==state::fractional)
        {
          cerr<<"Error: Invalid Open Parenthesis/Unary Minus must only precede a digit or decimal";
          return false;
        }
        current_state=state::open_par;
        break;
      case state::close_par:
        if (previous_state==state::decimal||previous_state==state::fractional
          ||previous_state==state::unary_minus||previous_state==state::binary_minus
          ||previous_state==state::operator_||previous_state==state::open_par)
        {
          cerr<<"Error: Invalid Close Parenthesis";
        }
        current_state=state::close_par;
        break;
      }
    }
    return true;
  }
    bool isValidInput(Context& reference)
    {
      if(reference.cleanInput.empty())
      {
        cerr<<"Error: Empty Field";
        return false;
      }
      //tests full string for allowable characters
      for(char v:reference.cleanInput)
      {
        if(validList.find(v)==string::npos)
        {
          cerr<<"Error: Invalid Input\n"<<"valid_list = "<<validList;
          return false;
        }
      }
        if(!isValidFirst(reference))
      {
          return false;
      }
        if(!isValidPar(reference))
      {
          return false;
      }
        if (!isValidSyntax(reference))
      {
          return false;
      }
          return true;
    }