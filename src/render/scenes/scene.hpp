#ifndef SCENE_HPP
#define SCENE_HPP

#include "../../def.hpp"

namespace hse {
// Unit class declaration
class unit {
    // Friend classes
    friend class scene;

private:
    bool isInitialized = false;

protected:
    // Class constructor
    explicit unit();

    /* Unit initialization pure-virtual function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual void initUnit() = 0;

    /* Unit response pure-virtual function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual void responseUnit() = 0;

    // Class pure-virtual destructor
    virtual ~unit() = default;
};  // End of 'unit' class

// Scene class declaration
class scene {
private:
    ::std::map<::std::string, unit *> unitsArray;  // Array of
                                                   // all
                                                   // scene's
                                                   // units
    bool enableScene;  // Flag indicating that the scene is enabled
                       // default value is 'true'

public:
    // Class default constructor
    explicit scene();

    /* Initialize scene function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void initScene();

    /* Add unit to the scene function.
     * ARGUMENTS:
     *   - unit name:
     *       const ::std::string &unitName;
     *   - unit instance:
     *       const unit *unitInstance;
     * RETURNS: None.
     */
    void addUnit(const ::std::string &unitName, const unit *unitInstance);

    /* Delete unit from the scene function.
     * ARGUMENTS:
     *   - unit name:
     *       const ::std::string &unitName;
     * RETURNS:
     *   (unit *) - not-owning pointer to deleted unit.
     */
    unit *deleteUnitFromScene(const ::std::string &unitName);

    /* Get scene status function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (bool) - true if scene is enabled, false if not.
     */
    [[nodiscard]] bool getSceneStatus() const;

    /* Set scene status function.
     * ARGUMENTS:
     *   - flag indicating that the scene is enabled:
     *       bool enableScene_;
     * RETURNS: None.
     */
    void setSceneStatus(bool enableScene_);

    /* Response scene function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void sceneResponse();

    // Class destructor
    ~scene();
};  // End of 'scene' class
}  // namespace hse

#endif  // SCENE_HPP
