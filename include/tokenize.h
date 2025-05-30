//
// Created by war on 5/28/25.
//
#pragma once
#include <string>
#include <context.h>

using namespace std;

const string operatorList{"^*/+-()"};

void tokenize(Context&);