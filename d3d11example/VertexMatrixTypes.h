#pragma once

struct vertex_pc
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};

struct matrix_WorldViewProj
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX proj;

};