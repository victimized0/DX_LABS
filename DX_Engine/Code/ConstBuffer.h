#ifndef _CONST_BUFFER_H_
#define _CONST_BUFFER_H_

enum class ConstBufferType : unsigned char {
    PerFrame        = 0,
    PerInstance     = 1,
    PerMaterial     = 2,
    PerLight        = 3,
    // <---- Add new types here

    Total
};

struct alignas(16) CBPerFrame {
    DirectX::SimpleMath::Vector4    EyePos;
    DirLight                        DirLight;
    PointLight                      PointLightsArr[3];

    static const ConstBufferType    Slot = ConstBufferType::PerFrame;
};

struct alignas(16) CBPerInstance {
    DirectX::SimpleMath::Matrix     WorldViewProj;
    DirectX::SimpleMath::Matrix     WorldView;
    DirectX::SimpleMath::Matrix     World;

    static const ConstBufferType    Slot = ConstBufferType::PerInstance;
};

struct alignas(16) CBPerMaterial {
    DirectX::SimpleMath::Vector4    AmbientColor;
    DirectX::SimpleMath::Vector4    DiffuseColor;
    DirectX::SimpleMath::Vector4    SpecularColor;

    static const ConstBufferType    Slot = ConstBufferType::PerMaterial;
};

//struct alignas(16) CBPerLight {
//    DirectX::SimpleMath::Vector4    Diffuse;
//    DirectX::SimpleMath::Vector4    Ambient;
//    DirectX::SimpleMath::Vector4    Attenuation;
//    DirectX::SimpleMath::Vector3    LightPos;
//    float                           LightRange;
//
//    static const ConstBufferType    Slot = ConstBufferType::PerLight;
//};

template <typename T>
class ConstBuffer {
public:
    ConstBuffer() : m_constBuffer(nullptr) {

    }

    explicit ConstBuffer(IDevice* device) {
        Create(device);
    }

    void CopyFrom(const ConstBuffer<T>& other) {
        m_constBuffer = other.m_constBuffer;
    }

    void Create(IDevice* device) {
        D3DBufferDesc desc = {};

        desc.ByteWidth          = sizeof(T);
        desc.Usage              = D3D11_USAGE_DYNAMIC;
        desc.BindFlags          = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags     = D3D11_CPU_ACCESS_WRITE;

        ThrowIfFailed(
            device->CreateBuffer(&desc, nullptr, &m_constBuffer)
        );
    }

    void SetData(IDevCon* deviceContext, T const& value) {
        assert(m_constBuffer);

        D3DMappedSubres mappedResource;

        ThrowIfFailed(
            deviceContext->Map(m_constBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)
        );

        *static_cast<T*>(mappedResource.pData) = value;
        deviceContext->Unmap(m_constBuffer.Get(), 0);
    }

    IConstBuffer* GetBuffer() const noexcept {
        return m_constBuffer.Get();
    }

private:
    Microsoft::WRL::ComPtr<IConstBuffer> m_constBuffer;
};

#endif //_CONST_BUFFER_H_
