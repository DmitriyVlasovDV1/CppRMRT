#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../../def.hpp"
#include "../math/math.hpp"

// Camera class declaration
class camera {
    math::vec3 location,               // Camera location
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
    explicit camera()
        : location(math::vec3(0)),
          direction(math::vec3(0)),
          right(math::vec3(0)),
          up(math::vec3(0)),
          width(0),
          height(0),
          projectionSize(0.1),
          view(math::matr4::identity()),
          projection(math::matr4::identity()),
          viewProjection(math::matr4::identity()) {
    }  // End of 'camera' function

    /* Class constructor function.
     * ARGUMENTS:
     *   - camera location:
     *       const vec3 &location_;
     *   - camera direction:
     *       const vec3 &direction_;
     * NOTE: Near and far can be changed in the future.
     */
    explicit camera(
        const math::vec3 &location_,
        const math::vec3 &direction_,
        const uint width_,
        const uint height_
    )
        : width(width_), height(height_), projectionSize(0.1) {
        setProjection();
        setView(location_, direction_);
    }  // End of 'camera' function

    explicit camera(
        const math::vec3 &location_,
        const math::vec3 &direction_,
        const math::vec3 &right_,
        const math::vec3 &up_,
        const uint width_,
        const uint height_
    )
        : location(location_),
          direction(direction_),
          right(right_),
          up(up_),
          width(width_),
          height(height_),
          projectionSize(0.1) {
        setProjection();
        view = math::matr4::getView(location, direction, right, up);
        viewProjection = view * projection;
    }  // End of 'camera' function

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
        const float projectionWidth = ::std::max(
                        1.0f,
                        static_cast<float>(width) / static_cast<float>(height)
                    ) * projectionSize,
                    projectionHeight = ::std::max(
                        1.0f,
                        static_cast<float>(height) / static_cast<float>(width)
                    ) * projectionSize;
        projection = math::matr4::getProjection(
            -projectionWidth / 2, projectionWidth / 2, -projectionHeight / 2,
            projectionHeight / 2, projectionSize, far
        );
        viewProjection = view * projection;
    }  // End of 'setProjection' function

    /* Calculate view matrix function.
     * ARGUMENTS:
     *   - new camera location:
     *       const vec3 &location_;
     *   - new look at direction:
     *       const vec3 &at_;
     *   - close up direction:
     *       const vec3 &up_;
     * RETURNS: None.
     */
    void setView(
        const math::vec3 &location_,
        const math::vec3 &at_,
        const math::vec3 &up_
    ) {
        location = location_;
        direction = (at_ + math::vec3(0.00001) - location).normalize();
        if (up_ != math::vec3(0))
            up = ((direction % up) % direction).normalize();
        else
            up = (right % direction).normalize();
        right = (direction % up).normalize();

        view = math::matr4::getView(location, direction, right, up);
        viewProjection = view * projection;
    }  // End of 'setView' function

    /* Calculate view matrix function.
     * ARGUMENTS:
     *   - new camera location:
     *       const vec3 &location_;
     *   - new camera direction:
     *       const vec3 &direction_;
     * RETURNS: None.
     */
    void setView(const math::vec3 &location_, const math::vec3 &direction_) {
        location = location_;
        direction = direction_.normalizing();
        if (up != math::vec3(0))
            up = ((direction % up) % direction).normalize();
        else
            up = (right % direction).normalize();
        right = (direction % up).normalize();

        view = math::matr4::getView(location, direction, right, up);
        viewProjection = view * projection;
    }  // End of 'setView' function

    /* Moving camera to new location function.
     * ARGUMENTS:
     *   - new location:
     *       const vec3 &location_;
     * RETURNS: None.
     */
    void setLocation(const math::vec3 &location_) {
        setView(location_, direction);
    }  // End of 'setLocation' function

    /* Set camera direction function.
     * ARGUMENTS:
     *   - new camera direction:
     *       const math::vec3 &direction_;
     * RETURNS: None.
     */
    void setDirection(const math::vec3 &direction_) {
        setView(location, direction_);
    }  // End of 'setDirection' function

    /* Get camera location function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (math::vec3) - camera location.
     */
    math::vec3 getLocation() const {
        return location;
    }  // End of 'getLocation' function

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
    ~camera() {
        ::std::cout << "Clear camera" << ::std::endl;
    }  // End of '~camera' function
};     // End of 'camera' class

#endif  // CAMERA_HPP
