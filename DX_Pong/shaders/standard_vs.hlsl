#pragma pack_matrix(row_major)

cbuffer cbPerObject : register(b0) {
	float4x4 wvpMatrix;
};

struct APP_INPUT {
	float4 Position	: POSITION;
	float4 Color	: COLOR;
};

struct VS_OUTPUT {
	float4 Position	: SV_POSITION;
	float4 Color	: COLOR;
};

VS_OUTPUT vs_main(in APP_INPUT input) {
	VS_OUTPUT output;

	output.Position = mul(input.Position, wvpMatrix);
	output.Color	= input.Color;

	return output;
}