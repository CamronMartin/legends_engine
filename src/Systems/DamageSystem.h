#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../Components/BoxColliderComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class DamageSystem : public System {
 public:
  DamageSystem() { RequireComponent<BoxColliderComponent>(); }

  void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus) {
    eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::OnCollision);
  }

  void OnCollision(CollisionEvent &event) {
    Entity a = event.a;
    Entity b = event.b;
    Logger::Log("Collision event emitted: " + std::to_string(a.GetId()) + " and " + std::to_string(b.GetId()));

    if (a.BelongsToGroup("projectiles") && b.HasTag("player")) {
      OnProjectileHitsPlayer(a, b);
    }

    if (b.BelongsToGroup("projectiles") && a.HasTag("player")) {
      OnProjectileHitsPlayer(b, a);
    }

    if (a.BelongsToGroup("projectiles") && b.BelongsToGroup("enemies")) {
      OnProjectileHitEnemy(a, b);
    }

    if (b.BelongsToGroup("projectiles") && a.BelongsToGroup("enemies")) {
      OnProjectileHitEnemy(b, a);
    }
  }

  void OnProjectileHitsPlayer(Entity projectile, Entity player) {
    auto projectileComponent = projectile.GetComponent<ProjectileComponent>();

    if (!projectileComponent.isFriendly) {
      auto &health = player.GetComponent<HealthComponent>();

      health.healthPercentage -= projectileComponent.hitPercentDamage;

      if (health.healthPercentage <= 0) {
        player.Kill();
      }

      projectile.Kill();
    }
  }

  void OnProjectileHitEnemy(Entity projectile, Entity enemy) {
    auto projectileComponent = projectile.GetComponent<ProjectileComponent>();

    if (projectileComponent.isFriendly) {
      auto &health = enemy.GetComponent<HealthComponent>();
      health.healthPercentage -= projectileComponent.hitPercentDamage;

      if (health.healthPercentage <= 0) {
        enemy.Kill();
      }

      projectile.Kill();
    }
  }

  void Update() {}
};

#endif
