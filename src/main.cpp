/**
 * @file main.cpp
 * @brief Application entry point and SDL3/ImGui initialization
 * @author jwarbax
 * @date 5/28/25
 *
 * Initializes SDL3 graphics, creates ImGui context, and runs the
 * main application loop for the high-precision calculator interface.
 */

/**
 * @brief Application entry point
 * @return 0 on success, -1 on initialization failure
 *
 * Sets up SDL3 window and renderer, initializes ImGui framework,
 * and runs the main event loop until user closes the application.
 */

#include <iostream>
#include "SDL3/SDL.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include <imgui_impl_sdlrenderer3.h>

#include "functions.h"

int main()
{
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
  {
    printf("Error: SDL_Init(): %s\n", SDL_GetError());
    return -1;
  }

  constexpr SDL_WindowFlags windowFlags = 0;
  SDL_Window* window = SDL_CreateWindow("Calc_128", windowWidth, windowHeight, windowFlags);
  if (window == nullptr)
  {
    printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
    return -1;
  }
  SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
  SDL_SetRenderVSync(renderer, 1);
  if (renderer == nullptr)
  {
    SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
    return -1;
  }
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui::StyleColorsDark();

  ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer3_Init(renderer);

  auto clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool running = true;
    while (running)
    {
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        ImGui_ImplSDL3_ProcessEvent(&event);
        if (event.type == SDL_EVENT_QUIT)
          running = false;
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
          running = false;
      }
      ImGui_ImplSDLRenderer3_NewFrame();
      ImGui_ImplSDL3_NewFrame();
      ImGui::NewFrame();
      ImGui::SetNextWindowPos(ImVec2(0, 0));
      ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
      renderGUI();
      ImGui::Render();
      SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
      SDL_SetRenderDrawColorFloat(renderer, clearColor.x, clearColor.y, clearColor.z, clearColor.w);
      SDL_RenderClear(renderer);
      ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
      SDL_RenderPresent(renderer);
    }
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
