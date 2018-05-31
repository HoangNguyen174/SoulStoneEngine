#ifndef SCENE_H
#define SCENE_H
#include "../Utilities/GameCommon.hpp"
#include "../Utilities/Clock.hpp"
#include "../Camera/Camera3D.hpp"
#include "BaseObject.hpp"
#include "ExporterLoader.hpp"

const unsigned int MAX_SUPPORTED_BONE = 100;

class GraphicManager;

class Scene
{
	friend GraphicManager;

	public:
		std::string						m_name;
		std::vector<BaseObject*>		m_rootObjectList;
		std::vector<BaseObject*>		m_boneList;
		int								m_maxFrameNumber;
		int								m_currentFrameNumber;
		Clock*							m_animationClock;
		const Camera3D*					m_camera;
		OpenGLShaderProgram*			m_shaderProgram;
		float							m_uniformScale;
		float							m_updateSpeed;

	private:
		ExporterLoader					m_loader;
		float							m_timeSinceLastUpdate;

	
	public:
		Scene( const std::string& name ,const std::string& filePath, OpenGLShaderProgram* shader, const Camera3D* camera );
		static Scene* CreateOrGetScene( const std::string& name ,const std::string& filePath, OpenGLShaderProgram* shader, const Camera3D* camera );

		void RenderSceneAtFrame( int frameIndex );
		void Render();
		void Update( float elapsedTime );

	private:
		static Scene* GetSceneByName( const std::string& name );
};


#endif