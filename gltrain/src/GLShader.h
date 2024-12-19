#pragma once

#include "GLGlobal.h"

#include <unordered_map>

//----------------------------------------------------------------------------
// Location names
//----------------------------------------------------------------------------
#define ATTRIB_VERTEX_POSITION_NAME					"vertexPosition"
#define ATTRIB_VERTEX_TEXCOORD_NAME					"vertexTexcoord"
#define ATTRIB_VERTEX_NORMAL_NAME					"vertexNormal"
#define ATTRIB_VERTEX_COLOR_NAME					"vertexColor"

#define UNIFORM_MVP_NAME							"mvp"

//----------------------------------------------------------------------------
// Location default values
//----------------------------------------------------------------------------
#define ATTRIB_VERTEX_POSITION_DEFAULT_LOC			0
#define ATTRIB_VERTEX_TEXCOORD_DEFAULT_LOC			1
#define ATTRIB_VERTEX_NORMAL_DEFAULT_LOC			2
#define ATTRIB_VERTEX_COLOR_DEFAULT_LOC				3

namespace Casic
{
namespace GL
{
	class GLShader
	{
	public:
		GLShader(const std::string& name = "Default Shader");
		~GLShader();

		void Bind() { glUseProgram(m_ProgramID); }
		void UnBind() { glUseProgram(0); }
		unsigned int ProgramID() const { return m_ProgramID; }
		unsigned int ID() const { return m_ProgramID; }
		std::string Name() const { return m_Name; }

		bool LoadShader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
		int GetAttribLocation(const std::string& name);
		int GetUniformLocation(const std::string& name);
		int Attribute(const std::string& name) { return GetAttribLocation(name); }
		int Uniform(const std::string& name) { return GetUniformLocation(name); }

		// NOTE: Set Uniform Functions
		// TODO: 后续考虑使用模板元编程优化一下
		void SetUniformMat4(const std::string& name, const Math::Matrix4& value);
	private:
		bool ShaderSrc(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
		bool CompileShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
		bool LinkShader(int vertexShaderID, int fragmentShaderID);

	private:
		std::string m_Name;

		unsigned int m_ProgramID;
		unsigned int m_VertexShaderID;
		unsigned int m_FragmentShaderID;
		std::string m_VertexShaderSrc;
		std::string m_FragmentShaderSrc;
		std::unordered_map<std::string, int> m_Locations;
	};
}
}