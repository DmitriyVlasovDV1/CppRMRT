#ifndef __MTH_H_
#define __MTH_H_

#include <cstring>


/* Math namespace */
namespace mth {
static long double MultiplierDegree2Radian = 0.01745329251994329576L; // degree to radian conversion multiplier

static long double Pi = 3.14159265358979323846L; // pi constant

static long double E = 2.71828182845904523536L; // e constant

/* 3D space vector class declaration */
class vec {
    friend class matr;

private:
    float X, Y, Z; // vector components

public:
    /* Class default constructor */
    explicit vec() {
    } /* End of 'vec' function */

    /* Class constructor.
       * ARGUMENTS:
       *   - vector coordinates:
       *       float A, B, C;
     */
    vec(float A, float B, float C) : X(A), Y(B), Z(C) {
    } /* End of 'vec' function */

    /* Class constructor.
       * ARGUMENTS:
       *   - vector all coordinates value:
       *       float A;
     */
    vec(float A) : X(A), Y(A), Z(A) {
    } /* End of 'vec' function */

    /* Class copying constructor.
       * ARGUMENTS:
       *   - vector to be copied:
       *       const vec &V;
     */
    vec(const vec &V) : X(V.X), Y(V.Y), Z(V.Z) {
    } /* End of 'vec' function */

    /* Sum of two vectors function.
       * ARGUMENTS:
       *   - vector to be add:
       *       const vec &V;
       * RETURNS:
       *   (vec) - result of addition.
     */
    vec operator+(const vec &V) const {
        return vec(X + V.X, Y + V.Y, Z + V.Z);
    } /* End of 'operator+' function */

    /* Add vector function.
       * ARGUMENTS:
       *   - vector to be add:
       *       const vec &V;
       * RETURNS:
       *   (vec &) - self reference.
     */
    vec &operator+=(const vec &V) {
        X += V.X;
        Y += V.Y;
        Z += V.Z;
        return *this;
    } /* End of 'operator+=' function */

    /* Subtract two vectors function.
       * ARGUMENTS:
       *   - vector to be subtract:
       *       const vec &V;
       * RETURNS:
       *   (vec) - result of subtract.
     */
    vec operator-(const vec &V) const {
        return vec(X - V.X, Y - V.Y, Z - V.Z);
    } /* End of 'operator-' function */

    /* Subtract vector function.
       * ARGUMENTS:
       *   - vector to be subtract:
       *       const vec &V;
       * RETURNS:
       *   (vec &) - self reference.
     */
    vec &operator-=(const vec &V) {
        X -= V.X;
        Y -= V.Y;
        Z -= V.Z;
        return *this;
    } /* End of 'operator-=' function */

    /* Multiply vector by number function.
       * ARGUMENTS:
       *   - number to be multiply:
       *       float N;
       * RETURNS:
       *   (vec) - result of multiplication.
     */
    vec operator*(float N) const {
        return vec(X * N, Y * N, Z * N);
    } /* End of 'operator*' function */

    /* Multiply vector by number function.
       * ARGUMENTS:
       *   - number to be multiply:
       *       float N;
       * RETURNS:
       *   (vec &) - self reference.
     */
    vec &operator*=(float N) {
        X *= N;
        Y *= N;
        Z *= N;
        return *this;
    } /* End of 'operator*=' function */

    /* Divide vector by number function.
       * ARGUMENTS:
       *   - number to be divide:
       *       float N;
       * RETURNS:
       *   (vec) - result of division.
     */
    vec operator/(float N) const {
        return vec(X / N, Y / N, Z / N);
    } /* End of 'operator/' function */

    /* Divide vector by number function.
       * ARGUMENTS:
       *   - number to be divide:
       *       float N;
       * RETURNS:
       *   (vec &) - self reference.
     */
    vec &operator/=(float N) {
        X /= N;
        Y /= N;
        Z /= N;
        return *this;
    } /* End of 'operator/=' function */

    /* Negate vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec) - result of negate.
     */
    vec operator-() const {
        return vec(-X, -Y, -Z);
    } /* End of 'operator-' function */

    /* Dot product of two vectors function.
       * ARGUMENTS:
       *   - vector to be producted:
       *       const vec &V;
       * RETURNS:
       *   (float) - result of dot product.
     */
    float operator&(const vec &V) const {
        return X * V.X + Y * V.Y + Z * V.Z;
    } /* End of 'operator&' function */

    /* Cross product of two vectors function.
       * ARGUMENTS:
       *   - vector to be producted:
       *       const vec &V;
       * RETURNS:
       *   (vec) - result of cross product.
     */
    vec operator%(const vec &V) const {
        return vec(Y * V.Z - Z * V.Y, Z * V.X - X * V.Z, X * V.Y - Y * V.X);
    } /* End of 'operator%' function */

    /* Vector coordinate access by number function.
       * ARGUMENTS:
       *   - coordinate number (0 - 'x', 1 - 'y', 2 - 'z'):
       *       int ind;
       * RETURNS:
       *   (float &) - reference to coordinate.
     */
    float &operator[](int ind) {
        assert(ind >= 0 && ind < 3);
        return *(&X + ind);
    } /* End of 'operator[]' function */

    /* Vector coordinate constant value obtain by number function.
       * ARGUMENTS:
       *   - coordinate number (0 - 'x', 1 - 'y', 2 - 'z'):
       *       int ind;
       * RETURNS:
       *   (float) - coordinate value.
     */
    float operator[](int ind) const {
        assert(ind >= 0 && ind < 3);
        return *(&X + ind);
    } /* End of 'operator[]' function */

    /* Normalizing vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec) - result of normalize.
     */
    vec Normalizing() const {
        float len = *this & *this;
        if (len != 0 && len != 1) {
            len = sqrt(len);
            return vec(X / len, Y / len, Z / len);
        }
        return *this;
    } /* End of 'Normalizing' function */

    /* Normalize self vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec &) - self reference.
     */
    vec &Normalize() {
        float len = *this & *this;
        if (len != 0 && len != 1) {
            len = sqrt(len);
            X /= len;
            Y /= len;
            Z /= len;
        }
        return *this;
    } /* End of 'Normalize' function */
};

/* Matrix 4x4 class declaration */
class alignas(16) matr {
public:
    alignas(16) float A[4][4]; // matrix elements


    /* Class default constructor */
    matr() {
    } /* End of 'matr' function */

    /* Class constructor.
       * ARGUMENTS:
       *   - matrix elements:
       *       float A00, A01, A02, A03,
       *             A10, A11, A12, A13,
       *             A20, A21, A22, A23,
       *             A30, A31, A32, A33;
     */
    matr( float A00, float A01, float A02, float A03,
         float A10, float A11, float A12, float A13,
         float A20, float A21, float A22, float A23,
         float A30, float A31, float A32, float A33 ) {
        A[0][0] = A00; A[0][1] = A01; A[0][2] = A02; A[0][3] = A03;
        A[1][0] = A10; A[1][1] = A11; A[1][2] = A12; A[1][3] = A13;
        A[2][0] = A20; A[2][1] = A21; A[2][2] = A22; A[2][3] = A23;
        A[3][0] = A30; A[3][1] = A31; A[3][2] = A32; A[3][3] = A33;
    } /* End of 'matr' function */

    /* Class constructor.
       * ARGUMENTS:
       *   - matrix elements:
       *       float R[4][4];
     */
    matr( float R[4][4] ) {
        memcpy(A, R, sizeof(A));
    } /* End of 'matr' function */

    /* Class copying constructor.
       * ARGUMENTS:
       *   - matrix to be copied:
       *       const matr &M;
     */
    matr( const matr &M ) {
        memcpy(A, M.A, sizeof(A));
    } /* End of 'matr' function */

    /* Identity matrix initialization function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (matr) - identity matrix.
     */
    static matr Identity() {
        return matr(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
    } /* End of 'Translate' function */

    /* Cast matrix to first element pointer function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (float *) - pointer to the (0; 0) element.
     */
    operator float *() {
        return A[0];
    } /* End of 'operator Type *' function */

    /* Ortho matrix initialization function.
       * ARGUMENTS:
       *   - matrix parameters:
       *       const float top, bottom, right, left, near, front;
       * RETURNS:
       *   (matr) - ortho matrix.
     */
    static matr Ortho( const float top, const float bottom, const float right, const float left, const float near_, const float front ) {
        return matr(2 / (right - left), 0, 0, -(right + left) / (right - left),
                    0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
                    0, 0, -2 / (front - near_), -(front + near_) / (front - near_),
                    0, 0, 0, 1);
    } /* End of 'Translate' function */

    /* Translation matrix initialization function.
       * ARGUMENTS:
       *   - delta coordinates values:
       *       const float Dx, Dy, Dz;
       * RETURNS:
       *   (matr) - translate matrix.
     */
    static matr Translate( const float Dx, const float Dy, const float Dz ) {
        return matr(1,  0,  0,  0,
                    0,  1,  0,  0,
                    0,  0,  1,  0,
                    Dx, Dy, Dz, 1);
    } /* End of 'Translate' function */

    /* Scale matrix initialization function.
       * ARGUMENTS:
       *   - coordinates scale values:
       *       const float Sx, Sy, Sz;
       * RETURNS:
       *   (matr) - scale matrix.
     */
    static matr Scale( const float Sx, const float Sy, const float Sz ) {
        return matr(Sx, 0,  0,  0,
                    0,  Sy, 0,  0,
                    0,  0,  Sz, 0,
                    0,  0,  0,  1);
    } /* End of 'Scale' function */

    /* Rotate around arbitrary vector matrix initialization function.
       * ARGUMENTS:
       *   - rotation angle in degree:
       *       const float AngleInDegree;
       *   - vector rotate around:
       *       const vec &V;
       * RETURNS:
       *   (matr) - rotate matrix.
     */
    static matr Rotate( const float AngleInDegree, const vec &V ) {
        float a = static_cast<float>(MultiplierDegree2Radian) * AngleInDegree;
        float sine = sin(a), cosine = cos(a);

        float len = V & V, X, Y, Z;
        if (len != 0 && len != 1)
            len = sqrt(len), X = V.X / len, Y = V.Y / len, Z = V.Z / len;
        else
            X = V.X, Y = V.Y, Z = V.Z;
        X *= sine;
        Y *= sine;
        Z *= sine;
        return matr(1 - 2 * (Y * Y + Z * Z),    2 * X * Y - 2 * cosine * Z, 2 * cosine * Y + 2 * X * Z, 0,
                    2 * X * Y + 2 * cosine * Z, 1 - 2 * (X * X + Z * Z),    2 * Y * Z - 2 * cosine * X, 0,
                    2 * X * Z - 2 * cosine * Y, 2 * cosine * X + 2 * Y * Z, 1 - 2 * (X * X + Y * Y),    0,
                    0,                          0,                          0,                          1);
    } /* End of 'Rotate' function */

    /* Rotate around 'X' axis matrix initialization function.
       * ARGUMENTS:
       *   - rotation angle in degree:
       *       const float AngleInDegree;
       * RETURNS:
       *   (matr) - rotate matrix.
     */
    static matr RotateX( const float AngleInDegree ) {
        float a = static_cast<float>(MultiplierDegree2Radian) * AngleInDegree;
        float sine = sin(a), cosine = cos(a);
        return matr(1,      0,      0,  0,
                    0, cosine,   sine,  0,
                    0,  -sine, cosine,  0,
                    0,      0,      0,  1);
    } /* End of 'RotateX' function */

    /* Rotate around 'Y' axis matrix initialization function.
       * ARGUMENTS:
       *   - rotation angle in degree:
       *       const float AngleInDegree;
       * RETURNS:
       *   (matr) - rotate matrix.
     */
    static matr RotateY( const float AngleInDegree ) {
        float a = static_cast<float>(MultiplierDegree2Radian) * AngleInDegree;
        float sine = sin(a), cosine = cos(a);
        return matr(cosine, 0,  -sine,  0,
                    0, 1,      0,  0,
                    sine, 0, cosine,  0,
                    0, 0,      0,  1);
    } /* End of 'RotateY' function */

    /* Rotate around 'Z' axis matrix initialization function.
       * ARGUMENTS:
       *   - rotation angle in degree:
       *       float AngleInDegree;
       * RETURNS:
       *   (matr) - rotate matrix.
     */
    static matr RotateZ( const float AngleInDegree ) {
        float a = static_cast<float>(MultiplierDegree2Radian) * AngleInDegree;
        float sine = sin(a), cosine = cos(a);
        return matr(cosine,   sine,  0,  0,
                    -sine, cosine,  0,  0,
                    0,      0,  1,  0,
                    0,      0,  0,  1);
    } /* End of 'RotateZ' function */

    /* View matrix initialization function.
       * ARGUMENTS:
       *   - view camera location:
       *       const vec &Loc;
       *   - look-at point:
       *       const vec &LookAt;
       *   - approximate up direction:
       *       const vec &Up1;
       * RETURNS:
       *   (matr) - view matrix.
     */
    static matr ViewLookAt( const vec &Loc, const vec &LookAt, const vec &Up1 ) {
        vec
            Dir = (LookAt - Loc).Normalizing(),
            Right = (Dir % Up1).Normalizing(),
            Up = Right % Dir;

        return matr(Right.X, Up.X, -Dir.X, 0,
                    Right.Y, Up.Y, -Dir.Y, 0,
                    Right.Z, Up.Z, -Dir.Z, 0,
                    -(Right & Loc), -(Up & Loc), Dir & Loc, 1);
    } /* End of 'ViewLookAt' function */

    /* Perspective projection matrix initialization function.
       * ARGUMENTS:
       *   - 3D frustum bound coordinates:
       *       const float Left, Right, Bottom, Top, Near, Far;
       * RETURNS:
       *   (matr) - perspective projection matrix.
     */
    static matr Frustum( const float Left, const float Right,
                        const float Bottom, const float Top,
                        const float Near, const float Far ) {
        return matr(2 * Near / (Right - Left), 0, 0, 0,
                    0, 2 * Near / (Top - Bottom), 0, 0,
                    (Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom), -(Far + Near) / (Far - Near), -1,
                    0, 0, -2 * Near * Far / (Far - Near),  0);
    } /* End of 'Frustum' function */

    /* Multiplication of two matrix function.
       * ARGUMENTS:
       *   - matrix to be multiply:
       *       const matr &M;
       * RETURNS:
       *   (matr) - result of multiplication.
     */
    matr operator*( const matr &M ) const {
        float R[4][4] = {0};

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                for (int k = 0; k < 4; k++)
                    R[i][j] += A[i][k] * M.A[k][j];
        return matr(R);
    } /* End of 'operator*' function */

    /* Additional determinant 3x3 matrix calculation function.
       * ARGUMENTS:
       *   - matrix for determinant obtain:
       *       const float A11, A12, A13,
       *                   A21, A22, A23,
       *                   A31, A32, A33;
       * RETURNS:
       *   (float) - matrix determinant.
     */
    static float Determ3x3( const float A11, const float A12, const float A13,
                           const float A21, const float A22, const float A23,
                           const float A31, const float A32, const float A33 ) {
        return A11 * A22 * A33 - A11 * A23 * A32 - A12 * A21 * A33 +
               A12 * A23 * A31 + A13 * A21 * A32 - A13 * A22 * A31;
    } /* End of 'Determ3x3' function */

    /* Matrix 4x4 determinant calculation function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (float) - matrix determinant.
     */
    float Determ() const {
        return
            A[0][0] * Determ3x3(A[1][1], A[1][2], A[1][3],
                                A[2][1], A[2][2], A[2][3],
                                A[3][1], A[3][2], A[3][3]) -
            A[0][1] * Determ3x3(A[1][0], A[1][2], A[1][3],
                                A[2][0], A[2][2], A[2][3],
                                A[3][0], A[3][2], A[3][3]) +
            A[0][2] * Determ3x3(A[1][0], A[1][1], A[1][3],
                                A[2][0], A[2][1], A[2][3],
                                A[3][0], A[3][1], A[3][3]) -
            A[0][3] * Determ3x3(A[1][0], A[1][1], A[1][2],
                                A[2][0], A[2][1], A[2][2],
                                A[3][0], A[3][1], A[3][2]);
    } /* End of 'Determ' function */

    /* Inverse matrix calculation function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (matr) - self reference.
     */
    matr EvalInverse() const {
        const float det = Determ();
        matr InverseA;

        if (det == 0) {
        }

        InverseA.A[0][0] =
            Determ3x3(A[1][1], A[1][2], A[1][3],
                      A[2][1], A[2][2], A[2][3],
                      A[3][1], A[3][2], A[3][3]) / det;
        InverseA.A[1][0] =
            -Determ3x3(A[1][0], A[1][2], A[1][3],
                       A[2][0], A[2][2], A[2][3],
                       A[3][0], A[3][2], A[3][3]) / det;
        InverseA.A[2][0] =
            Determ3x3(A[1][0], A[1][1], A[1][3],
                      A[2][0], A[2][1], A[2][3],
                      A[3][0], A[3][1], A[3][3]) / det;
        InverseA.A[3][0] =
            -Determ3x3(A[1][0], A[1][1], A[1][2],
                       A[2][0], A[2][1], A[2][2],
                       A[3][0], A[3][1], A[3][2]) / det;

        InverseA.A[0][1] =
            -Determ3x3(A[0][1], A[0][2], A[0][3],
                       A[2][1], A[2][2], A[2][3],
                       A[3][1], A[3][2], A[3][3]) / det;
        InverseA.A[1][1] =
            Determ3x3(A[0][0], A[0][2], A[0][3],
                      A[2][0], A[2][2], A[2][3],
                      A[3][0], A[3][2], A[3][3]) / det;
        InverseA.A[2][1] =
            -Determ3x3(A[0][0], A[0][1], A[0][3],
                       A[2][0], A[2][1], A[2][3],
                       A[3][0], A[3][1], A[3][3]) / det;
        InverseA.A[3][1] =
            Determ3x3(A[0][0], A[0][1], A[0][2],
                      A[2][0], A[2][1], A[2][2],
                      A[3][0], A[3][1], A[3][2]) / det;

        InverseA.A[0][2] =
            Determ3x3(A[0][1], A[0][2], A[0][3],
                      A[1][1], A[1][2], A[1][3],
                      A[3][1], A[3][2], A[3][3]) / det;
        InverseA.A[1][2] =
            -Determ3x3(A[0][0], A[0][2], A[0][3],
                       A[1][0], A[1][2], A[1][3],
                       A[3][0], A[3][2], A[3][3]) / det;
        InverseA.A[2][2] =
            Determ3x3(A[0][0], A[0][1], A[0][3],
                      A[1][0], A[1][1], A[1][3],
                      A[3][0], A[3][1], A[3][3]) / det;
        InverseA.A[3][2] =
            -Determ3x3(A[0][0], A[0][1], A[0][2],
                       A[1][0], A[1][1], A[1][2],
                       A[3][0], A[3][1], A[3][2]) / det;

        InverseA.A[0][3] =
            -Determ3x3(A[0][1], A[0][2], A[0][3],
                       A[1][1], A[1][2], A[1][3],
                       A[2][1], A[2][2], A[2][3]) / det;
        InverseA.A[1][3] =
            Determ3x3(A[0][0], A[0][2], A[0][3],
                      A[1][0], A[1][2], A[1][3],
                      A[2][0], A[2][2], A[2][3]) / det;
        InverseA.A[2][3] =
            -Determ3x3(A[0][0], A[0][1], A[0][3],
                       A[1][0], A[1][1], A[1][3],
                       A[2][0], A[2][1], A[2][3]) / det;
        InverseA.A[3][3] =
            Determ3x3(A[0][0], A[0][1], A[0][2],
                      A[1][0], A[1][1], A[1][2],
                      A[2][0], A[2][1], A[2][2]) / det;

        return InverseA;
    } /* End of 'EvalInverse' function */

    /* Point (x; y; z; 1) affine transformation function.
       * ARGUMENTS:
       *   - space point to be transformed:
       *       const vec &P;
       * RETURNS:
       *   (vec) - transformed point.
     */
    vec TransformPoint( const vec &P ) const {
        return vec(P.X * A[0][0] + P.Y * A[1][0] + P.Z * A[2][0] + A[3][0],
                   P.X * A[0][1] + P.Y * A[1][1] + P.Z * A[2][1] + A[3][1],
                   P.X * A[0][2] + P.Y * A[1][2] + P.Z * A[2][2] + A[3][2]);
    } /* End of 'TransformPoint' function */

    /* Vector (x; y; z; 0) affine transformation function.
       * ARGUMENTS:
       *   - space point to be transformed:
       *       const vec &V;
       * RETURNS:
       *   (vec) - transformed point.
     */
    vec TransformVector( const vec &V ) const {
        return vec(V.X * A[0][0] + V.Y * A[1][0] + V.Z * A[2][0],
                   V.X * A[0][1] + V.Y * A[1][1] + V.Z * A[2][1],
                   V.X * A[0][2] + V.Y * A[1][2] + V.Z * A[2][2]);
    } /* End of 'TransformVector' function */

    /* Point (x; y; z; 1) full transformation function.
       * ARGUMENTS:
       *   - space point to be transformed:
       *       const vec &P;
       * RETURNS:
       *   (vec) - transformed point.
     */
    vec TransformPointPerspective( const vec &P ) const {
        const float w = P.X * A[0][3] + P.Y * A[1][3] + P.Z * A[2][3] + A[3][3];
        return vec((P.X * A[0][0] + P.Y * A[1][0] + P.Z * A[2][0] + A[3][0]) / w,
                   (P.X * A[0][1] + P.Y * A[1][1] + P.Z * A[2][1] + A[3][1]) / w,
                   (P.X * A[0][2] + P.Y * A[1][2] + P.Z * A[2][2] + A[3][2]) / w);
    } /* End of 'TransformPointPerspective' function */

    /* Vector (x; y; z; 0) full transformation function.
       * ARGUMENTS:
       *   - space point to be transformed:
       *       const vec &P;
       * RETURNS:
       *   (vec) - transformed point.
     */
    vec TransformVectorPerspective( const vec &V ) const {
        const float w = V.X * A[0][3] + V.Y * A[1][3] + V.Z * A[2][3];
        return vec((V.X * A[0][0] + V.Y * A[1][0] + V.Z * A[2][0]) / w,
                   (V.X * A[0][1] + V.Y * A[1][1] + V.Z * A[2][1]) / w,
                   (V.X * A[0][2] + V.Y * A[1][2] + V.Z * A[2][2]) / w);
    } /* End of 'TransformVectorPerspective' function */

    /* Vector (x; y; z; 0) transformation function.
       * ARGUMENTS:
       *   - space point to be transformed:
       *       const vec &N;
       * RETURNS:
       *   (vec) - transformed point.
     */
    vec TransformNormal( const vec &N ) const {
        matr InverseA = EvalInverse();
        return vec(N.X * InverseA.A[0][0] + N.Y * InverseA.A[0][1] + N.Z * InverseA.A[0][2],
                   N.X * InverseA.A[1][0] + N.Y * InverseA.A[1][1] + N.Z * InverseA.A[1][2],
                   N.X * InverseA.A[2][0] + N.Y * InverseA.A[2][1] + N.Z * InverseA.A[2][2]);
    } /* End of 'TransformNormal' function */
}; /* End of 'matr' class */

/* Ray class declaration */
class ray {
public:
    vec
        Origin,    // origin of the ray
        Direction; // direction of the ray

    /* Default class constructor */
    explicit ray() : Origin(), Direction() {
    } /* End of 'ray' function */

    /* Class constructor.
       * ARGUMENTS:
       *   - vectors for initialization:
       *       const vec &org, dir
     */
    explicit ray( const vec &org, const vec &dir ) : Origin(org), Direction(dir) {
    } /* End of 'ray' function */
};

/* 3D view camera class declaration */
class camera {
public:
    vec Loc, Dir, Right, Up; // camera location and directions
    vec LookAt;              // saved look at position
    float
        Wp, Hp,                // projection plane size
        ProjDist;              // distance to projection plane

    /* Setup camera position and orientation function.
       * ARGUMENTS:
       *   - camera new location:
       *       const vec &NewLoc;
       *   - camera look-at position:
       *       const vec &NewLookAt;
       *   - camera approximately up direction:
       *       const vec &Up1;
       * RETURNS:
       *   (camera &) - self reference.
     */
    camera & Set( const vec &NewLoc,
                const vec &NewLookAt,
                const vec &Up1 ) {
        Loc = NewLoc;
        Dir = ((LookAt = NewLookAt) - Loc).Normalizing();
        Right = (Dir % Up1).Normalizing();
        Up = Right % Dir;
        return *this;
    } /* End of 'Set' function */

    /* Setup camera projection data function.
       * ARGUMENTS:
       *   - camera new projection plane size:
       *       const float NewWp, NewHp;
       *   - camera new distance to project plane:
       *       const float NewProjDist;
       * RETURNS:
       *   (camera &) - self reference.
     */
    camera & Set( const float NewWp, const float NewHp, const float NewProjDist ) {
        Wp = NewWp;
        Hp = NewHp;
        ProjDist = NewProjDist;
        return *this;
    } /* End of 'Set' function */

    /* Ray from camera and projection plane calculation function.
       * ARGUMENTS:
       *   - frame size in pixels:
       *       const int FrameW, FrameH;
       *   - frame pixel coordinates:
       *       const float FrameX, FrameY;
       * RETURNS:
       *   (ray) - result ray from project plane in specified frame direction.
     */
    ray Apply( const int FrameW, const int FrameH, const int FrameX, const int FrameY ) const {
        vec
            X = Dir * ProjDist +
                Right * ((FrameX - FrameW / 2.0f) / FrameW * Wp) +
                Up * ((FrameH / 2.0f - FrameY) / FrameH * Hp);

        return ray(Loc, (Loc + X).Normalizing());
    } /* End of 'Apply' function */
}; /* End of 'camera' class */
} /* end of 'mth' namespace */

#endif // __MTH_H_