#pragma once
#include "Box.h"

using namespace Microsoft::WRL;

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
	virtual void OnPaint(HWND hwnd) override;
	virtual void OnSize(HWND hwnd, UINT state, int cx, int cy);
	virtual BOOL OnEraseBkgnd(HWND hwnd, HDC hdc);
private:
	static const DXGI_FORMAT c_fmt = DXGI_FORMAT_R8G8B8A8_UNORM;
	static const UINT c_uBufferCount = 2;

	UINT64 m_u64Fence = 0;
	UINT m_uBackBufIdx; // 当前后备缓冲区索引

	ComPtr<ID3D12Device6> m_ptrDev;
	ComPtr<ID3D12Fence1> m_ptrFence;
	ComPtr<ID3D12CommandQueue> m_ptrCmdQueue;
	ComPtr<ID3D12CommandAllocator> m_ptrCmdAllocator;
	ComPtr<ID3D12GraphicsCommandList5> m_ptrCmdList;

	ComPtr<IDXGISwapChain1> m_ptrDXGISwapChain;
	ComPtr<ID3D12DescriptorHeap> m_ptrRTVHeap;
	ComPtr<ID3D12DescriptorHeap> m_ptrDSVHeap;
	ComPtr<ID3D12DescriptorHeap> m_ptrCBVHeap;
	ComPtr<ID3D12Resource1> m_ptrBackBuf[c_uBufferCount];
	ComPtr<ID3D12Resource1> m_ptrDepthBuf;

	HRESULT CreateDevice(ComPtr<IDXGIFactory7> &ptrDXGIFactory);
	HRESULT CreateD3D12Heaps();
	HRESULT Resize(int cx, int cy);
	HRESULT CreateSwapChain(ComPtr<IDXGIFactory7>& ptrDXGIFactory, int cx, int cy);
	HRESULT CreateDepthBuffer(int cx, int cy);
	void SetViewPort(int cx, int cy);

	void FlushCmdQueue();
private:
	Box m_box;
};

