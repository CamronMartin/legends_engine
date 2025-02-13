#ifndef LEVELLOADER_H
#define LEVELLOADER_H
#include <SDL2/SDL.h>

#include <memory>
#include <sol/sol.hpp>

#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"

class LevelLoader {
 public:
  LevelLoader();
  ~LevelLoader();

  void LoadLevel(sol::state& lua, const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer, int level);
};

#endif
