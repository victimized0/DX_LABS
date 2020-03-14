#ifndef _SHADER_H_
#define _SHADER_H_

#include "Interfaces/IRenderer.h"
using Microsoft::WRL::ComPtr;

enum class ShaderType : unsigned char {
	e_stVertex,
	e_stPixel
};

class Shader {
public:
	void				Create();

private:
	ShaderType			m_type;
	ComPtr<D3DBlob>		m_blob;

};

#endif //_SHADER_H_
