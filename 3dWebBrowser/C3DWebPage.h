#pragma once
#define WEB_BROWSER_RENDER_EXPORTS
#include "IWebRender.h"
#include <map>
#include <vector>
#include <string>
#include <xstring>
extern "C"
{
#include "browser.h"
};
typedef struct  
{
	HWND  mainWnd;
	HWND  hIEService;
	DWORD dwAviseWindow;
	DWORD dwAdviseCookie;
	RECT  dirtyRect;
	long  isNeedNewWindow;
	long  isDirty;
	long  needWebDynmaicUpdate;
	long  needPopupIe;
	long  scrollx;
	long  scrolly;
	LPVOID dwAdviseWindowBuf;
	LPVOID dwAdviseCookieBuf;
	LPVOID dwOleObjBuf;
	std::vector<HWND> vecChildWnds;
}IEWebItem,*PIEWebItem;

class C3DWebPage : public IWebPage
{
protected:
	IEWebItem webItem;
	std::wstring m_wstrWebIdentity;
	RECT* m_rect;
public:
	C3DWebPage(void);
	virtual ~C3DWebPage(void);
public:
	//根据url进行浏览
	virtual void Browser( const char* url );
	//网页是否有变化
	virtual bool IsNeedUpdate();
	//把页页绘制到指定HDC上
	virtual void DrawToTexture( HDC hdc );
	//发送消息给webpage
	virtual void SendMessage( UINT nMsg, WPARAM wparam, LPARAM lparam);
	//后退
	virtual void Back(void);
	//前进
	virtual void Front(void);
	// 设置是否弹出新的页面
	virtual void EnablePopNewWindow(BOOL BEnable);
	//设置局部网页
	virtual void SetPartPage(long scrollx,long scrolly,long width,long height,long startx = 0,long starty = 0);

public:
	// 返回是否是局部网页
	long IsPartWindow(){if(m_rect != NULL) return 1;else return 0;}
	//c++调用js脚本
	void CallJScript(const std::wstring& strFunc,VARIANT* pVarResult = NULL);
	
	void CallJScript(const std::wstring& strFunc,const std::wstring&strArg1,VARIANT*pVarResult = NULL);
	
	void CallJScript(const std::wstring& strFunc,const std::wstring&strArg1,const std::wstring&strArg2,VARIANT*pVarResult = NULL);
	
	void CallJScript(const std::wstring& strFunc,const std::wstring&strArg1,const std::wstring&strArg2,const std::wstring&strArg3,VARIANT*pVarResult = NULL);
	
	void CallJScript(const std::wstring& strFunc,const std::vector<std::wstring>&paramArray,VARIANT*pVarResult);
protected:
	VARIANT*   AllocFunctionRet(const VARIANT* pVarRst);     // this make wparam
	
	StrBuffer* AllocFunAndParams(const std::wstring&funname,const std::vector<std::wstring>& paramArray); // this make lparam
	
	void       FreeBufs(StrBuffer*pBuf);
public:
	inline PIEWebItem GetWebItem()
	{
		return &webItem;
	}
	
	inline void       SetWebItem_MainWnd(HWND mainWnd)
	{
		webItem.mainWnd = mainWnd;
	}
	
	inline void       SetWebItem_IEService(HWND hIEService)
	{
		webItem.hIEService = hIEService;
	}

	inline void       SetWebItem_ScrollX(long scrollx)
	{
		webItem.scrollx = scrollx;
	}

	inline void       SetWebItem_ScrollY(long scrolly)
	{
		webItem.scrolly = scrolly;
	}
	
	inline void       SetWebItem_AdviseCookie(DWORD dwAdviseCookie)
	{
		webItem.dwAdviseCookie = dwAdviseCookie;
	}
	
	inline void       SetWebItem_AdviseWindow(DWORD dwAdviseWindow)
	{
		webItem.dwAviseWindow  = dwAdviseWindow;
	}
	
	inline void       SetWebItem_Dirty(long isDirty)
	{
		webItem.isDirty = isDirty;
	}
	
	inline void       SetWebItem_NeedPopIE(long needPopupIe)
	{
		webItem.needPopupIe = needPopupIe;
	}
	
	inline void       SetWebItem_AdviseCookieBuf(LPVOID cookieBuf)
	{
		webItem.dwAdviseCookieBuf = cookieBuf;
	}
	
	inline void       SetWebItem_AdviseWindowBuf(LPVOID windowBuf)
	{
		webItem.dwAdviseWindowBuf = windowBuf;
	}
	
	inline void       SetWebItem_OleObjBuf(LPVOID oleObjBuf)
	{
		webItem.dwOleObjBuf = oleObjBuf;
	}
	
	inline void       SetWebItem_WebDynamicUpdate(long needDynamicUpdate)
	{
		webItem.needWebDynmaicUpdate = needDynamicUpdate;
	}

	inline void       ResetDirtyRect()
	{
		ZeroMemory(&webItem.dirtyRect,sizeof(long) * 4);
	}
	
	inline void       SetWebItem_DirtyRect(const RECT* rect)
	{
		if(rect)
		{
			if(webItem.dirtyRect.left == 0 && webItem.dirtyRect.top == 0 
				&& webItem.dirtyRect.right == 0 && webItem.dirtyRect.bottom == 0)
				webItem.dirtyRect = *rect;

			else
			{
				if(webItem.dirtyRect.left   > rect->left)  webItem.dirtyRect.left   = rect->left;
				if(webItem.dirtyRect.top    > rect->top)   webItem.dirtyRect.top    = rect->top;
				if(webItem.dirtyRect.right  < rect->right) webItem.dirtyRect.right  = rect->right;
				if(webItem.dirtyRect.bottom < rect->bottom)webItem.dirtyRect.bottom = rect->bottom;
			}		
		}
		else
		{
			webItem.dirtyRect.left = webItem.dirtyRect.top = 0;

			RECT windowRect;
			GetWindowRect(webItem.mainWnd,&windowRect);
			webItem.dirtyRect.right  = windowRect.right - windowRect.left;
			webItem.dirtyRect.bottom = windowRect.bottom - windowRect.top;
		}
	}
	
	void     SetWebItem_ChildWnd(HWND childWnd);
	
	inline const std::wstring&   GetWebUniqueStr()
	{
		return m_wstrWebIdentity;
	}
	
	inline void           SetWebUniqueStr(std::wstring wstrWebUnqiue)
	{
		m_wstrWebIdentity = wstrWebUnqiue;
	}
};

class C3DWebManager : public IWebManager
{
protected:
	std::map<DWORD,C3DWebPage*> m_mapThreadToWebItem;
	
	std::map<long,std::wstring> m_mapIdToFunname;
	
	PJS_CALL_3D      g_jsCall3DFun;
	
	PJS_CALL_3DEX    g_jsCall3DFunEx;

	PEVENT_NEWWINDOW g_eventNewWindow;
	
	FILE*       g_LogFile;
	
	FILE*       g_TraceLog;
public:
	C3DWebManager();
	~C3DWebManager();
public:
	virtual long CreateWebPage( int w, int h, const char* url = NULL );
	
	virtual void DestroyWebPage(long index);
	
	virtual IWebPage* GetWebPage(long index);
	
	virtual void D3D_CALL_JS(long webIndex,const std::wstring& funName,const std::wstring& params);
	
	virtual void D3D_CALL_JSEX(const std::wstring& toWeb/*to web*/,const std::wstring& param1,const std::wstring& param2/*fromWeb*/,const std::wstring& param3);
	
	virtual void Set_JS_CALL_D3D(PJS_CALL_3D js_call3dFun);
	
	virtual void Set_JS_CALL_D3DEX(PJS_CALL_3DEX js_call3dFun);

	virtual void Set_EventNewWindow(PEVENT_NEWWINDOW eventNewWindow);
public:
	C3DWebPage*      FindWebPage(DWORD dwThreadId);

	C3DWebPage*      InsertWebPage(DWORD dwThreadId);

	IWebPage*        GetWebPage(const std::wstring& wstrWeb);

	void             BeginPause(HANDLE* hEvent,long count,DWORD dwThreadId);

	void             EndPause(HANDLE* hEvent,long count,DWORD dwThreadId);

	PJS_CALL_3D      Get_JS_CALL_D3DFun(){return g_jsCall3DFun;}

	PJS_CALL_3DEX    Get_JS_CALL_D3DFunEx(){return g_jsCall3DFunEx;}

	PEVENT_NEWWINDOW GetNewWindowCall(){return g_eventNewWindow;}

	long             FindFunId(std::wstring wstrFun);

	const wchar_t*  FindFunName(long id);

	void            OutputLineString(char* str,size_t size);

	void            OutputTraceLog(wchar_t* wstr);

	void            ClearAll();
protected:
	std::map<DWORD,C3DWebPage*>::iterator DestroyWebPage(DWORD dwThreadId);
};
