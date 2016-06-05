#pragma once

#ifdef WEB_BROWSER_RENDER_EXPORTS
#define WEB_BROWSER_API __declspec(dllexport)
#else
#define WEB_BROWSER_API __declspec(dllimport)
#endif
#include <Windows.h>
#include <xstring>
#include <vector>

class IWebPage
{
public :
	//根据url进行浏览
	virtual void Browser(const char* url) = 0;

	//网页是否有变化
	virtual bool IsNeedUpdate() = 0;

	//把页页绘制到指定HDC上
	virtual void DrawToTexture( HDC hdc ) = 0;

	//发送消息给webpage
	virtual void SendMessage( UINT nMsg, WPARAM wparam, LPARAM lparam ) = 0;

	//页面的一些操作
	//后退
	virtual void Back(void) = 0;

	//前进
	virtual void Front(void) = 0;

	// 设置是否内部弹出新的页面
	virtual void EnablePopNewWindow(BOOL BEnable) = 0;

	//设置局部网页
	virtual void SetPartPage(long scrollx,long scrolly,long width,long height,long startx = 0,long starty = 0) = 0;
};
// js调用3d
typedef std::wstring (*PJS_CALL_3D)(long webIndex,const std::wstring& funName,const std::wstring& paramArray);

typedef std::wstring (*PJS_CALL_3DEX)(const std::vector<std::wstring>& paramArray);

// 打开新的页面
typedef void (*PEVENT_NEWWINDOW)(const std::wstring& url);

class IWebManager
{
public:
	//打开一个web页面,并返回索引号
	virtual long CreateWebPage( int w, int h, const char* url = NULL ) = 0;

	//销毁指定页面
	virtual void DestroyWebPage(long index) = 0;

	//根据索引擎取得web页面
	virtual IWebPage* GetWebPage(long index) = 0;

	// 3d客户端调用js
	virtual void D3D_CALL_JS(long webIndex,const std::wstring& funName,const std::wstring& params) = 0;

	// 3d客户端调用js
	virtual void D3D_CALL_JSEX(const std::wstring& toWeb/*to web*/,const std::wstring& param1,const std::wstring& param2/*fromWeb*/,const std::wstring& param3) = 0;

	// 设置js调用3d的回调
	virtual void Set_JS_CALL_D3D(PJS_CALL_3D js_call3dFun) = 0;

	// 设置新的页面的回调
	virtual void Set_EventNewWindow(PEVENT_NEWWINDOW eventNewWindow) = 0;

	// 设置js调用3d的回调
	virtual void Set_JS_CALL_D3DEX(PJS_CALL_3DEX js_call3dFun) = 0;
};

WEB_BROWSER_API extern IWebManager* GetWebManager();
