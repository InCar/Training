# Mouse
基本的鼠标处理

![mouse](http://ic-media.oss-cn-hangzhou.aliyuncs.com/2016%2F03%2F06Mouse.png)


## Model
+ 由一个点的集合构成
+ 提供了4个基本的函数
    - GetPoints 获取点的内存地址
    - GetCount  获取点的数目
    - Clear     清除所有的点
    - Put       加入一个点

## View
+ 用线段两两连接所有的点
    - OnPaint   绘制

## 拖放支持
+ Model 增加一个函数
    - Move      移动一个点
+ View 增加三个函数
    - DragStart 拖动开始
    - DragEnd   拖动结束
    - Dragging  拖动中

## 更佳的效果
+ 悬停时效果
+ 鼠标移动到了窗口外面
+ 提取共享的函数