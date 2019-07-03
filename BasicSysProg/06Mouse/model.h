#pragma once

#define MAX_POINT_NUM 16

// 向前引用
typedef struct tagModelFunctions ModelFunctions;

typedef struct tagPoint
{
    int x;
    int y;
} Point;

// 模型定义
typedef struct tagModel
{
	// 点缓冲区
	Point points[MAX_POINT_NUM];
    // 点数目
    int nCount;
	// API指针
	ModelFunctions *pAPI;
} Model;

// API定义
Model* ModelInit(Model*);

typedef Point*(*fnModelGetPoints)(Model*);
typedef int(*fnModelGetCount)(Model*);
typedef Model*(*fnModelClear)(Model*);
typedef Model*(*fnModelPut)(Model*, Point);
typedef Model*(*fnModelMove)(Model*, int, Point);

struct tagModelFunctions
{
	fnModelGetPoints GetPoints;
	fnModelGetCount GetCount;
	fnModelClear Clear;
	fnModelPut Put;
    fnModelMove Move;
};