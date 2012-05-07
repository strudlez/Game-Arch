#include "body.h"

body::body(egg* b) {
  model = b;
  ModelMatrix = glm::mat4(1.0f);
  LocalMatrix = glm::mat4(1.0f);
  TurnMatrix = glm::mat4(1.0f);
  MoveMatrix = glm::mat4(1.0f);

  length = 4;
  width = 2;
  height = 1.4;
  Tex = model->TexId;

}
