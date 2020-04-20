#ifndef _INTERFACE_CAMERA_H_
#define _INTERFACE_CAMERA_H_
#pragma once

class ICamera {
public:
	ICamera();
	virtual ~ICamera();

	DirectX::SimpleMath::Matrix GetView()const;
	DirectX::SimpleMath::Matrix GetProj()const;
	DirectX::SimpleMath::Vector3& Position()const;
	DirectX::SimpleMath::Vector2& Orientation()const;

};

#endif //_INTERFACE_CAMERA_H_
