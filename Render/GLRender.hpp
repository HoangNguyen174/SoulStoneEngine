#ifndef GL_MYRENDER
#define GL_MYRENDER
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "../Utilities/GameCommon.hpp"
#include "../Utilities/MatrixStack.hpp"
#include "../Utilities/AABB2.hpp"
#include "../Debugger/DebugPoint.hpp"
#include "../Render/OpenGLShaderProgram.hpp"
#include "Material.hpp"
#include "Light.hpp"
#include "FBO.hpp"
#include "BitmapFont.hpp"
#include "../Camera/Camera3D.hpp"
#include "../Camera/Camera2D.hpp"

#pragma comment( lib, "opengl32" )
#pragma comment( lib, "glu32")

const int MAX_LIGHT_NUM = 200;

struct SimpleVertex3D
{
	Vector3	m_position;
	RGBColor m_color;
	Vector2 m_texCoords;
	Vector3 m_normal;
	Vector3 m_tangent;
	Vector3 m_bitangent;
};

enum VERTEX_ATTRIB   { VERTEX_ATTRIB_POSITIONS = 0,
					   VERTEX_ATTRIB_COLORS,
					   VERTEX_ATTRIB_TEXCOORDS,
					   VERTEX_ATTRIB_NORMALS,
					   VERTEX_ATTRIB_TANGENT,
					   VERTEX_ATTRIB_BITANGENT,
					   VERTEX_ATTRIB_BONE_INDEX,
					   VERTEX_ATTRIB_BONE_WEIGHT,
					   VERTEX_ATTRIB_IS_STATIC,
					   VERTEX_ATTRIB_TERRAIN_WEIGHT };

class GLRender
{
	friend class GraphicManager;

	//static variable
	public:
		static set<Material*>				s_materialRegisteredList;

	public:
		std::string							m_name;
		RenderType							m_type;
		std::vector<DebugObject*>			m_debugPrimitiveToDraw;
		OpenGLShaderProgram*				m_defaultFontShaderProgram;
		BitmapFont*							m_defaultFont;
		std::vector<Light*>					m_lightList;

	private:
		std::vector<SimpleVertex3D>			m_vertexList;
		unsigned int						m_vboID;
		int									m_vertexListSize;
		MatrixStack							m_matrixStack;
		Matrix44							m_defaultOrthoMatrix;
		Matrix44							m_3dFontRotateMatrix;

	protected:
		GLRender( const std::string& name );
		~GLRender();

	public:
		void BeginDraw(int value);
		void Clear(int mode);
		void EndDraw();
		void Enable(int mode);
		void Disable(int mode);
		void BlendFunc(int mode, int mode1);
		void PushMatrix();
		void PopMatrix();
		void LoadIdentityMatrix();
		void Translatef(float x, float y, float z);
		void Translated(double x, double y, double z);
		void Rotatef(float angle, float x, float y, float z);
		void Rotated(double angle, double x, double y, double z);
		void Scalef(float x, float y, float z);
		void Scaled(double x, double y, double z);
		void BindTexture(int mode, int id);
		void Vertex3f(float x,float y, float z);
		void Vertex3d(double x, double y, double z);
		void Vertex3i(int x,int y,int z);
		void TexCoord2d(double x, double y);
		void TexCoord2f(float x, float y);
		void Color4f(float r, float g, float b, float alpha);
		void Color3f(float r, float g, float b);
		void ClearColor(float r, float g, float b, float alpha);
		void ClearDepth(double value);
		void EnableDepthMask();
		void DisableDepthMask();
		void DeleteBuffers(int size, unsigned int* id);
		void EnableClientState(int mode);
		void DisableClientState(int mode);
		void BindBuffer(int mode, unsigned int id);
		void BindFrameBuffer( int mode, unsigned int id );
		void DrawArray(int mode,int firstIndex, int size);
		void GenerateBuffer(int size, unsigned int* id);
		void PolygonMode(int mode, int mode1);
		void PointSize(float size);
		void VertexPointer(int, int, int,const GLvoid*);
		void ColorPointer(int, int, int,const GLvoid*);
		void TexCoordPointer(int, int, int, const GLvoid*);
		void BufferData(int mode, GLsizeiptr size, const GLvoid*, int mode1 );
		void FrontFace(int mode);
		void LineWidth(float size);
		void UseShaderProgram(int id);
		void DisableShaderProgram();
		void UseDefaultShaderProgram();
		void DisableDefaultShaderProgram();
		void Ortho( double left, double right, double bottom, double top, double near, double far );
		void RenderText( const Vector3& position, const Vector4& axisRotationDeg, const Vector3& scale, const RGBColor& color, BitmapFont* font, OpenGLShaderProgram* shader, const float& cellHeight,  const Camera* camera, const std::string& myStr );
		void DisableAllTextureUnit();
		void ActiveTexture( int textureUnitNumber );
		float CalcTextWidth(BitmapFont* font, const float& cellHeight, const std::string& myStr);
		void ComputeSurfaceTangentsAtVertex( Vector3& surfaceTangentAtVertex_out,
											  Vector3& surfaceBitangentAtVertex_out,
											  const Vector3& normalAtThisVertex,
											  const Vector3& positionOfThisVertex,
											  const Vector2& texCoordsOfThisVertex,
											  const Vector3& positionOfPreviousAdjacentVertex,
											  const Vector2& texCoordsOfPreviousAdjacentVertex,
											  const Vector3& positionOfNextAdjacentVertex,
											  const Vector2& texCoordsOfNextAdjacentVertex );

		//light
		void RenderLightSource();
		void CreateInfinitePointLight( const Vector3& position, const Vector3& direction, const RGBColor& color );
		void CreateAttenuatedPointLight( const Vector3& position, const Vector3& direction, const RGBColor& color, float innerRadius, float outerRadius );
		void CreateAttenuatedSpotLight( const Vector3& position, const Vector3& direction, const RGBColor& color, float innerRadius, float outerRadius, float m_innerApertureDeg, float m_outerApertureDegs );
		void CreateInfiniteSpotLight( const Vector3& position, const Vector3& direction, const RGBColor& color, float m_innerApertureDeg, float m_outerApertureDeg );
		void CreateDirectionalLight( const Vector3& position, const Vector3& direction, const RGBColor& color, float ambientness ); 
		void SetUniformValueOfLightToShader(int shaderProgramID);

		//draw primitives
		void Draw3DSquare( const Vector3& center, float length, const RGBColor& color );
		void Draw3DRectangle( const Vector3& center, float width, float depth, float height, const RGBColor& color );
		void Draw2DFilledCircle( const Vector2& center, float radius, const RGBColor& color );
		void Draw2DFilledRectangle( const Vector2& bottomLeft, float width, float height, const RGBColor& color );
		void Draw2DHollowCircle( const Vector2& center, float radius, const RGBColor& color );
		void Draw2DHollowRectangle( const Vector2& bottomLeft, float width, float height, const RGBColor& color );
		void Draw2DPartOfFilledCircle( const Vector2& center, float radius, const RGBColor& color, float percent );
		void Draw2DPartOfHollowCircle( const Vector2& center, float radius, const RGBColor& color, float percent );
		void DrawAABB2( const AABB2& aabb2, const RGBColor& color );

	private:
		void CreateVBOAndDrawFont(const Vector3& position, const Vector4& axisRotationDeg, const Vector3& scale, const RGBColor& color, BitmapFont* font, OpenGLShaderProgram* shader, const float& cellHeight, const Camera* camera, const std::string& myStr);
		static void InitializeAdvancedOpenGLFunctions();
};

/*extern GLRender* GraphicManager::s_render;*/

extern PFNGLGENBUFFERSPROC					glGenBuffers;
extern PFNGLBINDBUFFERPROC					glBindBuffer;
extern PFNGLBUFFERDATAPROC					glBufferData;
extern PFNGLGENERATEMIPMAPPROC				glGenerateMipmap;
extern PFNGLDELETEBUFFERSPROC				glDeleteBuffers;
extern PFNGLSHADERSOURCEPROC				glShaderSource;
extern PFNGLCOMPILESHADERPROC				glCompileShader;
extern PFNGLGETSHADERIVPROC					glGetShaderiv;
extern PFNGLCREATEPROGRAMPROC				glCreateProgram;
extern PFNGLCREATESHADERPROC				glCreateShader;
extern PFNGLLINKPROGRAMPROC					glLinkProgram;
extern PFNGLGETPROGRAMIVPROC				glGetProgramiv;
extern PFNGLUSEPROGRAMPROC					glUseProgram;
extern PFNGLUNIFORM1FPROC					glUniform1f;
extern PFNGLUNIFORM1IPROC					glUniform1i;
extern PFNGLGETUNIFORMLOCATIONPROC			glGetUniformLocation;
extern PFNGLDELETESHADERPROC				glDeleteShader;
extern PFNGLATTACHSHADERPROC				glAttachShader;
extern PFNGLGETSHADERINFOLOGPROC			glGetShaderInfoLog;
extern PFNGLDELETEPROGRAMPROC				glDeleteProgram;
extern PFNGLACTIVETEXTUREPROC				glActiveTexture;
extern PFNGLBINDATTRIBLOCATIONPROC			glBindAttribLocation;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC		glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC			glVertexAttribPointer;
extern PFNGLGETATTRIBLOCATIONPROC			glGetAttribLocation;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC	glDisableVertexAttribArray;
extern PFNGLUNIFORMMATRIX4FVPROC			glUniformMatrix4fv;
extern PFNGLUNIFORM3FPROC					glUniform3f;
extern PFNGLUNIFORM4FPROC					glUniform4f;
extern PFNGLUNIFORM2FPROC					glUniform2f;
extern PFNGLUNIFORM4FVPROC					glUniform4fv;
extern PFNGLUNIFORM3FVPROC					glUniform3fv;
extern PFNGLUNIFORM2FVPROC					glUniform2fV;
extern PFNGLUNIFORM1FVPROC					glUniform1fv;
extern PFNGLUNIFORM1IVPROC					glUniform1iv;
extern PFNGLBINDFRAMEBUFFERPROC				glBindFramebuffer;
extern PFNGLGENFRAMEBUFFERSPROC				glGenFramebuffers;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC		glFramebufferTexture2D;
extern PFNGLGETACTIVEUNIFORMPROC			glGetActiveUniform;
extern PFNGLDRAWBUFFERSPROC					glDrawBuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC		glCheckFramebufferStatus;
extern PFNGLBLITFRAMEBUFFERPROC				glBlitFramebuffer;
extern PFNGLBLENDEQUATIONPROC				glBlendEquation;
extern PFNGLSTENCILOPSEPARATEPROC			glStencilOpSeparate;

#endif