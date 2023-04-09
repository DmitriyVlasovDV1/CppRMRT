#ifndef VEC2_HPP
#define VEC2_HPP

#include "../math_def.hpp"

// Math module namespace
namespace math {
// Geom 2-component vector declaration
class vec2 {
public:
    float x, y;  // Two components of geom vector

    // Class default constructor
    explicit vec2() : x(0), y(0) {
    }  // End of 'vec2' function

    /* Class constructor.
     * ARGUMENTS:
     *   - vector coordinates:
     *       const float x_, y_.
     */
    explicit vec2(const float x_, const float y_) : x(x_), y(y_) {
    }  // End of 'vec2' function

    /* Class constructor.
     * ARGUMENTS:
     *   - vector coordinates:
     *       const float x_.
     */
    explicit vec2(const float x_) : x(x_), y(x_) {
    }  // End of 'vec2' function

    // Copy assigment operator
    inline vec2 &operator=(const vec2 &other) = default;

    /* Cast vector to float pointer function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (float *) - float pointer to the vector.
     */
    inline explicit operator float *() {
        return &x;
    }  // End of 'operator float *' function

    /* Cast vector to float pointer function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (float *) - const float pointer to the vector.
     */
    inline explicit operator const float *() const {
        return &x;
    }  // End of 'operator const float *' function

    /* Get vector length function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (float) - vector's length.
     */
    inline float operator!() const {
        return std::sqrt(x * x + y * y);
    }  // End of 'operator!' function

    /* Get vector length squared function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (float) - vector's length squared.
     */
    inline float operator$() const {
        return x * x + y * y;
    }  // End of 'operator!' function

    /* Print vector function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    inline void print() const {
        std::cout << "vec3: x:" << x << "; y: " << y << std::endl;
    }  // End of 'print' function

    /* Compare function.
     * ARGUMENTS:
     *   - vector to compare:
     *       const vec2 &V;
     * RETURNS:
     *   (bool) - result of compare.
     */
    inline bool operator>(const vec2 &other) const {
        return x > other.x || (x == other.x && y > other.y);
    }  // End of 'operator>' function

    /* Compare function.
     * ARGUMENTS:
     *   - vector to compare:
     *       const vec2 &V;
     * RETURNS:
     *   (bool) - result of compare.
     */
    inline bool operator<(const vec2 &other) const {
        return x < other.x || (x == other.x && y < other.y);
    }  // End of 'operator<' function

    /* Compare function.
     * ARGUMENTS:
     *   - vector to compare:
     *       const vec2 &V;
     * RETURNS:
     *   (bool) - result of compare.
     */
    inline bool operator==(const vec2 &other) const {
        return x == other.x && y == other.y;
    }  // End of 'operator==' function

    /* Compare function.
     * ARGUMENTS:
     *   - vector to compare:
     *       const vec2 &V;
     * RETURNS:
     *   (bool) - result of compare.
     */
    inline bool operator!=(const vec2 &other) const {
        return !(*this == other);
    }  // End of 'operator!=' function

    /* Sum of two vectors function.
     * ARGUMENTS:
     *   - other vector:
     *       const vec2 &other;
     * RETURNS:
     *   (vec2) - result of addition.
     */
    inline vec2 operator+(const vec2 &other) const {
        return vec2(x + other.x, y + other.y);
    }  // End of 'operator+' function

    /* Self-sum of two vectors function.
     * ARGUMENTS:
     *   - other vector:
     *       const vec2 &other;
     * RETURNS:
     *   (vec2) - itself.
     */
    inline vec2 operator+=(const vec2 &other) {
        x += other.x;
        y += other.y;
        return *this;
    }  // End of 'operator+=' function

    /* Subtraction of two vectors function.
     * ARGUMENTS:
     *   - other vector:
     *       const vec2 &other;
     * RETURNS:
     *   (vec2) - result of subtraction.
     */
    inline vec2 operator-(const vec2 &other) const {
        return vec2(x - other.x, y - other.y);
    }  // End of 'operator-' function

    /* Self-subtraction of two vectors function.
     * ARGUMENTS:
     *   - other vector:
     *       const vec2 &other;
     * RETURNS:
     *   (vec2) - itself.
     */
    inline vec2 operator-=(const vec2 &other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }  // End of 'operator-=' function

    /* Vector negation function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec2) - negative vector.
     */
    inline vec2 operator-() const {
        return vec2(-x, -y);
    }  // End of 'operator-' function

    /* Multiplication of vector and number function.
     * ARGUMENTS:
     *   - number:
     *       float n;
     * RETURNS:
     *   (vec2) - result of multiplication.
     */
    inline vec2 operator*(float n) const {
        return vec2(x * n, y * n);
    }  // End of 'operator*' function

    /* Division of vector and number function.
     * ARGUMENTS:
     *   - number:
     *       float n;
     * RETURNS:
     *   (vec2) - result of division.
     */
    inline vec2 operator/(float n) const {
        return vec2(x / n, y / n);
    }  // End of 'operator/' function

    /* Self-multiplication of vector and number function.
     * ARGUMENTS:
     *   - number:
     *       float n;
     * RETURNS:
     *   (vec2) - itself
     */
    inline vec2 operator*=(float n) {
        x *= n;
        y *= n;
        return *this;
    }  // End of 'operator*=' function

    /* Self-division of vector and number function.
     * ARGUMENTS:
     *   - number:
     *       float n;
     * RETURNS:
     *   (vec2) - itself.
     */
    inline vec2 operator/=(float n) {
        x /= n;
        y /= n;
        return *this;
    }  // End of 'operator/=' function

    /* Dot product of two vectors function.
     * ARGUMENTS:
     *   - other vector:
     *       const vec2 &other;
     * RETURNS:
     *   (float) - result of dot product.
     */
    inline float operator&(const vec2 &other) const {
        return x * other.x + y * other.y;
    }  // End of 'operator&' function

    /* Normalize vector function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec2) - normalized vector (itself).
     */
    inline vec2 normalize() {
        const float length = !(*this);
        if (length == 0) return vec2(0);
        *this /= length;
        return *this;
    }  // End of 'normalize' function

    /* Normalizing vector function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec2) - normalized vector (other).
     */
    inline vec2 normalizing() const {
        const float length = !(*this);
        if (length == 0) return vec2(0);
        vec2 tmp = *this / length;
        return tmp;
    }  // End of 'normalizing' function

    /* Get minimum of two vectors function.
     * ARGUMENTS:
     *   - vectors to compare:
     *       const vec2 &v1, &v2;
     * RETURNS:
     *   (vec2) - minimum vector.
     */
    inline static vec2 min(const vec2 &v1, const vec2 &v2) {
        return vec2(std::min(v1.x, v2.x), std::min(v1.y, v2.y));
    }  // End of 'min' function

    /* Get maximum of two vectors function.
     * ARGUMENTS:
     *   - vectors to compare:
     *       const vec2 &v1, &v2;
     * RETURNS:
     *   (vec2) - maximum vector.
     */
    inline static vec2 max(const vec2 &v1, const vec2 &v2) {
        return vec2(std::max(v1.x, v2.x), std::max(v1.y, v2.y));
    }  // End of 'max' function
};     // End of 'vec2' class
}  // namespace math

#endif  // VEC2_HPP
