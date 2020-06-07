#include "common.hlsli"

VS_PosColNmlTex main( in IA_PosColNmlTex input ) {
	VS_PosColNmlTex output;

	output.Position = mul( input.Position, worldViewProj );
	output.WPos		= mul( input.Position, world ).xyz;
	output.Normal	= normalize( mul(input.Normal, (float3x3)world) );
	output.TexCoord = input.TexCoord;
	output.Color	= input.Color;

	return output;
}