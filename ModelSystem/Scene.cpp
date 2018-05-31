#include "Scene.hpp"
#include "../Render/GraphicManager.hpp"

Scene::Scene( const std::string& name, const std::string& filePath, OpenGLShaderProgram* shader, const Camera3D* camera )
{
	m_loader.LoadAndReconstructSceneFromExportFile( filePath, m_rootObjectList, m_boneList );
	m_currentFrameNumber = 0;
	m_maxFrameNumber = 0;
	m_camera = camera;
	m_shaderProgram = shader;
	m_name = name;
	m_uniformScale = 1.f;
	m_updateSpeed = 1.f / 40.f;

	for( unsigned int objectIndex = 0; objectIndex < m_rootObjectList.size(); objectIndex++ )
	{
		m_rootObjectList[objectIndex]->m_shaderProgram = m_shaderProgram;
		if( m_maxFrameNumber < m_rootObjectList[objectIndex]->m_animationTM.size() );
			m_maxFrameNumber = m_rootObjectList[objectIndex]->m_animationTM.size();
	}

	GraphicManager::s_sceneRegistry[ name ] = this;
}

void Scene::Render()
{
	RenderSceneAtFrame( m_currentFrameNumber );
}

void Scene::RenderSceneAtFrame( int frameIndex )
{
	m_shaderProgram->SetVec3UniformValue( "u_cameraWorldPosition",  m_camera->m_cameraPosition );
	m_shaderProgram->SetIntUniformValue( "u_isNormalMapOn", 1 );

	Matrix44 scaleMatrix = Matrix44::CreateUniformScaleMatrix( m_uniformScale );
	m_shaderProgram->SetMat4UniformValue( "u_modelMatrix", scaleMatrix );

	std::vector<BaseObject*> nodeStack;
	
	for( unsigned int rootObjectIndex = 0; rootObjectIndex < m_rootObjectList.size(); rootObjectIndex++ )
	{
		nodeStack.clear();
		nodeStack.push_back(m_rootObjectList[rootObjectIndex]);
		while( nodeStack.size() != 0 )
		{
			BaseObject* object = nodeStack.back();
			if( object->m_animationTM.size() != 1 )
				m_maxFrameNumber = object->m_animationTM.size();

			nodeStack.pop_back();
			Matrix44 temp;
			if( object->m_mesh != nullptr )
			{
				Matrix44 parentTM;
				Matrix44 parentTMNoTranslate;

				int frame;
				if( object->m_parent != nullptr )
				{
					if( object->m_parent->m_animationTM.size() != 1 )
						frame = frameIndex;
					else
						frame = 0;

					parentTM = object->m_parent->m_animationTM[frame];
					parentTMNoTranslate = Matrix44::CancelTranslation( parentTM );	
				}

				if( object->m_animationTM.size() != 1 )
					frame = frameIndex;
				else
					frame = 0;

				int boneFrame = frameIndex;
				for( unsigned int i = 0; i < m_boneList.size(); i++ )
				{
					object->m_mesh->m_boneTM[i] = m_boneList[i]->m_animationTM[boneFrame];
				}
				object->m_mesh->m_toWorldTransformation = &( object->m_animationTM[frame] * parentTM );
				object->m_mesh->m_toWorldTransformationNoTranslation = &( Matrix44::CancelTranslation( object->m_animationTM[frame] ) * parentTMNoTranslate );
				
				m_shaderProgram->SetMat4UniformValue( "u_LocalToWorldMatrix", *( object->m_mesh->m_toWorldTransformation ) );
				m_shaderProgram->SetMat4UniformValue( "u_boneTM[0]", MAX_SUPPORTED_BONE, object->m_mesh->m_boneTM[0] );

				object->m_mesh->Render( m_shaderProgram );
			}

			for( unsigned int childIndex = 0; childIndex < object->m_childList.size(); childIndex++ )
			{
				nodeStack.push_back(object->m_childList[childIndex]);
			}
		}
	}
}

Scene* Scene::CreateOrGetScene( const std::string& name ,const std::string& filePath, OpenGLShaderProgram* shader, const Camera3D* camera )
{
	if( GetSceneByName( name ) != nullptr )
	{
		return GraphicManager::s_sceneRegistry[ name ];
	}
	else
	{
		Scene* scene = new Scene( name, filePath, shader, camera );
		return scene;
	}
}

Scene* Scene::GetSceneByName( const std::string& name )
{
	auto found = GraphicManager::s_sceneRegistry.find( name );
	if( found == GraphicManager::s_sceneRegistry.end() )
		return nullptr;
	else
		return GraphicManager::s_sceneRegistry[ name ];
}

void Scene::Update(float elapsedTime)
{
	float time = GetCurrentTimeSeconds() - m_timeSinceLastUpdate;

	if( time > m_updateSpeed )
	{
		m_currentFrameNumber++;
		m_timeSinceLastUpdate = GetCurrentTimeSeconds();
	}

	if( m_currentFrameNumber > m_maxFrameNumber - 1 )
		m_currentFrameNumber = 0;
}

