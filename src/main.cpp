#include "def.hpp" // Should be in all *.hpp files (for now it's right info)

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

GLuint shader_program, vertex_shader, fragment_shader;

bool check_shader_compile_status(GLuint obj) {
    GLint status;
    glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint length;
        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);
        ::std::vector<char> log(length);
        glGetShaderInfoLog(obj, length, &length, &log[0]);
        ::std::cout << &log[0];
        return false;
    }
    return true;
}

// helper to check and display for shader linker error
bool check_program_link_status(GLuint obj) {
    GLint status;
    glGetProgramiv(obj, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint length;
        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length);
        ::std::vector<char> log(length);
        glGetProgramInfoLog(obj, length, &length, &log[0]);
        ::std::cout << &log[0];
        return false;
    }
    return true;
}

void renderScene() {
    glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_program);
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(-0.5, -0.5, 0.0);
        glVertex3f(0.5, 0.0, 0.0);
        glVertex3f(0.0, 0.5, 0.0);
    }
    glEnd();
    glFlush();
}

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
    GLFWwindow * window = glfwCreateWindow(800, 800, "SK4", nullptr, nullptr);
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

    // shaders realization
    {
        ::std::string vertex_source =
                "#version 330\n"
                "layout(location = 0) in vec4 input_position;\n"
                "// layout(location = 1) in vec4 input_color;\n"
                "out vec4 output_color;\n"
                "void main() {\n"
                "   output_color = vec4(1, 0, 0, 1);\n"
                "   gl_Position = input_position;\n"
                "}\n";

        ::std::string fragment_source =
                "#version 330\n"
                "in vec4 input_color;\n"
                "layout(location = 0) out vec4 output_color;\n"
                "void main() {\n"
                "   output_color = vec4(1, 0, 0, 1);\n"
                "}\n";

        const char *source;
        int length;

        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        source = vertex_source.c_str();
        length = static_cast<int>(vertex_source.size());
        glShaderSource(vertex_shader, 1, &source, &length);
        glCompileShader(vertex_shader);
        if(!check_shader_compile_status(vertex_shader))
            assert("vertex shader failed to compile");

        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        source = fragment_source.c_str();
        length = static_cast<int>(fragment_source.size());
        glShaderSource(fragment_shader, 1, &source, &length);
        glCompileShader(fragment_shader);
        if(!check_shader_compile_status(fragment_shader))
            assert("fragment shader failed to compile");

        shader_program = glCreateProgram();

        glAttachShader(shader_program, vertex_shader);
        glAttachShader(shader_program, fragment_shader);

        glLinkProgram(shader_program);
        if(!check_program_link_status(shader_program))
            assert("program linking failed");
    }

    // Run window response infinity cycle (can be used in threads)
    while (!glfwWindowShouldClose(window)) {
        renderScene();
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }
    // Delete window instance
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
