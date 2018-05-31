#include "DebugArrow.hpp"
#include "../Render/GraphicManager.hpp"
#include "../JobSystem/MemoryPoolManager.hpp"

void DebugArrow::Render()
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


void DebugArrow::RenderDepthTestOn()
{
	GraphicManager::s_render->Enable(GL_DEPTH_TEST);
	GraphicManager::s_render->PushMatrix();
	GraphicManager::s_render->LineWidth(m_size * 3.f);
	GraphicManager::s_render->BeginDraw(GL_LINES);
	{
		//draw body
		GraphicManager::s_render->Color4f( m_startPosColor.m_red, m_startPosColor.m_green, m_startPosColor.m_blue, m_startPosColor.m_alpha );
		GraphicManager::s_render->Vertex3f( m_startPosition.x, m_startPosition.y, m_startPosition.z );

		GraphicManager::s_render->Color4f( m_endPosColor.m_red, m_endPosColor.m_green, m_endPosColor.m_blue, m_endPosColor.m_alpha );
		GraphicManager::s_render->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z );
	}
	GraphicManager::s_render->EndDraw();
	GraphicManager::s_render->PopMatrix();

	Vector3 v1 = m_endPosition - m_startPosition;

	Vector3 center = m_endPosition - ( 0.1f * v1 );
	//Vector3 v1 = m_startPosition - m_endPosition;
	v1.Normalize();

	Vector3 v2 = v1.CrossProduct( Vector3(1.f,0.f,0.f) );
	if( v2.CalcLength() < 0.001f )
	{
		v2 = v1.CrossProduct( Vector3(0.f,1.f,0.f) );
	}
	//v2.Normalize();

	Vector3 v3 = v1.CrossProduct(v2);
	//v3.Normalize();

	Vector3 h1 = center + 0.1f * v2;
	Vector3 h2 = center - 0.1f * v2;
	Vector3 h3 = center + 0.1f * v3;
	Vector3 h4 = center - 0.1f * v3;

	GraphicManager::s_render->PushMatrix();
	//GraphicManager::s_render->Translated(m_endPosition.x,m_endPosition.y,m_endPosition.z);
// 	glPointSize( 10.f );
// 	GraphicManager::s_render->BeginDraw(GL_POINTS);
// 	{
// 		GraphicManager::s_render->Color4f( m_endPosColor.m_red, m_endPosColor.m_green, m_endPosColor.m_blue, m_endPosColor.m_alpha );
// 		GraphicManager::s_render->Vertex3f( center.x,center.y,center.z);
// 	}
// 	GraphicManager::s_render->EndDraw();

	GraphicManager::s_render->BeginDraw(GL_LINES);
	{
		//draw head
		GraphicManager::s_render->Color4f( m_endPosColor.m_red, m_endPosColor.m_green, m_endPosColor.m_blue, m_endPosColor.m_alpha );
		GraphicManager::s_render->Vertex3f( m_endPosition.x,m_endPosition.y,m_endPosition.z);
		GraphicManager::s_render->Vertex3f( h1.x, h1.y, h1.z );

		GraphicManager::s_render->Vertex3f( m_endPosition.x,m_endPosition.y,m_endPosition.z );
		GraphicManager::s_render->Vertex3f( h2.x, h2.y, h2.z );

		GraphicManager::s_render->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z );
		GraphicManager::s_render->Vertex3f( h3.x, h3.y, h3.z );

		GraphicManager::s_render->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z );
		GraphicManager::s_render->Vertex3f( h4.x, h4.y, h4.z );
	}
	GraphicManager::s_render->EndDraw();
	GraphicManager::s_render->PopMatrix();

	GraphicManager::s_render->LineWidth(1.f);
}

void DebugArrow::RenderDepthTestOff()
{
	GraphicManager::s_render->Disable( GL_DEPTH_TEST );
	GraphicManager::s_render->PushMatrix();
	GraphicManager::s_render->LineWidth( 1.f );
	GraphicManager::s_render->BeginDraw(GL_LINES);
	{
		//draw body
		GraphicManager::s_render->Color4f( m_startPosColor.m_red * 0.7f, m_startPosColor.m_green * 0.7f, m_startPosColor.m_blue * 0.7f, m_startPosColor.m_alpha );
		GraphicManager::s_render->Vertex3f( m_startPosition.x, m_startPosition.y, m_startPosition.z );

		GraphicManager::s_render->Color4f( m_endPosColor.m_red * 0.7f, m_endPosColor.m_green * 0.7f, m_endPosColor.m_blue * 0.7f, m_endPosColor.m_alpha );
		GraphicManager::s_render->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z );
	}
	GraphicManager::s_render->EndDraw();
	GraphicManager::s_render->PopMatrix();

	Vector3 v1 = m_endPosition - m_startPosition;

	Vector3 center = m_endPosition - ( 0.1f * v1 );
	//Vector3 v1 = m_startPosition - m_endPosition;
	v1.Normalize();

	Vector3 v2 = v1.CrossProduct( Vector3(1.f,0.f,0.f) );
	if( v2.CalcLength() < 0.001f )
	{
		v2 = v1.CrossProduct( Vector3(0.f,1.f,0.f) );
	}
	//v2.Normalize();

	Vector3 v3 = v1.CrossProduct(v2);
	//v3.Normalize();

	Vector3 h1 = center + 0.1f * v2;
	Vector3 h2 = center - 0.1f * v2;
	Vector3 h3 = center + 0.1f * v3;
	Vector3 h4 = center - 0.1f * v3;

	GraphicManager::s_render->PushMatrix();
	//GraphicManager::s_render->Translated(m_endPosition.x,m_endPosition.y,m_endPosition.z);
	// 	glPointSize( 10.f );
	// 	GraphicManager::s_render->BeginDraw(GL_POINTS);
	// 	{
	// 		GraphicManager::s_render->Color4f( m_endPosColor.m_red, m_endPosColor.m_green, m_endPosColor.m_blue, m_endPosColor.m_alpha );
	// 		GraphicManager::s_render->Vertex3f( center.x,center.y,center.z);
	// 	}
	// 	GraphicManager::s_render->EndDraw();

	GraphicManager::s_render->BeginDraw(GL_LINES);
	{
		//draw head
		GraphicManager::s_render->Color4f( m_endPosColor.m_red * 0.7f, m_endPosColor.m_green * 0.7f, m_endPosColor.m_blue * 0.7f, m_endPosColor.m_alpha );
		GraphicManager::s_render->Vertex3f( m_endPosition.x,m_endPosition.y,m_endPosition.z);
		GraphicManager::s_render->Vertex3f( h1.x, h1.y, h1.z );

		GraphicManager::s_render->Vertex3f( m_endPosition.x,m_endPosition.y,m_endPosition.z );
		GraphicManager::s_render->Vertex3f( h2.x, h2.y, h2.z );

		GraphicManager::s_render->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z );
		GraphicManager::s_render->Vertex3f( h3.x, h3.y, h3.z );

		GraphicManager::s_render->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z );
		GraphicManager::s_render->Vertex3f( h4.x, h4.y, h4.z );
	}
	GraphicManager::s_render->EndDraw();
	GraphicManager::s_render->PopMatrix();

	GraphicManager::s_render->LineWidth(1.f);
}

void DebugArrow::RenderDualMode()
{
	RenderDepthTestOff();
	RenderDepthTestOn();
}