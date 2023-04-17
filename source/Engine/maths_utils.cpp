#include "maths_utils.h"

auto MathsUtils::x(const vertex *v) -> float { return (*v)[0]; }

auto MathsUtils::y(const vertex *v) -> float { return (*v)[1]; }

auto MathsUtils::z(const vertex *v) -> float { return (*v)[2]; }

auto MathsUtils::getNbVertex(const MathsUtils::vertex vertices[])
    -> unsigned int {
  return (unsigned int)(sizeof(*vertices) / MathsUtils::VERTEX_ELEMENTS_NB);
}

auto MathsUtils::getNbElements(const MathsUtils::vertex vertices[])
    -> unsigned int {
  return MathsUtils::getNbVertex(vertices) * VERTEX_ELEMENTS_NB;
}