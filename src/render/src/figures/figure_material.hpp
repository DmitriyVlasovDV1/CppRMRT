#ifndef HSE_PROJECT_FIGURE_MATERIAL_HPP
#define HSE_PROJECT_FIGURE_MATERIAL_HPP

/*
 * Materials
 */
class MaterialId {
public:
    MaterialId(int id) : m_id(id) {
    }
private:
    const int m_id;
}; // MaterialId

class Material {
    math::vec3 m_color;
}; // Material
#endif  // HSE_PROJECT_FIGURE_MATERIAL_HPP
