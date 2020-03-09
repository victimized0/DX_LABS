#ifndef _CONST_BUFFER_H_
#define _CONST_BUFFER_H_

struct CameraConstants {
    DirectX::XMFLOAT4X4 worldViewProj;
    DirectX::XMFLOAT4X4 world;
};

// Strongly typed wrapper around a D3D constant buffer.
template<typename T>
class ConstBuffer {
public:
    ConstBuffer()                                   = default;
    ConstBuffer(ConstBuffer const&)                 = delete;
    ConstBuffer& operator= (ConstBuffer const&)     = delete;

    explicit ConstBuffer(_In_ ID3D11Device* device) {
        Create(device);
    }

    void Create(_In_ ID3D11Device* device) {
        D3D11_BUFFER_DESC desc  = {};

        desc.ByteWidth          = sizeof(T);
        desc.Usage              = D3D11_USAGE_DYNAMIC;
        desc.BindFlags          = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags     = D3D11_CPU_ACCESS_WRITE;

        ThrowIfFailed(
            device->CreateBuffer(&desc, nullptr, mConstBuffer.ReleaseAndGetAddressOf())
        );
    }

    void SetData(_In_ ID3D11DeviceContext* deviceContext, T const& value) {
        assert(mConstBuffer);

        D3D11_MAPPED_SUBRESOURCE mappedResource;

        ThrowIfFailed(
            deviceContext->Map(mConstBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)
        );

        *static_cast<T*>(mappedResource.pData) = value;

        deviceContext->Unmap(mConstBuffer.Get(), 0);
    }

    ID3D11Buffer* GetBuffer() const noexcept {
        return mConstBuffer.Get();
    }

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> mConstBuffer;
};

#endif //_CONST_BUFFER_H_
