#ifndef MATH_DEF_HPP
#define MATH_DEF_HPP

#include <cmath>
#include <iostream>

typedef unsigned long dword;

// Math module namespace
namespace math {
// Constant for conversions
constexpr float MultiDegree2Radian = 0.0174532925199432957692;
constexpr float MultiRadian2Degree = 57.295779513082320876846364344191097757125;
// PI constant
const float PI = 3.14159265358979323846264338327950288;
// E constant
const float E = 2.71828182845904523536028747135266249;

/* Convert angle from degrees to radian function.
 * ARGUMENTS:
 *   - angle in degrees:
 *       const float alpha;
 * RETURNS:
 *   (float) - angle in radian.
 */
inline float degree2Radian(const float alpha) {
    return alpha * MultiDegree2Radian;
}  // End of 'degree2Radian' function

/* Convert angle from radian to degrees function.
 * ARGUMENTS:
 *   - angle in radians:
 *       const float alpha;
 * RETURNS:
 *   (float) - angle in degree.
 */
inline float radian2Degree(const float alpha) {
    return alpha * MultiRadian2Degree;
}  // End of 'radian2Degree' function
}  // namespace math

#endif  // MATH_DEF_HPP
