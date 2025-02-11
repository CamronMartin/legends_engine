#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include <src/Game/Game.h>

#include <string>

#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "SDL2/SDL_render.h"

class MovementSystem : public System {
 public:
  MovementSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<RigidBodyComponent>();
  }

  void SubscribeToEvents(const std::unique_ptr<EventBus>& eventBus) {
    eventBus->SubscribeToEvent<CollisionEvent>(this, &MovementSystem::OnCollision);
  }

  void OnCollision(CollisionEvent& event) {
    Entity a = event.a;
    Entity b = event.b;
    Logger::Log("Collision event emitted: " + std::to_string(a.GetId()) + " and " + std::to_string(b.GetId()));

    if (a.BelongsToGroup("enemies") && b.BelongsToGroup("obstacles")) {
      OnEnemyHitsObstacles(a, b);
    }
    if (a.BelongsToGroup("obstacles") && b.BelongsToGroup("enemies")) {
      OnEnemyHitsObstacles(a, b);
    }
  }

  void OnEnemyHitsObstacles(Entity enemy, Entity obstacle) {
    if (enemy.HasComponent<RigidBodyComponent>() && enemy.HasComponent<SpriteComponent>()) {
      auto& rigidbody = enemy.GetComponent<RigidBodyComponent>();
      auto& sprite = enemy.GetComponent<SpriteComponent>();

      if (rigidbody.velocity.x != 0) {
        rigidbody.velocity.x *= -1;
        sprite.flip = (sprite.flip == SDL_FLIP_NONE) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
      }
      if (rigidbody.velocity.y != 0) {
        rigidbody.velocity.y *= -1;
        sprite.flip = (sprite.flip == SDL_FLIP_NONE) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
      }
    }
  }
  void Update(double deltaTime) {
    for (auto entity : GetSystemEntity()) {
      auto& transform = entity.GetComponent<TransformComponent>();
      const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

      transform.position.x += rigidbody.velocity.x * deltaTime;
      transform.position.y += rigidbody.velocity.y * deltaTime;

      if (entity.HasTag("player")) {
        int left = 10;
        int top = 10;
        int right = 50;
        int bottom = 50;
        transform.position.x = transform.position.x < left ? left : transform.position.x;
        transform.position.x =
            transform.position.x > Game::mapWidth - right ? Game::mapWidth - right : transform.position.x;
        transform.position.y = transform.position.y < top ? top : transform.position.y;
        transform.position.y =
            transform.position.y > Game::mapHeight - bottom ? Game::mapHeight - bottom : transform.position.y;
      }
      bool isEntityOutsideMap = (transform.position.x < 0 || transform.position.x > Game::mapWidth ||
                                 transform.position.y < 0 || transform.position.y > Game::mapHeight);

      // Kill all entities that move outside the map boundaries
      if (isEntityOutsideMap && !entity.HasTag("player")) {
        entity.Kill();
      }
    }
  }
};

#endif
