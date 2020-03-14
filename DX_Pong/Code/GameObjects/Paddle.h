#ifndef _GAME_OBJECT_PADDLE_H_
#define _GAME_OBJECT_PADDLE_H_

#include "../GameObjects/Ball.h"
#include "SceneObjects/GeometryObject.h"

using namespace DirectX;

class Paddle : public GeometryObject {
public:
					Paddle(const std::string& name, XMFLOAT3 position);
					Paddle(const std::string& name);

	void			Update(float dt)override;

	BoundingBox		GetBoundingBox() { return m_bb; }
	void			SetSize(int x, int y);
	int				GetSizeX();
	int				GetSizeY();
	bool			Collides(BoundingBox objBB, float objSizeX, float objSizeY, float threshold);

private:
	void			UpdateBB();

private:
	BoundingBox		m_bb;
	int				m_sizeX;
	int				m_sizeY;
};

#endif //_GAME_OBJECT_PADDLE_H_
