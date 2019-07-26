#include "stdafx.h"
#include "Box.h"

Box::Box()
	: m_vertices{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White)   },
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Gray)   },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red)     },
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green)   },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue)    },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow)  },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan)    },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) }
	},
	m_indicies{
		// Ç°
		0, 1, 2,
		0, 2, 3,
		// ºó
		4, 6, 5,
		4, 7, 6,
		// ×ó
		4, 5, 1,
		4, 1, 0,
		// ÓÒ
		3, 2, 6,
		3, 6, 7,
		// ¶¥
		1, 5, 6,
		1, 6, 2,
		// µ×
		4, 0, 3,
		4, 3, 7
	}
{
}

Box::~Box()
{

}

HRESULT Box::CreateD3DRes(ComPtr<ID3D11Device>& ptrDev, const void* pVS, DWORD dwVS)
{
	HRESULT hr = CreateInputLayout(ptrDev, pVS, dwVS);
	if (FAILED(hr)) return hr;

	hr = CreateVerticesBuffer(ptrDev);
	if (FAILED(hr)) return hr;

	hr = CreateIndicesBuffer(ptrDev);
	return hr;
}

HRESULT Box::CreateInputLayout(ComPtr<ID3D11Device>& ptrDev, const void* pVS, DWORD dwVS)
{
	D3D11_INPUT_ELEMENT_DESC desc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = ptrDev->CreateInputLayout(desc, 2, pVS, dwVS, &m_ptrLayout);
	return hr;
}

HRESULT Box::CreateVerticesBuffer(ComPtr<ID3D11Device> &ptrDev)
{
	D3D11_BUFFER_DESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.ByteWidth = sizeof(m_vertices);
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = m_vertices;

	HRESULT hr = ptrDev->CreateBuffer(&desc, &data, &m_ptrVB);
	return hr;
}

HRESULT Box::CreateIndicesBuffer(ComPtr<ID3D11Device> &ptrDev)
{
	D3D11_BUFFER_DESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.ByteWidth = sizeof(m_indicies);
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = m_indicies;

	HRESULT hr = ptrDev->CreateBuffer(&desc, &data, &m_ptrIB);
	return hr;
}