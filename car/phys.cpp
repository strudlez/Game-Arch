#include "phys.h"


void phys::calc(glm::mat4* parent, glm::mat4* parent2) {
  if(parent == NULL) {
    ModelMatrix = LocalMatrix * TurnMatrix * MoveMatrix;
  } else {
    ModelMatrix = (*parent) * (*parent2) * LocalMatrix * TurnMatrix * MoveMatrix;
  }
}

void phys::Draw(glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix, bool wire) {
  if(!wire) {
    model->TexId = Tex;
  } else {
    model->TexId = 0;
  }
  model->DrawModel(ViewMatrix, ProjectionMatrix, ModelMatrix);
}
