#ifndef _CONST_BUFFER_H_
#define _CONST_BUFFER_H_

enum class ConstBufferType : unsigned char {
    PerFrame       = 0,
    PerInstance    = 1,
    PerMaterial    = 2,
    // <---- Add new types here

    Total
};

struct alignas(16) CBPerFrame {
    DirectX::SimpleMath::Vector4    EyePos;
    DirectX::SimpleMath::Vector4    LightCol;
    DirectX::SimpleMath::Vector4    LightAmb;
    DirectX::SimpleMath::Vector4    LightDir;

    static const ConstBufferType    Slot = ConstBufferType::PerFrame;
};

struct alignas(16) CBPerInstance {
    DirectX::SimpleMath::Matrix     WorldViewProj;
    DirectX::SimpleMath::Matrix     WorldView;
    DirectX::SimpleMath::Matrix     World;

    static const ConstBufferType    Slot = ConstBufferType::PerInstance;
};

struct alignas(16) CBPerMaterial {
public:
    DirectX::SimpleMath::Vector4    AmbientColor;
    DirectX::SimpleMath::Vector4    DiffuseColor;
    DirectX::SimpleMath::Vector4    SpecularColor;

    static const ConstBufferType    Slot = ConstBufferType::PerMaterial;
};

template <typename T>
class ConstBuffer {
public:
    ConstBuffer()                                   = default;
    ConstBuffer(ConstBuffer const& other) : mConstBuffer(other.mConstBuffer) {}
    ConstBuffer& operator=(ConstBuffer const& other) { other.mConstBuffer = mConstBuffer }

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
    Microsoft::WRL::ComPtr<IConstBuffer> mConstBuffer;
};

#endif //_CONST_BUFFER_H_
