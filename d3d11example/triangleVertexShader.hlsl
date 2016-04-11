cbuffer cbMatrixBuffer {
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};

struct Input {
	float4 position : POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL;
};

struct Output {
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL;
};

Output main(Input input) {
	Output output;

	input.position.w = 1.0f;

	output.position = mul(input.position,world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);

	output.texcoord = input.texcoord;

	output.normal = mul(input.normal, (float3x3)world);

	output.normal = normalize(output.normal);

	return output;
}
