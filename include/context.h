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
  __float128 float128_Result;
  string stringResult;
  string calculationTime;
  string inputBuffer;
  int userPrecision;
};

struct UserDefined
{
  bool displayElapsedTime;
};