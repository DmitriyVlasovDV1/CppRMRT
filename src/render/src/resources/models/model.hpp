#ifndef MODEL_HPP
#define MODEL_HPP

#include "../../../../def.hpp"
#include "../primitives/primitive.hpp"

// Project namespace
namespace hse {
// Model class declaration
class Model final : public Primitive {
    // Friend classes
    friend class Scene;

    std::vector<std::unique_ptr<Primitive>> primitivesArray;  // Array of all model's primitives

    // Material struct definition
    struct Material {
        math::vec3 color;
    }; // End of 'Material' struct

    /* Parse *.obj file function.
     * ARGUMENTS:
     *   - model's file name:
     *       const std::string &fileName;
     * RETURNS: None.
     */
    void parseObj(const std::string &fileName);

public:
    /* Class constructor.
     * ARGUMENTS:
     *   - model's shader program id:
     *       uint shaderProgramId_;
     *   - model's file name:
     *       const std::string &fileName.
     */
    explicit Model(uint shaderProgramId_, const std::string &fileName);

    /* Render model function.
     * ARGUMENTS:
     *   - camera for rendering model:
     *      const Camera &camera;
     * RETURNS: None;
     * NOTE: Don't use it - better to use scene's interface.
     */
    void onRender(const Camera &camera) const override;

    // Class destructor
    ~Model();

    /* Set primitive rendering type function.
     * ARGUMENTS:
     *   - new rendering type:
     *       VertexArray::renderType renderType_:
     * RETURNS: None.
     */
    void setRenderType(VertexArray::renderType renderType_) final;

    /* Get number of primitives in model function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (int) - number of primitives in model.
     */
    int getNumberOfChildren() const;

    /* Get primitive's pointer by index in model function.
     * ARGUMENTS:
     *   - index of the primitive:
     *       int index;
     * RETURNS:
     *   (Primitive *) - not-owning pointer to the primitive.
     */
    Primitive *getChild(int index) const;
};  // End of 'Model' class
}  // namespace hse

#endif  // MODEL_HPP
