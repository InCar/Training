#pragma once

using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;

struct BoxVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};

class Box
{
public:
	Box();
	virtual ~Box();

	const vector<D3D12_INPUT_ELEMENT_DESC>& GetInputDesc() { return m_vInputDesc; };
	const D3D12_VERTEX_BUFFER_VIEW& GetViewOfVB() { return m_viewVB; }
	const D3D12_INDEX_BUFFER_VIEW& GetViewOfIB() { return m_viewIB; }
	unsigned int GetIBCount() { return _countof(m_indicies); }

	HRESULT CreateD3DRes(ComPtr<ID3D12GraphicsCommandList>& ptrCmdList);
private:
	// input layout
	vector<D3D12_INPUT_ELEMENT_DESC> m_vInputDesc;
	// 顶点缓冲区视图
	D3D12_VERTEX_BUFFER_VIEW m_viewVB;
	// 索引缓冲区视图
	D3D12_INDEX_BUFFER_VIEW m_viewIB;
	// 8个顶点
	BoxVertex m_vertices[8];
	// 6个面,每面2个三角形
	unsigned int m_indicies[3 * 12];

	ComPtr<ID3D12Resource1> m_ptrVBuf;
	ComPtr<ID3D12Resource1> m_ptrIBuf;
	// 临时缓冲区
	vector<ComPtr<ID3D12Resource1>*> m_vPtrs;

	HRESULT CreateGPUBuffer(ComPtr<ID3D12GraphicsCommandList>& ptrCmdList, ComPtr<ID3D12Resource1>& ptrResDst, void* pSrc, rsize_t size);
};

