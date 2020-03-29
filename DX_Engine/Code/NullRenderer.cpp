#include "pch.h"
#include "NullRenderer.h"

#if USE_NULL_RENDERER

IRenderer* IRenderer::Create() {
	return new NullRenderer();
}

bool NullRenderer::Initialise() {

}

void NullRenderer::Render() {

}

bool NullRenderer::CreateDevice() {

}

bool NullRenderer::CreateResources() {

}

bool NullRenderer::OnDeviceLost() {

}

D3DDevice* NullRenderer::GetDevice() {

}

D3DContext* NullRenderer::GetDeviceContext() {

}

HRES NullRenderer::CreateBuffer(size_t size, size_t strideSize, const void* pData, D3DBindFlag bindFlag, D3DBuffer** pBuffer) {

}

HRES NullRenderer::CreateBlob(const char* path, D3DBlob** pBlob) {

}

#endif
