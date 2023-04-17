#ifndef HSE_PROJECT_FIGURE_MATERIAL_HPP
#define HSE_PROJECT_FIGURE_MATERIAL_HPP

/*
 * Materials
 */

struct alignas(16) Material {
    Material() : r(0.5), g(0.5), b(0.5), a(1), is_light_source(0) {
    }
    Material(const math::vec3 &color, bool is_light_source=false) :
        r(color.x), g(color.y), b(color.z), a(1), is_light_source(is_light_source){
    }
    float r, g, b, a;
    int is_light_source;
}; // Material
#endif  // HSE_PROJECT_FIGURE_MATERIAL_HPP
