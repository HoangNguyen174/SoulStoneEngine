#ifndef GRAPHIC_MANAGER_H
#define GRAPHIC_MANAGER_H
#include "GLRender.hpp"
#include "GBuffer.hpp"
#include "../Camera/Camera3D.hpp"
#include "../Camera/Camera2D.hpp"
#include "../ModelSystem/Scene.hpp"
#include "../ModelSystem/IcoSphere.hpp"
#include "../ModelSystem/3DShapes.hpp"

class GraphicManager
{
	friend class Camera3D;
	friend class Camera2D;
	friend class FBO;
	friend class Scene;

	public:
		static GLRender*							s_render;
		static OpenGLShaderProgram*					s_fboShaderProgram;
		static SimpleVertex3D						s_screenVertices[4];
		static Matrix44*							s_currentViewMatrix;
		static Matrix44*							s_currentProjectionMatrix;

	protected:
		static std::map< std::string, Camera3D* >	s_3DCameraRegistry;
		static std::map< std::string, Camera2D* >	s_2DCameraRegistry;
		static std::map< std::string, Camera* >		s_cameraRegistry;
		static std::map< std::string, GLRender* >	s_renderRegistry;
		static std::map< std::string, FBO* >		s_FBORegistry;
		static std::map< std::string, Scene* >		s_sceneRegistry;

	private:
		static GraphicManager*						s_instance;
		static Matrix44								s_orthoMatrix;

		//deferred rendering
		static GBuffer*								s_gBuffer;
		static FBO*									s_shadowMapBuffer;
		static FBO*									s_ssaoBuffer;
		static FBO*									s_blurSSAOBuffer;
		static OpenGLShaderProgram*					s_geometryPassShader;
		static OpenGLShaderProgram*					s_simpleShader;
		static OpenGLShaderProgram*					s_pointLightPassShader;
		static OpenGLShaderProgram*					s_directionalLightPassShader;
		static OpenGLShaderProgram*					s_shadowMapShader;
		static OpenGLShaderProgram*					s_ssaoShader;
		static OpenGLShaderProgram*					s_blurShader;

		static IcoSphere*							s_unitIcoSphere;
		static ScreenRectangle*						s_screenQuad;
		static const Camera*						s_activeCamera;

		static std::vector<Vector3>					s_ssaoKernel;
		static unsigned int							s_ssaoNoiseTextureID;

	public:
		static GraphicManager* Instance();
		static void Initialize();
		void CreateOpenGLRender( const std::string& name );
		void ActivateRender( const std::string& name );
		std::string GetCurrentRenderName();
		static void RenderFBOToScreenOrFBO( const FBO* destFBO, const FBO* srcFBO, int enableBlur, const Vector2& blurDirection );
		Camera*	GetActiveCamera();
		void	SetActiveCamera( const Camera* camera );

		//scene
		static void RenderSceneByName( const std::string& name );
		static void UpdateSceneByName( const std::string& name, float elapsedTime );

	private:
		GraphicManager();
		GraphicManager( const GraphicManager& );
		~GraphicManager();
		GraphicManager& operator=( const GraphicManager& );
		static void CreateScreenVertices();
		static void InitializeDeferredRenderPipeline();
		static void CalculateDirectionalLightViewMatrix( Matrix44& viewMatrix );

		//Deferred Rendering
		static void GenerateSSAOKernel();
		static void GenerateSSAONoiseTexture();
		static void GeometryPass( Scene* scene );
		static void PointLightPass();
		static void DirectionalLightPass();
		static void StencilPass();
		static void FinalPass();
		static void ShadowPass( Scene* scene );
		static void SSAOPass();
		static void BlurSSAOPass();
};

#endif