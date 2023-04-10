#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../../def.hpp"
#include "../math/math.hpp"

// Camera class declaration
class Camera {
    math::vec3 position,               // Camera position
        direction,                     // Camera direction
        right,                         // Camera right direction
        up;                            // Camera up direction
    const float far = 2000, near = 1;  // Values for clipping space
    float projectionSize;              // Projection size
    uint width,                        // Camera view width
        height;                        // Camera view height
    math::matr4 view,                  // View matrix for current camera
        projection,                    // Projection matrix for current camera
        viewProjection;                // VP matrix for current camera

public:
    // Class default constructor
    explicit Camera()
        : position(math::vec3(0)),
          direction(math::vec3(0)),
          right(math::vec3(0)),
          up(math::vec3(0)),
          width(0),
          height(0),
          projectionSize(0.1),
          view(math::matr4::identity()),
          projection(math::matr4::identity()),
          viewProjection(math::matr4::identity()) {
    }  // End of 'Camera' function

    /* Class constructor function.
     * ARGUMENTS:
     *   - camera position:
     *       const vec3 &position_;
     *   - camera direction:
     *       const vec3 &direction_;
     * NOTE: Near and far can be changed in the future.
     */
    explicit Camera(const math::vec3 &position_, const math::vec3 &direction_, const uint width_, const uint height_)
        : width(width_), height(height_), projectionSize(0.1) {
        setProjection();
        setPositionWithDirection(position_, direction_);
    }  // End of 'Camera' function

    explicit Camera(
        const math::vec3 &position_,
        const math::vec3 &direction_,
        const math::vec3 &right_,
        const math::vec3 &up_,
        const uint width_,
        const uint height_
    )
        : position(position_),
          direction(direction_),
          right(right_),
          up(up_),
          width(width_),
          height(height_),
          projectionSize(0.1) {
        setProjection();
        view = math::matr4::getView(position, direction, right, up);
        viewProjection = view * projection;
    }  // End of 'Camera' function

    /* Set projection matrix function. (when window sizes have been changed)
     * ARGUMENTS:
     *   - new camera view width:
     *       const uint &width_;
     *   - new camera view height:
     *       const uint &height_;
     * RETURNS: None.
     */
    void setProjection(const uint &width_, const uint &height_) {
        width = width_;
        height = height_;
        setProjection();
    }  // End of 'setProjection' function

    /* Set projection matrix function. (when window sizes have been changed)
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void setProjection() {
        const float projectionWidth =
                        std::max(1.0f, static_cast<float>(width) / static_cast<float>(height)) * projectionSize,
                    projectionHeight =
                        std::max(1.0f, static_cast<float>(height) / static_cast<float>(width)) * projectionSize;
        projection = math::matr4::getProjection(
            -projectionWidth / 2, projectionWidth / 2, -projectionHeight / 2, projectionHeight / 2, projectionSize, far
        );
        viewProjection = view * projection;
    }  // End of 'setProjection' function

    /* Re-set view and view projection matrix function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void setView() {
        view = math::matr4::getView(position, direction, right, up);
        viewProjection = view * projection;
    }  // End of 'setView' function

    /* Set view matrix function.
     * ARGUMENTS:
     *   - camera transform matrix:
     *       const math::matr4 &transformMatrix;
     * RETURNS: None;
     * NOTE: This method is the BEST method for moving/changing camera axis.
     */
    void setView(const math::matr4 &transformMatrix) {
        position = transformMatrix.transformPoint(position);
        direction = transformMatrix.transformPoint(direction);
        right = transformMatrix.transformPoint(right);
        up = transformMatrix.transformPoint(up);
        setView();
    }  // End of 'setView' function

    /* Set view matrix function.
     * ARGUMENTS:
     *   - new camera position:
     *       const vec3 &position_;
     *   - new look at direction:
     *       const vec3 &at_;
     *   - close up direction:
     *       const vec3 &up_;
     * RETURNS: None.
     */
    void setPositionWithAtDirection(const math::vec3 &position_, const math::vec3 &at_, const math::vec3 &up_) {
        position = position_;
        direction = (at_ + math::vec3(0.00001) - position).normalize();
        if (up_ != math::vec3(0))
            up = ((direction % up) % direction).normalize();
        else
            up = (right % direction).normalize();
        right = (direction % up).normalize();
        setView();
    }  // End of 'setPositionWithAtDirection' function

    /* Set view matrix function.
     * ARGUMENTS:
     *   - new camera direction:
     *       const math::vec3 &direction_;
     *   - new up camera direction:
     *       const math::vec3 &up_;
     * RETURNS: None.
     */
    void setDirectionWithUp(const math::vec3 &direction_, const math::vec3 &up_) {
        direction = direction_.normalizing();
        up = up_.normalizing();
        right = (direction % up).normalize();
        setView();
    }  // End of 'setDirectionWithUp' function

    /* Set view matrix function.
     * ARGUMENTS:
     *   - new camera direction:
     *       const math::vec3 &direction_;
     *   - new right camera direction:
     *       const math::vec3 &right_;
     * RETURNS: None.
     */
    void setDirectionWithRight(const math::vec3 &direction_, const math::vec3 &right_) {
        direction = direction_.normalizing();
        right = right_.normalizing();
        up = (right % direction).normalize();
        setView();
    }  // End of 'setDirectionWithRight' function

    /* Set view matrix function.
     * ARGUMENTS:
     *   - new up camera direction:
     *       const math::vec3 &up_;
     *   - new right camera direction:
     *       const math::vec3 &right_;
     * RETURNS: None.
     */
    void setUpWithRight(const math::vec3 &up_, const math::vec3 &right_) {
        if ((up_ & right_) != 0) return;
        right = right_.normalizing();
        up = up.normalize();
        direction = (up % right).normalize();
        setView();
    }  // End of 'setDirectionWithRight' function

    /* Set view matrix function.
     * ARGUMENTS:
     *   - new camera position:
     *       const math::vec3 &position_;
     *   - new right camera vector:
     *       const math::vec3 &right_;
     * RETURNS: None.
     */
    void setPositionWithRight(const math::vec3 &position_, const math::vec3 &right_) {
        position = position_;
        right = right_.normalizing();
        up = (right % direction).normalize();
        setView();
    }  // End of 'setPositionWithRight' function

    /* Set view matrix function.
     * ARGUMENTS:
     *   - new camera position:
     *       const vec3 &position_;
     *   - new camera direction:
     *       const vec3 &direction_;
     * RETURNS: None.
     */
    void setPositionWithDirection(const math::vec3 &position_, const math::vec3 &direction_) {
        position = position_;
        direction = direction_.normalizing();
        if (up != math::vec3(0))
            up = (((direction + ((direction % right).normalize() / 1000)) % up) % direction).normalize();
        else
            up = (right % direction).normalize();
        right = (direction % up).normalize();
        setView();
    }  // End of 'setPositionWithDirection' function

    /* Set view matrix function.
     * ARGUMENTS:
     *   - new camera position:
     *       const vec3 &position_;
     * RETURNS: None.
     */
    void setPosition(const math::vec3 &position_) {
        setPositionWithDirection(position_, direction);
    }  // End of 'setPosition' function

    /* Set view matrix function.
     * ARGUMENTS:
     *   - new camera position:
     *       const math::vec3 &position_;
     *   - new camera direction:
     *       const math::vec3 &direction_;
     *   - new camera up direction:
     *       const math::vec3 &up_;
     *   - new camera right direction:
     *       const math::vec3 &right_;
     * RETURNS: None.
     */
    void setAllAxis(
        const math::vec3 &position_,
        const math::vec3 &direction_,
        const math::vec3 &up_,
        const math::vec3 &right_
    ) {
        position = position_;
        direction = direction_.normalizing();
        up = up_.normalizing();
        right = right_.normalizing();
        setView();
    }  // End of 'setAllAxis' function

    /* Set view matrix function.
     * ARGUMENTS:
     *   - new camera position:
     *       const math::vec3 &position_;
     *   - new camera at direction:
     *       const math::vec3 &at_;
     *   - new camera up direction:
     *       const math::vec3 &up_;
     *   - new camera right direction:
     *       const math::vec3 &right_;
     * RETURNS: None.
     */
    void setAllAxisWithAtDirection(
        const math::vec3 &position_,
        const math::vec3 &at_,
        const math::vec3 &up_,
        const math::vec3 &right_
    ) {
        position = position_;
        direction = (at_ - position).normalize();
        up = up_.normalizing();
        right = right_.normalizing();
        setView();
    }  // End of 'setAllAxisWithAtDirection' function

    /* Get camera position function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (math::vec3) - camera position.
     */
    math::vec3 getPosition() const {
        return position;
    }  // End of 'getPosition' function

    /* Get camera direction function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (math::vec3) - camera direction.
     */
    math::vec3 getDirection() const {
        return direction;
    }  // End of 'getDirection' function

    /* Get camera right function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (math::vec3) - camera right vector.
     */
    math::vec3 getRight() const {
        return right;
    }  // End of 'getRight' function

    /* Get camera up function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (math::vec3) - camera up vector.
     */
    math::vec3 getUp() const {
        return up;
    }  // End of 'getUp' function

    /* Get camera view projection matrix function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (math::matr4) - get view projection matrix.
     */
    math::matr4 getViewProjection() const {
        return viewProjection;
    }  // End of 'getViewProjection' function

    // Class destructor
    ~Camera() {
    }  // End of '~Camera' function
};     // End of 'Camera' class

#endif  // CAMERA_HPP
