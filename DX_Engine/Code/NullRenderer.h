#ifndef _NULL_RENDERER_H_
#define _NULL_RENDERER_H_

#pragma once
#include "Interfaces/IRenderer.h"

class NullRenderer : public IRenderer {
public:
					NullRenderer() {}
	virtual			~NullRenderer() {}

	bool			Initialise()override;
	void			Render()override;

	bool			CreateDevice()override;
	bool			CreateResources()override;
	bool			OnDeviceLost()override;

	D3DDevice*		GetDevice()override;
	D3DContext*		GetDeviceContext()override;

	HRES			CreateBuffer(size_t size, size_t strideSize, const void* pData, D3DBindFlag bindFlag, D3DBuffer** pBuffer)override;
	HRES			CreateBlob(const char* path, D3DBlob** pBlob)override;
};

#endif //_NULL_RENDERER_H_
