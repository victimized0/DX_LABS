#ifndef _GAME_OBJECT_BALL_H_
#define _GAME_OBJECT_BALL_H_

#include "SceneObjects/GeometryObject.h"
using namespace DirectX;

class Ball : public GeometryObject {
public:
			Ball(const std::string& name, XMFLOAT3 position);
			Ball(const std::string& name);

	int		GetSize();
	float	GetSpeedX();
	float	GetSpeedY();

	void	SetSize(int value);
	void	SetSpeedX(float value);
	void	SetSpeedY(float value);

	void	Update(float dt)override;

private:
	static constexpr float DEFAULT_SPEED_X = 15.0f;
	static constexpr float DEFAULT_SPEED_Y = 3.0f;

	void	Reset();

private:
	int		m_size;
	float	m_speedX;
	float	m_speedY;

};

#endif //_GAME_OBJECT_BALL_H_
