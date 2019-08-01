#pragma once

using namespace Microsoft::WRL;
using namespace DirectX;

// 顶点数据结构
struct BoxVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};

// 一个立方体
class Box
{
public:
	Box();
	virtual ~Box();

	ID3D11InputLayout* GetLayout() { return m_ptrLayout.Get(); }
	ID3D11Buffer* GetVB() { return m_ptrVB.Get(); }
	ID3D11Buffer* GetIB() { return m_ptrIB.Get(); }
	int GetVBStride() { return sizeof(BoxVertex); }
	int GetIBCount() { return sizeof(m_indicies) / sizeof(unsigned int); }
	DXGI_FORMAT GetIBFmt() { return DXGI_FORMAT_R32_UINT; }

	HRESULT CreateD3DRes(ComPtr<ID3D11Device>& ptrDev, const void* pVS, DWORD dwVS);
private:
	// 8个顶点
	BoxVertex m_vertices[8];
	// 6个面,每面2个三角形
	unsigned int m_indicies[3*12];

	// 数据流格式
	ComPtr<ID3D11InputLayout> m_ptrLayout;
	// 顶点缓冲区
	ComPtr<ID3D11Buffer> m_ptrVB;
	// 索引缓冲区
	ComPtr<ID3D11Buffer> m_ptrIB;

	// 创建数据流格式
	HRESULT CreateInputLayout(ComPtr<ID3D11Device>& ptrDev, const void* pVS, DWORD dwVS);
	// 创建顶点缓冲区
	HRESULT CreateVerticesBuffer(ComPtr<ID3D11Device>& ptrDev);
	// 创建顶点索引缓冲区
	HRESULT CreateIndicesBuffer(ComPtr<ID3D11Device>& ptrDev);
};
