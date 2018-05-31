#ifndef MATERIAL_H
#define MATERIAL_H
#include "../Utilities/Texture.hpp"
#include "../Utilities/Vector3.h"
#include "../Utilities/Vector4.hpp"
#include "../Utilities/RGBColor.hpp"
#include "OpenGLShaderProgram.hpp"
#include <string>

class Material
{
	public:
		Texture*				m_diffuseTexture;
		Texture*				m_normalTexture;
		Texture*				m_emissiveTexture;
		Texture*				m_specularTexture;
		Texture*				m_depthTexture;
		std::string				m_materialName;
			
	public:
		Material( const std::string& name );
		void SetUniformValuesToShaderProgram( OpenGLShaderProgram* shaderProgram );
		void BindTexture();
};

#endif