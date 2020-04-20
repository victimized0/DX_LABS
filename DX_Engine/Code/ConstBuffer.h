#ifndef _CONST_BUFFER_H_
#define _CONST_BUFFER_H_

enum class ConstBufferType : unsigned char {
    PerFrame       = 0,
    PerInstance    = 1,
    PerMaterial    = 2,
    // <---- Add new types here

    Total
};

struct CBPerFrame {
    DirectX::SimpleMath::Matrix ViewProj;

    static const ConstBufferType Slot = ConstBufferType::PerFrame;
};

struct CBPerInstance {
    DirectX::SimpleMath::Matrix WorldViewProj;

    static const ConstBufferType Slot = ConstBufferType::PerInstance;
};

template <typename T>
class ConstBuffer {
public:
    ConstBuffer()                                   = default;
    ConstBuffer(ConstBuffer const&)                 = delete;
    ConstBuffer& operator= (ConstBuffer const&)     = delete;

    explicit ConstBuffer(IDevice* device) {
        Create(device);
    }

    void Create(IDevice* device) {
        D3DBufferDesc desc = {};

        desc.ByteWidth          = sizeof(T);
        desc.Usage              = D3D11_USAGE_DYNAMIC;
        desc.BindFlags          = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags     = D3D11_CPU_ACCESS_WRITE;

        ThrowIfFailed(
            device->CreateBuffer(&desc, nullptr, mConstBuffer.ReleaseAndGetAddressOf())
        );
    }

    void SetData(IDevCon* deviceContext, T const& value) {
        assert(mConstBuffer);

        D3DMappedSubres mappedResource;

        ThrowIfFailed(
            deviceContext->Map(mConstBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)
        );

        *static_cast<T*>(mappedResource.pData) = value;
        deviceContext->Unmap(mConstBuffer.Get(), 0);
    }

    IConstBuffer* GetBuffer() const noexcept {
        return mConstBuffer.Get();
    }

private:
    Microsoft::WRL::ComPtr<IConstBuffer>    mConstBuffer;
};

#endif //_CONST_BUFFER_H_
