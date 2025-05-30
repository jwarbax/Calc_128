#include <iostream>
#include "input.h"
#include "SDL3/SDL.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include <imgui_impl_sdlrenderer3.h>

#include "output.h"

int initializeSDL(SDL_Renderer*& renderer, SDL_Window*& window)
{
  // Setup SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        printf("Error: SDL_Init(): %s\n", SDL_GetError());
        return 1;
    }

    // Create window with SDL_Renderer graphics context
  constexpr SDL_WindowFlags resizable = SDL_WINDOW_RESIZABLE;
    window = SDL_CreateWindow("Calc_128", 1280, 720, resizable);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return 2;
    }
    renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);
    if (renderer == nullptr)
    {
        SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
        return 3;
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  return 0;
}

int main()
{
  SDL_Renderer* renderer = nullptr;
  SDL_Window* window = nullptr;

  if (initializeSDL(renderer, window)==0)
  {
    Context reference;
    bool running = true;
    while (running)
    {
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        ImGui_ImplSDL3_ProcessEvent(&event);
        if (event.type == SDL_EVENT_QUIT)
          running = false;
      }

      // Start the ImGui frame
      ImGui_ImplSDLRenderer3_NewFrame();
      ImGui_ImplSDL3_NewFrame();
      ImGui::NewFrame();

      // Your logic (this will block; see note below)
      calculationResult(reference);

      // Rendering
      ImGui::Render();
      SDL_SetRenderDrawColor(renderer, 45, 55, 60, 255);
      SDL_RenderClear(renderer);
      ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(),renderer);
      SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
  }
  return 0;
}
