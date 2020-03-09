#ifndef _VERTEX_FORMATS_H
#define _VERTEX_FORMATS_H

struct SimpleVertexColour {
	SimpleVertexColour(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 colour)
		: Position(position), Colour(colour) {}

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Colour;
};

#endif //_VERTEX_FORMATS_H
