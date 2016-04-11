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

struct vertex_ptn
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
};

struct matrix_WorldViewProj
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;

};