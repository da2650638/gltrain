#version 460 core
in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 outColor;
uniform sampler2D texture0;
void main()
{
	vec4 texelColor = texture(texture0, fragTexCoord);
	outColor = texelColor * fragColor;
}