#include "scene.hpp"

namespace hse {
// Class constructor
unit::unit() : isInitialized(false) {
}  // End of 'unit::unit' function

// Class default constructor
scene::scene() : enableScene(true) {
}  // End of 'scene' function

/* Initialize scene function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void scene::initScene() {
    for (auto &[unitName, unitInstance] : unitsArray)
        if (!unitInstance->isInitialized)
            unitInstance->initUnit(), unitInstance->isInitialized = true;
}

/* Add unit to the scene function.
 * ARGUMENTS:
 *   - unit name:
 *       ::std::string &&unitName;
 *   - unit instance:
 *       unit &&unitInstance;
 * RETURNS: None.
 */
void scene::addUnit(const ::std::string &unitName, const unit *unitInstance) {
    unitsArray[unitName] = const_cast<unit *>(unitInstance);
    initScene();
}  // End of 'scene::addUnit' function

/* Delete unit from the scene function.
 * ARGUMENTS:
 *   - unit name:
 *       const ::std::string &unitName;
 * RETURNS:
 *   (unit *) - not-owning pointer to deleted unit.
 */
unit *scene::deleteUnitFromScene(const ::std::string &unitName) {
    unit *tmp = unitsArray[unitName];
    unitsArray.erase(unitName);
    return tmp;
}  // End of 'scene::deleteUnitFromScene' function

/* Get scene status function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (bool) - true if scene is enabled, false if not.
 */
bool scene::getSceneStatus() const {
    return enableScene;
}  // End of 'scene::getSceneStatus' function

/* Set scene status function.
 * ARGUMENTS:
 *   - flag indicating that the scene is enabled:
 *       bool enableScene_;
 * RETURNS: None.
 */
void scene::setSceneStatus(bool enableScene_) {
    enableScene = enableScene_;
}  // End of 'scene::setSceneStatus' function

/* Response scene function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void scene::sceneResponse() {
    for (auto &[unitName, unitInstance] : unitsArray)
        unitInstance->responseUnit();
}  // End of 'scene::sceneResponse' function

// Class destructor
scene::~scene() {
    for (auto &[unitName, unitInstance] : unitsArray)
        delete unitInstance;
}  // End of 'scene::~scene' function
}  // namespace hse