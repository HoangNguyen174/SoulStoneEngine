#include "DebugLine.hpp"
#include "../Render/GraphicManager.hpp"
#include "../JobSystem/MemoryPoolManager.hpp"

void DebugLine::Render()
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

void DebugLine::RenderDepthTestOn()
{
	GraphicManager::s_render->Enable(GL_DEPTH_TEST);
	GraphicManager::s_render->PushMatrix();
	GraphicManager::s_render->LineWidth(m_size * 3.f);
	GraphicManager::s_render->BeginDraw(GL_LINES);
	{
		GraphicManager::s_render->Color4f( m_startPosColor.m_red, m_startPosColor.m_green, m_startPosColor.m_blue, m_startPosColor.m_alpha );
		GraphicManager::s_render->Vertex3f( m_startPosition.x, m_startPosition.y, m_startPosition.z );

		GraphicManager::s_render->Color4f( m_endPosColor.m_red, m_endPosColor.m_green, m_endPosColor.m_blue, m_endPosColor.m_alpha );
		GraphicManager::s_render->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z);
	}
	GraphicManager::s_render->EndDraw();
	GraphicManager::s_render->PopMatrix();
	GraphicManager::s_render->LineWidth(1.f);
}

void DebugLine::RenderDepthTestOff()
{
	GraphicManager::s_render->Disable(GL_DEPTH_TEST);
	GraphicManager::s_render->PushMatrix();
	GraphicManager::s_render->LineWidth(m_size);
	GraphicManager::s_render->BeginDraw(GL_LINES);
	{
		GraphicManager::s_render->Color4f( m_startPosColor.m_red, m_startPosColor.m_green, m_startPosColor.m_blue, m_startPosColor.m_alpha );
		GraphicManager::s_render->Vertex3f( m_startPosition.x, m_startPosition.y, m_startPosition.z );

		GraphicManager::s_render->Color4f( m_endPosColor.m_red, m_endPosColor.m_green, m_endPosColor.m_blue, m_endPosColor.m_alpha );
		GraphicManager::s_render->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z);
	}
	GraphicManager::s_render->EndDraw();
	GraphicManager::s_render->PopMatrix();
	GraphicManager::s_render->LineWidth(1.f);
	GraphicManager::s_render->Enable(GL_DEPTH_TEST);
}

void DebugLine::RenderDualMode()
{
	GraphicManager::s_render->PushMatrix();

	GraphicManager::s_render->Disable(GL_DEPTH_TEST);
	GraphicManager::s_render->LineWidth(m_size);
	GraphicManager::s_render->BeginDraw(GL_LINES);
	{
		GraphicManager::s_render->Color4f( m_startPosColor.m_red, m_startPosColor.m_green, m_startPosColor.m_blue, m_startPosColor.m_alpha );
		GraphicManager::s_render->Vertex3f( m_startPosition.x, m_startPosition.y, m_startPosition.z );

		GraphicManager::s_render->Color4f( m_endPosColor.m_red, m_endPosColor.m_green, m_endPosColor.m_blue, m_endPosColor.m_alpha );
		GraphicManager::s_render->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z);

	}
	GraphicManager::s_render->EndDraw();

	GraphicManager::s_render->Enable(GL_DEPTH_TEST);

	GraphicManager::s_render->LineWidth(m_size * 3.f);
	GraphicManager::s_render->BeginDraw(GL_LINES);
	{
		GraphicManager::s_render->Color4f( m_startPosColor.m_red, m_startPosColor.m_green, m_startPosColor.m_blue, m_startPosColor.m_alpha );
		GraphicManager::s_render->Vertex3f( m_startPosition.x, m_startPosition.y, m_startPosition.z );

		GraphicManager::s_render->Color4f( m_endPosColor.m_red, m_endPosColor.m_green, m_endPosColor.m_blue, m_endPosColor.m_alpha );
		GraphicManager::s_render->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z);
	}
	GraphicManager::s_render->EndDraw();
	glLineWidth(1.f);
	GraphicManager::s_render->PopMatrix();
}