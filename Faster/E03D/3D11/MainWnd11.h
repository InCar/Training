#pragma once
#include "Box.h"

using namespace Microsoft::WRL;

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

class MainWnd11 : public XWnd
{
public:
	MainWnd11();
	virtual ~MainWnd11();

	BOOL Create();
protected:
	ATOM Register();
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	
	BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct) override;
	void OnDestroy(HWND hwnd) override;
	virtual void OnPaint(HWND hwnd) override;
	virtual BOOL OnEraseBkgnd(HWND hwnd, HDC hdc);
	virtual void OnSize(HWND hwnd, UINT state, int cx, int cy);
private:
	ComPtr<ID3D11Device> m_ptrDevice;
	ComPtr<ID3D11DeviceContext> m_ptrDevCtx;
	ComPtr<IDXGISwapChain> m_ptrDXGISwapChain;
	ComPtr<ID3D11Texture2D> m_ptrZBuf;
	ComPtr<ID3D11RenderTargetView> m_ptrRTV;
	ComPtr<ID3D11DepthStencilView> m_ptrDSV;
	ComPtr<ID3D11RasterizerState> m_ptrRS;
	ComPtr<ID3D11Buffer> m_ptrConstant;

	BYTE *m_pVS, *m_pPS;
	DWORD m_dwVS, m_dwPS;
	ComPtr<ID3D11VertexShader> m_ptrVS;
	ComPtr<ID3D11PixelShader> m_ptrPS;

	// 创建D3D设备
	HRESULT CreateDevice(ComPtr<IDXGIFactory5> &ptrDXGIFactory5);
	// 创建双缓冲区
	HRESULT CreateSwapChain(ComPtr<IDXGIFactory5>& ptrDXGIFactory5);
	// 创建深度缓冲区
	HRESULT CreateDepthBuffer(int cx, int cy);
	// 创建RS资源
	HRESULT CreateRSState();
	// 加载GPU代码
	HRESULT LoadShaders();
	DWORD LoadShader(const wchar_t *shaderFileName, BYTE **ppBuf);
	// 设定坐标
	HRESULT Set3DSpace();
private:
	// 全局坐标定义
	ConstantBuffer m_cb;
	// 被绘制的目标
	Box m_box;
	// 顶点缓冲区
	ComPtr<ID3D11Buffer> m_ptrBoxVB;
	// 索引缓冲区
	ComPtr<ID3D11Buffer> m_ptrBoxIB;
};
