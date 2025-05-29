//
// Created by war on 5/28/25.
//
#include <string>
#include <iostream>
#include <cctype>
#include "context.h"

using namespace std;

void getRawInput(Context& reference)
{
  string input;
  getline(cin,input);
  input.insert(input.begin(),'(');
  input.push_back(')');
  reference.rawInput=input;
};

void getCleanInput(Context& reference)
{
  for (size_t i=0;i<reference.rawInput.size();i++)
  {
    if (isblank(reference.rawInput[i]))
    {
      reference.rawInput.erase(reference.rawInput.begin()+i);
      i--;
    }
  }
  reference.cleanInput={reference.rawInput};
};