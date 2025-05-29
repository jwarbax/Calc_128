//
// Created by war on 5/28/25.
//
#include <tokenize.h>
#include <iostream>
#include <string>
#include <cctype>
#include <vector>

void tokenize(Context& reference)
{
  int decimal_count{0};
  string substring;

  for (unsigned long long index{0};index<=reference.cleanInput.length();index++)
  {
    //input string index iterator
    char currentCharacter{reference.cleanInput[index]};
    if (isdigit(currentCharacter))
    {
      if (decimal_count>1)
      {
        cerr<< "Error: Too Many Decimals";
      }
      if (reference.cleanInput[index-1]==')')
      {
        reference.rawTokens.emplace_back("*");
      }
      substring.push_back(currentCharacter);
      continue;
    }
    if (currentCharacter=='.')
    {
      if (!isdigit(reference.cleanInput[index+1]))
      {
        cerr<< "Error: Invalid Decimal";
      }
      if (reference.cleanInput[index-1]==')')
      {
        reference.rawTokens.emplace_back("*");
      }
      decimal_count++;
      substring.push_back(currentCharacter);
      continue;
    }
    if (operatorList.find(currentCharacter)!=string::npos)
    {
      if (!empty(substring))
      {
        reference.rawTokens.emplace_back(substring);
        substring.clear();
      }
      decimal_count=0;
      if (currentCharacter=='(')
      {
        if (index==0)
        {
          reference.rawTokens.emplace_back("(");
          continue;
        }
        if (index>=1&&reference.cleanInput[index-1]=='-')
        {
          if (index>=2&&reference.cleanInput[index-2]!=')'&&!isdigit(reference.cleanInput[index-2]))
          {
            reference.rawTokens.emplace_back("-1");
            reference.rawTokens.emplace_back("*");
            reference.rawTokens.emplace_back("(");
            continue;
          }
          if (index>=2&&reference.cleanInput[index-2]==')')
          {
            reference.rawTokens.emplace_back("(");
            continue;
          }
        }
        if (reference.cleanInput[index-1]==')')
        {
          reference.rawTokens.emplace_back("*");
          reference.rawTokens.emplace_back("(");
          continue;
        }
        if (isdigit(reference.cleanInput[index-1]))
        {
          reference.rawTokens.emplace_back("*");
          reference.rawTokens.emplace_back("(");
          continue;
        }
        if (isalpha(reference.cleanInput[index-1]))
        {
          reference.rawTokens.emplace_back("*");
          reference.rawTokens.emplace_back("(");
          continue;
        }
        if (index!=0)
        {
          reference.rawTokens.emplace_back("(");
          continue;
        }
      }
      else if (currentCharacter==')')
      {
        reference.rawTokens.emplace_back(")");
        continue;
      }
      else if (currentCharacter=='^')
      {
        reference.rawTokens.emplace_back("^");
        continue;
      }
      else if (currentCharacter=='*')
      {
        reference.rawTokens.emplace_back("*");
        continue;
      }
      else if (currentCharacter=='/')
      {
        reference.rawTokens.emplace_back("/");
        continue;
      }
      else if (currentCharacter=='+')
      {
        reference.rawTokens.emplace_back("+");
        continue;
      }
      else if (currentCharacter=='-')
      {
        if (index==0)
        {
          if (reference.cleanInput[index+1]=='(')
          {
            continue;
          }
          substring.push_back('-');
          continue;
        }
        if (isblank(reference.cleanInput[index+1])||reference.cleanInput[index+1]==')')
        {
          cerr<<"Error: Invalid End\n";
          continue;
        }
        if ((isdigit(reference.cleanInput[index-1]) || isalpha(reference.cleanInput[index-1])||reference.cleanInput[index-1]==')')
          &&!isdigit(reference.cleanInput[index+1])&&!isalpha(reference.cleanInput[index+1])&&reference.cleanInput[index+1]!='.')
        {
          reference.rawTokens.emplace_back("-");
          continue;
        }
        if (isdigit(reference.cleanInput[index+1]) || isalpha(reference.cleanInput[index+1])
          ||reference.cleanInput[index+1]=='.'||reference.cleanInput[index-1]=='(')
        {
          if (reference.cleanInput[index-1]==')')
          {
            reference.rawTokens.emplace_back("+");
          }
          else if (isalpha(reference.cleanInput[index-1])||isdigit(reference.cleanInput[index-1]))
          {
            reference.rawTokens.emplace_back("+");
          }
          substring.push_back('-');
          continue;
        }
      }
    }
    //place-holder for future variables

    // if (isalpha(currentCharacter))
    // {
    //   substring.push_back(currentCharacter);
    //   rawTokens.emplace_back(substring);
    //   substring.clear();
    // }
    if (!empty(substring))
    {
      reference.rawTokens.emplace_back(substring);
    }
  }
}