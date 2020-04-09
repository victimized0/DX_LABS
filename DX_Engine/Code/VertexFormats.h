#ifndef _VERTEX_FORMATS_H_
#define _VERTEX_FORMATS_H_

#include "Interfaces/IRenderer.h"

///////////////////////////// VertexPosCol /////////////////////////////

struct VertexPosCol {
	VertexPosCol(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 colour)
		: Position(position)
		, Colour(colour)
	{}

	DirectX::XMFLOAT3				Position;
	DirectX::XMFLOAT4				Colour;

	static const short				ElementsCount = 2;
	static const InputElementDesc	InputElements[ElementsCount];
};

///////////////////////////// VertexPosNmlTexCoord /////////////////////////////

struct VertexPosNmlTex {
	VertexPosNmlTex(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 normal, DirectX::XMFLOAT2 texCoords)
		: Position(position)
		, Normal(normal)
		, TexCoords(texCoords)
	{}

	DirectX::XMFLOAT3				Position;
	DirectX::XMFLOAT3				Normal;
	DirectX::XMFLOAT2				TexCoords;

	static const short				ElementsCount = 3;
	static const InputElementDesc	InputElements[ElementsCount];
};

#endif //_VERTEX_FORMATS_H_
