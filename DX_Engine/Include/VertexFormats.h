#ifndef _VERTEX_FORMATS_H_
#define _VERTEX_FORMATS_H_

#include "Interfaces/IRenderer.h"

///////////////////////////// SimpleVertexColour /////////////////////////////
struct VertexPositionColour {
	VertexPositionColour(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 colour)
		: Position(position)
		, Colour(colour)
	{}

	DirectX::XMFLOAT3				Position;
	DirectX::XMFLOAT4				Colour;

	static const short				ElementsCount = 2;
	static const InputElementDesc	InputElements[ElementsCount];
};

#endif //_VERTEX_FORMATS_H_
