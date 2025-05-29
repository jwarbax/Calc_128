//
// Created by war on 5/28/25.
//
#pragma once

#include <vector>
#include <string>

using namespace std;

struct Context
{
  vector<string> rawTokens;
  string rawInput;
  string cleanInput;
  int zeroCount;
  long double roundedResult;
};