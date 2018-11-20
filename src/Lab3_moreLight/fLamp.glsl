#version 330 core
out vec4 LapmColor;
uniform vec3 color;
void main()
{
    LapmColor = vec4 (color, 1.0f); // set alle 4 vector values to 1.0
}
