#ifndef VEC3_HPP
#define VEC3_HPP

#include "../math_def.hpp"

// Math module namespace
namespace math {
// Geom 3-component vector declaration
class vec3 {
public:
    float x, y, z;  // Three components of geom vector

    // Class default constructor
    explicit vec3() : x(0), y(0), z(0) {
    }  // End of 'vec3' function

    /* Class constructor.
     * ARGUMENTS:
     *   - vector coordinates:
     *       const float x_, y_, z_.
     */
    explicit vec3(const float x_, const float y_, const float z_) : x(x_), y(y_), z(z_) {
    }  // End of 'vec3' function

    /* Class constructor.
     * ARGUMENTS:
     *   - vector coordinates:
     *       const float x_.
     */
    explicit vec3(const float x_) : x(x_), y(x_), z(x_) {
    }  // End of 'vec3' function

    // Copy assigment operator
    inline vec3 &operator=(const vec3 &other) = default;

    /* Cast vector to float pointer function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (float *) - float pointer to the vector.
     */
    inline explicit constexpr operator float *() {
        return &x;
    }  // End of 'operator float *' function

    /* Get vector length function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (float) - vector's length.
     */
    inline float operator!() const {
        return std::sqrt(x * x + y * y + z * z);
    }  // End of 'operator!' function

    /* Get vector length squared function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (float) - vector's length squared.
     */
    inline constexpr float squaredLength() const {
        return x * x + y * y + z * z;
    }  // End of 'squaredLength!' function

    /* Print vector function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    inline void print() const {
        std::cout << "vec3: x:" << x << "; y: " << y << "; z:" << z << std::endl;
    }  // End of 'print' function

    /* Compare function.
     * ARGUMENTS:
     *   - vector to compare:
     *       const vec3 &V;
     * RETURNS:
     *   (bool) - result of compare.
     */
    inline bool operator>(const vec3 &other) const {
        return x > other.x || (x == other.x && (y > other.y || (y == other.y && z > other.z)));
    }  // End of 'operator>' function

    /* Compare function.
     * ARGUMENTS:
     *   - vector to compare:
     *       const vec3 &V;
     * RETURNS:
     *   (bool) - result of compare.
     */
    inline bool operator<(const vec3 &other) const {
        return x < other.x || (x == other.x && (y < other.y || (y == other.y && z < other.z)));
    }  // End of 'operator<' function

    /* Compare function.
     * ARGUMENTS:
     *   - vector to compare:
     *       const vec3 &V;
     * RETURNS:
     *   (bool) - result of compare.
     */
    inline bool operator==(const vec3 &other) const {
        return x == other.x && y == other.y && z == other.z;
    }  // End of 'operator==' function

    /* Compare function.
     * ARGUMENTS:
     *   - vector to compare:
     *       const vec3 &V;
     * RETURNS:
     *   (bool) - result of compare.
     */
    inline bool operator!=(const vec3 &other) const {
        return !(*this == other);
    }  // End of 'operator!=' function

    /* Sum of two vectors function.
     * ARGUMENTS:
     *   - other vector:
     *       const vec3 &other;
     * RETURNS:
     *   (vec3) - result of addition.
     */
    inline vec3 operator+(const vec3 &other) const {
        return vec3(x + other.x, y + other.y, z + other.z);
    }  // End of 'operator+' function

    /* Self-sum of two vectors function.
     * ARGUMENTS:
     *   - other vector:
     *       const vec3 &other;
     * RETURNS:
     *   (vec3) - itself.
     */
    inline vec3 operator+=(const vec3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }  // End of 'operator+=' function

    /* Subtraction of two vectors function.
     * ARGUMENTS:
     *   - other vector:
     *       const vec3 &other;
     * RETURNS:
     *   (vec3) - result of subtraction.
     */
    inline vec3 operator-(const vec3 &other) const {
        return vec3(x - other.x, y - other.y, z - other.z);
    }  // End of 'operator-' function

    /* Self-subtraction of two vectors function.
     * ARGUMENTS:
     *   - other vector:
     *       const vec3 &other;
     * RETURNS:
     *   (vec3) - itself.
     */
    inline vec3 operator-=(const vec3 &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }  // End of 'operator-=' function

    /* Vector negation function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec3) - negative vector.
     */
    inline vec3 operator-() const {
        return vec3(-x, -y, -z);
    }  // End of 'operator-' function

    /* Multiplication of vector and number function.
     * ARGUMENTS:
     *   - number:
     *       float n;
     * RETURNS:
     *   (vec3) - result of multiplication.
     */
    inline vec3 operator*(float n) const {
        return vec3(x * n, y * n, z * n);
    }  // End of 'operator*' function

    /* Division of vector and number function.
     * ARGUMENTS:
     *   - number:
     *       float n;
     * RETURNS:
     *   (vec3) - result of division.
     */
    inline vec3 operator/(float n) const {
        return vec3(x / n, y / n, z / n);
    }  // End of 'operator/' function

    /* Self-multiplication of vector and number function.
     * ARGUMENTS:
     *   - number:
     *       float n;
     * RETURNS:
     *   (vec3) - itself
     */
    inline vec3 operator*=(float n) {
        x *= n;
        y *= n;
        z *= n;
        return *this;
    }  // End of 'operator*=' function

    /* Self-division of vector and number function.
     * ARGUMENTS:
     *   - number:
     *       float n;
     * RETURNS:
     *   (vec3) - itself.
     */
    inline vec3 operator/=(float n) {
        x /= n;
        y /= n;
        z /= n;
        return *this;
    }  // End of 'operator/=' function

    /* Dot product of two vectors function.
     * ARGUMENTS:
     *   - other vector:
     *       const vec3 &other;
     * RETURNS:
     *   (float) - result of dot product.
     */
    inline float operator&(const vec3 &other) const {
        return x * other.x + y * other.y + z * other.z;
    }  // End of 'operator&' function

    /* Cross product of two vectors function.
     * ARGUMENTS:
     *   - other vector:
     *       const vec3 &other;
     * RETURNS:
     *   (vec3) - result of cross product.
     */
    inline vec3 operator%(const vec3 &other) const {
        return vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }  // End of 'operator%' function

    /* Self-cross product of two vectors function.
     * ARGUMENTS:
     *   - other vector:
     *       const vec3 &other;
     * RETURNS:
     *   (vec3) - itself.
     */
    inline vec3 operator%=(const vec3 &other) {
        x = y * other.z - z * other.y;
        y = z * other.x - x * other.z;
        z = x * other.y - y * other.x;
        return *this;
    }  // End of 'operator%=' function

    /* Normalize vector function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec3) - normalized vector (itself).
     */
    inline vec3 normalize() {
        const float length = !(*this);
        if (length == 0) return vec3(0);
        *this /= length;
        return *this;
    }  // End of 'normalize' function

    /* Normalizing vector function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec3) - normalized vector (other).
     */
    inline vec3 normalizing() const {
        const float length = !(*this);
        if (length == 0) return vec3(0);
        vec3 tmp = *this / length;
        return tmp;
    }  // End of 'normalizing' function

    /* Convert vector to dword number (color in abgr format) function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (dword) - color from vector;
     */
    inline dword toDword() const {
        vec3 tmp = this->normalizing();
        return (static_cast<int>(tmp.z * 255) << 0) | (static_cast<int>(tmp.y * 255) << 8) |
               (static_cast<int>(tmp.x * 255) << 16) | 0xFF000000;
    } /* End of 'toDword' function */

    /* Get minimum of two vectors function.
     * ARGUMENTS:
     *   - vectors to compare:
     *       const vec3 &v1, &v2;
     * RETURNS:
     *   (vec3) - minimum vector.
     */
    inline static vec3 min(const vec3 &v1, const vec3 &v2) {
        return vec3(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z));
    }  // End of 'min' function

    /* Get maximum of two vectors function.
     * ARGUMENTS:
     *   - vectors to compare:
     *       const vec3 &v1, &v2;
     * RETURNS:
     *   (vec3) - maximum vector.
     */
    inline static vec3 max(const vec3 &v1, const vec3 &v2) {
        return vec3(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
    }  // End of 'max' function

    /* Get angle between (0-pi) two vectors function.
     * ARGUMENTS:
     *   - vectors to calculating:
     *       const vec3 &v1, &v2;
     * RETURNS:
     *   (float) - angle in radians (0-pi) between two vectors.
     */
    inline static float getAngleBetween(const vec3 &v1, const vec3 &v2) {
        return acos(v1.normalizing() & v2.normalizing());
    }  // End of 'getAngleBetween' function
};     // End of 'vec3' class
}  // namespace math

#endif  // VEC3_HPP
