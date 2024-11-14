#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <format>

#include <glm/glm.hpp>
#include <glad/glad.h>

class GLShader {
public:

	GLuint ProgramID() const { return m_ProgramID; }

	bool LoadShaderFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
private:
	bool GetShaderSrc(const std::string& vsPath, const std::string& fsPath, std::string& vsCode, std::string& fsCode);
	bool CompileShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
	bool LinkShader();

private:
	unsigned int m_ProgramID = 0;
	unsigned int m_VertexShaderID = 0;
	unsigned int m_FragmentShaderID = 0;
};