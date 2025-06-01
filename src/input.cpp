//
// Created by war on 5/28/25.
//
#include <string>
#include <iostream>
#include <cctype>
#include <imgui.h>

#include "context.h"

using namespace std;

void getRawInput(Context& reference)
{
  if (reference.input[0]!='('||reference.input[reference.input.size()-1]!=')')
  {
    reference.input.insert(reference.input.begin(),'(');
    reference.input.push_back(')');
  }
  reference.rawInput=reference.input;
};

void getCleanInput(Context& reference)
{
 for (size_t i=0;i<reference.rawInput.size();i++)
 {
   if (isspace(reference.rawInput[i]))
   {
     reference.rawInput.erase(reference.rawInput.begin()+i);
     i--;
   }
 }
  reference.cleanInput={reference.rawInput};
};