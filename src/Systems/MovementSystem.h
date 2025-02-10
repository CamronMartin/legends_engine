#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include <string>

#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"

class MovementSystem : public System {
 public:
  MovementSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<RigidBodyComponent>();
  }

  void Update(double deltaTime) {
    for (auto entity : GetSystemEntity()) {
      auto& transform = entity.GetComponent<TransformComponent>();
      const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

      transform.position.x += rigidbody.velocity.x * deltaTime;
      transform.position.y += rigidbody.velocity.y * deltaTime;

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
