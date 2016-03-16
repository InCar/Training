#include "stdafx.h"
#include "model.h"

wchar_t* ModelGetString(Model *this)
{
	return this->wcsBuffer;
}

int ModelGetStringCount(Model *this)
{
	return (int)wcslen(this->wcsBuffer);
}

Model* ModelClear(Model *this)
{
	this->wcsBuffer[0] = 0;
	return this;
}

Model* ModelPut(Model *this, wchar_t wchar)
{
	// ²âÊÔ»º³åÇø
	size_t len = wcslen(this->wcsBuffer);
	if(wchar == VK_BACK){
		if (len > 0) this->wcsBuffer[len - 1] = 0;
	}
	else if (wchar == VK_ESCAPE) {
		this->pAPI->Clear(this);
	}
	else if(wchar > 0x20){
		if (len < MODEL_MAX_BUFFER - 1) {
			// »º³åÇøÎ´Âú
			this->wcsBuffer[len] = wchar;
			this->wcsBuffer[len + 1] = 0;
		}
		else {
			//  »º³åÇøÒÑÂú
			wcscpy_s(this->wcsBuffer, MODEL_MAX_BUFFER, this->wcsBuffer + 1);
			this->wcsBuffer[MODEL_MAX_BUFFER - 2] = wchar;
			this->wcsBuffer[MODEL_MAX_BUFFER - 1] = 0;
		}
	}
	return this;
}

Model* ModelInit(Model *this)
{
	static ModelFunctions s_fns =
	{
		.GetString = ModelGetString,
		.GetStringCount = ModelGetStringCount,
		.Clear = ModelClear,
		.Put = ModelPut
	};

	memset(this, 0, sizeof(Model));
	this->pAPI = &s_fns;
	return this;
}