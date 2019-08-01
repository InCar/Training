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
		// 前
		0, 1, 2,
		0, 2, 3,
		// 后
		4, 6, 5,
		4, 7, 6,
		// 左
		4, 5, 1,
		4, 1, 0,
		// 右
		3, 2, 6,
		3, 6, 7,
		// 顶
		1, 5, 6,
		1, 6, 2,
		// 底
		4, 0, 3,
		4, 3, 7
	}
{
	m_vInputDesc = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
}

Box::~Box()
{
}


HRESULT Box::CreateD3DRes(ComPtr<ID3D12Device6> &ptrDev, ComPtr<ID3D12GraphicsCommandList5> &ptrCmdList)
{
	D3D12_HEAP_PROPERTIES heap{
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0
	};

	D3D12_RESOURCE_DESC desc{
		D3D12_RESOURCE_DIMENSION_BUFFER,
		0, // alignment
		sizeof(m_vertices),
		1,
		1,
		1,
		DXGI_FORMAT_UNKNOWN,
		{ 1, 0 }, // MSAA
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		D3D12_RESOURCE_FLAG_NONE
	};

	HRESULT hr = ptrDev->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&m_ptrVBuf));

	// D3D12_HEAP_TYPE_DEFAULT类型的缓冲区只能被GPU访问,因此,需要通过另一个D3D12_HEAP_TYPE_UPLOAD来拷贝数据
	heap.Type = D3D12_HEAP_TYPE_UPLOAD;
	hr = ptrDev->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_ptrUploadBuf));

	void* pData;
	hr = m_ptrUploadBuf->Map(0, nullptr, &pData);
	memcpy_s(pData, sizeof(m_vertices), m_vertices, sizeof(m_vertices));
	m_ptrUploadBuf->Unmap(0, nullptr);
	
	D3D12_RESOURCE_BARRIER barrier{
		D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		D3D12_RESOURCE_BARRIER_FLAG_NONE,
		{
			m_ptrVBuf.Get(),
			D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_COPY_DEST
		}
	};
	ptrCmdList->ResourceBarrier(1, &barrier);

	// copy verties data
	ptrCmdList->CopyBufferRegion(m_ptrVBuf.Get(), 0, m_ptrUploadBuf.Get(), 0, sizeof(m_vertices));

	barrier.Transition.StateBefore = barrier.Transition.StateAfter;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	ptrCmdList->ResourceBarrier(1, &barrier);

	return hr;
}