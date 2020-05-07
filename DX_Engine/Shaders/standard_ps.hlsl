#include "common.hlsli"

float4 main( in VS_PosColNmlTex input ) : SV_TARGET {
	return input.Color;
}