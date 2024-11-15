#include "GLShader.h"
#include "GLGlobal.h"

GLShader::GLShader()
{
}

bool GLShader::LoadShaderFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath,
	const std::string& shaderConfigFile, const std::string& shaderName)
{
	auto curShaderInfo = GLGlobalState::GetInstance().LoadShaderInfos(shaderConfigFile, shaderName);
	m_LocationDatas = curShaderInfo.LocationDatas;

	std::string vertexShaderCode, fragmentShaderCode;
	if (!GetShaderSrc(vertexShaderPath, fragmentShaderPath, vertexShaderCode, fragmentShaderCode))
	{
		GLSimpleLogger::GetInstance().Error("Get shader source code error:[Vertex Shader Code File: {}][Fragment Shader Code File: {}]", vertexShaderPath, fragmentShaderPath);
		return false;
	}

	if (!CompileShader(vertexShaderCode, fragmentShaderCode))
	{
		GLSimpleLogger::GetInstance().Error("Compile shader error.");
		return false;
	}

	if (!LinkShader(shaderConfigFile, shaderName))
	{
		GLSimpleLogger::GetInstance().Error("Link shader error.");
		return false;
	}

	// TODO: 设计不合理，LoadShaderInfos这个函数使用太多次了

	glUseProgram(m_ProgramID);
	for (const auto& [key, locData] : m_LocationDatas)
	{
		m_Locations[locData.Name] = locData.LocationType == "attribute" ? glGetAttribLocation(m_ProgramID, locData.Name.data()) : glGetUniformLocation(m_ProgramID, locData.Name.data());
	}
	glUseProgram(0);
}

bool GLShader::GetShaderSrc(const std::string& vsPath, const std::string& fsPath, std::string& vsCode, std::string& fsCode)
{
	vsCode.clear();
	fsCode.clear();
	std::ifstream vsIfs(vsPath), fsIfs(fsPath);
	if (!vsIfs.is_open() || !fsIfs.is_open())
	{
		GLSimpleLogger::GetInstance().Error("Read shader code failed. Vertex Shader File:{}, Fragment Shader File: {}", vsPath, fsPath);
		return false;
	}
	GLSimpleLogger::GetInstance().Info("Successfully read vertex shader file: {}", vsPath);
	GLSimpleLogger::GetInstance().Info("Successfully read fragment shader file: {}", fsPath);

	std::string line;
	std::stringstream vsSS;
	std::stringstream fsSS;
	while (std::getline(vsIfs, line))
	{
		vsSS << line << "\n";
	}
	while (std::getline(fsIfs, line))
	{
		fsSS << line << "\n";
	}

	vsCode = std::move(vsSS.str());
	fsCode = std::move(fsSS.str());
	return true;
}

bool GLShader::CompileShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode)
{
	m_VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	const char* vsCode = vertexShaderCode.data();
	glShaderSource(m_VertexShaderID, 1, &vsCode, 0);
	glCompileShader(m_VertexShaderID);
	
	int success;
	glGetShaderiv(m_VertexShaderID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length;
		glGetShaderiv(m_VertexShaderID, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length + 1];
		memset(log, 0, length);
		glGetShaderInfoLog(m_VertexShaderID, length + 1, 0, log);
		GLSimpleLogger::GetInstance().Error("vertex shader:[{}], compile error: {}", m_VertexShaderID, log);
		delete[] log;

		glDeleteShader(m_VertexShaderID);
		m_VertexShaderID = -1;
		return false;
	}

	m_FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fsCode = fragmentShaderCode.data();
	glShaderSource(m_FragmentShaderID, 1, &fsCode, 0);
	glCompileShader(m_FragmentShaderID);

	glGetShaderiv(m_FragmentShaderID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length;
		glGetShaderiv(m_FragmentShaderID, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length+1];
		memset(log, 0, length);
		glGetShaderInfoLog(m_FragmentShaderID, length + 1, 0, log);
		GLSimpleLogger::GetInstance().Error("fragment shader:[{}], compile error: {}", m_FragmentShaderID, log);
		delete[] log;

		glDeleteShader(m_FragmentShaderID);
		glDeleteShader(m_VertexShaderID);
		m_VertexShaderID = 0;
		m_FragmentShaderID = 0;
		return false;
	}

	return true;
}

bool GLShader::LinkShader(const std::string& shaderConfigFile, const std::string& shaderName)
{
	m_ProgramID = glCreateProgram();

	glAttachShader(m_ProgramID, m_VertexShaderID);
	glAttachShader(m_ProgramID, m_FragmentShaderID);

	auto curShaderInfo = GLGlobalState::GetInstance().LoadShaderInfos(shaderConfigFile, shaderName);

	GLGlobalState::LocationData data = m_LocationDatas["vertexPosition"];
	glBindAttribLocation(m_ProgramID, data.DefaultLoc, data.Name.data());

	data = m_LocationDatas["vertexColor"];
	glBindAttribLocation(m_ProgramID, data.DefaultLoc, data.Name.data());

	glLinkProgram(m_ProgramID);

	int success;
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length;
		glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length + 1];
		memset(log, 0, length);
		glGetProgramInfoLog(m_ProgramID, length + 1, 0, log);
		GLSimpleLogger::GetInstance().Error("link error, program id:[{}], link error: {}", m_ProgramID, log);
		delete[] log;

		glDeleteProgram(m_ProgramID);
		m_ProgramID = 0;
		return false;
	}

	return true;
}
