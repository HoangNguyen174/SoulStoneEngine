#include "DebugSphere.hpp"
#include "../Render/GraphicManager.hpp"
#include "../JobSystem/MemoryPoolManager.hpp"

void DebugSphere::Render()
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

void DebugSphere::RenderDepthTestOn()
{
	GraphicManager::s_render->PushMatrix();
	GraphicManager::s_render->Enable( GL_DEPTH_TEST );
	GraphicManager::s_render->Color4f( m_color.m_red, m_color.m_green, m_color.m_blue, m_color.m_alpha);
	GraphicManager::s_render->Translatef( m_center.x, m_center.y, m_center.z );
	GraphicManager::s_render->Scalef( m_radius, m_radius, m_radius );
	GraphicManager::s_render->PolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	GraphicManager::s_render->LineWidth( 3.f );
	DrawSphere( 1.f, 20, 20);
	GraphicManager::s_render->PopMatrix();
	GraphicManager::s_render->PolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	GraphicManager::s_render->LineWidth( 1.f );
}

void DebugSphere::RenderDepthTestOff()
{
	GraphicManager::s_render->PushMatrix();
	GraphicManager::s_render->Disable( GL_DEPTH_TEST );
	GraphicManager::s_render->Color4f( m_color.m_red * 0.8f, m_color.m_green * 0.8f, m_color.m_blue * 0.8f, m_color.m_alpha);
	GraphicManager::s_render->Translatef( m_center.x, m_center.y, m_center.z );
	GraphicManager::s_render->Scalef( m_radius, m_radius, m_radius );
	GraphicManager::s_render->PolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	DrawSphere( 1.f, 20, 20);
	GraphicManager::s_render->PopMatrix();
	GraphicManager::s_render->PolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	GraphicManager::s_render->Enable( GL_DEPTH_TEST );
}

void DebugSphere::RenderDualMode()
{
	RenderDepthTestOff();
	RenderDepthTestOn();
}

void DebugSphere::DrawSphere(float, int lats, int longs)
{
	int i, j;
    for(i = 0; i <= lats; i++) 
	{
	    double lat0 = 3.14f * (-0.5 + (double) (i - 1) / lats);
		double z0  = sin(lat0);
		double zr0 =  cos(lat0);

		double lat1 = 3.14f * (-0.5 + (double) i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		GraphicManager::s_render->BeginDraw( GL_QUAD_STRIP );
		for(j = 0; j <= longs; j++) 
		{
			double lng = 2 * 3.14f * (double) (j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3d(x * zr0, y * zr0, z0);
			glVertex3d(x * zr0, y * zr0, z0);
			glNormal3d(x * zr1, y * zr1, z1);
			glVertex3d(x * zr1, y * zr1, z1);
		}
		GraphicManager::s_render->EndDraw();
	}
}