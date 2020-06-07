#ifndef _VERTEX_FORMATS_H_
#define _VERTEX_FORMATS_H_

#include "Interfaces/IRenderer.h"

struct VertexPosition {
	VertexPosition() : VertexPosition(DirectX::SimpleMath::Vector3::Zero) {}
	VertexPosition(DirectX::SimpleMath::Vector3 position) : Position(position) {}

	DirectX::SimpleMath::Vector3		Position;

	static const short					ElementsCount = 1;
	static const D3DInputElementDesc	InputElements[ElementsCount];
};

///////////////////////////// VertexPosCol /////////////////////////////

struct VertexPosCol : public VertexPosition {
	VertexPosCol() : VertexPosCol(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero) {}

	VertexPosCol(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 color)
		: VertexPosition(position)
		, Color(color)
	{}

	//DirectX::SimpleMath::Vector3		Position;
	DirectX::SimpleMath::Vector3		Color;

	static const short					ElementsCount = 2;
	static const D3DInputElementDesc	InputElements[ElementsCount];
};

///////////////////////////// VertexPosNmlTex /////////////////////////////

struct VertexPosNmlTex : public VertexPosition {
	VertexPosNmlTex() : VertexPosNmlTex(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector2::Zero) {}

	VertexPosNmlTex(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 normal, DirectX::SimpleMath::Vector2 texCoords)
		: VertexPosition(position)
		, Normal(normal)
		, TexCoords(texCoords)
	{}

	//DirectX::SimpleMath::Vector3		Position;
	DirectX::SimpleMath::Vector3		Normal;
	DirectX::SimpleMath::Vector2		TexCoords;

	static const short					ElementsCount = 3;
	static const D3DInputElementDesc	InputElements[ElementsCount];
};

///////////////////////////// VertexPosColNmlTex /////////////////////////////

struct VertexPosColNmlTex : public VertexPosition {
	VertexPosColNmlTex() : VertexPosColNmlTex(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector2::Zero) {}

	VertexPosColNmlTex(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 color, DirectX::SimpleMath::Vector3 normal, DirectX::SimpleMath::Vector2 texCoords)
		: VertexPosition(position)
		, Color(color)
		, Normal(normal)
		, TexCoords(texCoords)
	{}

	//DirectX::SimpleMath::Vector3		Position;
	DirectX::SimpleMath::Vector3		Color;
	DirectX::SimpleMath::Vector3		Normal;
	DirectX::SimpleMath::Vector2		TexCoords;

	static const short					ElementsCount = 4;
	static const D3DInputElementDesc	InputElements[ElementsCount];
};

#endif //_VERTEX_FORMATS_H_
