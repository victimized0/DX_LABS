#include "common.hlsli"

VS_PosCol main(in IA_PosCol input) {
	VS_PosCol output;

	output.Position = mul(input.Position, worldViewProj);
	output.Color	= input.Color;

	return output;
}