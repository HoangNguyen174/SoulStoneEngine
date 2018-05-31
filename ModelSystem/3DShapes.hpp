#ifndef SHAPES_H
#define SHAPES_H

#include "../Render/GLRender.hpp"

enum Engine3DShapeType { ENGINE_SPHERE, ENGINE_CUBE, ENGINE_CONE };

class Shape
{
	public:
		std::vector<SimpleVertex3D>			m_vertices;
		unsigned int						m_vboID;
		bool								m_isVboDirty;

	public:
		Shape() {}
		~Shape() {}
		virtual void Render() {}
};


class Sphere : public Shape
{
	public:
		float	m_radius;

	private:
		int		m_lattitude;
		int		m_longtitude;

	public:
		Sphere( float radius, int lats, int longs );
		Sphere();
		~Sphere() {}
		void Render();

	private:
		void GenerateSphereVertices();
		void CreateVBO();
		void RenderVBO();

};

class ScreenRectangle : public Shape
{
	public:
		ScreenRectangle();
		~ScreenRectangle();
		void Render();

	private:
		void CreateVBO();
		void RenderVBO();
};

#endif