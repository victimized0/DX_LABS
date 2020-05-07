#include "common.hlsli"

VS_PosColNmlTex main(in IA_PosColNmlTex input) {
	VS_PosColNmlTex output;

	output.Position = mul(input.Position, worldViewProj);
	output.Color	= input.Color;
	output.Normal   = input.Normal;

	return output;
}