#ifndef _CONST_BUFFER_H_
#define _CONST_BUFFER_H_

struct CBPerObject {
    DirectX::XMFLOAT4X4 worldViewProj;
};

template <typename T>
class ConstBuffer {
public:
    ConstBuffer()                                   = default;
    ConstBuffer(ConstBuffer const&)                 = delete;
    ConstBuffer& operator= (ConstBuffer const&)     = delete;

    explicit ConstBuffer(D3DDevice* device) {
        Create(device);
    }

    void Create(D3DDevice* device) {
        D3DBufferDesc desc = {};

        desc.ByteWidth          = sizeof(T);
        desc.Usage              = D3D11_USAGE_DYNAMIC;
        desc.BindFlags          = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags     = D3D11_CPU_ACCESS_WRITE;

        ThrowIfFailed(
            device->CreateBuffer(&desc, nullptr, mConstBuffer.ReleaseAndGetAddressOf())
        );
    }

    void SetData(D3DContext* deviceContext, T const& value) {
        assert(mConstBuffer);

        D3DMappedSubres mappedResource;

        ThrowIfFailed(
            deviceContext->Map(mConstBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)
        );

        *static_cast<T*>(mappedResource.pData) = value;
        deviceContext->Unmap(mConstBuffer.Get(), 0);
    }

    ConstantBuffer* GetBuffer() const noexcept {
        return mConstBuffer.Get();
    }

private:
    Microsoft::WRL::ComPtr<ConstantBuffer>  mConstBuffer;
};

#endif //_CONST_BUFFER_H_
