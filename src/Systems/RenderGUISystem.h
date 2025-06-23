#ifndef RENDERGUISYSTEM_H
#define RENDERGUISYSTEM_H

#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>

#include <fstream>
#include <memory>

#include "../Components/BoxColliderComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "SDL2/SDL_render.h"

struct Tile {
  int id;
  SDL_Texture* texture;
  std::string name;
};

struct Tilemap {
  int num_cols;
  int num_rows;
  std::vector<int> tiles;  // 1D vector storing tile IDs per cell
};
class RenderGUISystem : public System {
 public:
  RenderGUISystem() = default;

  SDL_Texture* LoadTexture(const std::string& file, SDL_Renderer* renderer) {
    SDL_Surface* surface = SDL_LoadBMP(file.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
  }

  void RenderTilemapEditor(Tilemap& tilemap, const ImVec2& tile_size, const ImVec2& grid_pos) {
    ImGui::Begin("Tilemap Editor");

    // --- Tile Palette ---
    ImGui::Text("Tile Palette:");
    for (auto& [id, tile] : tileset) {
      ImGui::PushID(id);
      if (ImGui::ImageButton((void*)tile.texture, tile_size)) {
        selectedTileId = id;
      }
      ImGui::SameLine();
      ImGui::Text("%s", tile.name.c_str());
      ImGui::PopID();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Click to paint on tilemap:");

    // --- Tilemap Grid ---
    for (int row = 0; row < tilemap.num_rows; ++row) {
      for (int col = 0; col < tilemap.num_cols; ++col) {
        int index = row * tilemap.num_cols + col;
        ImVec2 tile_pos = ImVec2(grid_pos.x + col * tile_size.x, grid_pos.y + row * tile_size.y);
        ImGui::SetCursorScreenPos(tile_pos);

        int tileId = tilemap.tiles[index];
        SDL_Texture* tex = tileset[tileId].texture;
        ImGui::Image((void*)tex, tile_size);

        if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
          tilemap.tiles[index] = selectedTileId;
        }
      }
    }

    ImGui::End();
  }
  void Update(std::unique_ptr<Registry>& registry, const SDL_Rect& camera) {
    // Draw all the imgui objects in the screen
    ImGui::NewFrame();

    if (ImGui::Begin("Spawn enemies")) {
      // Input for the enemy X and Y position
      static int posX = 0;
      static int posY = 0;
      static int scaleX = 1;
      static int scaleY = 1;
      static int velX = 0;
      static int velY = 0;
      static float projSpeed;
      static int projRepeat;
      static int projDuration;
      static float rotation = 0.0;
      static float projAngle = 0.0;
      static int health;
      const char* sprites[] = {"tank-tiger-right-texture", "truck-image"};
      static int selectedSpriteIndex = 0;

      if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Combo("texture id", &selectedSpriteIndex, sprites, IM_ARRAYSIZE(sprites));
      }
      ImGui::Spacing();

      if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputInt("position x", &posX);
        ImGui::InputInt("position y", &posY);
        ImGui::SliderInt("scale x", &scaleX, 1, 10);
        ImGui::SliderInt("scale y", &scaleY, 1, 10);
        ImGui::SliderAngle("rotation (deg)", &rotation, 0, 360);
      }
      ImGui::Spacing();

      if (ImGui::CollapsingHeader("Rigid body", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputInt("velocity x", &velX);
        ImGui::InputInt("velocity y", &velY);
      }
      ImGui::Spacing();

      if (ImGui::CollapsingHeader("Projectile Emitter", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SliderAngle("angle (deg)", &projAngle, 0, 360);
        ImGui::SliderFloat("speed (px/sec)", &projSpeed, 10, 500);
        ImGui::InputInt("repeat (sec)", &projRepeat);
        ImGui::InputInt("duration (sec)", &projDuration);
      }
      ImGui::Spacing();

      if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SliderInt("%", &health, 0, 100);
      }

      ImGui::Spacing();
      ImGui::Separator();
      ImGui::Spacing();

      if (ImGui::Button("Create new enemy")) {
        // TODO: Create a new entity
        Entity enemy = registry->CreateEntity();
        enemy.Group("enemies");
        enemy.AddComponent<TransformComponent>(glm::vec2(posX, posY), glm::vec2(scaleX, scaleY), glm::degrees(rotation));
        enemy.AddComponent<RigidBodyComponent>(glm::vec2(velX, velY));
        enemy.AddComponent<SpriteComponent>(sprites[selectedSpriteIndex], 32, 32, 2);

        enemy.AddComponent<BoxColliderComponent>(25, 20, glm::vec2(5, 5));
        double projVelX = cos(projAngle) * projSpeed;
        double projVelY = sin(projAngle) * projSpeed;
        enemy.AddComponent<ProjectileEmitterComponent>(glm::vec2(projVelX, projVelY), projRepeat * 1000, projDuration * 1000, 10, false);
        enemy.AddComponent<HealthComponent>(health);

        posX = posY = scaleY = scaleX = rotation = projAngle = 0;
        projRepeat = projDuration = 0;
        projSpeed = 100;
        health = 100;
      }
    }
    ImGui::End();

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0, 0));
    ImGui::SetNextWindowBgAlpha(0.9f);
    if (ImGui::Begin("Map Coordinates", NULL, windowFlags)) {
      ImGui::Text("Map coordinates (x=%1.f, y=%.1f)", ImGui::GetIO().MousePos.x + camera.x, ImGui::GetIO().MousePos.y + camera.y);
    }
    ImGui::End();

    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());
  }

 private:
  std::unordered_map<int, Tile> tileset;
  int selectedTileId;
};

#endif
