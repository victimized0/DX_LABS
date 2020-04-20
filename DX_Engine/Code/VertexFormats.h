#ifndef _VERTEX_FORMATS_H_
#define _VERTEX_FORMATS_H_

#include "Interfaces/IRenderer.h"
#include "Math/SimpleMath.h"

///////////////////////////// VertexPosCol /////////////////////////////

struct VertexPosCol {
	VertexPosCol() : VertexPosCol(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector4::Zero) {}

	VertexPosCol(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector4 colour)
		: Position(position)
		, Colour(colour)
	{}

	DirectX::SimpleMath::Vector3				Position;
	DirectX::SimpleMath::Vector4				Colour;

	static const short							ElementsCount = 2;
	static const D3DInputElementDesc			InputElements[ElementsCount];
};

///////////////////////////// VertexPosNmlTexCoord /////////////////////////////

struct VertexPosNmlTex {
	VertexPosNmlTex(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 normal, DirectX::SimpleMath::Vector2 texCoords)
		: Position(position)
		, Normal(normal)
		, TexCoords(texCoords)
	{}

	DirectX::SimpleMath::Vector3		Position;
	DirectX::SimpleMath::Vector3		Normal;
	DirectX::SimpleMath::Vector2		TexCoords;

	static const short					ElementsCount = 3;
	static const D3DInputElementDesc	InputElements[ElementsCount];
};

#endif //_VERTEX_FORMATS_H_
