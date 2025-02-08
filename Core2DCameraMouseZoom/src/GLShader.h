#pragma once

#include "GLGlobal.h"

#include "Casic/CasicUtils.h"
#include "Casic/CasicGraphics.h"

#include <unordered_map>
#include <type_traits>
#include <typeinfo>

//----------------------------------------------------------------------------
// Location names
//----------------------------------------------------------------------------
#define ATTRIB_VERTEX_POSITION_NAME					"vertexPosition"
#define ATTRIB_VERTEX_TEXCOORD_NAME					"vertexTexCoord"
#define ATTRIB_VERTEX_NORMAL_NAME					"vertexNormal"
#define ATTRIB_VERTEX_COLOR_NAME					"vertexColor"

#define UNIFORM_MVP_NAME							"mvp"
#define UNIFORM_TEXTURE0_NAME						"texture0"

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
	class GLTRAIN_API GLShader
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
		void SetUniformInt(const std::string& name, int value);
		template <typename _Ty>
		void SetUniform(const std::string& name, const _Ty& value)
		{
			using LocType = typename std::remove_cv_t<_Ty>;
			int location = GetUniformLocation(name);
			if (location == -1)
			{
				SimpleLogger::GetInstance().Error("Shader:[ID {}] Get Uniform [Name: {}, Type: {}] error.", m_ProgramID, name, typeid(LocType).name());
				return;
			}
			// TODO: 根据LocType执行不同的glUniform**(例如glUniform1i, glUniformMat4fv等等等等，要包罗万象
			if constexpr (std::is_same_v<LocType, int>) 
			{
				glUniform1i(location, value);
			}
			else if constexpr (std::is_same_v<LocType, float>)
			{
				glUniform1f(location, value);
			}
			else if constexpr (std::is_same_v<LocType, Math::Vector2>)
			{
				glUniform2f(location, value.x, value.y);
			}
			else if constexpr (std::is_same_v<LocType, Math::Vector3>)
			{
				glUniform3f(location, value.x, value.y, value.z);
			}
			else if constexpr (std::is_same_v<LocType, Math::Vector4>)
			{
				glUniform4f(location, value.x, value.y, value.z, value.w);
			}
			else if constexpr (std::is_same_v<LocType, Math::Matrix4>)
			{
				glUniformMatrix4fv(location, 1, GL_FALSE, Graphics::ToOpenGLMatrix4(value).v);
			}
			else
			{
				static_assert(Util::always_false<LocType>, "Unsupported uniform type.");
			}
		}
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