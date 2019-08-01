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

	HRESULT CreateD3DRes(ComPtr<ID3D12Device6> &ptrDev, ComPtr<ID3D12GraphicsCommandList5>& ptrCmdList);
	const vector<D3D12_INPUT_ELEMENT_DESC>& GetInputDesc() { return m_vInputDesc; };
private:
	// input layout
	vector<D3D12_INPUT_ELEMENT_DESC> m_vInputDesc;
	// 8个顶点
	BoxVertex m_vertices[8];
	// 6个面,每面2个三角形
	unsigned int m_indicies[3 * 12];

	ComPtr<ID3D12Resource1> m_ptrVBuf;
	ComPtr<ID3D12Resource> m_ptrUploadBuf;
};

