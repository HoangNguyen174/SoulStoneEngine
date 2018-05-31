#include "DebugPoint.hpp"
#include "../Render/GraphicManager.hpp"
#include "../JobSystem/MemoryPoolManager.hpp"

void DebugPoint::Render()
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

void DebugPoint::RenderDepthTestOn()
{
	GraphicManager::s_render->Disable( GL_TEXTURE_2D );
	GraphicManager::s_render->Enable( GL_DEPTH_TEST );
	GraphicManager::s_render->Color4f(m_color.m_red ,m_color.m_green ,m_color.m_blue ,m_color.m_alpha);

	GraphicManager::s_render->LineWidth(6.f);
	GraphicManager::s_render->BeginDraw(GL_LINES);
	{
		GraphicManager::s_render->Vertex3f( m_position.x - m_size,  m_position.y + m_size, m_position.z + m_size);
		GraphicManager::s_render->Vertex3f( m_position.x + m_size,  m_position.y - m_size, m_position.z - m_size);

		GraphicManager::s_render->Vertex3f( m_position.x + m_size,  m_position.y + m_size, m_position.z + m_size);
		GraphicManager::s_render->Vertex3f( m_position.x - m_size,  m_position.y - m_size, m_position.z - m_size);

		GraphicManager::s_render->Vertex3f( m_position.x - m_size,  m_position.y - m_size, m_position.z + m_size);
		GraphicManager::s_render->Vertex3f( m_position.x + m_size,  m_position.y + m_size, m_position.z - m_size);

		GraphicManager::s_render->Vertex3f( m_position.x + m_size,  m_position.y - m_size, m_position.z + m_size);
		GraphicManager::s_render->Vertex3f( m_position.x - m_size,  m_position.y + m_size, m_position.z - m_size);

	}
	GraphicManager::s_render->EndDraw();
	GraphicManager::s_render->Enable(GL_TEXTURE_2D);
}

void DebugPoint::RenderDepthTestOff()
{
	GraphicManager::s_render->PushMatrix();
	GraphicManager::s_render->Disable(GL_DEPTH_TEST);
	GraphicManager::s_render->Color4f(m_color.m_red * 0.5f,m_color.m_green * 0.5f ,m_color.m_blue * 0.5f,m_color.m_alpha);
	GraphicManager::s_render->BeginDraw(GL_LINES);
	{
		GraphicManager::s_render->Vertex3f( m_position.x - m_size,  m_position.y + m_size, m_position.z + m_size);
		GraphicManager::s_render->Vertex3f( m_position.x + m_size,  m_position.y - m_size, m_position.z - m_size);
	
		GraphicManager::s_render->Vertex3f( m_position.x + m_size,  m_position.y + m_size, m_position.z + m_size);
		GraphicManager::s_render->Vertex3f( m_position.x - m_size,  m_position.y - m_size, m_position.z - m_size);

		GraphicManager::s_render->Vertex3f( m_position.x - m_size,  m_position.y - m_size, m_position.z + m_size);
		GraphicManager::s_render->Vertex3f( m_position.x + m_size,  m_position.y + m_size, m_position.z - m_size);

		GraphicManager::s_render->Vertex3f( m_position.x + m_size,  m_position.y - m_size, m_position.z + m_size);
		GraphicManager::s_render->Vertex3f( m_position.x - m_size,  m_position.y + m_size, m_position.z - m_size);

	}
	GraphicManager::s_render->EndDraw();
	GraphicManager::s_render->Enable(GL_DEPTH_TEST);
	GraphicManager::s_render->PopMatrix();
}

void DebugPoint::RenderDualMode()
{
	GraphicManager::s_render->PushMatrix();
	GraphicManager::s_render->Disable(GL_DEPTH_TEST);
	GraphicManager::s_render->Color4f(m_color.m_red * 0.5f,m_color.m_green * 0.5f ,m_color.m_blue * 0.5f,m_color.m_alpha);
	GraphicManager::s_render->BeginDraw(GL_LINES);
	{
		GraphicManager::s_render->Vertex3f( m_position.x - m_size,  m_position.y + m_size, m_position.z + m_size);
		GraphicManager::s_render->Vertex3f( m_position.x + m_size,  m_position.y - m_size, m_position.z - m_size);

		GraphicManager::s_render->Vertex3f( m_position.x + m_size,  m_position.y + m_size, m_position.z + m_size);
		GraphicManager::s_render->Vertex3f( m_position.x - m_size,  m_position.y - m_size, m_position.z - m_size);

		GraphicManager::s_render->Vertex3f( m_position.x - m_size,  m_position.y - m_size, m_position.z + m_size);
		GraphicManager::s_render->Vertex3f( m_position.x + m_size,  m_position.y + m_size, m_position.z - m_size);

		GraphicManager::s_render->Vertex3f( m_position.x + m_size,  m_position.y - m_size, m_position.z + m_size);
		GraphicManager::s_render->Vertex3f( m_position.x - m_size,  m_position.y + m_size, m_position.z - m_size);

	}
	GraphicManager::s_render->EndDraw();

	GraphicManager::s_render->Enable(GL_DEPTH_TEST);

	GraphicManager::s_render->Color4f(m_color.m_red,m_color.m_green,m_color.m_blue,m_color.m_alpha);
	GraphicManager::s_render->LineWidth(3.f);
	GraphicManager::s_render->BeginDraw(GL_LINES);
	{
		GraphicManager::s_render->Vertex3f( m_position.x - m_size,  m_position.y + m_size, m_position.z + m_size);
		GraphicManager::s_render->Vertex3f( m_position.x + m_size,  m_position.y - m_size, m_position.z - m_size);

		GraphicManager::s_render->Vertex3f( m_position.x + m_size,  m_position.y + m_size, m_position.z + m_size);
		GraphicManager::s_render->Vertex3f( m_position.x - m_size,  m_position.y - m_size, m_position.z - m_size);

		GraphicManager::s_render->Vertex3f( m_position.x - m_size,  m_position.y - m_size, m_position.z + m_size);
		GraphicManager::s_render->Vertex3f( m_position.x + m_size,  m_position.y + m_size, m_position.z - m_size);

		GraphicManager::s_render->Vertex3f( m_position.x + m_size,  m_position.y - m_size, m_position.z + m_size);
		GraphicManager::s_render->Vertex3f( m_position.x - m_size,  m_position.y + m_size, m_position.z - m_size);

	}
	GraphicManager::s_render->EndDraw();
	GraphicManager::s_render->PopMatrix();
	glLineWidth(1.f);
}