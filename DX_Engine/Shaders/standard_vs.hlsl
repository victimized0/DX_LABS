#include "common.hlsli"

VS_PosColNmlTex main(in IA_PosColNmlTex input) {
	VS_PosColNmlTex output = (VS_PosColNmlTex)0;

	input.Position.w	= 1.0f;
	output.Position		= mul(input.Position, worldViewProj);
	output.WPos			= mul(input.Position, world).xyz;
	output.Color		= input.Color;
	output.Normal		= input.Normal.xyz;
	output.TexCoord		= input.TexCoord;

	return output;
}
