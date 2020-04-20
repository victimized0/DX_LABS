#include "common.hlsli"

float4 main( in VS_PosCol input ) : SV_TARGET {
	float4 col = input.Color;
	return col;
}