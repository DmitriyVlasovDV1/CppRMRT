#include "def.hpp" // Should be in all *.hpp files (for now it's right information)

// Project namespace
namespace cgsg {
    // Render class declaration
    class render {
    public:
        // Default constructor
        explicit render() noexcept = default;
    }; // End of 'render' class

    // Shader class declaration
    class shader {
    private:
        GLuint shader_id; // shader id

        // Default constructor
        explicit shader() noexcept : shader_id(0) {
        } // End of 'shader' function
    }; // End of 'shader' class

    // Vertex buffer class declaration
    class vertex_buffer {
    private:
        // Default constructor
        explicit vertex_buffer() noexcept = default;
    }; // End of 'vertex_buffer' class

    // Primitive class declaration
    class primitive {
    private:
        // Default constructor
        explicit primitive() noexcept = default;
    }; // End of 'primitive' class

    // Manager class declaration
    class manager : public render {
    private:
        ::std::vector<::std::unique_ptr<primitive>> primiteves; // Storage of all primitives
        ::std::vector<::std::unique_ptr<shader>> shaders;       // Storage of all shaders

        // Default constructor
        explicit manager() noexcept : render() {
        } // End of 'manager' function

    public:
        /* Get instance of manager class function.
         * ARGUMENTS: None.
         * RETURNS:
         *   (manager &) - instance of manager class
         */
        [[nodiscard]] static manager & get_manager() {
            static manager singleton;
            return singleton;
        } // End of 'get_manager' function
    }; // End of 'manager' class
} // End of 'cgsg' namespace

// Main program function
int main() {
    // glfw initialization
    if (!glfwInit())
        assert("error in glfw initialization");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create our window (pointer)
    GLFWwindow * window = glfwCreateWindow(400, 400, "SK4", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        assert("error in glfw window creation");
    }
    // Attach OpenGL context to the created window thread
    glfwMakeContextCurrent(window);

    // Initialization of OpenGL
    if (!glewInit())
        assert("error in glew initialization");

    // Debug information (OpenGL/glsl version)
    ::std::cout << "OpenGL: " << glGetString(GL_VERSION) << "\n";
    ::std::cout << "Shader language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << ::std::endl;

    // Run window response infinity cycle (can be used in threads)
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }
    // Delete window instance
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
} // End of 'main' function
