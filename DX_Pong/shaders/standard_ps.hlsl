#pragma pack_matrix(row_major)

struct VS_OUTPUT {
	float4 Position	: SV_POSITION;
	float4 Color	: COLOR;
};

float4 ps_main( in VS_OUTPUT input ) : SV_TARGET {
	return input.Color;
}