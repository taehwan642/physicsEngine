#ifndef _MATHS_UTILS_H
#define _MATHS_UTILS_H

namespace MathsUtils {

const unsigned int VERTEX_ELEMENTS_NB = 3;

/**
 * @brief Reassemble all elements of a vertex in a dedicated type
 * Contains, in the following order: x, y, z (position, in 3D), r, g, b (RGB
 * colors)
 */
typedef float vertex[VERTEX_ELEMENTS_NB];

/**
 * @brief Returns the x position accessor in a 3 dimensional vector
 *
 * @param v - the vertex array
 * @return The x position
 */
auto x(const vertex *v) -> float;

/**
 * @brief Returns the y position accessor in a 3 dimensional vector
 *
 * @param v - the vertex array
 * @return The y position
 */
auto y(const vertex *v) -> float;

/**
 * @brief Returns the z position accessor in a 3 dimensional vector
 *
 * @param v - the vertex array
 * @return The z position
 */
auto z(const vertex *v) -> float;

/**
 * @brief Returns the number of individual vertex arrays inside a vertices array
 *
 * @param vertices - an array of vertex
 * @return The number of the individual vertex arrays
 */
auto getNbVertex(const vertex vertices[]) -> unsigned int;

/**
 * @brief Sums and returns the number of individual elements stored in the
 * vertices array
 *
 * @param vertices - an array of vertex
 * @return The sum of all individual elements stored in the vertices array
 */
auto getNbElements(const vertex vertices[]) -> unsigned int;

}  // namespace MathsUtils

#endif