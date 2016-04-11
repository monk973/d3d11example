#pragma once

#include "GameStatic.h"

class ImportModel
{
public:
	ImportModel();
	virtual ~ImportModel();

public:
	void Init(char* dirWithFileName);
	char* LoadStringFromFile(char* dirWithFileName);

	void LoadAtOnce();

	void LoadVertex();
	void LoadUV();
	void LoadNormal();

	void LoadMaterial();
	
	vertex_ptn* GetVerticesInfo();
	int GetVertexCount();
private:
	string m_string;
	vertex_ptn* m_vertices=nullptr;
	int m_vertexCount = 0;
};

