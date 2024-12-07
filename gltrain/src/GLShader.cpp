#include "GLShader.h"

#include "SimpleLogger.h"

#include <sstream>
#include <fstream>
#include <string>

namespace Casic
{
namespace GL
{
	GLShader::GLShader()
	{

	}

	GLShader::~GLShader()
	{

	}

	bool GLShader::LoadShader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
	{
		if (!ShaderSrc(vertexShaderFile, fragmentShaderFile)) return false;
		if (!CompileShader(m_VertexShaderSrc, m_FragmentShaderSrc)) return false;

		return true;
	}

	bool GLShader::ShaderSrc(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
	{
		std::ifstream ifsVs(vertexShaderFile), ifsFs(fragmentShaderFile);
		if (!ifsVs.is_open())
		{
			SimpleLogger::GetInstance().Error("Can not open vertex shader file: {}", vertexShaderFile);
			return false;
		}
		if (!ifsFs.is_open())
		{
			SimpleLogger::GetInstance().Error("Can not open fragment shader file: {}", fragmentShaderFile);
			return false;
		}

		std::string line;

		std::stringstream strStreamVs;
		while (std::getline(ifsVs, line))
		{
			strStreamVs << line << "\n";
		}
		std::stringstream strStreamFs;
		while (std::getline(ifsFs, line))
		{
			strStreamFs << line << "\n";
		}

		m_VertexShaderSrc = strStreamVs.str();
		m_FragmentShaderSrc = strStreamFs.str();
		SimpleLogger::GetInstance().Info("Get shader code successfully.");
	}
	
	bool GLShader::CompileShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode)
	{
		m_VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		const char* vsCodeCStr = vertexShaderCode.data();
		glShaderSource(m_VertexShaderID, 1, &vsCodeCStr, NULL);
		glCompileShader(m_VertexShaderID);
		int status;
		glGetShaderiv(m_VertexShaderID, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			int length = 0;
			glGetShaderiv(m_VertexShaderID, GL_INFO_LOG_LENGTH, &length);
			char* log = new char[length + 1];
			glGetShaderInfoLog(m_VertexShaderID, length + 1, NULL, log);
			SimpleLogger::GetInstance().Error("Compile vertex shader failed. Error msg:{}", log);
			delete[] log;

			glDeleteShader(m_VertexShaderID);
			return false;
		}

		m_FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		const char* fsCodeCStr = fragmentShaderCode.data();
		glShaderSource(m_FragmentShaderID, 1, &fsCodeCStr, NULL);
		glCompileShader(m_FragmentShaderID);
		glGetShaderiv(m_FragmentShaderID, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			int length = 0;
			glGetShaderiv(m_FragmentShaderID, GL_INFO_LOG_LENGTH, &length);
			char* log = new char[length + 1];
			glGetShaderInfoLog(m_FragmentShaderID, length + 1, NULL, log);
			SimpleLogger::GetInstance().Error("Compile fragmen shader failed. Error msg:{}", log);
			delete[] log;
			glDeleteShader(m_FragmentShaderID);
			glDeleteShader(m_VertexShaderID);
			return false;
		}

		SimpleLogger::GetInstance().Info("Compile shader successfully.");
		return true;
	}
}
}