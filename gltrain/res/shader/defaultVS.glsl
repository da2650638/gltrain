#version 460 core
//layout(location = 0) in vec3 vertexPosition;
//layout(location = 1) in vec2 vertexTexcoord;
//layout(location = 2) in vec3 vertexNormal;
//layout(location = 3) in vec4 vertexColor;
in vec3 vertexPosition;
in vec2 vertexTexcoord;
in vec3 vertexNormal;
in vec4 vertexColor;
out vec4 fragColor;
uniform mat4 mvp;
void main()
{
	gl_Position = mvp * vec4(vertexPosition, 1.0);
	fragColor = vertexColor;
}