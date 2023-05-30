#ifndef MATR4_HPP
#define MATR4_HPP

#include "../math_def.hpp"

// Math module namespace
namespace math {
// Matrix 4x4 class definition
class matr4 {
public:
    float matrix[4][4];

    // Class default constructor
    explicit matr4() : matrix{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}} {
    }

    /* Class constructor.
     * ARGUMENTS:
     *   - matrix components:
     *       const float Aij;
     */
    explicit matr4(
        const float A00,
        const float A01,
        const float A02,
        const float A03,
        const float A10,
        const float A11,
        const float A12,
        const float A13,
        const float A20,
        const float A21,
        const float A22,
        const float A23,
        const float A30,
        const float A31,
        const float A32,
        const float A33
    )
        : matrix{{A00, A01, A02, A03}, {A10, A11, A12, A13}, {A20, A21, A22, A23}, {A30, A31, A32, A33}} {
    }  // End of 'matr4' function

    /* Class constructor.
     * ARGUMENTS:
     *   - float array 4x4:
     *       const float M_[4][4].
     */
    explicit matr4(const float M_[4][4])
        : matrix{
              {M_[0][0], M_[0][1], M_[0][2], M_[0][3]},
              {M_[1][0], M_[1][1], M_[1][2], M_[1][3]},
              {M_[2][0], M_[2][1], M_[2][2], M_[2][3]},
              {M_[3][0], M_[3][1], M_[3][2], M_[3][3]}} {
    }  // End of 'matr4' function

    /* Get pointer to a matrix function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (float *) - float pointer to the matrix.
     */
    inline explicit operator float *() {
        return matrix[0];
    }  // End of 'operator float *' function

    /* Get constant pointer to a matrix function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (const float *) - const float pointer to the matrix.
     */
    inline explicit operator const float *() const {
        return matrix[0];
    }  // End of 'operator const float *' function

    /* Get position vector from transform matrix function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec3) - position vector.
     */
    inline vec3 getPosition() const {
        return vec3(matrix[3][0], matrix[3][1], matrix[3][2]);
    }  // End of 'getPosition' function

    /* Get forward vector from transform matrix function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec3) - forward vector.
     */
    inline vec3 getForward() const {
        return vec3(matrix[2][0], matrix[2][1], matrix[2][2]);
    }  // End of 'getForward' function

    /* Get right vector from transform matrix function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec3) - right vector.
     */
    inline vec3 getRight() const {
        return vec3(matrix[0][0], matrix[0][1], matrix[0][2]);
    }  // End of 'getRight' function

    /* Get up vector from transform matrix function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec3) - up vector.
     */
    inline vec3 getUp() const {
        return vec3(matrix[1][0], matrix[1][1], matrix[1][2]);
    }  // End of 'getUp' function

    /* Calculate determinant of 3x3 matrix function.
     * ARGUMENTS:
     *   - matrix components:
     *       const float Aij;
     * RETURNS:
     *   (float) - matrix 3x3 determinant.
     */
    inline static float matr3Det(
        const float A00,
        const float A01,
        const float A02,
        const float A10,
        const float A11,
        const float A12,
        const float A20,
        const float A21,
        const float A22
    ) {
        return A00 * (A11 * A22 - A12 * A21) - A01 * (A10 * A22 - A12 * A20) + A02 * (A10 * A21 - A11 * A20);
    }  // End of 'matr3Det' function

    /* Count matrix 4x4 determinant function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (float) - matrix 4x4 determinant.
     */
    inline float det() const {
        return matrix[0][0] * matr3Det(
                                  matrix[1][1], matrix[1][2], matrix[1][3], matrix[2][1], matrix[2][2], matrix[2][3],
                                  matrix[3][1], matrix[3][2], matrix[3][3]
                              ) -
               matrix[0][1] * matr3Det(
                                  matrix[1][0], matrix[1][2], matrix[1][3], matrix[2][0], matrix[2][2], matrix[2][3],
                                  matrix[3][0], matrix[3][2], matrix[3][3]
                              ) +
               matrix[0][2] * matr3Det(
                                  matrix[1][0], matrix[1][1], matrix[1][3], matrix[2][0], matrix[2][1], matrix[2][3],
                                  matrix[3][0], matrix[3][1], matrix[3][3]
                              ) -
               matrix[0][3] * matr3Det(
                                  matrix[1][0], matrix[1][1], matrix[1][2], matrix[2][0], matrix[2][1], matrix[2][2],
                                  matrix[3][0], matrix[3][1], matrix[3][2]
                              );
    }  // End of 'det' function

    /* Assignment operator function.
     * ARGUMENTS:
     *   - other matrix:
     *       const matr4 &other;
     * RETURNS:
     *   (matr4 &) - itself.
     */
    inline matr4 &operator=(const matr4 &other) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                matrix[i][j] = other.matrix[i][j];
        return *this;
    }  // End of 'operator=' function

    /* Multiplication of two matrices function.
     * ARGUMENTS:
     *   - other matrix:
     *       const matr4 &other;
     * RETURNS:
     *   (matr4) - result of multiplication.
     */
    inline matr4 operator*(const matr4 &other) const {
        matr4 tmp;

        tmp.matrix[0][0] = matrix[0][0];
        tmp.matrix[0][0] *= other.matrix[0][0];
        tmp.matrix[0][0] += matrix[0][1] * other.matrix[1][0];
        tmp.matrix[0][0] += matrix[0][2] * other.matrix[2][0];
        tmp.matrix[0][0] += matrix[0][3] * other.matrix[3][0];
        tmp.matrix[0][1] = matrix[0][0];
        tmp.matrix[0][1] *= other.matrix[0][1];
        tmp.matrix[0][1] += matrix[0][1] * other.matrix[1][1];
        tmp.matrix[0][1] += matrix[0][2] * other.matrix[2][1];
        tmp.matrix[0][1] += matrix[0][3] * other.matrix[3][1];
        tmp.matrix[0][2] = matrix[0][0];
        tmp.matrix[0][2] *= other.matrix[0][2];
        tmp.matrix[0][2] += matrix[0][1] * other.matrix[1][2];
        tmp.matrix[0][2] += matrix[0][2] * other.matrix[2][2];
        tmp.matrix[0][2] += matrix[0][3] * other.matrix[3][2];
        tmp.matrix[0][3] = matrix[0][0];
        tmp.matrix[0][3] *= other.matrix[0][3];
        tmp.matrix[0][3] += matrix[0][1] * other.matrix[1][3];
        tmp.matrix[0][3] += matrix[0][2] * other.matrix[2][3];
        tmp.matrix[0][3] += matrix[0][3] * other.matrix[3][3];

        tmp.matrix[1][0] = matrix[1][0];
        tmp.matrix[1][0] *= other.matrix[0][0];
        tmp.matrix[1][0] += matrix[1][1] * other.matrix[1][0];
        tmp.matrix[1][0] += matrix[1][2] * other.matrix[2][0];
        tmp.matrix[1][0] += matrix[1][3] * other.matrix[3][0];
        tmp.matrix[1][1] = matrix[1][0];
        tmp.matrix[1][1] *= other.matrix[0][1];
        tmp.matrix[1][1] += matrix[1][1] * other.matrix[1][1];
        tmp.matrix[1][1] += matrix[1][2] * other.matrix[2][1];
        tmp.matrix[1][1] += matrix[1][3] * other.matrix[3][1];
        tmp.matrix[1][2] = matrix[1][0];
        tmp.matrix[1][2] *= other.matrix[0][2];
        tmp.matrix[1][2] += matrix[1][1] * other.matrix[1][2];
        tmp.matrix[1][2] += matrix[1][2] * other.matrix[2][2];
        tmp.matrix[1][2] += matrix[1][3] * other.matrix[3][2];
        tmp.matrix[1][3] = matrix[1][0];
        tmp.matrix[1][3] *= other.matrix[0][3];
        tmp.matrix[1][3] += matrix[1][1] * other.matrix[1][3];
        tmp.matrix[1][3] += matrix[1][2] * other.matrix[2][3];
        tmp.matrix[1][3] += matrix[1][3] * other.matrix[3][3];

        tmp.matrix[2][0] = matrix[2][0];
        tmp.matrix[2][0] *= other.matrix[0][0];
        tmp.matrix[2][0] += matrix[2][1] * other.matrix[1][0];
        tmp.matrix[2][0] += matrix[2][2] * other.matrix[2][0];
        tmp.matrix[2][0] += matrix[2][3] * other.matrix[3][0];
        tmp.matrix[2][1] = matrix[2][0];
        tmp.matrix[2][1] *= other.matrix[0][1];
        tmp.matrix[2][1] += matrix[2][1] * other.matrix[1][1];
        tmp.matrix[2][1] += matrix[2][2] * other.matrix[2][1];
        tmp.matrix[2][1] += matrix[2][3] * other.matrix[3][1];
        tmp.matrix[2][2] = matrix[2][0];
        tmp.matrix[2][2] *= other.matrix[0][2];
        tmp.matrix[2][2] += matrix[2][1] * other.matrix[1][2];
        tmp.matrix[2][2] += matrix[2][2] * other.matrix[2][2];
        tmp.matrix[2][2] += matrix[2][3] * other.matrix[3][2];
        tmp.matrix[2][3] = matrix[2][0];
        tmp.matrix[2][3] *= other.matrix[0][3];
        tmp.matrix[2][3] += matrix[2][1] * other.matrix[1][3];
        tmp.matrix[2][3] += matrix[2][2] * other.matrix[2][3];
        tmp.matrix[2][3] += matrix[2][3] * other.matrix[3][3];

        tmp.matrix[3][0] = matrix[3][0];
        tmp.matrix[3][0] *= other.matrix[0][0];
        tmp.matrix[3][0] += matrix[3][1] * other.matrix[1][0];
        tmp.matrix[3][0] += matrix[3][2] * other.matrix[2][0];
        tmp.matrix[3][0] += matrix[3][3] * other.matrix[3][0];
        tmp.matrix[3][1] = matrix[3][0];
        tmp.matrix[3][1] *= other.matrix[0][1];
        tmp.matrix[3][1] += matrix[3][1] * other.matrix[1][1];
        tmp.matrix[3][1] += matrix[3][2] * other.matrix[2][1];
        tmp.matrix[3][1] += matrix[3][3] * other.matrix[3][1];
        tmp.matrix[3][2] = matrix[3][0];
        tmp.matrix[3][2] *= other.matrix[0][2];
        tmp.matrix[3][2] += matrix[3][1] * other.matrix[1][2];
        tmp.matrix[3][2] += matrix[3][2] * other.matrix[2][2];
        tmp.matrix[3][2] += matrix[3][3] * other.matrix[3][2];
        tmp.matrix[3][3] = matrix[3][0];
        tmp.matrix[3][3] *= other.matrix[0][3];
        tmp.matrix[3][3] += matrix[3][1] * other.matrix[1][3];
        tmp.matrix[3][3] += matrix[3][2] * other.matrix[2][3];
        tmp.matrix[3][3] += matrix[3][3] * other.matrix[3][3];

        return tmp;
    }  // End of 'operator*' function

    /* Multiplication of two matrices function.
     * ARGUMENTS:
     *   - other matrix:
     *       const matr4 &other;
     * RETURNS:
     *   (matr4 &) - itself.
     */
    inline matr4 &operator*=(const matr4 &other) {
        *this = *this * other;
        return *this;
    }  // End of 'operator*=' function

    /* Adding of two matrices function.
     * ARGUMENTS:
     *   - other matrix:
     *       const matr4 &other;
     * RETURNS:
     *   (matr4) - result of adding.
     */
    inline matr4 operator+(const matr4 &other) const {
        matr4 tmp;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tmp.matrix[i][j] = matrix[i][j] + other.matrix[i][j];
        return tmp;
    }  // End of 'operator+' function

    /* Getter matrix line function.
     * ARGUMENTS:
     *   - index:
     *       int i;
     * RETURNS:
     *   (const float *) - pointer for matrix line
     */
    inline float* operator[](int i) {
        return matrix[i];
    }

    /* Getter matrix line function.
     * ARGUMENTS:
     *   - index:
     *       int i;
     * RETURNS:
     *   (const float *) - const pointer for matrix line
     */
    inline const float* operator[](int i) const {
        return matrix[i];
    }

    /* Adding of two matrices function.
     * ARGUMENTS:
     *   - other matrix:
     *       const matr4 &other;
     * RETURNS:
     *   (matr4 &) - itself.
     */
    inline matr4 &operator+=(const matr4 &other) {
        *this = *this + other;
        return *this;
    }  // End of 'operator+=' function

    /* Subtraction of two matrices function.
     * ARGUMENTS:
     *   - other matrix:
     *       const matr4 &other;
     * RETURNS:
     *   (matr4) - result of subtraction.
     */
    inline matr4 operator-(const matr4 &other) const {
        matr4 tmp;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tmp.matrix[i][j] = matrix[i][j] - other.matrix[i][j];
        return tmp;
    }  // End of 'operator-' function

    /* Subtraction of two matrices function.
     * ARGUMENTS:
     *   - other matrix:
     *       const matr4 &other;
     * RETURNS:
     *   (matr4 &) - itself.
     */
    inline matr4 &operator-=(const matr4 &other) {
        *this = *this - other;
        return *this;
    }  // End of 'operator-=' function

    /* Matrix unary minus function.
     * ARGUMENTS:
     * RETURNS:
     *   (matr4 &) - result matrix.
     */
    inline matr4 operator-() {
        matr4 res;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                res[i][j] = -matrix[i][j];
        return res;
    }  // End of 'operator-' function

    /* Inverse matrix function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (matr4) - inverse matrix (other).
     */
    inline matr4 inverting() const {
        const float det = this->det();
        if (det != 0)
            return matr4(
                matr3Det(
                    matrix[1][1], matrix[1][2], matrix[1][3], matrix[2][1], matrix[2][2], matrix[2][3], matrix[3][1],
                    matrix[3][2], matrix[3][3]
                ) / det,
                -matr3Det(
                    matrix[0][1], matrix[0][2], matrix[0][3], matrix[2][1], matrix[2][2], matrix[2][3], matrix[3][1],
                    matrix[3][2], matrix[3][3]
                ) / det,
                matr3Det(
                    matrix[0][1], matrix[0][2], matrix[0][3], matrix[1][1], matrix[1][2], matrix[1][3], matrix[3][1],
                    matrix[3][2], matrix[3][3]
                ) / det,
                -matr3Det(
                    matrix[0][1], matrix[0][2], matrix[0][3], matrix[1][1], matrix[1][2], matrix[1][3], matrix[2][1],
                    matrix[2][2], matrix[2][3]
                ) / det,

                -matr3Det(
                    matrix[1][0], matrix[1][2], matrix[1][3], matrix[2][0], matrix[2][2], matrix[2][3], matrix[3][0],
                    matrix[3][2], matrix[3][3]
                ) / det,
                matr3Det(
                    matrix[0][0], matrix[0][2], matrix[0][3], matrix[2][0], matrix[2][2], matrix[2][3], matrix[3][0],
                    matrix[3][2], matrix[3][3]
                ) / det,
                -matr3Det(
                    matrix[0][0], matrix[0][2], matrix[0][3], matrix[1][0], matrix[1][2], matrix[1][3], matrix[3][0],
                    matrix[3][2], matrix[3][3]
                ) / det,
                matr3Det(
                    matrix[0][0], matrix[0][2], matrix[0][3], matrix[1][0], matrix[1][2], matrix[1][3], matrix[2][0],
                    matrix[2][2], matrix[2][3]
                ) / det,

                matr3Det(
                    matrix[1][0], matrix[1][1], matrix[1][3], matrix[2][0], matrix[2][1], matrix[2][3], matrix[3][0],
                    matrix[3][1], matrix[3][3]
                ) / det,
                -matr3Det(
                    matrix[0][0], matrix[0][1], matrix[0][3], matrix[2][0], matrix[2][1], matrix[2][3], matrix[3][0],
                    matrix[3][1], matrix[3][3]
                ) / det,
                matr3Det(
                    matrix[0][0], matrix[0][1], matrix[0][3], matrix[1][0], matrix[1][1], matrix[1][3], matrix[3][0],
                    matrix[3][1], matrix[3][3]
                ) / det,
                -matr3Det(
                    matrix[0][0], matrix[0][1], matrix[0][3], matrix[1][0], matrix[1][1], matrix[1][3], matrix[2][0],
                    matrix[2][1], matrix[2][3]
                ) / det,

                -matr3Det(
                    matrix[1][0], matrix[1][1], matrix[1][2], matrix[2][0], matrix[2][1], matrix[2][2], matrix[3][0],
                    matrix[3][1], matrix[3][2]
                ) / det,
                matr3Det(
                    matrix[0][0], matrix[0][1], matrix[0][2], matrix[2][0], matrix[2][1], matrix[2][2], matrix[3][0],
                    matrix[3][1], matrix[3][2]
                ) / det,
                -matr3Det(
                    matrix[0][0], matrix[0][1], matrix[0][2], matrix[1][0], matrix[1][1], matrix[1][2], matrix[3][0],
                    matrix[3][1], matrix[3][2]
                ) / det,
                matr3Det(
                    matrix[0][0], matrix[0][1], matrix[0][2], matrix[1][0], matrix[1][1], matrix[1][2], matrix[2][0],
                    matrix[2][1], matrix[2][2]
                ) / det
            );
        return matr4();
    }  // End of 'inverting' function

    /* Inverse matrix function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (matr4) - inverse matrix (itself).
     */
    inline matr4 &inverse() {
        *this = this->inverting();
        return *this;
    }  // End of 'inverse' function

    /* Matrix transpose function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (matr4 &) - transposed matrix (itself).
     */
    inline constexpr matr4 &transpose() {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                ::std::swap(matrix[i][j], matrix[j][i]);
        return *this;
    }  // End of 'transpose' function

    /* Matrix transposing function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (matr4) - transposed matrix (other).
     */
    inline matr4 transposing() const {
        return matr4(
            matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0], matrix[0][1], matrix[1][1], matrix[2][1],
            matrix[3][1], matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2], matrix[0][3], matrix[1][3],
            matrix[2][3], matrix[3][3]
        );
    }  // End of 'transposing' function

    /* Matrix identity function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (matr4) - identity matrix.
     */
    inline static matr4 identity() {
        return matr4();
    }  // End of 'identity' function

    /* Get view matrix function.
     * ARGUMENTS:
     *   - camera location:
     *       const vec3 &location;
     *   - camera at point:
     *       const vec3 &at;
     *   - camera close up direction:
     *       const vec3 &up_.
     * RETURNS:
     *   (matr4) - view matrix.
     */
    inline static matr4 getView(const vec3 &location, const vec3 &at, const vec3 &up_) {
        vec3 direction((at - location).normalize()), right((direction % up_).normalize()), up(right % direction);

        return matr4(
            right.x, up.x, -direction.x, 0, right.y, up.y, -direction.y, 0, right.z, up.z, -direction.z, 0,
            -(location & right), -(location & up), location & direction, 1
        );
    }  // End of 'getView' function

    /* Get view matrix function.
     * ARGUMENTS:
     *   - camera location:
     *       const vec3 &location;
     *   - camera direction:
     *       const vec3 &direction;
     *   - camera right vector:
     *       const vec3 &right
     *   - camera up direction:
     *       const vec3 &up.
     * RETURNS:
     *   (matr4) - view matrix.
     */
    inline static matr4 getView(const vec3 &location, const vec3 &direction, const vec3 &right, const vec3 &up) {
        return matr4(
            right.x, up.x, -direction.x, 0, right.y, up.y, -direction.y, 0, right.z, up.z, -direction.z, 0,
            -(location & right), -(location & up), location & direction, 1
        );
    }  // End of 'getView' function

    /* Get orthographic matrix function.
     * ARGUMENTS:
     *   - projection parameters:
     *       const float left, right, bottom, top, near, far.
     * RETURNS:
     *   (matr4) - orthographic matrix.
     */
    inline static matr4 getOrthographic(
        const float left,
        const float right,
        const float bottom,
        const float top,
        const float near,
        const float far
    ) {
        return matr4(
            2 / (right - left), 0, 0, 0, 0, 2 / (top - bottom), 0, 0, 0, 0, -2 / (far - near), 0,
            -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1
        );
    }  // End of 'getOrthographic' function

    /* Get projection matrix function.
     * ARGUMENTS:
     *   - projection parameters:
     *       const float left, right, bottom, top, near, far.
     * RETURNS:
     *   (matr4) - projection matrix.
     */
    inline static matr4 getProjection(
        const float left,
        const float right,
        const float bottom,
        const float top,
        const float near,
        const float far
    ) {
        return matr4(
            2 * near / (right - left), 0, 0, 0, 0, 2 * near / (top - bottom), 0, 0, (right + left) / (right - left),
            (top + bottom) / (top - bottom), -(far + near) / (far - near), -1, 0, 0, -2 * far * near / (far - near), 0
        );
        //        return matr4(
        //            2 * near / (right - left), 0, 0, 0, 0, 2 * near / (top - bottom), 0,
        //            0, (right + left) / (right - left), (top + bottom) / (top - bottom),
        //            -(far) / (far - near), -1, 0, 0, -far * near / (far - near), 0
        //        );
    }  // End of 'getProjection' function

    /* Get scale matrix function.
     * ARGUMENTS:
     *   - scale parameters vector:
     *       const vec3 value;
     * RETURNS:
     *   (matr4) - scale matrix.
     */
    inline static matr4 scale(const vec3 value) {
        return matr4(value.x, 0, 0, 0, 0, value.y, 0, 0, 0, 0, value.z, 0, 0, 0, 0, 1);
    }  // End of 'scale' function

    /* Get translate matrix function.
     * ARGUMENTS:
     *   - translate vector:
     *       const vec3 &value;
     * RETURNS:
     *   (matr4) - translate matrix.
     */
    inline static matr4 translate(const vec3 &value) {
        return matr4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, value.x, value.y, value.z, 1);
    }  // End of 'translate' function

    /* Get rotating matrix by random axis function.
     * ARGUMENTS:
     *   - rotation angle:
     *       float angle;
     *   - rotation vector:
     *       vec3 axis;
     * RETURNS:
     *   (matr4) - rotating matrix by random axis.
     */
    inline static matr4 rotateRad(float angle, vec3 axis) {
        axis.normalize();
        const float sine = sin(angle), cosine = cos(angle), cosine1 = 1 - cosine;

        return matr4(
            cosine + axis.x * axis.x * cosine1, axis.x * axis.y * cosine1 + axis.z * sine,
            axis.x * axis.z * cosine1 - axis.y * sine, 0, axis.y * axis.x * cosine1 - axis.z * sine,
            cosine + axis.y * axis.y * cosine1, axis.y * axis.z * cosine1 + axis.x * sine, 0,
            axis.z * axis.x * cosine1 + axis.y * sine, axis.z * axis.y * cosine1 - axis.x * sine,
            cosine + axis.z * axis.z * cosine1, 0, 0, 0, 0, 1
        );
    }  // End of 'rotateRad' function

    /* Get rotating matrix by random axis function.
     * ARGUMENTS:
     *   - rotation angle:
     *       float angle;
     *   - rotation vector:
     *       vec3 axis;
     * RETURNS:
     *   (matr4) - rotating matrix by random axis.
     */
    inline static matr4 rotate(float angle, vec3 axis) {
        return rotateRad(degree2Radian(angle), axis);
    }  // End of 'rotate' function

    /* Get rotating by y axis matrix function.
     * ARGUMENTS:
     *   - angle:
     *       float angle;
     * RETURNS:
     *   (matr4) - rotating by y axis matrix.
     */
    inline static matr4 rotateYRad(float angle) {
        const float sine(sin(angle)), cosine(cos(angle));
        return matr4(cosine, 0, -sine, 0, 0, 1, 0, 0, sine, 0, cosine, 0, 0, 0, 0, 1);
    }  // End of 'rotateYRad' function

    /* Get rotating by y axis matrix function.
     * ARGUMENTS:
     *   - angle:
     *       float angle;
     * RETURNS:
     *   (matr4) - rotating by y axis matrix.
     */
    inline static matr4 rotateY(float angle) {
        return rotateYRad(degree2Radian(angle));
    }  // End of 'rotateY' function

    /* Get rotating by x axis matrix function.
     * ARGUMENTS:
     *   - angle:
     *       float angle;
     * RETURNS:
     *   (matr4) - rotating by x axis matrix.
     */
    inline static matr4 rotateXRad(float angle) {
        const float sine(sin(angle)), cosine(cos(angle));
        return matr4(1, 0, 0, 0, 0, cosine, sine, 0, 0, -sine, cosine, 0, 0, 0, 0, 1);
    }  // End of 'rotateXRad' function

    /* Get rotating by x axis matrix function.
     * ARGUMENTS:
     *   - angle:
     *       float angle;
     * RETURNS:
     *   (matr4) - rotating by x axis matrix.
     */
    inline static matr4 rotateX(float angle) {
        return rotateXRad(degree2Radian(angle));
    }  // End of 'rotateX' function

    /* Get rotating by z axis matrix function.
     * ARGUMENTS:
     *   - angle:
     *       float angle;
     * RETURNS:
     *   (matr4) - rotating by z axis matrix.
     */
    inline static matr4 rotateZRad(float angle) {
        const float sine(sin(angle)), cosine(cos(angle));
        return matr4(cosine, sine, 0, 0, -sine, cosine, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    }  // End of 'RotateZRad' function

    /* Get rotating by z axis matrix function.
     * ARGUMENTS:
     *   - angle:
     *       float angle;
     * RETURNS:
     *   (matr4) - rotating by z axis matrix.
     */
    inline static matr4 rotateZ(float angle) {
        return rotateZRad(degree2Radian(angle));
    }  // End of 'rotateZ' function

    /* Multiply 3d vector by matrix function.
     * ARGUMENTS:
     *   - vector for multiplying:
     *       const vec3 &point;
     * RETURNS:
     *   (vec3) - multiplication result.
     */
    inline vec3 transformPoint(const vec3 &point) const {
        return vec3(
            point.x * matrix[0][0] + point.y * matrix[1][0] + point.z * matrix[2][0] + matrix[3][0],
            point.x * matrix[0][1] + point.y * matrix[1][1] + point.z * matrix[2][1] + matrix[3][1],
            point.x * matrix[0][2] + point.y * matrix[1][2] + point.z * matrix[2][2] + matrix[3][2]
        );
    }  // End of 'transformPoint' function

    /* Print matrix function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    inline void print() const {
        std::cout << "Matrix: { ";
        for (auto i : matrix) {
            std::cout << "{";
            for (int j = 0; j < 4; j++)
                std::cout << i[j] << (j != 3 ? ", " : "");
            std::cout << "} ";
        }
        std::cout << "}" << std::endl;
    }  // End of 'print' function
};     // End of 'matr4' class
}  // namespace math

#endif  // MATR4_HPP
