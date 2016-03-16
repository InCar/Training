# Basic Drawing
![pentangle](http://ic-media.oss-cn-hangzhou.aliyuncs.com/2016%2F03%2Fpentangle.png)

## DeviceContext
    - 坐标系
	- 字体 前景色 背景色
	- 画笔
	- 画刷

## 坐标系
+ SetMapMode SetViewportOrgEx SetWindowExtEx SetViewportExtEx
	- **MM_TEXT** 1单位是1个像素点，X正半轴在右，Y正半轴在下
	- **MM_LOMETRIC** 1单位是0.1毫米，X正半轴在右，Y正半轴在上
	- **MM_HIMETRIC** 1单位是0.01毫米，X正半轴在右，Y正半轴在下
	- **MM_ISOTROPIC** X轴和Y轴的1单位保持相同，朝向自定义，1单位代表多少自定义
	- **MM_ANISOTROPIC** X轴和Y轴可以完全独立设定

## 基本绘图API
    - 点： SetPixel GetPixel
	- 直线： MoveTo LineTo
	- 填充平面图形： Rectangle Ellipse RoundRect Chord Pie Polygon
