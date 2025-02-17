#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include "../Components/ScriptComponent.h"
#include "../ECS/ECS.h"

class ScriptSystem : public System {
 public:
  ScriptSystem() { RequireComponent<ScriptComponent>(); }

  void Update() {
    // Loop all entities that have a script component and invoke their Lua function
    for (auto entity : GetSystemEntity()) {
      const auto script = entity.GetComponent<ScriptComponent>();
      script.func();
    }
  }
};

#endif
