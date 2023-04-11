#ifndef HSE_PROJECT_FIGURE_MATERIAL_HPP
#define HSE_PROJECT_FIGURE_MATERIAL_HPP

/*
 * Materials
 */

struct Material {
    Material() : r(0.5), g(0.5), b(0.5), a(1) {
    }
    Material(const math::vec3 &color) :
        r(color.x), g(color.y), b(color.z), a(1) {
    }
    float r, g, b, a;
}; // Material
#endif  // HSE_PROJECT_FIGURE_MATERIAL_HPP
