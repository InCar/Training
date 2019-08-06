#pragma once
#include "Box.h"

using namespace Microsoft::WRL;

struct WorldViewProj
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProj;
};

class MainWnd12 : public XWnd
{
public:
	MainWnd12();
	virtual ~MainWnd12();

	BOOL Create();
protected:
	ATOM Register();
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	
	BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct) override;
	void OnDestroy(HWND hwnd) override;
	virtual void OnSize(HWND hwnd, UINT state, int cx, int cy);
	virtual void OnPaint(HWND hwnd) override;
	virtual BOOL OnEraseBkgnd(HWND hwnd, HDC hdc);
private:
	static const DXGI_FORMAT c_fmtBack = DXGI_FORMAT_R8G8B8A8_UNORM;
	static const DXGI_FORMAT c_fmtDepthStencil = DXGI_FORMAT_D24_UNORM_S8_UINT;
	static const UINT c_uBufferCount = 2;
	static const DXGI_SAMPLE_DESC c_mass;

	bool m_bWARP = false;
	UINT64 m_u64Fence = 0;
	UINT m_uBackBufIdx; // 当前后备缓冲区索引

	ComPtr<ID3D12Device> m_ptrDev;
	ComPtr<ID3D12Fence1> m_ptrFence;
	ComPtr<ID3D12CommandQueue> m_ptrCmdQueue;
	ComPtr<ID3D12CommandAllocator> m_ptrCmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> m_ptrCmdList;
	ComPtr<ID3D12PipelineState> m_ptrPSO;

	ComPtr<IDXGISwapChain1> m_ptrDXGISwapChain;
	ComPtr<ID3D12DescriptorHeap> m_ptrRTVHeap;
	ComPtr<ID3D12DescriptorHeap> m_ptrDSVHeap;
	ComPtr<ID3D12DescriptorHeap> m_ptrCBVHeap;
	ComPtr<ID3D12RootSignature> m_ptrRootSig;
	ComPtr<ID3DBlob> m_ptrVSBlob;
	ComPtr<ID3DBlob> m_ptrPSBlob;

	ComPtr<ID3D12Resource1> m_ptrBackBuf[c_uBufferCount];
	ComPtr<ID3D12Resource1> m_ptrDepthBuf;
	ComPtr<ID3D12Resource1> m_ptrCB;

	D3D12_VIEWPORT m_viewPort;
	D3D12_RECT m_rectScissor;

	HRESULT CreateDevice(ComPtr<IDXGIFactory4> &ptrDXGIFactory);
	HRESULT CreateD3D12Heaps();
	HRESULT Set3DSpace();
	HRESULT CreateRootSignature();
	HRESULT LoadShaders();
	HRESULT LoadShader(const wchar_t* shaderFileName, ComPtr<ID3DBlob> &ptrBlob);
	HRESULT BuildPSO();

	HRESULT CreateSwapChain(ComPtr<IDXGIFactory4>& ptrDXGIFactory, int cx, int cy);
	HRESULT Resize(int cx, int cy);
	HRESULT ResizeBackBuffer(int cx, int cy);
	HRESULT CreateDepthBuffer(int cx, int cy);

	HRESULT Update();
	HRESULT Render();

	void FlushCmdQueue();
private:
	WorldViewProj m_wvProj;
	Box m_box;
	__int64 m_i64Freq;
	__int64 m_i64Start;
};

