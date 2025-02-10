
#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H

#include <SDL2/SDL_keycode.h>

#include <cstddef>

#include "../Components/KeyboardControlComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "SDL2/SDL_events.h"

class KeyboardControlSystem : public System {
 public:
  KeyboardControlSystem() {
    RequireComponent<SpriteComponent>();
    RequireComponent<RigidBodyComponent>();
    RequireComponent<KeyboardControlComponent>();
  }

  void OnKeyPressed(KeyPressedEvent &event) {
    // Change the sprite and velocity of my entity
    for (auto entity : GetSystemEntity()) {
      const auto keyboardcontrol = entity.GetComponent<KeyboardControlComponent>();
      auto &sprite = entity.GetComponent<SpriteComponent>();
      auto &rigidbody = entity.GetComponent<RigidBodyComponent>();

      bool move_up = false;
      bool move_right = false;
      bool move_left = false;
      bool move_down = false;
      // TODO: Add a SDL_KEYUP and SDL_KEYDOWN switch clause to
      // be able to make units move diagonally

      switch (event.symbol) {
        case SDLK_UP:
          rigidbody.velocity = keyboardcontrol.upVelocity;
          sprite.srcRect.y = sprite.height * 0;
          move_up = true;
          break;
        case SDLK_RIGHT:
          rigidbody.velocity = keyboardcontrol.rightVelocity;
          sprite.srcRect.y = sprite.height * 1;
          move_right = true;
          break;
        case SDLK_DOWN:
          rigidbody.velocity = keyboardcontrol.downVelocity;
          sprite.srcRect.y = sprite.height * 2;
          move_down = true;
          break;
        case SDLK_LEFT:
          rigidbody.velocity = keyboardcontrol.leftVelocity;
          sprite.srcRect.y = sprite.height * 3;
          move_left = true;
          break;
      }
    }
  }
  void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus) {
    eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
  }
  void Update() {}
};

#endif  // KEYBOARDMOVEMENTSYSTEM_H
