#pragma once

#include <glm/glm.hpp> //feel free to use your own math library!
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/core/func_geometric.hpp>

namespace tritest {
  struct Sphere {
    glm::vec3 c; // Sphere center
    float r; // Sphere radius
  };

  // The next two functions are
  // from Real-Time Collision Detection by Christer Ericson, published
  // by Morgan Kaufmann Publishers, Copyright 2005 Elsevier Inc

  glm::vec3 ClosestPointTriangle(glm::vec3& p, glm::vec3& a, glm::vec3& b, glm::vec3& c);

  // Returns true if sphere s intersects triangle ABC, false otherwise.
  // The point p on abc closest to the sphere center is also returned
  int TestSphereTriangle(Sphere& s, glm::vec3& a, glm::vec3& b, glm::vec3& c, glm::vec3 &p);
}
