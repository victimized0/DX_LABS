#ifndef _HELPER_H_
#define _HELPER_H_

#include "pch.h"

namespace
{
    template<typename T>
    void CreateBuffer(_In_ ID3D11Device* device, T const& data, D3D11_BIND_FLAG bindFlags, D3D11_USAGE usage, _Outptr_ ID3D11Buffer** pBuffer) {
        assert(pBuffer != nullptr);

        uint64_t sizeInBytes = uint64_t(data.size()) * sizeof(typename T::value_type);

        if (sizeInBytes > uint64_t(D3D11_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM * 1024u * 1024u)) {
            throw std::exception("Buffer too large for DirectX 11");
        }

        D3D11_BUFFER_DESC bufferDesc = {};

        bufferDesc.ByteWidth        = static_cast<UINT>(sizeInBytes);
        bufferDesc.BindFlags        = bindFlags;
        bufferDesc.CPUAccessFlags   = usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
        bufferDesc.Usage            = usage;

        D3D11_SUBRESOURCE_DATA dataDesc = {};

        dataDesc.pSysMem = data.data();

        ThrowIfFailed(device->CreateBuffer(&bufferDesc, &dataDesc, pBuffer));

        assert(pBuffer != nullptr && *pBuffer != nullptr);
    }

    template<typename T>
    void CreateBuffer(_In_ ID3D11Device* device, T const& data, D3D11_BIND_FLAG bindFlags, _Outptr_ ID3D11Buffer** pBuffer) {
        ::CreateBuffer(device, data, bindFlags, D3D11_USAGE_DEFAULT, pBuffer);
    }

}

#endif //_HELPER_H_
