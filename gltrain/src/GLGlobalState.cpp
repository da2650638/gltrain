#include "GLGlobalState.h"

std::once_flag GLGlobalState::m_InitFlag;
std::unique_ptr<GLGlobalState> GLGlobalState::m_Instance;

GLGlobalState::ShaderInfo GLGlobalState::LoadShaderInfos(const std::string& filename, const std::string& shaderName)
{
	YAML::Node config = YAML::LoadFile(filename);

	ShaderInfo shaderInfo;
	shaderInfo.LocationDatas = std::unordered_map<std::string, LocationData>{};
	for (const auto& shader : config["shaders"])
	{
		if (shader["name"].as<std::string>() == shaderName)
		{
			if (shader["attributes"])
			{
				for (const auto& attribute : shader["attributes"])
				{
					LocationData locData;
					locData.Name = attribute.second["name"].as<std::string>();
					locData.DefaultLoc = attribute.second["defaultLoc"].as<unsigned int>();
					locData.IndexInLocArray = attribute.second["indexInLocArray"].as<unsigned int>();
					locData.Type = attribute.second["type"].as < std::string>();
					locData.LocationType = "attribute";
					shaderInfo.LocationDatas[locData.Name] = locData;
				}
			}
			if (shader["uniforms"])
			{
				for (const auto& uniform : shader["uniforms"])
				{
					LocationData locData;
					locData.Name = uniform.second["name"].as<std::string>();
					locData.DefaultLoc = 0;  // Uniform需要实时从shader获取
					locData.IndexInLocArray = uniform.second["indexInLocArray"].as<unsigned int>();
					locData.Type = uniform.second["type"].as<std::string>();
					locData.LocationType = "uniform";
					shaderInfo.LocationDatas[locData.Name] = locData;
				}
			}
		}
	}

	//GLSimpleLogger& logger = GLSimpleLogger::GetInstance();
	//logger.Debug("ShaderInfo: ", shaderInfo.VertexShaderFile);
	//logger.Debug("Vertex Shader File: {}", shaderInfo.VertexShaderFile);
	//logger.Debug("Fragment Shader File: {}", shaderInfo.FragmentShaderFile);

	//for (const auto& [key, locData] : shaderInfo.LocationDatas) {
	//	logger.Debug("Location Data - Name: {}, DefaultLoc: {}, IndexInLocArray: {}, Type: {}",
	//		locData.Name, locData.DefaultLoc, locData.IndexInLocArray, locData.Type);
	//}
	return shaderInfo;
}
