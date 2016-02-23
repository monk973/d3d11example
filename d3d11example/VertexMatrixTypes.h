#pragma once

struct vertex_pc
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};

struct vertex_pt
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

struct matrix_WorldViewProj
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;

};