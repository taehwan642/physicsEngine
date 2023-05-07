#include "Mesh.hpp"

#include <iostream>
bool Engine::Mesh::Initialize(const std::string& fileName) {
  vertices.push_back({math::Vector3(-.5f, -.5f, .5f)});
  vertices.push_back({math::Vector3(-.5f, .5f, .5f)});
  vertices.push_back({math::Vector3(.5f, .5f, .5f)});
  vertices.push_back({math::Vector3(.5f, -.5f, .5f)});
  vertices.push_back({math::Vector3(-.5f, -.5f, -.5f)});
  vertices.push_back({math::Vector3(-.5f, .5f, -.5f)});
  vertices.push_back({math::Vector3(.5f, .5f, -.5f)});
  vertices.push_back({math::Vector3(.5f, -.5f, -.5f)});

  // Vertex Buffer Object = VBO
  glGenBuffers(1, &vertexBufferObject);

  // Something failed when generating buffers
  if (glGetError() != GL_NO_ERROR) {
    std::cout << "error when generating buffers" << std::endl;
    glDeleteBuffers(1, &vertexBufferObject);  // TODO: Needed?
    return false;
  }
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(float), vertices.data(),
               GL_STATIC_DRAW | GL_MAP_READ_BIT);

  // Vertex Arrays Object = VAO
  glGenVertexArrays(1, &vertexArraysObject);
  glBindVertexArray(vertexArraysObject);

  // Specify position attribute -> 0 as offset
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        (GLvoid*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  indices = std::vector<unsigned int>({0, 2, 1, 0, 3, 2, 4, 3, 0, 4, 7, 3,
                                       4, 1, 5, 4, 0, 1, 3, 6, 2, 3, 7, 6,
                                       1, 6, 5, 1, 2, 6, 7, 5, 6, 7, 4, 5});

  glGenBuffers(1, &elementBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);
  /* END OF DRAWING */

  return true;
}
void Engine::Mesh::Render() {
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBindVertexArray(vertexArraysObject);
  glDrawElements(GL_TRIANGLES,     // mode
                 36,               // count
                 GL_UNSIGNED_INT,  // type
                 (void*)0          // element array buffer offset
  );
}
void Engine::Mesh::Exit() {
  glDeleteBuffers(1, &vertexBufferObject);
  glDeleteVertexArrays(1, &vertexArraysObject);
  glDeleteBuffers(1, &elementBuffer);
}