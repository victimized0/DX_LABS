#ifndef _GAME_OBJECT_PADDLE_H_
#define _GAME_OBJECT_PADDLE_H_

#include "../GameObjects/Ball.h"
#include "SceneObjects/GeometryObject.h"

using namespace DirectX;

class Paddle : public GeometryObject {
public:
			Paddle(const std::string& name, XMFLOAT3 position);
			Paddle(const std::string& name);

	void	Update(float dt)override;

	void	SetBall(Ball* pBall);
	void	SetSize(int x, int y);
	int		GetSizeX();
	int		GetSizeY();

private:
	Ball*	m_pBall;
	int		m_sizeX;
	int		m_sizeY;
};

#endif //_GAME_OBJECT_PADDLE_H_
