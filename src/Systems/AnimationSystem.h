#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include <SDL2/SDL.h>

#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"
#include "../ECS/ECS.h"

class AnimationSystem : public System {
 public:
  AnimationSystem() {
    RequireComponent<AnimationComponent>();
    RequireComponent<SpriteComponent>();
  }

  void Update() {
    for (auto entity : GetSystemEntity()) {
      auto& animation = entity.GetComponent<AnimationComponent>();
      auto& sprite = entity.GetComponent<SpriteComponent>();

      animation.currentFrame = ((SDL_GetTicks() - animation.startTime) * animation.frameSpeedRate / 1000) % animation.numFrames;
      sprite.srcRect.x = animation.currentFrame * sprite.width;
      // TODO: Change the current frame
      // Change the SRC rectangle of the sprite
    }
  }
};

#endif
