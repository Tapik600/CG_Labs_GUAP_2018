#include <GLXW/glxw.h> //Лоадер OpenGL
#include <GLFW/glfw3.h> //Отрисовка контента

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "Shader.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
GLfloat RotCount = 0.0f;
//glm::vec3 lightPos(0.0f, 0.0f, 2.0f);

static const GLfloat vertices[] = {
        //   позиция        |    нормаль
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 1.5f,  0.0f, 0.0f),
        glm::vec3(-1.5f,  0.0f, 0.0f)
};
glm::vec3 pointLightPositions[] = {
        glm::vec3( 1.0f,  0.2f,  2.0f),
        glm::vec3( -1.0f, 1.0f,  -2.0f),
        glm::vec3(-1.0f,  -0.2f,  2.0f)
};
glm::vec3 spotLightPosition = glm::vec3( -3.0f,  1.5f,  -1.0f);
glm::vec3 pointLightColors[] = {
        glm::vec3(1.0f,  0.0f,  0.0f),
        glm::vec3(0.0f,  1.0f,  0.0f),
        glm::vec3(0.0f,  0.0f,  1.0f)
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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CG-Lab-OGL3x", nullptr, nullptr);
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
    Shader Program{};
    Program.loadShader("vShader.glsl", GL_VERTEX_SHADER);
    Program.loadShader("fShader.glsl", GL_FRAGMENT_SHADER);
    Program.linkShaders();

    Shader LampShader{};
    LampShader.loadShader("vLamp.glsl", GL_VERTEX_SHADER);
    LampShader.loadShader("fLamp.glsl", GL_FRAGMENT_SHADER);
    LampShader.linkShaders();

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
            6 * sizeof(float), //шаг (описывает расстояние между наборами данных)
            nullptr); // смещение начала данных в буфере
    glEnableVertexAttribArray(0);

    // Устанавливаем указатели на вектор нормали
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glEnableVertexAttribArray(0);

    // Настройка отрисовки объекта источника света
    GLuint L_VAO;
    glGenVertexArrays(1, &L_VAO);
    glBindVertexArray(L_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //    GLfloat radius = 5.0f;
    GLfloat camX = 0.0f;
    GLfloat camY = 1.7f;
    GLfloat camZ = 5.0f;

    // Игровой цикл
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Program.use();

//        camX = sin(glfwGetTime()) * radius;
//        camZ = cos(glfwGetTime()) * radius;

        glm::vec3 cameraPos (camX, camY, camZ);

        Program.setVec3("viewPos", cameraPos);

        // Нстройка параметров источника света
        // directional light
        Program.setVec3("dirLight.direction", 0.0f, 0.0f, 0.0f);
        Program.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        Program.setVec3("dirLight.diffuse", 0.04f, 0.04f, 0.04f);
        Program.setVec3("dirLight.specular", 0.05f, 0.05f, 0.05f);
        // point light 1
        Program.setVec3("pointLights[0].position", pointLightPositions[0]);
        Program.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        Program.setVec3("pointLights[0].diffuse", 0.5f, 0.0f, 0.0f);
        Program.setVec3("pointLights[0].specular", 1.0f, 0.0f, 0.0f);
        Program.setFloat("pointLights[0].constant", 1.0f);
        Program.setFloat("pointLights[0].linear", 0.09);
        Program.setFloat("pointLights[0].quadratic", 0.032);
        // point light 2
        Program.setVec3("pointLights[1].position", pointLightPositions[1]);
        Program.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        Program.setVec3("pointLights[1].diffuse", 0.0f, 0.5f, 0.0f);
        Program.setVec3("pointLights[1].specular", 0.0f, 1.0f, 0.0f);
        Program.setFloat("pointLights[1].constant", 1.0f);
        Program.setFloat("pointLights[1].linear", 0.09);
        Program.setFloat("pointLights[1].quadratic", 0.032);
        // point light 3
        Program.setVec3("pointLights[2].position", pointLightPositions[2]);
        Program.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        Program.setVec3("pointLights[2].diffuse", 0.0f, 0.0f, 0.5);
        Program.setVec3("pointLights[2].specular", 0.0f, 0.0f, 1.0f);
        Program.setFloat("pointLights[2].constant", 1.0f);
        Program.setFloat("pointLights[2].linear", 0.09);
        Program.setFloat("pointLights[2].quadratic", 0.032);

        // spotLight
        Program.setVec3("spotLight.position", spotLightPosition);
        Program.setVec3("spotLight.direction", 3.0f, -1.0f, 0.5f);
        Program.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        Program.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
        Program.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        Program.setFloat("spotLight.constant", 1.0f);
        Program.setFloat("spotLight.linear", 0.09);
        Program.setFloat("spotLight.quadratic", 0.032);
        Program.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        Program.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        glm::mat4 view;
        glm::mat4 projection;

        view = glm::lookAt( glm::vec3(cameraPos), // Положение камеры
                            glm::vec3(0.0f, 0.0f, 0.0f), // Направление камеры (смотрит в точку (0,0,0))
                            glm::vec3(0.0f, 1.0f, 0.0f)); // Камера расположена горизонтально

        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


        Program.setMat4("view", view);
        Program.setMat4("projection", projection);


        //~~~~~~~~~~~~~~~~~~ CUBE-1  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        Program.setVec3("material.ambient", 0.2f, 0.2f, 0.2f);
        Program.setVec3("material.diffuse", 0.2f, 0.2f, 0.2f);
        Program.setVec3("material.specular", 1.0f, 1.0f, 1.0f);
        Program.setFloat("material.shininess", 32.0f);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[0]);
        model = glm::rotate(model, glm::radians(RotCount*57), glm::vec3(0.0f, 1.0f, 0.0f));
        Program.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //~~~~~~~~~~~~~~~~~~ CUBE-2  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        Program.setVec3("material.ambient", 0.2f, 0.2f, 0.2f);
        Program.setVec3("material.diffuse", 0.4f, 0.4f, 0.4f);
        Program.setVec3("material.specular", 0.2f, 0.2f, 0.2f);
        Program.setFloat("material.shininess", 32.0f);

        cubePositions[1] = glm::vec3( sin(RotCount+1.6)*1.5,  0.0f,  cos(RotCount+1.6)*1.5);
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, cubePositions[1]);
        model2 = glm::rotate(model2, glm::radians(RotCount*57), glm::vec3(0.0f, 1.0f, 0.0f));
        Program.setMat4("model", model2);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //~~~~~~~~~~~~~~~~~~ CUBE-3  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        Program.setVec3("material.ambient", 0.1f, 0.1f, 0.1f);
        Program.setVec3("material.diffuse", 0.4f, 0.4f, 0.4f);
        Program.setVec3("material.specular", 1.0f, 1.0f, 0.4f);
        Program.setFloat("material.shininess", 16.0f);

        cubePositions[2] = glm::vec3( -cos(RotCount)*1.5,  0.0f,  sin(RotCount)*1.5);
        glm::mat4 model3 = glm::mat4(1.0f);
        model3 = glm::translate(model3, cubePositions[2]);
        model3 = glm::rotate(model3, glm::radians(RotCount*57), glm::vec3(0.0f, 1.0f, 0.0f));
        Program.setMat4("model", model3);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //~~~~~~~~~~~~~~~~~~ LAMP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        LampShader.use();
        LampShader.setMat4("view", view);
        LampShader.setMat4("projection", projection);
        glBindVertexArray(L_VAO);
        for (unsigned int i = 0; i < 3; i++) {
            glm::mat4 L_model = glm::mat4(1.0f);
            L_model = glm::translate(L_model, pointLightPositions[i]);
            L_model = glm::scale(L_model, glm::vec3(0.2f));
            LampShader.setMat4("model", L_model);
            LampShader.setVec3("color", pointLightColors[i]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glm::mat4 L_model = glm::mat4(1.0f);
        L_model = glm::translate(L_model, spotLightPosition);
        L_model = glm::scale(L_model, glm::vec3(0.2f));
        LampShader.setMat4("model", L_model);
        LampShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
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
    glDeleteVertexArrays(1, &L_VAO);
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
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        RotCount += 0.02;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        RotCount -= 0.02;
    }
}
//-----------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
//-----------------------------------------------------------------------------------

