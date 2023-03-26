#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../../../def.hpp"
#include "../math.hpp"

// Math module namespace
namespace math {
// Camera class declaration
class camera {
    vec3 location,                       // Camera location
        direction,                       // Camera direction
        at,                              // Camera at point
        right,                           // Camera right direction
        up;                              // Camera up direction
    const float far = 2000, near = 0.5;  // Values for clipping space
    float projectionSize;                // Projection size
    uint width,                          // Camera view width
        height;                          // Camera view height
    matr4 view,                          // View matrix for current camera
        projection,                      // Projection matrix for current camera
        viewProjection;                  // VP matrix for current camera

public:
    // Class default constructor
    explicit camera()
        : location(vec3(0)),
          direction(vec3(0)),
          right(vec3(0)),
          up(vec3(0)),
          width(0),
          height(0),
          projectionSize(0.1),
          view(matr4::identity()),
          projection(matr4::identity()),
          viewProjection(matr4::identity()) {
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
        const vec3 &location_,
        const vec3 &direction_,
        const uint width_,
        const uint height_
    )
        : width(width_), height(height_), projectionSize(0.1) {
        setProjection();
        setView(location_, direction_);
    }  // End of 'camera' function

    explicit camera(
        const vec3 &location_,
        const vec3 &direction_,
        const vec3 &right_,
        const vec3 &up_,
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
        at = location + direction;
        setProjection();
        view = matr4::getView(location, direction, right, up);
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
                    ),
                    projectionHeight = ::std::max(
                        1.0f,
                        static_cast<float>(height) / static_cast<float>(width)
                    );
        projection = matr4::getProjection(
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
    void setView(const vec3 &location_, const vec3 &at_, const vec3 &up_) {
        location = location_;
        at = at_ + vec3(0.00001);
        direction = (at - location).normalize();
        if (up_ != vec3(0))
            up = (direction % up_) % direction;
        else
            up = (right % direction).normalize();
        right = (direction % up).normalize();

        view = matr4::getView(location, direction, right, up);
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
    void setView(const vec3 &location_, const vec3 &direction_) {
        location = location_;
        at = location + direction_;
        direction = direction_.normalizing();
        if (up != vec3(0))
            up = (direction % up) % direction;
        else
            up = (right % direction).normalize();
        right = (direction % up).normalize();

        view = matr4::getView(location, direction, right, up);
        viewProjection = view * projection;
    }  // End of 'setView' function

    /* Moving camera to new location function.
     * ARGUMENTS:
     *   - new location:
     *       const vec3 &location_;
     * RETURNS: None.
     */
    void moveTo(const vec3 &location_) {
        at += location_ - location;
        location = location_;
        view = matr4::getView(location, direction, right, up);
        viewProjection = view * projection;
    }  // End of 'moveTo' function

    /* Get camera location function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    vec3 getLocation() const {
        return location;
    }  // End of 'getLocation' function

    /* Get camera direction function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    vec3 getDirection() const {
        return direction;
    }  // End of 'getDirection' function

    /* Get camera right function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    vec3 getRight() const {
        return right;
    }  // End of 'getRight' function

    /* Get camera up function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    vec3 getUp() const {
        return up;
    }  // End of 'getUp' function

    /* Get camera view projection matrix function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    matr4 getViewProjection() const {
        return viewProjection;
    }  // End of 'getViewProjection' function

    // Class destructor
    ~camera() {
        ::std::cout << "Clear camera" << ::std::endl;
    }  // End of '~camera' function
};     // End of 'camera' class
}  // namespace math

#endif  // CAMERA_HPP
