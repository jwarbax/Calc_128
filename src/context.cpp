//
// Created by war on 5/28/25.
//
#include "context.h"

#include "examples/example_sdl3_sdlrenderer3/imgui.h"

using namespace std;

void renderCalculationProgress(Context& context)
{
    switch (context.currentPhase)
    {
      // case Context::phase::idle:
      //     break;
      // case Context::phase::verifying:
      //     break;
      // case Context::phase::validating:
      //     break;
      // case Context::phase::calculating:
      //     double progress{1.0 - context.currentTokens / context.totalTokens};
      //     ImGui::Text("Calculating...");
      //     ImGui::ProgressBar(static_cast<float>(progress));
      //     ImGui::Text("Operations remaining: %zu", context.currentTokens);
      //     break;
      // case Context::phase::complete:
      //     ImGui::Text("Completed in: %s", 0);
      //     break;
      // case Context::phase::error:
      //     break;
      // default: ;
    }
}