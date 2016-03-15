#include "stdafx.h"
#include "model.h"

wchar_t* ModelGetString(Model *pModel)
{
	return pModel->wcsBuffer;
}

int ModelGetStringCount(Model *pModel)
{
	return (int)wcslen(pModel->wcsBuffer);
}

Model* ModelClear(Model *pModel)
{
	pModel->wcsBuffer[0] = 0;
	return pModel;
}

Model* ModelPut(Model *pModel, wchar_t wchar)
{
	// ²âÊÔ»º³åÇø
	size_t len = wcslen(pModel->wcsBuffer);
	if(wchar == VK_BACK){
		if (len > 0) pModel->wcsBuffer[len - 1] = 0;
	}
	else if (wchar == VK_ESCAPE) {
		pModel->pAPI->Clear(pModel);
	}
	else if(wchar >= 0x30){
		if (len < MODEL_MAX_BUFFER - 1) {
			// »º³åÇøÎ´Âú
			pModel->wcsBuffer[len] = wchar;
			pModel->wcsBuffer[len + 1] = 0;
		}
		else {
			//  »º³åÇøÒÑÂú
			wcscpy_s(pModel->wcsBuffer, MODEL_MAX_BUFFER, pModel->wcsBuffer + 1);
			pModel->wcsBuffer[MODEL_MAX_BUFFER - 2] = wchar;
			pModel->wcsBuffer[MODEL_MAX_BUFFER - 1] = 0;
		}
	}
	return pModel;
}

Model* ModelInit(Model *pModel)
{
	static ModelFunctions s_fns =
	{
		.GetString = ModelGetString,
		.GetStringCount = ModelGetStringCount,
		.Clear = ModelClear,
		.Put = ModelPut
	};

	memset(pModel, 0, sizeof(Model));
	pModel->pAPI = &s_fns;
	return pModel;
}