
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

      switch (event.symbol) {
        case SDLK_UP:
          rigidbody.velocity = keyboardcontrol.upVelocity;
          sprite.srcRect.y = sprite.height * 0;
          break;
        case SDLK_RIGHT:
          rigidbody.velocity = keyboardcontrol.rightVelocity;
          sprite.srcRect.y = sprite.height * 1;
          break;
        case SDLK_DOWN:
          rigidbody.velocity = keyboardcontrol.downVelocity;
          sprite.srcRect.y = sprite.height * 2;
          break;
        case SDLK_LEFT:
          rigidbody.velocity = keyboardcontrol.leftVelocity;
          sprite.srcRect.y = sprite.height * 3;
          break;
      }
    }
  }
  void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus) { eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed); }
  void Update() {}
};

#endif  // KEYBOARDMOVEMENTSYSTEM_H
