#include "OpenGLShaderProgram.hpp"
#include "GLRender.hpp"
#include "../JobSystem/MemoryPoolManager.hpp"

EngineUniformShaderType ConvertOpenGLUniformTypeToEngineType(GLenum openglType)
{
	switch (openglType)
	{
		case GL_BOOL:			return SHADER_UNIFORM_BOOL;
		case GL_INT:			return SHADER_UNIFORM_INT;
		case GL_FLOAT:			return SHADER_UNIFORM_FLOAT;
		case GL_FLOAT_VEC2:		return SHADER_UNIFORM_VEC2;
		case GL_FLOAT_VEC3:		return SHADER_UNIFORM_VEC3;
		case GL_FLOAT_VEC4:		return SHADER_UNIFORM_VEC4;
		case GL_FLOAT_MAT4:		return SHADER_UNIFORM_MAT4;
		case GL_SAMPLER_2D:		return SHADER_UNIFORM_SAMPLER2D;
		default:				return SHADER_UNIFORM_UNDEFINED;
	}
}

OpenGLShaderProgram::OpenGLShaderProgram( const std::string& name, const std::string& vertexShader, const std::string& fragmentShader )
{
	m_shaderProgramID = glCreateProgram();
	m_fragmentShaderFullPath = fragmentShader;
	m_shaderProgramName = name;
	m_numberOfShader = 0;

	OpenGLShader vertShader(VERTEX_SHADER);
	vertShader.LoadAndCompileShader(vertexShader);

	OpenGLShader fragShader(FRAGMENT_SHADER);
	fragShader.LoadAndCompileShader(fragmentShader);

	AttachShader( vertShader );
	AttachShader( fragShader );

	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_POSITIONS,	   "a_vertexPosition");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_COLORS,		   "a_vertexColor");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_TEXCOORDS,	   "a_vertexTexCoords");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_NORMALS,		   "a_vertexNormal");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_TANGENT,		   "a_vertexTangent");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_BITANGENT,	   "a_vertexBitangent");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_BONE_INDEX,	   "a_vertexBoneIndex");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_BONE_WEIGHT,    "a_vertexBoneWeight");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_IS_STATIC,      "a_isStatic");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_TERRAIN_WEIGHT, "a_vertexTerrainWeight");

	LinkProgram();

	GetUniformLocation();
}

OpenGLShaderProgram::OpenGLShaderProgram(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader)
{
	m_shaderProgramID = glCreateProgram();
	m_fragmentShaderFullPath = fragmentShader;
	m_shaderProgramName = name;
	m_numberOfShader = 0;

	OpenGLShader geoShader( GEOMETRY_SHADER );
	geoShader.LoadAndCompileShader(geometryShader);

	OpenGLShader vertShader(VERTEX_SHADER);
	vertShader.LoadAndCompileShader(vertexShader);

	OpenGLShader fragShader(FRAGMENT_SHADER);
	fragShader.LoadAndCompileShader(fragmentShader);

	AttachShader( geoShader );
	AttachShader( vertShader );
	AttachShader( fragShader );

	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_POSITIONS,	   "a_vertexPosition");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_COLORS,		   "a_vertexColor");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_TEXCOORDS,	   "a_vertexTexCoords");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_NORMALS,		   "a_vertexNormal");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_TANGENT,		   "a_vertexTangent");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_BITANGENT,	   "a_vertexBitangent");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_BONE_INDEX,	   "a_vertexBoneIndex");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_BONE_WEIGHT,    "a_vertexBoneWeight");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_IS_STATIC,      "a_isStatic");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_TERRAIN_WEIGHT, "a_vertexTerrainWeight");

	LinkProgram();

	GetUniformLocation();
}

OpenGLShaderProgram::~OpenGLShaderProgram()
{
	glDeleteProgram(m_shaderProgramID);
}

void OpenGLShaderProgram::AttachShader(const OpenGLShader& shader)
{
	glAttachShader( m_shaderProgramID, shader.m_shaderID );
	m_numberOfShader++;
	if(shader.m_shaderType == FRAGMENT_SHADER)
	{
		const int LONG_PATH = 1024;
		char currentDirectory[ LONG_PATH ];
		GetCurrentDirectoryA( LONG_PATH, currentDirectory );
		for( int i = 0; i < LONG_PATH; i++ )
		{
			if( currentDirectory[i] == '\\' )
				currentDirectory[i] = '/';
		}
		m_fragmentShaderFullPath = currentDirectory + shader.m_shaderName ;
	}
}

void OpenGLShaderProgram::LinkProgram()
{
	char windowMessage[2048];
	const GLubyte* openglVersion = glGetString( GL_VERSION );
	const GLubyte* shadingLanguageVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );

	if(m_numberOfShader < 2)
	{
		DebuggerPrintf( "Shader Program Linking failed.\n");
		DebuggerPrintf( "Error : Need at least 2 shaders for program to link.\n");

		strcpy_s(windowMessage, "OpenGL Version: ");
		strcat_s(windowMessage, (const char*)openglVersion );
		strcat_s(windowMessage, "\n" );
		strcat_s(windowMessage, "Shading Language Version: ");
		strcat_s(windowMessage, (const char*)shadingLanguageVersion );
		strcat_s(windowMessage, "\n\n" );
		strcat_s(windowMessage, "Shader Program Linking failed.\n" );
		strcat_s(windowMessage, "Error : Need at least 2 shaders for program to link.\n");
		strcat_s(windowMessage, "\n\n" );

		int msgBoxID1 = MessageBoxA( NULL,
									windowMessage,
									"Linking Error!" ,
									MB_ICONSTOP | MB_OK | MB_DEFBUTTON1);

		if( msgBoxID1 == IDOK )
		{
			exit(-1);
		}
	}

	glLinkProgram(m_shaderProgramID);

	GetUniformInformation();

	int linkStatus;
	glGetProgramiv( m_shaderProgramID, GL_LINK_STATUS, &linkStatus );
	if ( linkStatus == GL_FALSE)
	{

		DebuggerPrintf( "Shader Program Linking failed.\n");
		DebuggerPrintf( "%s\n", m_fragmentShaderFullPath.c_str() );

		strcpy_s(windowMessage, "OpenGL Version: ");
		strcat_s(windowMessage, (const char*)openglVersion );
		strcat_s(windowMessage, "\n" );
		strcat_s(windowMessage, "Shading Language Version: ");
		strcat_s(windowMessage, (const char*)shadingLanguageVersion );
		strcat_s(windowMessage, "\n\n" );
		strcat_s(windowMessage, "Error : Shader Program Linking failed.\n\n" );
		strcat_s(windowMessage, m_fragmentShaderFullPath.c_str() );

		int msgBoxID = MessageBoxA( NULL,
									windowMessage,
									"Linking Error!" ,
									MB_ICONSTOP | MB_OK | MB_DEFBUTTON1 );

		if( msgBoxID == IDOK )
		{
			exit(-1);
		}
	}
	else
	{
		DebuggerPrintf( "Shader Program Linking Successes.\n");
	}
}

void OpenGLShaderProgram::UseShaderProgram()
{
	glUseProgram(m_shaderProgramID);
}

void OpenGLShaderProgram::DisableShaderProgram()
{
	glUseProgram(0);
}

void OpenGLShaderProgram::GetUniformLocation()
{
	m_diffuseTextureUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_diffuseTexture");
	m_normalTextureUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_normalTexture");
	m_specularTextureUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_specularTexture");
	m_emissiveTextureUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_emissiveTexture");
	m_depthTextureUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_depthTexture");
	m_worldToScreenMatrixLoc = glGetUniformLocation(m_shaderProgramID,"u_WorldToScreenMatrix");
	m_lightWorldPosLoc = glGetUniformLocation(m_shaderProgramID,"u_lightPosition");
	m_timeUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_time");
	m_cameraWorldPositionUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_cameraWorldPosition");
	m_fogColorUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_fogColor");
	m_fogEndDistanceUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_fogEndDistance");
	m_fogStartDistanceUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_fogStartDistance");
	m_scaleBiasUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_scaleBias");
	m_deltaTimeUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_deltaTime");
	m_reflectCameraTransformationMatrixLoc = glGetUniformLocation(m_shaderProgramID,"u_reflectCameraTransformationMatrix");
	m_clipPlaneUniformLoc = glGetUniformLocation(m_shaderProgramID, "u_clipPlane" );
	m_modelViewMatrixUniformLoc = glGetUniformLocation(m_shaderProgramID, "u_ModelViewMatrix");
}

void OpenGLShaderProgram::CreateShaderUniform( const std::string& name, EngineUniformShaderType type, int location, int textureUnit )
{
	ShaderUniform* shaderUniform = nullptr;
	switch( type )
	{
		case SHADER_UNIFORM_BOOL : shaderUniform = new ShaderBoolUniform( name, type, location );
								   break;
		case SHADER_UNIFORM_INT : shaderUniform = new ShaderIntUniform( name, type, location );
								  break;
		case SHADER_UNIFORM_FLOAT : shaderUniform = new ShaderFloatUniform( name, type, location );
								    break;
		case SHADER_UNIFORM_VEC2 : shaderUniform = new ShaderVec2Uniform( name, type, location );
								   break;
		case SHADER_UNIFORM_VEC3 : shaderUniform = new ShaderVec3Uniform( name, type, location );
								   break;
		case SHADER_UNIFORM_VEC4 : shaderUniform = new ShaderVec4Uniform( name, type, location );
								   break;
		case SHADER_UNIFORM_MAT4 : shaderUniform = new ShaderMat4Uniform( name, type, location );
								   break;
		case SHADER_UNIFORM_SAMPLER2D :
		{
			ShaderIntUniform* textureUniform = new ShaderIntUniform( name, type, location );
			textureUniform->m_value = textureUnit;
			shaderUniform = textureUniform;
			break;
		}
	}
	m_uniforms.push_back( shaderUniform );
}

ShaderUniform* OpenGLShaderProgram::GetUniformByName( const std::string& name )
{
	for (unsigned int index = 0; index < m_uniforms.size(); index++)
	{
		ShaderUniform* uniform = m_uniforms[index];
		if (uniform->m_name.compare(name) == 0)
		{
			return uniform;
		}
	}
	return nullptr;
}

void OpenGLShaderProgram::GetUniformInformation()
{
	int numUniforms = 0;
	int uniformNameLength = 0;
	glGetProgramiv( m_shaderProgramID, GL_ACTIVE_UNIFORMS, &numUniforms );
	glGetProgramiv( m_shaderProgramID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformNameLength );

	int textureUnit = 0;
	for( int uniformIndex = 0; uniformIndex < numUniforms; uniformIndex++ )
	{
		char* uniformName = new char[ uniformNameLength + 1];
		GLenum uniformType = GL_ZERO;
		int nameLength;
		int uniformSize;
		int uniformLocation;
		glGetActiveUniform( m_shaderProgramID, (GLuint)uniformIndex, uniformNameLength + 1, &nameLength, &uniformSize, &uniformType, uniformName );
		uniformLocation = glGetUniformLocation( m_shaderProgramID, uniformName );
		EngineUniformShaderType engineShaderUniformType = ConvertOpenGLUniformTypeToEngineType(uniformType);
		CreateShaderUniform( uniformName, engineShaderUniformType, uniformLocation, textureUnit );
		if( engineShaderUniformType == SHADER_UNIFORM_SAMPLER2D )
			textureUnit++;
	}
}

void OpenGLShaderProgram::SetFloatUniformValue( const std::string& name, float value )
{
	ShaderUniform* uniform = GetUniformByName(name);
	if ( uniform )
	{
		ShaderFloatUniform* floatUniform = dynamic_cast< ShaderFloatUniform* >(uniform);
		floatUniform->m_value = value;
		glUniform1f( floatUniform->m_location, value );
	}
}

void OpenGLShaderProgram::SetIntUniformValue( const std::string& name, int value )
{
	ShaderUniform* uniform = GetUniformByName(name);
	if ( uniform )
	{
		ShaderIntUniform* intUniform = dynamic_cast< ShaderIntUniform* >(uniform);
		intUniform->m_value = value;
		glUniform1i( intUniform->m_location, value );
	}
}

void OpenGLShaderProgram::SetVec2UniformValue( const std::string& name, const Vector2& value )
{
	ShaderUniform* uniform = GetUniformByName(name);
	if ( uniform )
	{
		ShaderVec2Uniform* vec2Uniform = dynamic_cast< ShaderVec2Uniform* >(uniform);
		vec2Uniform->m_value = value;
		glUniform2f( vec2Uniform->m_location, value.x, value.y );
	}
}

void OpenGLShaderProgram::SetVec3UniformValue( const std::string& name, const Vector3& value )
{
	ShaderUniform* uniform = GetUniformByName(name);
	if ( uniform )
	{
		ShaderVec3Uniform* vec3Uniform = dynamic_cast< ShaderVec3Uniform* >(uniform);
		vec3Uniform->m_value = value;
		glUniform3f( vec3Uniform->m_location, value.x, value.y, value.z );
	}
}

void OpenGLShaderProgram::SetVec4UniformValue( const std::string& name, const Vector4& value )
{
	ShaderUniform* uniform = GetUniformByName(name);
	if ( uniform )
	{
		ShaderVec4Uniform* vec4Uniform = dynamic_cast< ShaderVec4Uniform* >(uniform);
		vec4Uniform->m_value = value;
		glUniform4f( vec4Uniform->m_location, value.x, value.y, value.z, value.w );
	}
}

void OpenGLShaderProgram::SetMat4UniformValue( const std::string& name, const Matrix44& value )
{
	ShaderUniform* uniform = GetUniformByName(name);
	if ( uniform )
	{
		ShaderMat4Uniform* mat4Uniform = dynamic_cast< ShaderMat4Uniform* >(uniform);
		if( mat4Uniform )
		{
			mat4Uniform->m_value = value;
			glUniformMatrix4fv( mat4Uniform->m_location, 1, GL_FALSE, value.m_matrix );
		}
	}
}

void OpenGLShaderProgram::SetMat4UniformValue( const std::string& name, unsigned int count, const Matrix44& value )
{
	ShaderUniform* uniform = GetUniformByName(name);
	if ( uniform )
	{
		ShaderMat4Uniform* mat4Uniform = dynamic_cast< ShaderMat4Uniform* >(uniform);
		if( mat4Uniform )
		{
			mat4Uniform->m_value = value;
			glUniformMatrix4fv( mat4Uniform->m_location, count, GL_FALSE, value.m_matrix );
		}
	}
}

void OpenGLShaderProgram::SetVec3ArrayUniformValue( const std::string& name, int arraySize , const float* arrayAddress )
{
	ShaderUniform* uniform = GetUniformByName(name);
	if ( uniform )
	{
		ShaderVec3Uniform* vec3Uniform = dynamic_cast< ShaderVec3Uniform* >(uniform);
		glUniform3fv( vec3Uniform->m_location, arraySize, arrayAddress );
	}
}


