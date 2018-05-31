#ifndef SHADER_UNIFORM_H
#define SHADER_UNIFORM_H
#include "../Utilities/Matrix44.hpp"
#include "../Utilities/Vector2.hpp"
#include "../Utilities/Vector3.h"
#include "../Utilities/Vector4.hpp"

enum EngineUniformShaderType { SHADER_UNIFORM_BOOL,
							   SHADER_UNIFORM_FLOAT,
							   SHADER_UNIFORM_INT,
							   SHADER_UNIFORM_VEC2,
							   SHADER_UNIFORM_VEC3,
							   SHADER_UNIFORM_VEC4,
							   SHADER_UNIFORM_MAT4,
							   SHADER_UNIFORM_SAMPLER2D,
							   SHADER_UNIFORM_UNDEFINED	};

class ShaderUniform
{
	public:
		std::string						m_name;
		int								m_location;
		EngineUniformShaderType			m_type;

	public:
		ShaderUniform( const std::string name, EngineUniformShaderType type, int location )
					 : m_name(name), m_type(type), m_location(location) {};
		virtual ~ShaderUniform(){};
};

template<typename UniformValueType>
class ShaderUniformTemplate : public ShaderUniform
{
	public:
		 UniformValueType				m_value;

	public:
		ShaderUniformTemplate( const std::string name, EngineUniformShaderType type, int location ) : ShaderUniform( name, type, location ){};
		virtual ~ShaderUniformTemplate(){};
};

typedef ShaderUniformTemplate<bool>			ShaderBoolUniform;
typedef ShaderUniformTemplate<float>		ShaderFloatUniform;
typedef ShaderUniformTemplate<int>			ShaderIntUniform;
typedef ShaderUniformTemplate<Vector2>		ShaderVec2Uniform;
typedef ShaderUniformTemplate<Vector3>		ShaderVec3Uniform;
typedef ShaderUniformTemplate<Vector4>		ShaderVec4Uniform;
typedef ShaderUniformTemplate<Matrix44>		ShaderMat4Uniform;


#endif