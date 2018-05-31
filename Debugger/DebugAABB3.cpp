#include "DebugAABB3.hpp"
#include "../Render/GraphicManager.hpp"
#include "../JobSystem/MemoryPoolManager.hpp"

void DebugAABB3::Render()
{
	switch(m_drawMode)
	{
		case DepthTestOn : RenderDepthTestOn();
			break;
		case DepthTestOff : RenderDepthTestOff();
			break;
		case Dual: RenderDualMode();
			break;
	}
}

void DebugAABB3::RenderDepthTestOn()
{
	GraphicManager::s_render->PushMatrix();
	GraphicManager::s_render->Enable( GL_DEPTH_TEST );

	//Draw Edges
	GraphicManager::s_render->Color4f( m_edgeColor.m_red, m_edgeColor.m_green, m_edgeColor.m_blue, m_edgeColor.m_alpha );
	GraphicManager::s_render->LineWidth( 3.f );
	GraphicManager::s_render->PolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	GraphicManager::s_render->BeginDraw( GL_QUADS );
	{
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );

		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );

		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );

		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );
		
	}
	GraphicManager::s_render->EndDraw();
	GraphicManager::s_render->PolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	//draw faces
	GraphicManager::s_render->Color4f( m_faceColor.m_red, m_faceColor.m_green, m_faceColor.m_blue, m_faceColor.m_alpha );
	GraphicManager::s_render->BeginDraw( GL_QUADS );
	{
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );

		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );

		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );

		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );

		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );

		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );

	}
	GraphicManager::s_render->EndDraw();
	GraphicManager::s_render->LineWidth( 1.f );
	GraphicManager::s_render->PopMatrix();
}

void DebugAABB3::RenderDepthTestOff()
{
	GraphicManager::s_render->PushMatrix();

	//draw faces
	GraphicManager::s_render->Disable( GL_DEPTH_TEST );
	GraphicManager::s_render->Color4f( m_faceColor.m_red * 0.9f, m_faceColor.m_green * 0.9f, m_faceColor.m_blue * 0.9f, m_faceColor.m_alpha );
	GraphicManager::s_render->BeginDraw( GL_QUADS );
	{
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );

		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );

		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );

		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );

		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );

		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );

	}
	GraphicManager::s_render->EndDraw();

	//Draw Edges
	GraphicManager::s_render->Color4f( m_edgeColor.m_red * 0.9f, m_edgeColor.m_green * 0.9f, m_edgeColor.m_blue * 0.9f, m_edgeColor.m_alpha );
	GraphicManager::s_render->LineWidth( 1.f );
	GraphicManager::s_render->PolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	GraphicManager::s_render->BeginDraw( GL_QUADS );
	{
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );

		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );

		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );

		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );
		GraphicManager::s_render->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );

	}
	GraphicManager::s_render->EndDraw();
	GraphicManager::s_render->PopMatrix();
	GraphicManager::s_render->PolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	GraphicManager::s_render->Enable( GL_DEPTH_TEST );
	GraphicManager::s_render->LineWidth( 1.f );
}

void DebugAABB3::RenderDualMode()
{
	RenderDepthTestOff();
	RenderDepthTestOn();
}