#ifndef RENDERTILEMAPSYSTEM_H
#define RENDERTILEMAPSYSTEM_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>

#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"

struct Tile {
  int id;
  SDL_Texture* texture;
  std::string name;
  int srcRectY;
  int srcRectX;
};

struct Tilemap {
  int num_cols;
  int num_rows;
  std::vector<Tile> tiles;
};

extern Tilemap tilemap;

class RenderTilemapSystem : public System {
 public:
  RenderTilemapSystem() { RequireComponent<TransformComponent>(); }

  void Update(std::unique_ptr<Registry>& registry, SDL_Renderer* renderer, SDL_Window* window, SDL_Rect& camera) {
    DrawTiles(renderer, selectedTileIndex, tilemap);  // <--- PLACE THIS HERE

    RenderGrid(renderer, camera);

    if (!user_texture) {
      user_texture = IMG_LoadTexture(renderer, "/Users/brycemartin/2dgameengine/assets/tilemaps/jungle.png");
    }
    if (user_texture == NULL) {
      // int selectedTileIndex = -1;
      Logger::Err("Error loading tilemap spritesheet");
    }

    ImGui::NewFrame();
    int tileIndex = 0;

    if (ImGui::CollapsingHeader("Tiles", ImGuiTreeNodeFlags_DefaultOpen)) {
      if (ImGui::BeginChild("TileSelector", ImVec2(0, 300), false)) {
        for (int row = 0; row < num_rows; ++row) {
          for (int col = 0; col < num_cols; ++col) {
            float x = col * tileWidth;
            float y = row * tileHeight;

            ImVec2 uv0 = ImVec2(x / (float)sheetWidth, y / (float)sheetHeight);
            ImVec2 uv1 = ImVec2((x + tileWidth) / (float)sheetWidth, (y + tileHeight) / (float)sheetHeight);

            // Unique ID for each tile
            ImGui::PushID(tileIndex);

            // std::cout << "Tile index: " << tileIndex << std::endl;
            if (ImGui::ImageButton((void*)user_texture, ImVec2(tileWidth * 2, tileHeight * 2), uv0, uv1)) {
              selectedTileIndex = tileIndex;
              Logger::Log("Selected tile index: " + std::to_string(selectedTileIndex));
            }

            // Hover selected outline
            if (ImGui::IsItemHovered() || selectedTileIndex == tileIndex) {
              ImVec2 topLeft = ImGui::GetItemRectMin();
              ImVec2 bottomRight = ImGui::GetItemRectMax();
              ImU32 color = selectedTileIndex == tileIndex ? IM_COL32(255, 255, 0, 255) : IM_COL32(255, 255, 255, 128);
              ImGui::GetWindowDrawList()->AddRect(topLeft, bottomRight, color, 0.0f, 0, 2.0f);
            }

            ImGui::PopID();

            if (col < num_cols - 1) {
              ImGui::SameLine();
            }

            ++tileIndex;
          }
        }
      }
      ImGui::EndChild();
    }
    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());
    // Handle placing a tile when user clicks on the map
    int mouseX, mouseY;
    Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);
    ImGuiIO& io = ImGui::GetIO();

    // Only place if left-click and not over ImGui
    if ((buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) && !io.WantCaptureMouse) {
      int col = mouseX / tileWidth;
      int row = mouseY / tileHeight;
      int index = row * tilemap.num_cols + col;

      if (index >= 0 && index < tilemap.tiles.size() && selectedTileIndex >= 0) {
        int selectedCol = selectedTileIndex % num_cols;
        int selectedRow = selectedTileIndex / num_cols;

        tilemap.tiles[index].id = selectedTileIndex;
        tilemap.tiles[index].srcRectX = selectedCol * tileWidth;
        tilemap.tiles[index].srcRectY = selectedRow * tileHeight;
        tilemap.tiles[index].texture = (SDL_Texture*)user_texture;
      }
    }

    if (user_texture) {
      SDL_DestroyTexture((SDL_Texture*)user_texture);
      user_texture = nullptr;
    }
  }

  void RenderGrid(SDL_Renderer* renderer, SDL_Rect& camera) {
    // for (auto entity : GetSystemEntity()) {
    //   // if (entity.HasTag("player") || entity.BelongsToGroup("enemies") || entity.BelongsToGroup("projectiles") || entity.BelongsToGroup("vegetation") ||
    //   entity.BelongsToGroup("obstacles")) {
    //   //   continue;
    //   // }
    //   const auto transform = entity.GetComponent<TransformComponent>();
    //
    //   SDL_Rect colliderRect = {static_cast<int>(transform.position.x - camera.x), static_cast<int>(transform.position.y - camera.y), static_cast<int>(32 * transform.scale.x),
    //                            static_cast<int>(32 * transform.scale.y)};
    //   SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //   SDL_RenderDrawRect(renderer, &colliderRect);
    // }
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 100);

    int tileSize = 32;
    int mapWidth = 1280;  // adjust this
    int mapHeight = 720;  // adjust this

    for (int x = 0; x < mapWidth; x += tileSize) {
      SDL_RenderDrawLine(renderer, x - camera.x, 0 - camera.y, x - camera.x, mapHeight - camera.y);
    }
    for (int y = 0; y < mapHeight; y += tileSize) {
      SDL_RenderDrawLine(renderer, 0 - camera.x, y - camera.y, mapWidth - camera.x, y - camera.y);
    }
  }

  void DrawTiles(SDL_Renderer* renderer, int selectedTileId, Tilemap tilemap) {
    int tileSize = tileHeight;  // assuming square tiles

    int tileIndex = 0;
    for (int row = 0; row < tilemap.num_rows; ++row) {
      for (int col = 0; col < tilemap.num_cols; ++col) {
        if (tileIndex >= tilemap.tiles.size()) continue;

        Tile& tile = tilemap.tiles[tileIndex];

        int srcRectY = tile.srcRectY;
        int srcRectX = tile.srcRectX;
        SDL_Rect srcRect = {srcRectX, srcRectY, tileSize, tileSize};
        SDL_Rect dstRect = {col * tileSize, row * tileSize, tileSize, tileSize};
        if (!tile.texture) {
          Logger::Err("Tile texture is null for tile index: " + std::to_string(tileIndex));
          continue;
        }
        SDL_RenderCopy(renderer, tile.texture, &srcRect, &dstRect);

        tileIndex++;
      }
    }
  }

  ImTextureID user_texture = nullptr;

 private:
  int num_cols = 10;
  int num_rows = 3;
  int tileWidth = 32;
  int tileHeight = 32;
  int sheetWidth = 320;
  int sheetHeight = 96;
  int selectedTileIndex = -1;
};
#endif
