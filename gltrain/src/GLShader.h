#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <format>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <yaml-cpp/yaml.h>

#include "GLGlobalState.h"

class GLShader {
public:
	GLShader();

	void Bind() const { glUseProgram(m_ProgramID); }
	void UnBind() const { glUseProgram(0); }
	GLuint ProgramID() const { return m_ProgramID; }

	bool LoadShaderFromFile(const std::string& vertexShaderPath = "resource/shader/default_vs.glsl", const std::string& fragmentShaderPath = "resource/shader/default_fs.glsl",
		const std::string& shaderConfigFile = "resource/config/shader.yml", const std::string& shaderName = "default shader");
private:
	bool GetShaderSrc(const std::string& vsPath, const std::string& fsPath, std::string& vsCode, std::string& fsCode);
	bool CompileShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
	bool LinkShader(const std::string& shaderConfigFile, const std::string& shaderName);

public:
	std::unordered_map<std::string, int> m_Locations;
	std::unordered_map<std::string, GLGlobalState::LocationData> m_LocationDatas;

private:
	unsigned int m_ProgramID = 0;
	unsigned int m_VertexShaderID = 0;
	unsigned int m_FragmentShaderID = 0;
};