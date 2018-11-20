#include <GLXW/glxw.h> //Лоадер OpenGL
#include <GLFW/glfw3.h> //Отрисовка контента

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

static const std::string vertexShaderSource = ""
                                              "#version 330 core\n"
                                              "layout (location = 0) in vec3 position; // Устанавливаем позицию атрибута в 0\n"
                                              "uniform mat4 model;\n"
                                              "uniform mat4 view;\n"
                                              "uniform mat4 projection;\n"
                                              "void main() {\n"
                                              "  gl_Position = projection * view * model * vec4(position, 1.0f);\n"
                                              "}";

static const std::string fragmentShaderSource = ""
                                                "#version 330 core\n"
                                                "out vec4 color;\n"
                                                "void main() { color = vec4(0.5f, 0.0f, 0.0f, 1.0f); }\n";

static const GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
};

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


int main() {
    //************************************************************************************
    // GLFW
    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    // Минимальная требуемая версия OpenGL.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Установка профайла для которого создается контекст
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Возможность изменения размера окна
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "CG-Lab-OGL3x", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLXW
    if(glxwInit()) {
        std::cerr << "Failed to init GLXW" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //************************************************************************************
    // SETTINGS SHADERS
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    const GLchar * const vertexShaderSourcePtr = vertexShaderSource.c_str();
    glShaderSource(vertexShaderId, 1, &vertexShaderSourcePtr, nullptr);
    glCompileShader(vertexShaderId);

    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar * const fragmentShaderSourcePtr = fragmentShaderSource.c_str();
    glShaderSource(fragmentShaderId, 1, &fragmentShaderSourcePtr, nullptr);
    glCompileShader(fragmentShaderId);

    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    // SETTINGS BUFFERS
    GLuint VBO; // Объект вершинного буфера
    GLuint VAO; // Объект вершинного массива

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);


    glBindVertexArray(VAO);
    // Копируем массив с вершинами в буфер OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Установим указатели на вершинные атрибуты
    glVertexAttribPointer(
            0, // какой аргумент шейдера хотим настроить
            3, // размер аргумента в шейдере (vec3)
            GL_FLOAT, // используемый тип данных
            GL_FALSE, // необходимость нормализовать входные данные
            3 * sizeof(float), //шаг (описывает расстояние между наборами данных)
            nullptr); // смещение начала данных в буфере

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Игровой цикл
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programId);

        GLfloat radius = 5.0f;
        GLfloat camX = sin(glfwGetTime()) * radius;
        GLfloat camZ = cos(glfwGetTime()) * radius;

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view;
        glm::mat4 projection;

//    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.3f, 0.5f, 0.0f));
        view = glm::lookAt( glm::vec3(camX, 2.0f, camZ), // Положение камеры
                            glm::vec3(0.0f, 0.0f, 0.0f), // Направление камеры (смотрит в точку (0,0,0))
                            glm::vec3(0.0f, 1.0f, 0.0f)); // Камера расположена горизонтально

        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(programId, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window); // Заменяет цветовой буфер
        /* Передний буфер - результирующее изображение
         * Задний буфер - отрисовка.
         * Как только отрисовка будет закончена, эти буферы меняются местами
         * */
        glfwPollEvents(); // Проверка события
    }

    // Очистка ресурсов
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

//-----------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
//-----------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
//-----------------------------------------------------------------------------------