#ifndef _GAME_OBJECT_BALL_H_
#define _GAME_OBJECT_BALL_H_

#include "SceneObjects/GeometryObject.h"

class Ball : public GeometryObject {
private:
	static constexpr float DEFAULT_SPEED_X = 30.0f;
	static constexpr float DEFAULT_SPEED_Y = 6.0f;

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
