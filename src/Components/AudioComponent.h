#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H

#include <SDL2/SDL_mixer.h>

struct AudioComponent {
  Mix_Chunk* sound;
  int loop;
  AudioComponent(Mix_Chunk* sound = NULL, int loop = 0) {
    this->sound = sound;
    this->loop = loop;
  }
};

#endif
