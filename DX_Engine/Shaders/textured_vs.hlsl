#include "common.hlsli"

VS_PosNmlTex main( in IA_PosNmlTex input ) {
	VS_PosNmlTex output;

	output.Position = mul(input.Position, worldViewProj);

	return output;
}