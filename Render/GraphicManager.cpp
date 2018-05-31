#include "GraphicManager.hpp"
#include <random>

GraphicManager*						GraphicManager::s_instance = nullptr;
GLRender*							GraphicManager::s_render = nullptr;
std::map< std::string, Camera3D* >	GraphicManager::s_3DCameraRegistry;
std::map< std::string, Camera2D* >	GraphicManager::s_2DCameraRegistry;
std::map< std::string, Camera* >	GraphicManager::s_cameraRegistry;
std::map< std::string, GLRender* >	GraphicManager::s_renderRegistry;
std::map< std::string, FBO* >		GraphicManager::s_FBORegistry;
std::map< std::string, Scene* >		GraphicManager::s_sceneRegistry;
SimpleVertex3D						GraphicManager::s_screenVertices[4];
Matrix44							GraphicManager::s_orthoMatrix;
Matrix44*							GraphicManager::s_currentProjectionMatrix = nullptr;
Matrix44*							GraphicManager::s_currentViewMatrix = nullptr;
OpenGLShaderProgram*				GraphicManager::s_fboShaderProgram = nullptr;
GBuffer*							GraphicManager::s_gBuffer = nullptr;
FBO*								GraphicManager::s_shadowMapBuffer = nullptr;
FBO*								GraphicManager::s_ssaoBuffer = nullptr;
FBO*								GraphicManager::s_blurSSAOBuffer = nullptr;
OpenGLShaderProgram*				GraphicManager::s_geometryPassShader = nullptr;
OpenGLShaderProgram*				GraphicManager::s_simpleShader = nullptr;
OpenGLShaderProgram*				GraphicManager::s_pointLightPassShader = nullptr;
OpenGLShaderProgram*				GraphicManager::s_directionalLightPassShader = nullptr;
OpenGLShaderProgram*				GraphicManager::s_shadowMapShader = nullptr;
OpenGLShaderProgram*				GraphicManager::s_ssaoShader = nullptr;
OpenGLShaderProgram*				GraphicManager::s_blurShader = nullptr;
IcoSphere*							GraphicManager::s_unitIcoSphere = nullptr;
const Camera*						GraphicManager::s_activeCamera = nullptr;
std::vector<Vector3>				GraphicManager::s_ssaoKernel;
unsigned int						GraphicManager::s_ssaoNoiseTextureID = 0;
ScreenRectangle*					GraphicManager::s_screenQuad = nullptr;

//#define DEBUG_SHADOW_MAP
//#define DEBUG_SSAO
//#define DEBUG_GEOMETRY_BUFFER
//#define DEBUG_BLUR_SSAO

GraphicManager::GraphicManager()
{
	s_render = nullptr;
}

GraphicManager::GraphicManager( const GraphicManager& )
{

}

GraphicManager* GraphicManager::Instance()
{
	if( s_instance == nullptr )
		s_instance = new GraphicManager();
	return s_instance;
}

void GraphicManager::Initialize()
{
	bool somethingWrong = false;

	GLRender::InitializeAdvancedOpenGLFunctions();
	InitializeDeferredRenderPipeline();

	s_fboShaderProgram = new OpenGLShaderProgram( "Display FBO Shader", "./Data/Shader/FBOVertexShader.glsl","./Data/Shader/FBOFragmentShader.glsl" );
	s_orthoMatrix = Matrix44::CreateOrthoMatrix( 0.f, WINDOW_PHYSICAL_WIDTH, 0.f, WINDOW_PHYSICAL_HEIGHT, 0.f, 1.f );
	CreateScreenVertices();

	if( somethingWrong )
	{
		DebuggerPrintf( "Fail to Initialize Graphic Manager.\n" );
		exit(0);
	}
}

void GraphicManager::InitializeDeferredRenderPipeline()
{
	s_geometryPassShader = new OpenGLShaderProgram( "Geometry Pass Shader", "./Data/Shader/GeometryPassVertexShader.glsl", "./Data/Shader/GeometryPassFragmentShader.glsl");
	s_simpleShader = new OpenGLShaderProgram( "Basic Shader", "./Data/Shader/SimpleVertexShader.glsl", "./Data/Shader/EmptyFragmentShader.glsl" );
	s_pointLightPassShader = new OpenGLShaderProgram( "Point Light Pass Shader", "./Data/Shader/PointLightPassVertexShader.glsl", "./Data/Shader/PointLightPassFragmentShader.glsl" );
	s_directionalLightPassShader = new OpenGLShaderProgram( "Directional Light Pass Shader", "./Data/Shader/DirectionalLightPassVertexShader.glsl", "./Data/Shader/DirectionalLightPassFragmentShader.glsl" );
	s_shadowMapShader = new OpenGLShaderProgram( "Shadow Map Shader", "./Data/Shader/GeometryPassVertexShader.glsl", "./Data/Shader/EmptyFragmentShader.glsl" );
	s_ssaoShader = new OpenGLShaderProgram( "SSAO Shader", "./Data/Shader/SSAOVertexShader.glsl", "./Data/Shader/SSAOFragmentShader.glsl" );
	s_blurShader = new OpenGLShaderProgram( "Blur Shader",  "./Data/Shader/BlurVertexShader.glsl", "./Data/Shader/BlurFragmentShader.glsl" );
	s_gBuffer = new GBuffer( "GBuffer" );
	s_ssaoBuffer = FBO::CreateOrGetFBOByName( "SSAO Buffer" );
	s_shadowMapBuffer = FBO::CreateOrGetFBOByName( "Shadow Map Buffer" );
	s_blurSSAOBuffer = FBO::CreateOrGetFBOByName( "Blur SSAO Buffer" );

	s_unitIcoSphere = new IcoSphere( Vector3( 0.f, 0.f, 0.f ), 1.f, 10 );

	s_screenQuad = new ScreenRectangle();

	GenerateSSAOKernel();
	GenerateSSAONoiseTexture();
}

void GraphicManager::CreateScreenVertices()
{
	SimpleVertex3D vertex;

	vertex.m_color = RGBColor::White();
	vertex.m_texCoords = Vector2( 0.0f, 0.0f ); 
	vertex.m_position = Vector3( 0.f, 0.f, 0.f );
	s_screenVertices[0] = vertex;

	vertex.m_texCoords = Vector2( 1.0f, 0.0f ); 
	vertex.m_position = Vector3( (float)WINDOW_PHYSICAL_WIDTH, 0.f, 0.f );
	s_screenVertices[1] = vertex;

	vertex.m_texCoords = Vector2( 1.0f, 1.0f ); 
	vertex.m_position = Vector3( (float)WINDOW_PHYSICAL_WIDTH , (float)WINDOW_PHYSICAL_HEIGHT, 0.f  );
	s_screenVertices[2] = vertex;

	vertex.m_texCoords = Vector2( 0.f, 1.f ); 
	vertex.m_position = Vector3( 0.f, (float)WINDOW_PHYSICAL_HEIGHT, 0.f );
	s_screenVertices[3] = vertex;
}

GraphicManager& GraphicManager::operator=( const GraphicManager& /*other*/ )
{
	//fix later
	GraphicManager* ret = new GraphicManager();
	return *ret;
}

void GraphicManager::CreateOpenGLRender( const std::string& name )
{
	GLRender* render = nullptr;

	auto found = s_renderRegistry.find( name );
	if( found == s_renderRegistry.end() )
	{
		render = new GLRender( name );
		s_renderRegistry[ name ] = render;
		DebuggerPrintf( "Create OpenGL Render with name - %s - successfully.\n", name.c_str() );
	}
	else
	{
		DebuggerPrintf( "OpenGL Render with name - %s - already exists.\n", name.c_str() );
	}
}

void GraphicManager::ActivateRender( const std::string& name )
{
	auto found = s_renderRegistry.find( name );
	if( found != s_renderRegistry.end() )
	{
		s_render = s_renderRegistry[ name ];
		DebuggerPrintf( "Activate OpenGL Render with name - %s - successfully.\n", name.c_str() );
		return;
	}

	DebuggerPrintf( "Fail to Activate OpenGL Render with Name - %s -.\n", name.c_str() );
}

std::string GraphicManager::GetCurrentRenderName()
{
	if( s_render != nullptr )
		return s_render->m_name;
	else
		return "ERROR";
}

void GraphicManager::RenderFBOToScreenOrFBO ( const FBO* destFBO, const FBO* srcFBO, int enableBlur, const Vector2& blurDirection )
{
	if( destFBO == nullptr )
		s_render->BindFrameBuffer( GL_FRAMEBUFFER, 0 );
	else
		s_render->BindFrameBuffer( GL_FRAMEBUFFER, destFBO->m_fboID );

	glClearColor( 0.f, 0.f, 0.f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glClearDepth( 1.f );
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_TEXTURE_2D );
	glDepthMask( GL_FALSE );

	s_render->UseShaderProgram( s_fboShaderProgram->m_shaderProgramID );

	static float hackTime = 0.f;
	hackTime += 0.017f;

	s_fboShaderProgram->SetIntUniformValue( "u_colorTexture", 0 );
	s_fboShaderProgram->SetIntUniformValue( "u_depthTexture", 1 );
	s_fboShaderProgram->SetMat4UniformValue( "u_WorldToScreenMatrix", s_orthoMatrix.m_matrix );
	s_fboShaderProgram->SetFloatUniformValue( "u_time", hackTime );
	s_fboShaderProgram->SetIntUniformValue( "u_enableBlur", enableBlur );
	s_fboShaderProgram->SetVec2UniformValue( "u_blurDirection", blurDirection );
	s_fboShaderProgram->SetVec2UniformValue( "u_resolution", Vector2( WINDOW_PHYSICAL_WIDTH, WINDOW_PHYSICAL_HEIGHT ) );

	s_render->Enable( GL_TEXTURE_2D );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, srcFBO->m_fboColorTextureID );

	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, srcFBO->m_fboBufferDepthTextureID );

	static unsigned int id = 0;
	if( id == 0 )
	{
		s_render->GenerateBuffer( 1, &id );
	}

	s_render->BindBuffer( GL_ARRAY_BUFFER, id );
	s_render->BufferData( GL_ARRAY_BUFFER, sizeof( SimpleVertex3D ) * 4, s_screenVertices, GL_STATIC_DRAW );

	glEnableVertexAttribArray( VERTEX_ATTRIB_POSITIONS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_COLORS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_TEXCOORDS );
	s_render->BindBuffer( GL_ARRAY_BUFFER, id );

	glVertexAttribPointer( VERTEX_ATTRIB_POSITIONS, 3, GL_FLOAT, GL_FALSE, sizeof( SimpleVertex3D ), ( const GLvoid* ) offsetof( SimpleVertex3D,m_position ) );
	glVertexAttribPointer( VERTEX_ATTRIB_COLORS, 4, GL_FLOAT, GL_FALSE, sizeof( SimpleVertex3D ), ( const GLvoid* ) offsetof( SimpleVertex3D,m_color ) );
	glVertexAttribPointer( VERTEX_ATTRIB_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof( SimpleVertex3D ), ( const GLvoid* ) offsetof( SimpleVertex3D,m_texCoords ) );

	s_render->DrawArray( GL_QUADS, 0, 4 );

	glDisableVertexAttribArray( VERTEX_ATTRIB_POSITIONS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_COLORS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_TEXCOORDS );
	s_render->BindBuffer( GL_ARRAY_BUFFER, 0 );
	s_render->Disable( GL_TEXTURE_2D );
	s_render->Disable( GL_DEPTH_TEST );

	s_render->DisableShaderProgram();

	glDisable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );
}

void GraphicManager::RenderSceneByName( const std::string& name )
{
	auto found = s_sceneRegistry.find( name );
	if( found == s_sceneRegistry.end() )
	{
		DebuggerPrintf( "Cannot find scene with name - %s -.\n", name.c_str() );
		return;
	}
	Scene* scene = s_sceneRegistry[ name ];

	s_gBuffer->ClearFinalTexture();

	ShadowPass( scene );

#ifdef DEBUG_SHADOW_MAP
	return;
#endif

	GeometryPass( scene );
	
#ifdef DEBUG_GEOMETRY_BUFFER
	return;
#endif

	SSAOPass();

#ifdef DEBUG_SSAO
	return;
#endif

	BlurSSAOPass();

#ifdef DEBUG_BLUR_SSAO
	return;
#endif

	s_gBuffer->BindForGeometryPass();

 	glEnable( GL_STENCIL_TEST );
 
 	StencilPass();
 	PointLightPass();
  
 	glDisable( GL_STENCIL_TEST );
 
 	DirectionalLightPass();

	FinalPass();
}

void GraphicManager::GeometryPass( Scene* scene )
{
	s_gBuffer->BindForGeometryPass();

	GraphicManager::s_render->EnableDepthMask();
	GraphicManager::s_render->Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	GraphicManager::s_render->Enable( GL_DEPTH_TEST );
	GraphicManager::s_render->Disable( GL_BLEND );

	scene->m_shaderProgram = s_geometryPassShader;
	//scene->m_uniformScale = 0.1f;
	
	scene->m_shaderProgram->UseShaderProgram();

	scene->m_shaderProgram->SetMat4UniformValue( "u_projectionMatrix", ( *s_currentProjectionMatrix ) );
	scene->m_shaderProgram->SetMat4UniformValue( "u_viewMatrix", ( *s_currentViewMatrix ) );

// 	scene->m_shaderProgram->SetFloatUniformValue( "u_fogEndDistance", 5.f );
// 	scene->m_shaderProgram->SetFloatUniformValue( "u_fogStartDistance", 2.f );
// 	scene->m_shaderProgram->SetVec2UniformValue( "u_scaleBias", Vector2( 0.04f, 0.03f ) );
// 	scene->m_shaderProgram->SetVec4UniformValue( "u_fogColor", Vector4( 1.f, 1.f, 1.f, 0.f ) );

//	scene->RenderSceneAtFrame( 0 );
	scene->Render();

	scene->m_shaderProgram->DisableShaderProgram();

	GraphicManager::s_render->DisableDepthMask();

#ifdef DEBUG_GEOMETRY_BUFFER
	GraphicManager::s_render->BindFrameBuffer( GL_FRAMEBUFFER, 0 );

	s_gBuffer->BindForReading();

	float halfWidth = WINDOW_PHYSICAL_WIDTH * 0.5f;
	float halfHeight = WINDOW_PHYSICAL_HEIGHT * 0.5f;

	s_gBuffer->SetReadBuffer( GBUFFER_TEXTURE_TYPE_POSITION );
	glBlitFramebuffer( 0, 0, WINDOW_PHYSICAL_WIDTH, WINDOW_PHYSICAL_HEIGHT, 0, 0, halfWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR );
	
	s_gBuffer->SetReadBuffer( GBUFFER_TEXTURE_TYPE_NORMAL );
	glBlitFramebuffer( 0, 0, WINDOW_PHYSICAL_WIDTH, WINDOW_PHYSICAL_HEIGHT, halfWidth, 0, WINDOW_PHYSICAL_WIDTH, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR );

	s_gBuffer->SetReadBuffer( GBUFFER_TEXTURE_TYPE_DIFFUSE );
	glBlitFramebuffer( 0, 0, WINDOW_PHYSICAL_WIDTH, WINDOW_PHYSICAL_HEIGHT, halfWidth, halfHeight, WINDOW_PHYSICAL_WIDTH, WINDOW_PHYSICAL_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR );

	s_gBuffer->SetReadBuffer( GBUFFER_TEXTURE_TYPE_NORMAL_VIEW );
	glBlitFramebuffer( 0, 0, WINDOW_PHYSICAL_WIDTH, WINDOW_PHYSICAL_HEIGHT, 0, halfHeight, halfWidth, WINDOW_PHYSICAL_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR );
#endif

}

void GraphicManager::PointLightPass()
{
//	GraphicManager::s_render->BindFrameBuffer( GL_FRAMEBUFFER, 0 );

	s_gBuffer->BindForLightPass();

	glStencilFunc(GL_NOTEQUAL, 0, 0xFF );

	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendEquation( GL_FUNC_ADD );
	glBlendFunc( GL_ONE, GL_ONE );
//	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	GraphicManager::s_render->Enable( GL_CULL_FACE );
	glCullFace( GL_FRONT );

	GraphicManager::s_render->Clear( GL_COLOR_BUFFER_BIT );

//	s_pointLightPassShader = s_simpleShader;

	s_pointLightPassShader->UseShaderProgram();

	s_pointLightPassShader->SetMat4UniformValue( "u_viewMatrix", *s_currentViewMatrix );
	s_pointLightPassShader->SetMat4UniformValue( "u_projectionMatrix", *s_currentProjectionMatrix );
	s_pointLightPassShader->SetFloatUniformValue( "u_viewportWidth", WINDOW_PHYSICAL_WIDTH );
	s_pointLightPassShader->SetFloatUniformValue( "u_viewportHeight", WINDOW_PHYSICAL_HEIGHT );

	s_pointLightPassShader->SetIntUniformValue( "u_positionTexture", 0 );
	s_pointLightPassShader->SetIntUniformValue( "u_normalTexture", 1 );
	s_pointLightPassShader->SetIntUniformValue( "u_diffuseTexture", 2 );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, s_gBuffer->m_textureIDs[ GBUFFER_TEXTURE_TYPE_POSITION ] );

	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, s_gBuffer->m_textureIDs[ GBUFFER_TEXTURE_TYPE_NORMAL ] );

	glActiveTexture( GL_TEXTURE2 );
	glBindTexture( GL_TEXTURE_2D, s_gBuffer->m_textureIDs[ GBUFFER_TEXTURE_TYPE_DIFFUSE ] );
	
	for( unsigned int i = 0;  i < GraphicManager::s_render->m_lightList.size(); i++ )
	{
		Light* light = GraphicManager::s_render->m_lightList[i];

		if( light->m_lightType != ATTENUATED_POINT_LIGHT )
			continue;

		Matrix44 modelMatrix;
		Matrix44 scale;
		Matrix44 translation;

		scale = Matrix44::CreateUniformScaleMatrix( light->m_outerRadius );
		translation = Matrix44::CreateTranslationMatrix( light->m_position );
		modelMatrix = translation * scale;

		s_pointLightPassShader->SetMat4UniformValue( "u_modelMatrix", modelMatrix );
		s_pointLightPassShader->SetFloatUniformValue( "u_lightOuterRadius", light->m_outerRadius );
		s_pointLightPassShader->SetFloatUniformValue( "u_lightInnerRadius", light->m_innerRadius );
		s_pointLightPassShader->SetVec3UniformValue( "u_lightPosition", light->m_position );
		Vector4 colorAndBrightness( light->m_colorAndBrightness.m_red, light->m_colorAndBrightness.m_green, light->m_colorAndBrightness.m_blue, light->m_colorAndBrightness.m_alpha );
		s_pointLightPassShader->SetVec4UniformValue( "u_lightColorAndBrightness", colorAndBrightness );
		s_pointLightPassShader->SetMat4UniformValue( "u_modelMatrix", modelMatrix );

		s_unitIcoSphere->Render();
	}

	s_pointLightPassShader->DisableShaderProgram();
	
	glCullFace( GL_BACK );
	glDisable( GL_BLEND );
}

void GraphicManager::StencilPass()
{
	s_gBuffer->BindForStencilPass();

	glEnable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );
	glClear( GL_STENCIL_BUFFER_BIT );

	glStencilFunc( GL_ALWAYS, 0, 0 );

	glStencilOpSeparate( GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP );
	glStencilOpSeparate( GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP );

	s_simpleShader->UseShaderProgram();

	s_simpleShader->SetMat4UniformValue( "u_viewMatrix", *s_currentViewMatrix );
	s_simpleShader->SetMat4UniformValue( "u_projectionMatrix", *s_currentProjectionMatrix );

	for( unsigned int i = 0;  i < GraphicManager::s_render->m_lightList.size(); i++ )
	{
		Light* light = GraphicManager::s_render->m_lightList[i];

		if( light->m_lightType != ATTENUATED_POINT_LIGHT )
			continue;

		Matrix44 modelMatrix;
		Matrix44 scale;
		Matrix44 translation;

		scale = Matrix44::CreateUniformScaleMatrix( light->m_outerRadius );
		translation = Matrix44::CreateTranslationMatrix( light->m_position );
		modelMatrix = translation * scale;

		s_simpleShader->SetMat4UniformValue( "u_modelMatrix", modelMatrix );

		s_unitIcoSphere->Render();
	}

	s_simpleShader->DisableShaderProgram();
}

void GraphicManager::DirectionalLightPass()
{
	s_gBuffer->BindForLightPass();

	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendEquation( GL_FUNC_ADD );
	glBlendFunc( GL_ONE, GL_ONE );

	s_directionalLightPassShader->UseShaderProgram();

	Matrix44 identityMatrix;
	s_directionalLightPassShader->SetMat4UniformValue( "u_modelMatrix", identityMatrix );
	s_directionalLightPassShader->SetMat4UniformValue( "u_viewMatrix", identityMatrix );
	s_directionalLightPassShader->SetMat4UniformValue( "u_projectionMatrix", identityMatrix );
	s_directionalLightPassShader->SetFloatUniformValue( "u_viewportWidth", WINDOW_PHYSICAL_WIDTH );
	s_directionalLightPassShader->SetFloatUniformValue( "u_viewportHeight", WINDOW_PHYSICAL_HEIGHT );
	
	Matrix44 projection = Matrix44::CreateOrthoMatrix( -400.f, 400.f, -225.f, 225.f, 0.1f, 500.f );
//	s_directionalLightPassShader->SetMat4UniformValue( "u_lightProjectionMatrix", *s_currentProjectionMatrix );
	s_directionalLightPassShader->SetMat4UniformValue( "u_lightProjectionMatrix", projection );


	Matrix44 lightViewMatrix;
	CalculateDirectionalLightViewMatrix( lightViewMatrix );

	s_directionalLightPassShader->SetMat4UniformValue( "u_lightViewMatrix", lightViewMatrix );

	const Camera3D* camera = dynamic_cast<const Camera3D*>(s_activeCamera);
	s_directionalLightPassShader->SetVec3UniformValue( "u_cameraPosition", camera->m_cameraPosition );

	s_directionalLightPassShader->SetIntUniformValue( "u_positionTexture", 0 );
	s_directionalLightPassShader->SetIntUniformValue( "u_normalTexture", 1 );
	s_directionalLightPassShader->SetIntUniformValue( "u_diffuseTexture", 2 );
	s_directionalLightPassShader->SetIntUniformValue( "u_shadowTexture", 3 );
	s_directionalLightPassShader->SetIntUniformValue( "u_ssaoTexture", 4 );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, s_gBuffer->m_textureIDs[ GBUFFER_TEXTURE_TYPE_POSITION ] );

	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, s_gBuffer->m_textureIDs[ GBUFFER_TEXTURE_TYPE_NORMAL ] );

	glActiveTexture( GL_TEXTURE2 );
	glBindTexture( GL_TEXTURE_2D, s_gBuffer->m_textureIDs[ GBUFFER_TEXTURE_TYPE_DIFFUSE ] );

	glActiveTexture( GL_TEXTURE3 );
	glBindTexture( GL_TEXTURE_2D, s_shadowMapBuffer->m_fboBufferDepthTextureID );

	glActiveTexture( GL_TEXTURE4 );
	glBindTexture( GL_TEXTURE_2D, s_blurSSAOBuffer->m_fboColorTextureID );

	for( unsigned int i = 0;  i < GraphicManager::s_render->m_lightList.size(); i++ )
	{
		Light* light = GraphicManager::s_render->m_lightList[i];

		if( light->m_lightType != DIRECTIONAL_LIGHT )
			continue;

		Vector4 colorAndBrightness( light->m_colorAndBrightness.m_red, light->m_colorAndBrightness.m_green, light->m_colorAndBrightness.m_blue, light->m_colorAndBrightness.m_alpha );
		s_directionalLightPassShader->SetVec4UniformValue( "u_lightColorAndBrightness", colorAndBrightness );
		s_directionalLightPassShader->SetFloatUniformValue( "u_lightAmbientness", light->m_ambientness );
		s_directionalLightPassShader->SetVec3UniformValue( "u_lightDirection", light->m_forwardDirection );

		s_screenQuad->Render();
	}

	s_directionalLightPassShader->DisableShaderProgram();

	glDisable( GL_BLEND );
}

void GraphicManager::FinalPass()
{
	s_gBuffer->BindForFinalPass();
	glBlitFramebuffer(0, 0, WINDOW_PHYSICAL_WIDTH, WINDOW_PHYSICAL_HEIGHT, 
					  0, 0, WINDOW_PHYSICAL_WIDTH, WINDOW_PHYSICAL_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

}

void GraphicManager::ShadowPass( Scene* scene )
{
	s_shadowMapBuffer->BindForWriting();

	GraphicManager::s_render->EnableDepthMask();
	GraphicManager::s_render->Clear( GL_DEPTH_BUFFER_BIT );
	GraphicManager::s_render->Enable( GL_DEPTH_TEST );
	GraphicManager::s_render->Enable( GL_CULL_FACE );

	glCullFace( GL_FRONT );

	Matrix44 lightViewMatrix;
	
	CalculateDirectionalLightViewMatrix( lightViewMatrix );

	scene->m_shaderProgram = s_shadowMapShader;

	scene->m_shaderProgram->UseShaderProgram();

	Matrix44 projection = Matrix44::CreateOrthoMatrix( -400.f, 400.f, -225.f, 225.f, 0.1f, 500.f );
//	Matrix44 projection = Matrix44::CreateOrthoMatrix( -8.f, 8.f, -4.5f, 4.5f, 0.1f, 1000.f );

	scene->m_shaderProgram->SetMat4UniformValue( "u_projectionMatrix", projection );
	scene->m_shaderProgram->SetMat4UniformValue( "u_viewMatrix", lightViewMatrix );

//	scene->RenderSceneAtFrame( 0 );
	scene->Render();

	scene->m_shaderProgram->DisableShaderProgram();

	GraphicManager::s_render->DisableDepthMask();

	glCullFace( GL_BACK );

#ifdef DEBUG_SHADOW_MAP
	GraphicManager::s_render->BindFrameBuffer( GL_FRAMEBUFFER, 0 );

	s_shadowMapBuffer->BindForReading();
	
	glReadBuffer( GL_COLOR_ATTACHMENT0 );
	glBlitFramebuffer( 0, 0, WINDOW_PHYSICAL_WIDTH, WINDOW_PHYSICAL_HEIGHT, 0, 0, WINDOW_PHYSICAL_WIDTH, WINDOW_PHYSICAL_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR );
#endif
}

void GraphicManager::CalculateDirectionalLightViewMatrix( Matrix44& viewMatrix )
{
	Vector3 lightPosition = GraphicManager::s_render->m_lightList[0]->m_position;

	Vector3 lightDirection = ( lightPosition + GraphicManager::s_render->m_lightList[0]->m_forwardDirection ).Normalize();

	Vector3 worldUp( 0.f, 0.f, 1.f );

	Vector3 rightVector = worldUp.CrossProduct( lightDirection ).Normalize();
	Vector3 upVector = lightDirection.CrossProduct( rightVector ).Normalize();

	viewMatrix.m_matrix[0] = rightVector.x;
	viewMatrix.m_matrix[4] = rightVector.y;
	viewMatrix.m_matrix[8] = rightVector.z;

	viewMatrix.m_matrix[1] = upVector.x;
	viewMatrix.m_matrix[5] = upVector.y;
	viewMatrix.m_matrix[9] = upVector.z;

	viewMatrix.m_matrix[2] = lightDirection.x;
	viewMatrix.m_matrix[6] = lightDirection.y;
	viewMatrix.m_matrix[10] = lightDirection.z;

	viewMatrix.m_matrix[12] = -rightVector.DotProduct( lightPosition );
	viewMatrix.m_matrix[13] = -upVector.DotProduct( lightPosition );
	viewMatrix.m_matrix[14] = -lightDirection.DotProduct( lightPosition );
}

void GraphicManager::SetActiveCamera(const Camera* camera)
{
	s_activeCamera = camera;
}

void GraphicManager::UpdateSceneByName(const std::string& name, float elapsedTime)
{
	auto found = s_sceneRegistry.find( name );
	if( found == s_sceneRegistry.end() )
	{
		DebuggerPrintf( "Cannot find scene with name - %s -.\n", name.c_str() );
		return;
	}
	Scene* scene = s_sceneRegistry[ name ];

	scene->Update(elapsedTime);
}

void GraphicManager::GenerateSSAOKernel()
{
	std::uniform_real_distribution<double>	randomFloats( 0.0, 1.0 );
	std::default_random_engine randomGenerator;

	int kernelSize = 64;

	for( unsigned int i = 0; i < kernelSize; i++ )
	{
		Vector3 sample( randomFloats(randomGenerator) * 2.0 - 1.0 , 
						randomFloats(randomGenerator) * 2.0 - 1.0 ,
						randomFloats(randomGenerator) );

		sample = sample.Normalize();
		sample *= randomFloats(randomGenerator);
		float scale = float(i) / (float)kernelSize;
		scale = MathUtilities::Interpolate( 0.1f, 1.f, scale * scale );
		sample *= scale;
		s_ssaoKernel.push_back( sample );
	}
}

void GraphicManager::GenerateSSAONoiseTexture()
{	
	std::vector<Vector3> ssaoNoise;
	std::uniform_real_distribution<double>	randomFloats( 0.0, 1.0 );
	std::default_random_engine randomGenerator;

	for (unsigned int i = 0; i < 64; i++)
	{
		Vector3 noise( randomFloats(randomGenerator) * 2.f - 1.f, 
					   randomFloats(randomGenerator) * 2.f - 1.f, 
					   0.0f);

		ssaoNoise.push_back(noise);
	}  

	glGenTextures(1, &s_ssaoNoiseTextureID );
	glBindTexture(GL_TEXTURE_2D, s_ssaoNoiseTextureID );
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 8, 8, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0] );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void GraphicManager::SSAOPass()
{
	s_ssaoBuffer->BindForWriting();

	s_ssaoShader->UseShaderProgram();

	GraphicManager::s_render->Clear( GL_COLOR_BUFFER_BIT );

	s_ssaoShader->SetMat4UniformValue( "u_viewMatrix", *s_currentViewMatrix );
	s_ssaoShader->SetMat4UniformValue( "u_projectionMatrix", *s_currentProjectionMatrix );
	s_ssaoShader->SetVec2UniformValue( "u_screenDimension", Vector2( WINDOW_PHYSICAL_WIDTH, WINDOW_PHYSICAL_HEIGHT ) );
	s_ssaoShader->SetIntUniformValue( "u_kernelSize", 64 );
	s_ssaoShader->SetFloatUniformValue( "u_radius", 5.f );
	s_ssaoShader->SetVec3ArrayUniformValue( "u_ssaoKernel[0]", 64, &s_ssaoKernel[0].x );

	s_ssaoShader->SetIntUniformValue( "u_positionTexture", 0 );
	s_ssaoShader->SetIntUniformValue( "u_normalViewTexture", 1 );
	s_ssaoShader->SetIntUniformValue( "u_ssaoNoiseTexture", 2 );
	s_ssaoShader->SetIntUniformValue( "u_depthTexture", 3 );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, s_gBuffer->m_textureIDs[ GBUFFER_TEXTURE_TYPE_POSITION ] );

	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, s_gBuffer->m_textureIDs[ GBUFFER_TEXTURE_TYPE_NORMAL_VIEW ] );

	glActiveTexture( GL_TEXTURE2 );
	glBindTexture( GL_TEXTURE_2D, s_ssaoNoiseTextureID );

	glActiveTexture( GL_TEXTURE3 );
	glBindTexture( GL_TEXTURE_2D, s_gBuffer->m_depthTextureID );

	s_screenQuad->Render();

	s_ssaoShader->DisableShaderProgram();
	

#ifdef DEBUG_SSAO
	GraphicManager::s_render->BindFrameBuffer( GL_FRAMEBUFFER, 0 );
	
	s_ssaoBuffer->BindForReading();

	glReadBuffer( GL_COLOR_ATTACHMENT0 );
	glBlitFramebuffer( 0, 0, WINDOW_PHYSICAL_WIDTH, WINDOW_PHYSICAL_HEIGHT, 0, 0, WINDOW_PHYSICAL_WIDTH, WINDOW_PHYSICAL_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR );
#endif


}

void GraphicManager::BlurSSAOPass()
{
	s_blurSSAOBuffer->BindForWriting();

 	s_blurShader->UseShaderProgram();
 
 	s_blurShader->SetIntUniformValue( "u_ssaoTexture", 0 );
 
 	glActiveTexture( GL_TEXTURE0 );
 	glBindTexture( GL_TEXTURE_2D, s_ssaoBuffer->m_fboColorTextureID );
 
 	s_screenQuad->Render();

 	s_blurShader->DisableShaderProgram();

#ifdef DEBUG_BLUR_SSAO
	GraphicManager::s_render->BindFrameBuffer( GL_FRAMEBUFFER, 0 );

	s_blurSSAOBuffer->BindForReading();

	glReadBuffer( GL_COLOR_ATTACHMENT0 );
	glBlitFramebuffer( 0, 0, WINDOW_PHYSICAL_WIDTH, WINDOW_PHYSICAL_HEIGHT, 0, 0, WINDOW_PHYSICAL_WIDTH, WINDOW_PHYSICAL_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR );
#endif
}
