#include "pch.h"
#include "NullRenderer.h"

#ifdef USE_NULLRND

IRenderer* IRenderer::Create() {
	return new NullRenderer();
}

bool NullRenderer::Initialise() {
	return true;
}

void NullRenderer::Render() {

}

bool NullRenderer::CreateDevice() {
	return true;
}

bool NullRenderer::CreateResources() {
	return true;
}

bool NullRenderer::OnDeviceLost() {
	return true;
}

D3DDevice* NullRenderer::GetDevice() {
	return nullptr;
}

D3DContext* NullRenderer::GetDeviceContext() {
	return nullptr;
}

HRES NullRenderer::CreateBuffer(size_t size, size_t strideSize, const void* pData, D3DBindFlag bindFlag, D3DBuffer** pBuffer) {
	return S_OK;
}

HRES NullRenderer::CreateBlob(const char* path, D3DBlob** pBlob) {
	return S_OK;
}

#endif //USE_NULLRND
