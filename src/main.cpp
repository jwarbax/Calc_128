#include <main.h>
#include <iostream>

#include "input.h"

int main()
{
  Context reference;
  while (true)
  {
    reference.cleanInput="";
    reference.rawInput="";
    reference.rawTokens.clear();
    reference.roundedResult={0};
    cout<<"\nEnter Problem\n\n";
    calculationResult(reference);
  }
  return 0;
}


// while (true)
// {
//   cout<<"\nEnter Problem\n\n";

//   getline(cin, user_input);

//   if (valid_input(user_input))
//   {
//     auto start{chrono::high_resolution_clock::now()};
//process_input(user_input);
//     auto end{chrono::high_resolution_clock::now()};
//     auto duration{std::chrono::duration_cast<std::chrono::microseconds>(end - start)};
//     cout<<"\nTime elapsed: "<<duration.count()<<" microseconds\n";
//   }
// }