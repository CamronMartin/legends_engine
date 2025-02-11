#include "../Game/LevelLoader.h"

#include <fstream>
#include <sol/sol.hpp>

#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/TransformComponent.h"
#include "./Game.h"

LevelLoader::LevelLoader() { Logger::Log("LevelLoader constructor called."); }

LevelLoader::~LevelLoader() { Logger::Log("LevelLoader destructor called."); }

void LevelLoader::LoadLevel(sol::state& lua, const std::unique_ptr<Registry>& registry, std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer, int levelNumber) {
  // Load entities and components from asset script
  lua.script_file("./assets/scripts/Level" + std::to_string(levelNumber) + ".lua");

  Logger::Log("We just opened the level1.lua");

  // Adding assets to the asset store
  assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
  assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
  assetStore->AddTexture(renderer, "tree-image", "./assets/images/tree.png");
  assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");
  assetStore->AddTexture(renderer, "chopper-image", "./assets/images/chopper-spritesheet.png");
  assetStore->AddTexture(renderer, "radar-image", "./assets/images/radar.png");
  assetStore->AddTexture(renderer, "bullet-image", "./assets/images/bullet.png");
  assetStore->AddFont("charriot-font", "./assets/fonts/charriot.ttf", 14);
  assetStore->AddFont("charriot-font-5", "./assets/fonts/charriot.ttf", 5);
  assetStore->AddFont("charriot-font-10", "./assets/fonts/charriot.ttf", 10);

  // Load the tilemap
  int tileSize = 32;
  double tileScale = 2.0;
  int mapNumCols = 25;
  int mapNumRows = 20;

  std::fstream mapFile;
  mapFile.open("./assets/tilemaps/jungle.map");

  for (int y = 0; y < mapNumRows; y++) {
    for (int x = 0; x < mapNumCols; x++) {
      char ch;
      mapFile.get(ch);
      int srcRectY = std::atoi(&ch) * tileSize;
      mapFile.get(ch);
      int srcRectX = std::atoi(&ch) * tileSize;
      mapFile.ignore();

      Entity tile = registry->CreateEntity();
      tile.Group("tiles");
      tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
      tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, false, srcRectX, srcRectY);
    }
  }
  mapFile.close();
  Game::mapWidth = mapNumCols * tileSize * tileScale;
  Game::mapHeight = mapNumRows * tileSize * tileScale;

  // Entities
  Entity chopper = registry->CreateEntity();
  chopper.Tag("player");
  chopper.AddComponent<TransformComponent>(glm::vec2(240.0, 116.0), glm::vec2(1.0, 1.0), 0.0);
  chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
  chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 1);
  chopper.AddComponent<AnimationComponent>(2, 12, true);
  chopper.AddComponent<BoxColliderComponent>(32, 32);
  chopper.AddComponent<ProjectileEmitterComponent>(glm::vec2(150.0, 150.0), 0, 10000, 10, true);
  chopper.AddComponent<KeyboardControlComponent>(glm::vec2(0, -100), glm::vec2(100, 0), glm::vec2(0, 100), glm::vec2(-100, 0));
  chopper.AddComponent<CameraFollowComponent>();
  chopper.AddComponent<HealthComponent>(100);

  Entity radar = registry->CreateEntity();
  radar.AddComponent<TransformComponent>(glm::vec2(Game::windowWidth - 74, 10.0), glm::vec2(1.0, 1.0), 0.0);
  radar.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
  radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 2, true);
  radar.AddComponent<AnimationComponent>(8, 5, true);

  Entity tank = registry->CreateEntity();
  tank.Group("enemies");
  tank.AddComponent<TransformComponent>(glm::vec2(500.0, 500.0), glm::vec2(1.0, 1.0), 0.0);
  tank.AddComponent<RigidBodyComponent>(glm::vec2(20.0, 0.0));
  tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 1);
  tank.AddComponent<BoxColliderComponent>(32, 32);
  tank.AddComponent<ProjectileEmitterComponent>(glm::vec2(100.0, 0.0), 5000, 3000, 10, false);
  tank.AddComponent<HealthComponent>(100);

  Entity truck = registry->CreateEntity();
  truck.Group("enemies");
  truck.AddComponent<TransformComponent>(glm::vec2(100.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
  truck.AddComponent<RigidBodyComponent>(glm::vec2(00.0, 0.0));
  truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 1);
  truck.AddComponent<BoxColliderComponent>(32, 32);
  truck.AddComponent<ProjectileEmitterComponent>(glm::vec2(0.0, 100.0), 2000, 5000, 10, false);
  truck.AddComponent<HealthComponent>(100);

  Entity treeA = registry->CreateEntity();
  treeA.Group("obstacles");
  treeA.AddComponent<TransformComponent>(glm::vec2(600.0, 495.0), glm::vec2(1.0, 1.0), 0.0);
  treeA.AddComponent<SpriteComponent>("tree-image", 16, 32, 2);
  treeA.AddComponent<BoxColliderComponent>(16, 32);

  Entity treeB = registry->CreateEntity();
  treeB.Group("obstacles");
  treeB.AddComponent<TransformComponent>(glm::vec2(400.0, 495.0), glm::vec2(1.0, 1.0), 0.0);
  treeB.AddComponent<SpriteComponent>("tree-image", 16, 32, 2);
  treeB.AddComponent<BoxColliderComponent>(16, 32);

  Entity label = registry->CreateEntity();
  SDL_Color green = {0, 255, 0};
  label.AddComponent<TextLabelComponent>(glm::vec2(Game::windowWidth / 2 - 40, 50), "CHOPPER 1.0", "charriot-font", green, true);
}
