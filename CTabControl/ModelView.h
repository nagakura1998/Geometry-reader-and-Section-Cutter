#pragma once
#include"Mesh.h"
class CModelView {
private:
	static CModelView* m_View;
	CModelView();
public:
	static CModelView* CreateModelView();

	void OnDraw();
	std::vector <Vertex> m_vertices;
	std::vector <GLuint> m_indexes;
	std::vector <Vertex> m_sectionVertices;
	std::vector <GLuint> m_sectionIndexes;
};