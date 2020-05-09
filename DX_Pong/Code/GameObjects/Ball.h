#ifndef _GAME_OBJECT_BALL_H_
#define _GAME_OBJECT_BALL_H_

#include "SceneObjects/GameObject.h"

class Ball : public GameObject {
private:
	static constexpr float DEFAULT_SPEED_X = 50.0f;
	static constexpr float DEFAULT_SPEED_Y = 8.0f;

public:
					Ball(const std::string& name, const DirectX::SimpleMath::Vector3& m_position);
					Ball(const std::string& name);

	float			GetSpeedX();
	float			GetSpeedY();

	void			SetSpeedX(float value);
	void			SetSpeedY(float value);

	void			InverseSpeedX();
	void			InverseSpeedY();

	void			Reset();

private:
	float			m_speedX;
	float			m_speedY;

};

#endif //_GAME_OBJECT_BALL_H_
