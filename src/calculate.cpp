//
// Created by war on 5/28/25.
//
#include <calculate.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <quadmath.h>
#include <cmath>
#include <cstdlib>
#include <cstring>

inline void dividedZero()
{
  map<string,int>zero_table;

  zero_table["Error: Division by Zero"]=0;
  zero_table["Error: Division by Zero"]=1;

  cerr<<"Big Error:\n";
}

inline string to_string128(__float128 value, int precision = 40) {
  char buf[256];
  char fmt[32];
  snprintf(fmt, sizeof(fmt), "%%.%dQg", precision);
  quadmath_snprintf(buf, sizeof(buf), fmt, value);
  return {buf};
}

  void calculate(Context& reference)
  {
    for (size_t i=0;i<reference.rawTokens.size();)
    {
      if (reference.rawTokens[i].empty())
      {
        reference.rawTokens.erase(reference.rawTokens.begin()+i);
      }
      else
      {
        ++i;
      }
    }
    const string power{"^"};
    const string mult{"*"};
    const string div{"/"};
    const string add{"+"};
    const string sub{"-"};

    reference.rawTokens.emplace_back(" ");

    BEGIN:

    for (size_t i{0};i<reference.rawTokens.size();++i)
    {
      if (reference.rawTokens[i]==")")
      {

        for (size_t t{0};t<i;++t) //double check it's the first closed par
        {
          if (reference.rawTokens[t]==")"&&i>t)
          {
            i=t;
          }
        }
        for (size_t j{i};j>=0;--j)
        {
          if (reference.rawTokens[j]=="(")
          {

            for (size_t pwr=i;pwr>j;--pwr)
            {
              if (reference.rawTokens[pwr]=="^")
              {
                __float128 base=strtoflt128(reference.rawTokens[pwr-1].c_str(),nullptr);
                __float128 exponent=strtoflt128(reference.rawTokens[pwr+1].c_str(),nullptr);
                __float128 result=powq(base,exponent);

                reference.rawTokens[pwr-1]=to_string128(result);

                reference.rawTokens.erase(reference.rawTokens.begin()+pwr,reference.rawTokens.begin()+pwr+2);

                goto BEGIN;
              }
            }
            for (size_t o=j;o<i;++o)
            {
              if (reference.rawTokens[o]=="*"||reference.rawTokens[o]=="/")
              {
                __float128 left=strtoflt128(reference.rawTokens[o-1].c_str(),nullptr);
                __float128 right=strtoflt128(reference.rawTokens[o+1].c_str(),nullptr);
                __float128 result;
                if (reference.rawTokens[o]=="*")
                {
                  result=left*right;
                  reference.rawTokens[o-1]=to_string128(result);
                }
                else
                {
                  if (right==0)
                  {
                    dividedZero();
                  }
                  {
                    result=left/right;
                    reference.rawTokens[o-1]=to_string128(result);
                  }
                }

                reference.rawTokens.erase(reference.rawTokens.begin()+o,reference.rawTokens.begin()+o+2);

                goto BEGIN;
              }

            }
            for (size_t o=j;o<i;++o)
            {
              if (reference.rawTokens[o]=="+"||reference.rawTokens[o]=="-")
              {
                __float128 left=strtoflt128(reference.rawTokens[o-1].c_str(),nullptr);
                __float128 right=strtoflt128(reference.rawTokens[o+1].c_str(),nullptr);
                __float128 result;
                if (reference.rawTokens[o]=="+")
                {
                  result=left+right;
                  reference.rawTokens[o-1]=to_string128(result);
                }
                else
                {
                  result=left-right;
                  reference.rawTokens[o-1]=to_string128(result);
                }

                reference.rawTokens.erase(reference.rawTokens.begin()+o,reference.rawTokens.begin()+o+2);

                goto BEGIN;
              }
            }
            if (reference.rawTokens[j]=="("&&reference.rawTokens[j+2]==")")
            {

              reference.rawTokens.erase(reference.rawTokens.begin()+j);
              reference.rawTokens.erase(reference.rawTokens.begin()+j+1);

              goto BEGIN;
            }
          }
        }
      }
    }
  reference.float128_Result=strtoflt128(reference.rawTokens[0].c_str(),nullptr);
  }