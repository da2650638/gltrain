#include "GLShader.h"

#include "SimpleLogger.h"

#include "Casic/CasicGraphics.h"

#include <sstream>
#include <fstream>
#include <string>

namespace Casic
{
namespace GL
{
	GLShader::GLShader(const std::string& name)
		: m_Name(name)
	{

	}

	GLShader::~GLShader()
	{

	}

	bool GLShader::LoadShader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
	{
		SimpleLogger::GetInstance().Info("Load shader: {}", m_Name);
		if (!ShaderSrc(vertexShaderFile, fragmentShaderFile)) return false;
		if (!CompileShader(m_VertexShaderSrc, m_FragmentShaderSrc)) return false;
		if (!LinkShader(m_VertexShaderID, m_FragmentShaderID))
		{
			glDeleteShader(m_VertexShaderID);
			glDeleteShader(m_FragmentShaderID);
			return false;
		}

		return true;
	}

	int GLShader::GetAttribLocation(const std::string& name)
	{
		if (m_Locations.find(name) != m_Locations.end())
		{
#ifdef USE_CPP20
			[[likely]]
#else

#endif
			return m_Locations[name];
		}
		int location = glGetAttribLocation(m_ProgramID, name.data());
		if (location != -1)
		{
			m_Locations[name] = location;
		}
		return location;
	}

	int GLShader::GetUniformLocation(const std::string& name)
	{
		if (m_Locations.find(name) != m_Locations.end())
		{
#ifdef USE_CPP20
			[[likely]]
#else

#endif
			return m_Locations[name];
		}
		int location = glGetUniformLocation(m_ProgramID, name.data());
		if (location != -1)
		{
			m_Locations[name] = location;
		}
		return location;
	}

	void GLShader::SetUniformMat4(const std::string& name, const Math::Matrix4& value)
	{
		int location = GetUniformLocation(name);
		if (location == -1)
		{
			SimpleLogger::GetInstance().Error("Shader:[ID {}] Get Uniform [Name: {}, Type: Mat4] error.", m_ProgramID, name);
			return;
		}
		glUniformMatrix4fv(location, 1, GL_FALSE, Graphics::ToOpenGLMatrix4(value).v);
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

		SimpleLogger::GetInstance().Info("Compile shader successfully.vertex shader ID: {}, fragment shader ID: {}", m_VertexShaderID, m_FragmentShaderID);
		return true;
	}

	bool GLShader::LinkShader(int vertexShaderID, int fragmentShaderID)
	{
		m_ProgramID = glCreateProgram();

		glUseProgram(m_ProgramID);

		glAttachShader(m_ProgramID, vertexShaderID);
		glAttachShader(m_ProgramID, fragmentShaderID);

		// Must bind these locations before glLinkProgram()
		glBindAttribLocation(m_ProgramID, ATTRIB_VERTEX_POSITION_DEFAULT_LOC, ATTRIB_VERTEX_POSITION_NAME);
		glBindAttribLocation(m_ProgramID, ATTRIB_VERTEX_TEXCOORD_DEFAULT_LOC, ATTRIB_VERTEX_TEXCOORD_NAME);
		glBindAttribLocation(m_ProgramID, ATTRIB_VERTEX_NORMAL_DEFAULT_LOC, ATTRIB_VERTEX_NORMAL_NAME);
		glBindAttribLocation(m_ProgramID, ATTRIB_VERTEX_COLOR_DEFAULT_LOC, ATTRIB_VERTEX_COLOR_NAME);

		glLinkProgram(m_ProgramID);
		int status;
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			int length;
			glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &length);
			char* log = new char[length + 1];
			glGetProgramInfoLog(m_ProgramID, length + 1, NULL, log);
			SimpleLogger::GetInstance().Error("Link program error. ID: {}, msg: {}", m_ProgramID, log);
			return false;
		}

		SimpleLogger::GetInstance().Info("Link program successfully. ID:{}", m_ProgramID);
		return true;
	}
}
}