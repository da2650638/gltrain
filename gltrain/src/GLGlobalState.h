#pragma once

#include <mutex>
#include <memory>
#include <unordered_map>

#include "GLSimpleLogger.h"

#include <yaml-cpp/yaml.h>

class GLGlobalState {
public:
	// shader configs
	typedef struct LocationData {
		std::string Name;
		int DefaultLoc;
		unsigned int IndexInLocArray;
		std::string Type;
		std::string LocationType;
	}LocationData;
	typedef struct ShaderInfo {
		std::string VertexShaderFile;
		std::string FragmentShaderFile;
		std::unordered_map<std::string, LocationData> LocationDatas;
	}ShaderInfo;
	ShaderInfo LoadShaderInfos(const std::string& filename, const std::string& shaderName);

	static GLGlobalState& GetInstance()
	{
		std::call_once(m_InitFlag, []() {
			m_Instance.reset( new GLGlobalState() );
		});
		return *m_Instance;
	}
private:
	GLGlobalState() {};

	static std::once_flag m_InitFlag;
	static std::unique_ptr<GLGlobalState> m_Instance;
};