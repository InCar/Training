#pragma once

#define MODEL_MAX_BUFFER 64

// 向前引用
typedef struct tagModelFunctions ModelFunctions;

// 模型定义
typedef struct tagModel
{
	// 字符缓冲区,存放键入的字符序列
	wchar_t wcsBuffer[MODEL_MAX_BUFFER];
	// API指针
	ModelFunctions *pAPI;
} Model;

// API定义
Model* ModelInit(Model*);

typedef wchar_t*(*fnModelGetString)(Model*);
typedef int(*fnModelGetStringCount)(Model*);
typedef Model*(*fnModelClear)(Model*);
typedef Model*(*fnModelPut)(Model*, wchar_t);

struct tagModelFunctions
{
	fnModelGetString GetString;
	fnModelGetStringCount GetStringCount;
	fnModelClear Clear;
	fnModelPut Put;
};