cbuffer cbMatrixBuffer {
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};

struct Input {
	float4 position : POSITION;
	float4 color : COLOR;
};

struct Output {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

Output main(Input input) {
	Output output;

	input.position.w = 1.0f;

	output.position = mul(input.position,world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);

	output.color = input.color;

	return output;
}
