#ifndef __BROWSER_H__
#define __BROWSER_H__
#define WEBPAGE_GOBACK		0
#define WEBPAGE_GOFORWARD	1
#define WEBPAGE_GOHOME		2
#define WEBPAGE_SEARCH		3
#define WEBPAGE_REFRESH		4
#define WEBPAGE_STOP		5
#define WEBPAGE_GET_WINDOWADVISE 6
#define WEBPAGE_GET_COOKIEADVISE 7
#define WEBPAGE_SET_WINDOWADVISE 8
#define WEBPAGE_SET_COOKIEADVISE 9
#define WEBPAGE_GET_MAINWINDOW   10
#define WEBPAGE_SET_MAINWINDOW   11
#define WEBPAGE_GET_IESERVICEWINDOW 12
#define WEBPAGE_SET_IESERVICEWINDOW 13
#define WEBPAGE_SET_CHILDWINDOW     14
#define WEBPAGE_GET_OBJECT_CLASSID  15
#define WEBPAGE_SET_OBJECT_CLASSID  16
#define WEBPAGE_SET_NEED_POPUPIE    17
#define WEBPAGE_GET_NEED_POPUPIE    18
#define WEBPAGE_SET_COOKIEBUFFER    19
#define WEBPAGE_GET_COOKIEBUFFER    20
#define WEBPAGE_SET_WINDOWBUFFER    21
#define WEBPAGE_GET_WINDOWBUFFER    22
#define WEBPAGE_SET_OLEOBJBUFFER    23
#define WEBPAGE_GET_OLEOBJBUFFER    24
#define WEBPAGE_SET_DYNAMIC_UPDATE  25
#define WEBPAGE_GET_DYNAMIC_UPDATE  26
#define WEBPAGE_SET_IESERVICE       44
#define WEBPAGE_GET_IESERVICE       45
#define WEBPAGE_GET_SCROLLX         46
#define WEBPAGE_GET_SCROLLY         47
#define WEBPAGE_SET_SCROLLX         48
#define WEBPAGE_SET_SCROLLY         49

#define WEBPAGE_WEBDYNAMIC_UPDATE1     27
#define WEBPAGE_WEBDYNAMIC_UPDATE2     28
#define WEBPAGE_WEBDYNAMIC_NOUPDATE    29
#define WEBPAGE_WEBDYNMAIC_RESETUPDATE 30

#define WEBPAGE_JS_CALL_3D             34
#define WEBPAGE_GET_JSNAME             35
#define WEBPAGE_GET_JSID               36
#define WEBPAGE_CALL_NEWWINDOW         37
#define WEBPAGE_IS_VALID_INDEX         38
#define WEBPAGE_SET_DIRTY_RECT         39
#define WEBPAGE_GET_NEWWINDOW          40
#define WEBPAGE_IS_PARTWINDOW          41
#define WEBPAGE_IS_DIRTY               42
#define WEBPAGE_RESET_DIRTYRECT        43
// #define DISPID_MSG_IsOurCustomBrowser  31
// #define DISPID_MSG_SetJsId             32
// #define WSTR_IsBrowserMine L"IsSnailWebBrowser"
#define WEBPAGE_INVALIDE_VALUE      0

#define WM_WEBPAGE_DRAW             WM_USER + 39
#define WM_PAUSE_THREAD             WM_USER + 40
#define WM_PRE_PAUSE_THREAD         WM_USER + 41

#define WM_BROWSER_URL              WM_USER + 500
#define WM_BROWSER_BACK             WM_USER + 501
#define WM_BROWSER_FRONT            WM_USER + 502
#define WM_CALL_JS_ARG_0            WM_USER + 503
#define WM_CALL_JS_ARG_1            WM_USER + 504
#define WM_CALL_JS_ARG_2            WM_USER + 505
#define WM_CALL_JS_ARG_3            WM_USER + 506
#define WM_CALL_JS_ARG_X            WM_USER + 507

#define WM_TELL_THREAD_END			WM_USER + 508
#define WM_LIST_INFO_ELEMENT        WM_USER + 1000
#define WM_PAGE_SCROLL              WM_USER + 509
#define WM_WEB_STATE                WM_USER + 510




#define WEBRENDER_TRACE_LOG         WM_USER + 800
#define WEBRENDER_REGISTER_CALL     L"CallFrameMyId"
#define WEBRENDER_JS_CALL           L"JsCallExternJs"
#define WEBRENDER_JS_CALL_BACK      L"CallBackExternJs"
#define WEBRENDER_JS_CALL_ME        L"CallFrame"
#define WEBRENDER_JS_TRACE          L"Trace"

#define WEBRENDER_REGISTER_CALL_ID  0x10000000
#define WEBRENDER_JS_CALL_ID        0x10000001
#define WEBRENDER_GET_REGISTER      0x10000002
#define WEBRENDER_JS_CALLBACK_ID    0x10000003
#define WEBRENDER_JS_CALLME_ID      0x10000004

#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_CLIENTBLOCK
#endif
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif

typedef struct tagStrBuffer
{
	wchar_t* str;
	void* pNext;
}StrBuffer;
//typedef StrBuffer *LPStrBuffer;
typedef BOOL    (WINAPI *PRedrawWindow)(HWND hWnd,CONST RECT *lprcUpdate,HRGN hrgnUpdate,UINT flags);
typedef void    (WINAPI *PSetDirty)(DWORD dwThreadId,long dirty);
typedef DWORD   (WINAPI *PGetValFromThreadId)(DWORD dwThreadId,long wndIndex);
typedef void    (WINAPI *PSetDwordToThreadId)(DWORD dwThreadId,DWORD dwValue,long adviseIndex);
typedef BOOL    (WINAPI *PShowLayer)(HWND,COLORREF,BYTE,DWORD);
typedef HRESULT (STDMETHODCALLTYPE *PCoGetClassObjectFromURL)(REFCLSID rclsid,LPCWSTR szCodeURL,DWORD dwFileVersionMS,DWORD dwFileVersionLS,LPCWSTR szContentType,LPBINDCTX pBindCtx,DWORD dwClsContext,LPVOID pvReserved,REFIID riid,VOID **ppv);
typedef HRESULT (STDMETHODCALLTYPE *PCreateInstance)(IClassFactory * This,IUnknown *pUnkOuter,REFIID riid,void **ppvObject);
extern PCreateInstance          g_pOldCreateInstaceFlash;
extern PCreateInstance          g_pOldCreateInstanceWMP;
extern PCoGetClassObjectFromURL g_pOldCoGetClassObjectFromURL;
extern PRedrawWindow            g_pOldRedrawWindow;
void  WINAPI PopupIeWindows(HWND hwnd);
long  WINAPI DisplayHTMLPage(HWND hwnd, LPCTSTR webPageName);
void  WINAPI DoPageAction(HWND hwnd, DWORD action);
HWND  WINAPI GethDocObjectView(HWND hwnd);
void  WINAPI DestroyWeb(DWORD dwThreadId);
void  WINAPI PaintWebDynamic(HDC hdc,HWND hIESerivce,DWORD dwNeedUpdate);
long  WINAPI IsWebDynamicDirty(HWND hIEService);
void  WINAPI PaintWeb(HWND hIESerivce,HDC hdc);
void  WINAPI PaintDirtyWeb(HWND hIESerivce,HDC hdc,CONST RECT dirtyRect);
BOOL  WINAPI CallKernelJScript(HWND hwnd,LPCTSTR funName,VARIANT*pVarResult,int numArgs,StrBuffer* pFirstBuffer);
void  WINAPI InjectEvent(HWND mainWindow,HWND hIESerivce,UINT uMsg,WPARAM wParam,LPARAM lParam);
void  WINAPI ListElements(HWND mainWindow);
DWORD WINAPI CreateWebTexture(long left,long top,long width,long height,TCHAR* url);
long  WINAPI InitGlobaFuns(PSetDirty setDirtyFun,PGetValFromThreadId getFromThreadFun,PSetDwordToThreadId setAdviseToThreadFun);

BOOL  WINAPI MyRedrawWindow(HWND hWnd,CONST RECT *lprcUpdate,HRGN hrgnUpdate,UINT flags);
HRESULT STDMETHODCALLTYPE MyCoGetClassObjectFromURL(REFCLSID rclsid,LPCWSTR szCodeURL,DWORD dwFileVersionMS,DWORD dwFileVersionLS,LPCWSTR szContentType,LPBINDCTX pBindCtx,DWORD dwClsContext,LPVOID pvReserved,REFIID riid,VOID **ppv);
HRESULT STDMETHODCALLTYPE MyCreateInstanceFlash(IClassFactory * This,IUnknown *pUnkOuter,REFIID riid,void **ppvObject);
HRESULT STDMETHODCALLTYPE MyCreateInstanceWMP(IClassFactory * This,IUnknown *pUnkOuter,REFIID riid,void **ppvObject);
#endif
