#pragma once

#include "GLGlobal.h"

#include <unordered_map>

namespace Casic
{
namespace GL
{
	class GLShader
	{
	public:
		GLShader();
		~GLShader();

		bool LoadShader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);

	private:
		bool ShaderSrc(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
		bool CompileShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);

	private:
		unsigned int m_ProgramID;
		unsigned int m_VertexShaderID;
		unsigned int m_FragmentShaderID;
		std::string m_VertexShaderSrc;
		std::string m_FragmentShaderSrc;
		std::unordered_map<std::string, int> m_Locations;
	};
}
}