//
// Created by war on 5/28/25.
//
#include <string>
#include <iostream>
#include <cctype>
#include <imgui.h>

#include "context.h"

using namespace std;

static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
  if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
  {
    // Resize string and update buffer pointer
    auto* str = static_cast<std::string*>(data->UserData);
    str->resize(data->BufTextLen);
    data->Buf = str->data();
  }
  return 0;
}

void getRawInput(Context& reference)
{
  string input;
  input.reserve(1024);
  ImGui::InputText("Problem", input.data(), input.capacity()+1,
  ImGuiInputTextFlags_CallbackResize, InputTextCallback, &input);

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