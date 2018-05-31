#include "GLRender.hpp"
#include "Material.hpp"
#include "../JobSystem/MemoryPoolManager.hpp"

Material::Material( const std::string& name )
{
	m_materialName = name;
	m_diffuseTexture = nullptr;
	m_normalTexture = nullptr;
	m_emissiveTexture = nullptr;
	m_specularTexture = nullptr;
	m_depthTexture = nullptr;
}

void Material::SetUniformValuesToShaderProgram( OpenGLShaderProgram* shaderProgram )
{
	shaderProgram->SetIntUniformValue( "u_diffuseTexture", 0 );
 	shaderProgram->SetIntUniformValue( "u_normalTexture", 1 );
 	shaderProgram->SetIntUniformValue( "u_specularTexture", 2 );
 	shaderProgram->SetIntUniformValue( "u_emissiveTexture", 3 );
 	shaderProgram->SetIntUniformValue( "u_depthTexture", 4 );

}

void Material::BindTexture()
{
	if( m_diffuseTexture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_diffuseTexture->m_openglTextureID);
	}

	if( m_normalTexture != nullptr )
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_normalTexture->m_openglTextureID);
	}

	if( m_specularTexture != nullptr )
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_specularTexture->m_openglTextureID);
	}

	if( m_emissiveTexture != nullptr)
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_emissiveTexture->m_openglTextureID);
	}

	if( m_depthTexture != nullptr )
	{
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture->m_openglTextureID);
	}
}