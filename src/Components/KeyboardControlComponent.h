#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H

#include <glm/glm.hpp>

struct KeyboardControlComponent {
  glm::vec2 upVelocity;
  glm::vec2 rightVelocity;
  glm::vec2 downVelocity;
  glm::vec2 leftVelocity;
  glm::vec2 upRightVelocity;
  glm::vec2 upLeftVelocity;
  glm::vec2 downRightVelocity;
  glm::vec2 downLeftVelocity;
  KeyboardControlComponent(glm::vec2 upVelocity = glm::vec2(0), glm::vec2 rightVelocity = glm::vec2(0),
                           glm::vec2 downVelocity = glm::vec2(0), glm::vec2 leftVelocity = glm::vec2(0)) {
    this->upVelocity = upVelocity;
    this->rightVelocity = rightVelocity;
    this->downVelocity = downVelocity;
    this->leftVelocity = leftVelocity;
    this->upRightVelocity = upVelocity + rightVelocity;
    this->upLeftVelocity = upVelocity + leftVelocity;
    this->downRightVelocity = downVelocity + rightVelocity;
    this->downLeftVelocity = downVelocity + leftVelocity;
  }
};

#endif
