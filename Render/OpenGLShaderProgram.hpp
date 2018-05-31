#ifndef OPENGL_SHADER_PROGRAM_H
#define OPENGL_SHADER_PROGRAM_H
#include <string>
#include <iostream>
#include "OpenGLShader.hpp"
#include "ShaderUniform.hpp"

class OpenGLShaderProgram
{
	public:
		std::vector<ShaderUniform*>		m_uniforms;
		std::string						m_fragmentShaderFullPath;
		std::string						m_shaderProgramName;
		int								m_numberOfShader;
		int								m_shaderProgramID;

	//uniform location
	public:
		int								m_diffuseTextureUniformLoc;
		int								m_normalTextureUniformLoc;
		int								m_emissiveTextureUniformLoc;
		int								m_specularTextureUniformLoc;
		int								m_depthTextureUniformLoc;
		int								m_worldToScreenMatrixLoc;
		int								m_lightWorldPosLoc;
		int								m_timeUniformLoc;
		int								m_deltaTimeUniformLoc;
		int								m_cameraWorldPositionUniformLoc;
		int								m_fogColorUniformLoc;
		int								m_fogStartDistanceUniformLoc;
		int								m_fogEndDistanceUniformLoc;
		int								m_scaleBiasUniformLoc;
		int								m_reflectCameraTransformationMatrixLoc;
		int								m_clipPlaneUniformLoc;
		int								m_modelViewMatrixUniformLoc;

	public:
		OpenGLShaderProgram( const std::string& name, const std::string& vertexShader, const std::string& fragmentShader );
		OpenGLShaderProgram( const std::string& name, const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader );
		~OpenGLShaderProgram();
		void UseShaderProgram();
		void DisableShaderProgram();
		void SetFloatUniformValue( const std::string& name, float value );
		void SetIntUniformValue( const std::string& name, int value );
		void SetVec2UniformValue( const std::string& name, const Vector2& value );
		void SetVec3UniformValue( const std::string& name, const Vector3& value );
		void SetVec3ArrayUniformValue( const std::string& name, int arraySize, const float* arrayAddress );
		void SetVec4UniformValue( const std::string& name, const Vector4& value );
		void SetMat4UniformValue( const std::string& name, const Matrix44& value );
		void SetMat4UniformValue( const std::string& name, unsigned int count, const Matrix44& value );


	private:
		void GetUniformLocation();
		void GetUniformInformation();
		void CreateShaderUniform( const std::string& name, EngineUniformShaderType type, int location, int textureUnit );
		void AttachShader(const OpenGLShader& shader);
		void LinkProgram();
		ShaderUniform* GetUniformByName(const std::string& name);
};


#endif