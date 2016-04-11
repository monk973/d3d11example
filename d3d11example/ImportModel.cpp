#include "ImportModel.h"
#include <fstream>


ImportModel::ImportModel()
{
}


ImportModel::~ImportModel()
{
	delete m_vertices;
}

void ImportModel::Init(char* dirWithFileName)
{
	char* tmp_read = LoadStringFromFile(dirWithFileName);
	m_string = tmp_read;
	delete[] tmp_read;

	LoadAtOnce();

	/*
	LoadVertex();
	LoadUV();
	LoadNormal();
	LoadMaterial();
	*/
}

char * ImportModel::LoadStringFromFile(char* dirWithFileName)
{
	char* tmp_read=nullptr;

	fstream fs(dirWithFileName);

	fs.seekg(0,ios::end);
	int size=fs.tellg();
	fs.seekg(0,ios::beg);

	tmp_read = new  char[size];

	fs.read(tmp_read, size);

	fs.close();

	return tmp_read;
}

void ImportModel::LoadAtOnce()
{
	//first encounter colon is "vertex count" info.
	int pos = m_string.find(':', 0);
	int cut = pos;

	while (m_string[pos++] != ' ');
	while (m_string[cut++] != '\n');

	string tmp = m_string.substr(pos, (cut - pos));

	//size_t tmp2 = 0;
	//string tmp_string = "123 asdasd";
	//base 는 십진수냐 8진수냐 16진수냐를 결정하는 인자. 디폴트는 10으로되어있으며 8진수값을 읽고싶으면 8을, 16진수는 16을 입력해주면됨.
	//std::stoi(tmp_string/*m_string.substr(pos,(cut-pos) )*/, &tmp2);
	m_vertexCount = std::stoi(m_string.substr(pos,(cut-pos)));

	m_vertices = new vertex_ptn[m_vertexCount];

	pos = m_string.find(':', pos);
	pos++;
	while (m_string[pos++] != '\n');

	size_t endByte=0;

	for (int i = 0; i < m_vertexCount; ++i)
	{
		m_vertices[i].pos.x = std::stof(m_string.substr(pos), &endByte);
		pos += endByte;

		m_vertices[i].pos.y = std::stof(m_string.substr(pos), &endByte);
		pos += endByte;

		m_vertices[i].pos.z= std::stof(m_string.substr(pos), &endByte);
		pos += endByte;

		m_vertices[i].uv.x = std::stof(m_string.substr(pos), &endByte);
		pos += endByte;

		m_vertices[i].uv.y = std::stof(m_string.substr(pos), &endByte);
		pos += endByte;


		m_vertices[i].normal.x = std::stof(m_string.substr(pos), &endByte);
		pos += endByte;

		m_vertices[i].normal.y = std::stof(m_string.substr(pos), &endByte);
		pos += endByte;

		m_vertices[i].normal.z = std::stof(m_string.substr(pos), &endByte);
		pos += endByte;


	}

	int a = 0;
}

void ImportModel::LoadVertex()
{
	


}

void ImportModel::LoadUV()
{
}

void ImportModel::LoadNormal()
{
}

void ImportModel::LoadMaterial()
{
}

vertex_ptn * ImportModel::GetVerticesInfo()
{
	return m_vertices;
}

int ImportModel::GetVertexCount()
{
	return m_vertexCount;
}
