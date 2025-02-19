#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include <SDL2/SDL.h>

#include "../Components/AudioComponent.h"
#include "../ECS/ECS.h"

class AudioSystem : public System {
 public:
  AudioSystem() { RequireComponent<AudioComponent>(); }

  void Update() {
    for (auto entity : GetSystemEntity()) {
      if (entity.HasTag("player")) {
        int channel = -1;             // Use any free channel
        if (!Mix_Playing(channel)) {  // Prevent overlapping
          Mix_PlayChannel(channel, entity.GetComponent<AudioComponent>().sound, -1);
        }
      }
    }
  }
};

#endif
