
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_sdl.h>

#include "../ECS/ECS.h"
#include "../Systems/RenderTilemapSystem.h"
#include "./LevelLoader.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_mixer.h"

int main(int argc, char* argv[]) {
  SDL_Init(SDL_INIT_EVERYTHING);
  IMG_Init(IMG_INIT_PNG);
  SDL_Window* window = SDL_CreateWindow("Tile Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  std::unique_ptr<Registry> registry = std::make_unique<Registry>();
  std::unique_ptr<AssetStore> assetStore = std::make_unique<AssetStore>();

  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::table);

  // Just load the tilemap and maybe the UI system
  LevelLoader levelLoader;
  levelLoader.LoadLevel(lua, registry, assetStore, renderer, /*levelNumber=*/1);

  RenderTilemapSystem renderTilemapSystem;

  SDL_Rect camera = {0, 0, 1280, 720};
  bool isRunning = true;
  SDL_Event sdlEvent;
  // ImGui initialization
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  // ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui::StyleColorsDark();
  ImGuiSDL::Initialize(renderer, 1280, 720);  // Pass your window size

  while (isRunning) {
    while (SDL_PollEvent(&sdlEvent)) {
      ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
      ImGuiIO& io = ImGui::GetIO();
      int mouseX, mouseY;
      const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
      io.MousePos = ImVec2(mouseX, mouseY);
      io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
      io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

      switch (sdlEvent.type) {
        case SDL_QUIT:
          isRunning = false;
          break;
        case SDL_KEYDOWN:
          if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
            isRunning = false;
          }
          break;
      }
    }

    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    renderTilemapSystem.Update(registry, renderer, window, camera);

    SDL_RenderPresent(renderer);
  }

  ImGui_ImplSDL2_Shutdown();
  ImGuiSDL::Deinitialize();

  ImGui::DestroyContext();
  if (renderTilemapSystem.user_texture) {
    SDL_DestroyTexture((SDL_Texture*)renderTilemapSystem.user_texture);
    renderTilemapSystem.user_texture = nullptr;
  }
  SDL_DestroyRenderer(renderer);
  renderer = nullptr;

  SDL_DestroyWindow(window);
  window = nullptr;

  Mix_Quit();
  IMG_Quit();
  SDL_Quit();
  return 0;
}
