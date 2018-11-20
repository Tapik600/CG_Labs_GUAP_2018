#version 330 core
layout (location = 0) in vec3 position; // Устанавливаем позицию атрибута в 0
layout (location = 1) in vec3 normal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(position, 1.0f));
    //              матрица нормалей
    Normal = mat3(transpose(inverse(model))) * normal; // для устранения эффекта неправильного масштабирования нормалей
    gl_Position = projection * view * model * vec4(position, 1.0f);
}