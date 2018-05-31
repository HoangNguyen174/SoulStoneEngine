#ifndef GRAPHIC_TEXT
#define GRAPHIC_TEXT
#include "GraphicManager.hpp"

class GraphicText
{
	public:
		Vector3							m_bottomLeft;
		Vector4							m_rotateAxisAndAngleDeg;
		Vector3							m_scale;
		float							m_cellHeight;
		float							m_contentLength;
		RGBColor						m_color;
		std::string						m_content;
		BitmapFont*						m_font;
		const Camera*					m_camera;
		Matrix44						m_modelMatrix;
		OpenGLShaderProgram*			m_shaderProgram;
		bool							m_isVboDirty;

	private:
		unsigned int					m_vboID;
		std::vector<SimpleVertex3D>     m_vertices;
		static OpenGLShaderProgram*		m_defaultShaderProgram;
		static BitmapFont*				m_defaultFont;
		static Matrix44					m_3dFontRotateMatrix;
		static Matrix44					m_defaultOrthoMatrix;

	public:
		GraphicText( const Vector3& bottomLeft, const Vector4& rotateAxisAndAngleDeg, const Vector3& scale, const RGBColor& color, BitmapFont* font, OpenGLShaderProgram* shader, const float& cellHeight, const Camera* camera, const std::string& myStr );
		~GraphicText();
		void Render();

	private:
		void RenderVBO();
		void CreateVBO();
		void CreateVertices();
		void CalculateModelMatrix();
};

#endif