#include <windows.h>
#include <exdisp.h>		
#include <mshtml.h>		
#include <mshtmhst.h>	
#include <crtdbg.h>		
#include <shlobj.h>
#include <exdispid.h>
#include <MsHtmdid.h>
#include "browser.h"
#include <tchar.h>
#include <locale.h>
#pragma warning(disable : 4312)


typedef BOOL    (WINAPI *PShowLayer)(HWND,COLORREF,BYTE,DWORD);
typedef HRESULT (STDMETHODCALLTYPE *PCoGetClassObjectFromURL)(REFCLSID rclsid,LPCWSTR szCodeURL,DWORD dwFileVersionMS,DWORD dwFileVersionLS,LPCWSTR szContentType,LPBINDCTX pBindCtx,DWORD dwClsContext,LPVOID pvReserved,REFIID riid,VOID **ppv);
typedef HRESULT (STDMETHODCALLTYPE *PCreateInstance)(IClassFactory * This,IUnknown *pUnkOuter,REFIID riid,void **ppvObject);

PCreateInstance          g_pOldCreateInstaceFlash;
PCreateInstance          g_pOldCreateInstanceWMP;
PCoGetClassObjectFromURL g_pOldCoGetClassObjectFromURL;
PRedrawWindow            g_pOldRedrawWindow;

WNDPROC                  g_pHookerWndOldProc;
WNDPROC                  g_pHookerWndOldProcIE_6;

extern DWORD InitializeWebEvents(HWND hwnd);
extern BOOL  DestroyWebEvents(HWND hwnd, DWORD dwAdviseCookie);
extern long  EmbedBrowserObject(HWND hwnd);
extern void  SetDocumentToAdvise(HWND hwnd);
extern DWORD InitializeWindowEvents(HWND hwnd);
extern BOOL  DestroyWindowEvents(HWND hwnd, DWORD dwAdviseCookie);
extern void  SetDocumentToAdvise(HWND hwnd);
extern void  UnEmbedBrowserObject(HWND hwnd);

// this global fun must be initalize first!!
PSetDirty                g_pSetDirtyFun;
PGetValFromThreadId      g_pGetFromThreadFun;
PSetDwordToThreadId      g_pSetToThreadFun;
long WINAPI InitGlobaFuns(PSetDirty             setDirtyFun,
						  PGetValFromThreadId   getFromThreadFun,
						  PSetDwordToThreadId   setAdviseToThreadFun)
{
	g_pSetDirtyFun            = setDirtyFun;
	g_pGetFromThreadFun       = getFromThreadFun;
	g_pSetToThreadFun         = setAdviseToThreadFun;
	return 1;
}

LRESULT CALLBACK pHookerWndNewProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == 33024)
	{
		SendMessage((HWND)g_pGetFromThreadFun(GetCurrentThreadId(),WEBPAGE_GET_IESERVICEWINDOW),
			WM_KEYDOWN,VK_RETURN,0);
		return 0;
	}
	else
		return g_pHookerWndOldProc(hwnd,uMsg,wParam,lParam);
}
LRESULT CALLBACK pHookerWndNewProcIE_6(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lret = 0;
	if(uMsg == WM_ERASEBKGND)
		return lret;
	else
		lret = g_pHookerWndOldProcIE_6(hwnd,uMsg,wParam,lParam);
	return lret;
}
 long WINAPI DisplayHTMLPage(HWND hwnd, LPCTSTR webPageName)
{
	IWebBrowser2	*webBrowser2;
	VARIANT			myURL;
	IOleObject		*browserObject;
	HRESULT         hret;
	browserObject = *((IOleObject **)GetWindowLong(hwnd, GWL_USERDATA));
	if(browserObject == NULL)return -5;
	if (!browserObject->lpVtbl->QueryInterface(browserObject, &IID_IWebBrowser2, (void**)&webBrowser2))
	{
		VariantInit(&myURL);
		myURL.vt = VT_BSTR;

#ifndef UNICODE
		{
			wchar_t		*buffer;
			DWORD		size;
			size = MultiByteToWideChar(CP_ACP, 0, webPageName, -1, 0, 0);
			if (!(buffer = (wchar_t *)GlobalAlloc(GMEM_FIXED, sizeof(wchar_t) * size))) goto badalloc;
			MultiByteToWideChar(CP_ACP, 0, webPageName, -1, buffer, size);
			myURL.bstrVal = SysAllocString(buffer);
			GlobalFree(buffer);
		}
#else
		myURL.bstrVal = SysAllocString(webPageName);
#endif
		if (!myURL.bstrVal)
		{
			webBrowser2->lpVtbl->Release(webBrowser2);
			return(-6);
		}
		hret = webBrowser2->lpVtbl->Navigate2(webBrowser2, &myURL, NULL, 0, 0, 0);

		SysFreeString(myURL.bstrVal);
		VariantClear(&myURL); 
		webBrowser2->lpVtbl->Release(webBrowser2);
		return(0);
	}

	return(-5);
}

void WINAPI DoPageAction(HWND hwnd, DWORD action)
{	
	IWebBrowser2	*webBrowser2;
	IOleObject		*browserObject;
	browserObject = *((IOleObject **)GetWindowLong(hwnd, GWL_USERDATA));
	if(browserObject == NULL)return;
	if (!browserObject->lpVtbl->QueryInterface(browserObject, &IID_IWebBrowser2, (void**)&webBrowser2))
	{
		switch (action)
		{
		case WEBPAGE_GOBACK:
			{
				webBrowser2->lpVtbl->GoBack(webBrowser2);
				break;
			}

		case WEBPAGE_GOFORWARD:
			{
				webBrowser2->lpVtbl->GoForward(webBrowser2);
				break;
			}

		case WEBPAGE_GOHOME:
			{
				webBrowser2->lpVtbl->GoHome(webBrowser2);
				break;
			}

		case WEBPAGE_SEARCH:
			{
				webBrowser2->lpVtbl->GoSearch(webBrowser2);
				break;
			}

		case WEBPAGE_REFRESH:
			{
				webBrowser2->lpVtbl->Refresh(webBrowser2);
				break;
			}

		case WEBPAGE_STOP:
			{
				webBrowser2->lpVtbl->Stop(webBrowser2);
			}
		}

		webBrowser2->lpVtbl->Release(webBrowser2);
	}
}

void  ResizeBrowser(HWND hwnd, DWORD width, DWORD height)
{
	IWebBrowser2	*webBrowser2;
	IOleObject		*browserObject;
	browserObject = *((IOleObject **)GetWindowLong(hwnd, GWL_USERDATA));
	if(browserObject == NULL)return;
	if (!browserObject->lpVtbl->QueryInterface(browserObject, &IID_IWebBrowser2, (void**)&webBrowser2))
	{
		webBrowser2->lpVtbl->put_Width(webBrowser2, width);
		webBrowser2->lpVtbl->put_Height(webBrowser2, height);
		webBrowser2->lpVtbl->Release(webBrowser2);
	}
}


 HWND WINAPI GethDocObjectView(HWND hwnd)
{
	IOleObject			*browserObject;
	IWebBrowser2		*webBrowser2;
	IServiceProvider    *pServiceProvider;
	IOleWindow          *pOleWindow;
	HWND                hDocObjectView;
	HWND                hShellEmbedding;
	HWND                hIESerivce;
	browserObject = *((IOleObject**)GetWindowLong(hwnd,GWL_USERDATA));
	if(browserObject == NULL)return NULL;
	browserObject->lpVtbl->QueryInterface(browserObject,&IID_IWebBrowser2,(void**)&webBrowser2);
	if(!browserObject->lpVtbl->QueryInterface(browserObject, &IID_IServiceProvider,(void**)&pServiceProvider) &&
		!pServiceProvider->lpVtbl->QueryService(pServiceProvider,&IID_IShellBrowser,&IID_IOleWindow,(void**)&pOleWindow))
	{
		pOleWindow->lpVtbl->GetWindow(pOleWindow,&hShellEmbedding);
		hDocObjectView = FindWindowEx(hShellEmbedding,NULL,TEXT("Shell DocObject View"),NULL);
		hIESerivce     = FindWindowEx(hDocObjectView,NULL,TEXT("Internet Explorer_Server"),NULL);
		browserObject->lpVtbl->Release(browserObject);

		return hIESerivce;
	}
	browserObject->lpVtbl->Release(browserObject);
	return NULL;
}

void  WINAPI PopupIeWindows(HWND hwnd)
{
	IWebBrowser2	*webBrowser2;
	IOleObject		*browserObject;
	IHTMLDocument2  *htmlDoc2;
	BSTR            url;
	HRESULT         hret;
	IDispatch       *lpDispatch;
	browserObject = *((IOleObject **)GetWindowLong(hwnd, GWL_USERDATA));
	if(browserObject == NULL)return;
	hret = browserObject->lpVtbl->QueryInterface(browserObject, &IID_IWebBrowser2, (void**)&webBrowser2);
	if(hret != S_OK)return;
	if(webBrowser2 == NULL)return;
	hret = webBrowser2->lpVtbl->get_Document(webBrowser2, &lpDispatch);
	webBrowser2->lpVtbl->Release(webBrowser2);
	if(hret != S_OK)return;
	if(lpDispatch == NULL)return;
	hret = lpDispatch->lpVtbl->QueryInterface(lpDispatch,&IID_IHTMLDocument2,&htmlDoc2);
	lpDispatch->lpVtbl->Release(lpDispatch);
	if(hret != S_OK)return;
	if(htmlDoc2 == NULL)return;
	htmlDoc2->lpVtbl->get_URL(htmlDoc2,&url);
	ShellExecuteW(NULL,L"open",L"iexplore",url,NULL,SW_SHOWNORMAL);
	htmlDoc2->lpVtbl->Release(htmlDoc2);
}

void  DisableDebug(HWND hwnd)
{
	IWebBrowser2	*webBrowser2;
	IOleObject		*browserObject;
	IHTMLDocument3  *htmlDoc3;
	HRESULT         hret;
	IDispatch       *lpDispatch;
	IHTMLElement    *htmlelement;
	browserObject = *((IOleObject **)GetWindowLong(hwnd, GWL_USERDATA));
	hret = browserObject->lpVtbl->QueryInterface(browserObject, &IID_IWebBrowser2, (void**)&webBrowser2);
	if(hret != S_OK)return;
	if(webBrowser2 == NULL)return;
	hret = webBrowser2->lpVtbl->get_Document(webBrowser2, &lpDispatch);
	webBrowser2->lpVtbl->Release(webBrowser2);
	if(hret != S_OK)return;
	if(lpDispatch == NULL)return;
	hret = lpDispatch->lpVtbl->QueryInterface(lpDispatch,&IID_IHTMLDocument3,&htmlDoc3);
	lpDispatch->lpVtbl->Release(lpDispatch);
	if(hret != S_OK)return;
	if(htmlDoc3 == NULL)return;
	hret = htmlDoc3->lpVtbl->QueryInterface(htmlDoc3,&IID_IHTMLElement,&htmlelement);
	htmlDoc3->lpVtbl->Release(htmlDoc3);
	if(hret != S_OK)return;
	if(htmlelement == NULL)return;
	htmlelement->lpVtbl->insertAdjacentHTML(htmlelement,L"afterBegin",
		L"&nbsp;<SCRIPT For=\"window\"  Event=\"onerror\" defer=\"false\">var noOp = null;</SCRIPT>");
	htmlelement->lpVtbl->Release(htmlelement);
}

long WINAPI IsReadyState(HWND hwnd)
{
	IWebBrowser2	*webBrowser2;
	IOleObject		*browserObject;
	HRESULT         hret;
	READYSTATE      readyState;
	browserObject = *((IOleObject **)GetWindowLong(hwnd, GWL_USERDATA));
	if(browserObject == NULL)return 0;
	hret = browserObject->lpVtbl->QueryInterface(browserObject, &IID_IWebBrowser2, (void**)&webBrowser2);
	browserObject->lpVtbl->Release(browserObject);
	if(hret != S_OK)return 0;
	if(webBrowser2 == NULL)return 0;	
	if(S_OK == webBrowser2->lpVtbl->get_ReadyState(webBrowser2,&readyState))
	{
		webBrowser2->lpVtbl->Release(webBrowser2);
		return readyState;
	}

	webBrowser2->lpVtbl->Release(webBrowser2);
	return -1;
}

long WINAPI ScrollIeWindow(HWND hwnd,long scrollx,long scrolly)
{
	IWebBrowser2	*webBrowser2;
	IOleObject		*browserObject;
	IHTMLDocument2  *htmlDoc2;
	HRESULT         hret;
	IDispatch       *lpDispatch;
	IHTMLWindow2    *htmlWindow2;
	READYSTATE      readyState = 0;
	browserObject = *((IOleObject **)GetWindowLong(hwnd, GWL_USERDATA));
	if(browserObject == NULL)return -1;
	hret = browserObject->lpVtbl->QueryInterface(browserObject, &IID_IWebBrowser2, (void**)&webBrowser2);
	if(hret != S_OK)return -1;
	if(webBrowser2 == NULL)return -1;
	hret = webBrowser2->lpVtbl->get_ReadyState(webBrowser2,&readyState);
	if(hret != S_OK)
	{
		webBrowser2->lpVtbl->Release(webBrowser2);
		return -1;
	}
	if(readyState != READYSTATE_COMPLETE)
	{
		webBrowser2->lpVtbl->Release(webBrowser2);
		return -1;
	}
	hret = webBrowser2->lpVtbl->get_Document(webBrowser2, &lpDispatch);
	webBrowser2->lpVtbl->Release(webBrowser2);
	if(hret != S_OK)return -1;
	if(lpDispatch == NULL)return -1;
	hret = lpDispatch->lpVtbl->QueryInterface(lpDispatch,&IID_IHTMLDocument2,&htmlDoc2);
	lpDispatch->lpVtbl->Release(lpDispatch);
	if(hret != S_OK)return -1;
	if(htmlDoc2 == NULL)return -1;

	if(S_OK == htmlDoc2->lpVtbl->get_parentWindow(htmlDoc2,&htmlWindow2) && htmlWindow2)
	{
		hret = htmlWindow2->lpVtbl->scrollTo(htmlWindow2,scrollx,scrolly);
		htmlWindow2->lpVtbl->Release(htmlWindow2);
		return 1;
	}

	htmlDoc2->lpVtbl->Release(htmlDoc2);

	return 0;
}

LRESULT CALLBACK browserWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		{
			ResizeBrowser(hwnd, LOWORD(lParam), HIWORD(lParam));
			return(0);
		}

	case WM_CREATE:
		{
			if (EmbedBrowserObject(hwnd)) return(-1);
			else
			{
				DWORD dwCurrThreadId = GetCurrentThreadId();
				DWORD dwAdviseCookie = InitializeWebEvents(hwnd);
				DWORD dwAviseWindow  = 0;
				g_pSetToThreadFun(dwCurrThreadId,dwAdviseCookie,WEBPAGE_SET_COOKIEADVISE);
				g_pSetToThreadFun(dwCurrThreadId,(DWORD)hwnd,WEBPAGE_SET_MAINWINDOW);
#ifndef _DEBUG
				DisableDebug(hwnd);
#endif
				SetDocumentToAdvise(hwnd);
				dwAviseWindow = InitializeWindowEvents(hwnd);
				g_pSetToThreadFun(dwCurrThreadId,dwAviseWindow,WEBPAGE_SET_WINDOWADVISE);
			}
			return(0);
		}
	case WM_PAUSE_THREAD:
		{
			HANDLE hasFinish = (HANDLE)wParam;
			HANDLE hEvent = (HANDLE)lParam;
			SetEvent(hasFinish);			
			WaitForSingleObject(hEvent,INFINITE);
		}
		break;
	case WM_BROWSER_URL:
		{
			DisplayHTMLPage(hwnd,(LPCTSTR)lParam);
		}
		return 1;
	case WM_BROWSER_BACK:
		{
			DoPageAction(hwnd,WEBPAGE_GOBACK);
		}
		return 1;
	case WM_BROWSER_FRONT:
		{
			DoPageAction(hwnd,WEBPAGE_GOFORWARD);
		}
		return 1;
	case WM_CALL_JS_ARG_0:
		{
			VARIANT*   pVarRet   = (VARIANT*)wParam;
			StrBuffer* pFunName  = (StrBuffer*)lParam;

			CallKernelJScript(hwnd,pFunName->str,pVarRet,0,NULL);
		}
		return 1;
	case WM_CALL_JS_ARG_1:
		{
			VARIANT*   pVarRet   = (VARIANT*)wParam;
			StrBuffer* pFunName  = (StrBuffer*)lParam;
			StrBuffer* pArgx     = pFunName->pNext;

    		CallKernelJScript(hwnd,pFunName->str,pVarRet,1,pArgx);
		}
		return 1;
	case WM_CALL_JS_ARG_2:
		{
			VARIANT*   pVarRet   = (VARIANT*)wParam;
			StrBuffer* pFunName  = (StrBuffer*)lParam;
			StrBuffer* pArgx     = pFunName->pNext;

			CallKernelJScript(hwnd,pFunName->str,pVarRet,2,pArgx);
		}
		return 1;
	case WM_CALL_JS_ARG_3:
		{
			VARIANT*   pVarRet   = (VARIANT*)wParam;
			StrBuffer* pFunName  = (StrBuffer*)lParam;
			StrBuffer* pArgx     = pFunName->pNext;

			CallKernelJScript(hwnd,pFunName->str,pVarRet,3,pArgx);
		}
		return 1;
	case WM_CALL_JS_ARG_X:
		{
			VARIANT*   pVarRet   = (VARIANT*)wParam;
			StrBuffer* pFunName  = (StrBuffer*)lParam;
			StrBuffer* pArgx     = pFunName->pNext;
			long       numArg    = 0;

			StrBuffer* tmpArg    = pArgx;
			while(tmpArg)
			{
				numArg++;
				tmpArg = (StrBuffer*)(tmpArg->pNext);				
			}

			CallKernelJScript(hwnd,pFunName->str,pVarRet,numArg,pArgx);
		}
		return 1;
	case WM_WEB_STATE:
		{
			return IsReadyState(hwnd);
		}
	}
	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

void WINAPI DestroyWeb(DWORD dwThreadId)
{
	HWND  mainWnd  = (HWND)g_pGetFromThreadFun(dwThreadId,WEBPAGE_GET_MAINWINDOW);
	long  bRet     = 0;
	HANDLE hEvent  = CreateEvent(NULL,TRUE,FALSE,NULL);
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS,FALSE,dwThreadId);
	if( hThread == INVALID_HANDLE_VALUE )
		return;
	if(IsWindow(mainWnd))
	   bRet = PostMessage(mainWnd,WM_TELL_THREAD_END,0,(LPARAM)hEvent);

	if(bRet)
	{
		DWORD dwRet = WaitForSingleObject(hEvent,10000);

		if(dwRet == WAIT_TIMEOUT)
			TerminateThread(hThread,0);
		
		CloseHandle(hEvent);
	}
	else
	{
		bRet = TerminateThread(hThread,0);
	}
	CloseHandle(hThread);
}
typedef struct
{
	long left,top;
	long width,height;
	TCHAR url[MAX_PATH];
}WebItem,*PWebItem;

DWORD WINAPI ThreadCall(LPVOID lparam)
{
	PWebItem    pWebItem = (PWebItem)lparam;
	MSG			msg;
	DWORD       dwExStyle;
	HINSTANCE   hInst;
	HWND        mainWindow;
	HANDLE      hFinishEvent;
	long BRet = 0;
	if(pWebItem == NULL)return -1;
	if (OleInitialize(NULL) == S_OK)
	{
		WNDCLASSEX		wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.hInstance     = GetModuleHandle(NULL);
		wc.lpfnWndProc   = browserWindowProc;
		wc.lpszClassName = TEXT("Browser Object");
		wc.style = CS_HREDRAW|CS_VREDRAW;
		RegisterClassEx(&wc);
		if ((mainWindow = CreateWindowEx(0, wc.lpszClassName, TEXT(""), WS_OVERLAPPEDWINDOW,
			pWebItem->left, pWebItem->top, pWebItem->width,pWebItem->height,
			HWND_DESKTOP, NULL, wc.hInstance , 0)))
		{	
			g_pSetToThreadFun(GetCurrentThreadId(),(DWORD)mainWindow,WEBPAGE_SET_MAINWINDOW);
			SetWindowLong(mainWindow, GWL_EXSTYLE, GetWindowLong(mainWindow, GWL_EXSTYLE) | 0x80000);
			dwExStyle = GetWindowLong(mainWindow, GWL_EXSTYLE); 
			dwExStyle |= WS_EX_TOOLWINDOW; 
			SetWindowLong(mainWindow, GWL_EXSTYLE, dwExStyle); 
			hInst = LoadLibrary(L"User32.DLL");
			if(hInst)
			{
				PShowLayer fun = NULL;
				fun = (PShowLayer)GetProcAddress(hInst, "SetLayeredWindowAttributes");
				if (fun)
					fun(mainWindow, 0, 0, 2);
				FreeLibrary(hInst);
			}
			UnregisterClass(wc.lpszClassName,wc.hInstance);
			DisplayHTMLPage(mainWindow,pWebItem->url);
			ShowWindow(mainWindow,SW_SHOW);
			UpdateWindow(mainWindow);
			GlobalFree(pWebItem);
			
			while( (BRet = GetMessage(&msg,NULL,0,0)) != 0 )
			{
				if(BRet == -1)
				{
					OutputDebugStringA("error");
				}
				if( msg.message == WM_TELL_THREAD_END )
				{
					// »ØÊÕ
					DWORD dwAdviseCookie = g_pGetFromThreadFun(GetCurrentThreadId(),WEBPAGE_GET_COOKIEADVISE);
					DWORD dwAdviseWindow = g_pGetFromThreadFun(GetCurrentThreadId(),WEBPAGE_GET_WINDOWADVISE);
					HWND  hIEService     = GethDocObjectView(msg.hwnd);
					hFinishEvent         = (HANDLE)(msg.lParam);

					DefWindowProc(hIEService,WM_CLOSE,0,0);
					DestroyWebEvents(msg.hwnd,dwAdviseCookie);
					DestroyWindowEvents(msg.hwnd,dwAdviseWindow);
					UnEmbedBrowserObject(msg.hwnd);
					//DestroyWindow(msg.hwnd);
					break;
				}

				if(msg.message == WM_WEBPAGE_DRAW)
				{
					HWND hIEService = GethDocObjectView(msg.hwnd);
					if(g_pGetFromThreadFun(GetCurrentThreadId(),WEBPAGE_GET_IESERVICEWINDOW) != (DWORD)hIEService)
						g_pSetToThreadFun(GetCurrentThreadId(),(DWORD)hIEService,WEBPAGE_SET_IESERVICE);
				}
				if(msg.message == WM_PAGE_SCROLL)
				{
					DWORD dwCurrThreadId = GetCurrentThreadId();
					DWORD scrollX        = g_pGetFromThreadFun(dwCurrThreadId,WEBPAGE_GET_SCROLLX);
					DWORD scrollY        = g_pGetFromThreadFun(dwCurrThreadId,WEBPAGE_GET_SCROLLY);
					if(scrollX && scrollY)
					{
						if(1 == ScrollIeWindow(mainWindow,scrollX,scrollY))
						{
							g_pSetToThreadFun(dwCurrThreadId,0,WEBPAGE_SET_SCROLLX);
							g_pSetToThreadFun(dwCurrThreadId,0,WEBPAGE_SET_SCROLLY);
						}
					}
				}
				
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		OleUninitialize();
		SetEvent(hFinishEvent);

		return(0);
	}
	return 0;
}
long WINAPI IsWebDynamicDirty(HWND hIEService)
{
	HWND    topWnd;
	DWORD   dwCurProcess;
	topWnd = GetTopWindow(NULL);
	GetWindowThreadProcessId(topWnd,&dwCurProcess);
	if(dwCurProcess == GetCurrentProcessId() && IsWindowVisible(topWnd) == 1)
	{
		TCHAR   clsName[MAX_PATH];
		RECT    topWindowRect;
		RECT    mainWindowRect;
		GetClassName(topWnd,clsName,MAX_PATH);
		GetWindowRect(topWnd,    &topWindowRect);
		GetWindowRect(hIEService,&mainWindowRect);
		if(dwCurProcess == GetCurrentProcessId() && wcscmp(clsName,TEXT("Internet Explorer_Server")) == 0
			&& topWindowRect.left > mainWindowRect.left && topWindowRect.left < mainWindowRect.right 
			&& topWindowRect.top  > mainWindowRect.top  && topWindowRect.top  < mainWindowRect.bottom)
			return WEBPAGE_WEBDYNAMIC_UPDATE1;
		else if(dwCurProcess == GetCurrentProcessId() && wcscmp(clsName,TEXT("ComboLBox")) == 0
			&& topWindowRect.left > mainWindowRect.left && topWindowRect.left < mainWindowRect.right 
			&& topWindowRect.top  > mainWindowRect.top  && topWindowRect.top  < mainWindowRect.bottom)
			return WEBPAGE_WEBDYNAMIC_UPDATE2;
		else
			return WEBPAGE_WEBDYNAMIC_NOUPDATE;
	}
	return WEBPAGE_WEBDYNAMIC_NOUPDATE;
}
void WINAPI PaintWebDynamic(HDC hdc,HWND hIESerivce,DWORD dwNeedUpdate)
{	
	HWND    topWnd;
	RECT    topWindowRect;
	RECT    mainWindowRect;
	topWnd       = GetTopWindow(NULL);
	GetWindowRect(topWnd,    &topWindowRect);
	GetWindowRect(hIESerivce,&mainWindowRect);	
	if(dwNeedUpdate == WEBPAGE_WEBDYNAMIC_UPDATE1)
	{
		HDC     topDC;
		DWORD   dwExStyle;
		HMODULE hInst;
		SetWindowLong(topWnd, GWL_EXSTYLE, GetWindowLong(topWnd, GWL_EXSTYLE) | 0x00080000);
		dwExStyle = GetWindowLong(topWnd, GWL_EXSTYLE); 
		dwExStyle |= WS_EX_TOOLWINDOW; 
		SetWindowLong(topWnd, GWL_EXSTYLE, dwExStyle); 
		hInst = LoadLibrary(L"User32.DLL");
		if(hInst)
		{
			PShowLayer fun = NULL;
			fun = (PShowLayer)GetProcAddress(hInst, "SetLayeredWindowAttributes");
			if (fun)
				fun(topWnd, 0, 0, 2);
			FreeLibrary(hInst);
		}
		if(GetWindowLong(topWnd,GWL_WNDPROC) != (long)pHookerWndNewProc)
		{
			g_pHookerWndOldProc = (WNDPROC)GetWindowLong(topWnd,GWL_WNDPROC);
			SetWindowLong(topWnd,GWL_WNDPROC,(long)pHookerWndNewProc);
		}
		topDC = GetDC(topWnd);
		BitBlt(hdc,topWindowRect.left - mainWindowRect.left,topWindowRect.top - mainWindowRect.top,
			topWindowRect.right - topWindowRect.left,
			topWindowRect.bottom - topWindowRect.top,topDC,0,0,SRCCOPY);
		ReleaseDC(topWnd,topDC);

	}	
	if(dwNeedUpdate == WEBPAGE_WEBDYNAMIC_UPDATE2)
	{
		HDC     topDC;
		DWORD   dwExStyle;
		HMODULE hInst;
		SetWindowLong(topWnd, GWL_EXSTYLE, GetWindowLong(topWnd, GWL_EXSTYLE) | 0x00080000);
		dwExStyle = GetWindowLong(topWnd, GWL_EXSTYLE); 
		dwExStyle |= WS_EX_TOOLWINDOW; 
		SetWindowLong(topWnd, GWL_EXSTYLE, dwExStyle); 
		hInst = LoadLibrary(L"User32.DLL");
		if(hInst)
		{
			PShowLayer fun = NULL;
			fun = (PShowLayer)GetProcAddress(hInst, "SetLayeredWindowAttributes");
			if (fun)
				fun(topWnd, 0, 0, 2);
			FreeLibrary(hInst);
		}
		if(GetWindowLong(topWnd,GWL_WNDPROC) != (long)pHookerWndNewProcIE_6 && !g_pHookerWndOldProcIE_6)
		{
			g_pHookerWndOldProcIE_6 = (WNDPROC)GetWindowLong(topWnd,GWL_WNDPROC);
			SetWindowLong(topWnd,GWL_WNDPROC,(long)pHookerWndNewProcIE_6);
		}
		else if(g_pHookerWndOldProcIE_6)
		{
			SetWindowLong(topWnd,GWL_WNDPROC,(long)pHookerWndNewProcIE_6);
		}
		topDC = GetDC(topWnd);
		BitBlt(hdc,topWindowRect.left - mainWindowRect.left,topWindowRect.top - mainWindowRect.top,
			topWindowRect.right - topWindowRect.left,
			topWindowRect.bottom - topWindowRect.top,topDC,0,0,SRCCOPY);
		ReleaseDC(topWnd,topDC);
	}
}
void WINAPI PaintDirtyWeb(HWND hIESerivce,HDC hdc,CONST RECT dirtyRect)
{
	RECT               rc;
	HDC                srcDC;
	GetClientRect(hIESerivce,&rc);
	srcDC = GetDC(hIESerivce);
 	BitBlt(hdc,dirtyRect.left,dirtyRect.top,dirtyRect.right - dirtyRect.left,
 		dirtyRect.bottom - dirtyRect.top,srcDC,dirtyRect.left,dirtyRect.top,SRCCOPY);
	ReleaseDC(hIESerivce,srcDC);
}
void WINAPI PaintWeb(HWND hIESerivce,HDC hdc)
{
	RECT               rc;
	HDC                srcDC;
	GetClientRect(hIESerivce,&rc);
	srcDC = GetDC(hIESerivce);
	BitBlt(hdc,0,0,rc.right - rc.left,rc.bottom - rc.top,srcDC,0,0,SRCCOPY);
	ReleaseDC(hIESerivce,srcDC);
}


 BOOL WINAPI CallKernelJScript(HWND hwnd,LPCTSTR funName,VARIANT*pVarResult,int numArgs,StrBuffer* pFirstBuffer)
 {
	 IWebBrowser2	*webBrowser2;
	 IOleObject		*browserObject;
	 IHTMLDocument2  *htmlDoc2;
	 HRESULT         hret;
	 IDispatch       *lpDispatch;
	 IDispatch       *pScript;
	 DISPID          dispid;
	 int             indexArg;
	 EXCEPINFO       excepInfo;
	 UINT nArgErr = (UINT)-1;
	 DISPPARAMS dispparams = {0};

	 LPOLESTR oleFunName = SysAllocString(funName);
	 browserObject = *((IOleObject **)GetWindowLong(hwnd, GWL_USERDATA));
	 hret = browserObject->lpVtbl->QueryInterface(browserObject, &IID_IWebBrowser2, (void**)&webBrowser2);
	 if(hret != S_OK)return 0;
	 if(webBrowser2 == NULL)return 0;
	 hret = webBrowser2->lpVtbl->get_Document(webBrowser2, &lpDispatch);
	 if(hret != S_OK)return 0;
	 if(lpDispatch == NULL)return 0;
	 hret = lpDispatch->lpVtbl->QueryInterface(lpDispatch,&IID_IHTMLDocument2,&htmlDoc2);
	 if(hret != S_OK)return 0;
	 if(htmlDoc2 == NULL)return 0;
	 hret = htmlDoc2->lpVtbl->get_Script(htmlDoc2,&pScript);
	 if(!pScript)return 0;
 	 hret = pScript->lpVtbl->GetIDsOfNames(pScript,&IID_NULL,&oleFunName,1,LOCALE_SYSTEM_DEFAULT,&dispid);
	 SysFreeString(oleFunName);
	 if(hret != S_OK)return 0;

	 dispparams.cArgs  = numArgs;
	 if(numArgs > 0)
	    dispparams.rgvarg = GlobalAlloc(sizeof(VARIANT) * numArgs,GPTR);
	 else
		 dispparams.rgvarg = NULL;

	 if(numArgs > 0)
	 {
		 StrBuffer* tmpBufer = pFirstBuffer;
		 indexArg = numArgs - 1;

		 while(tmpBufer)
		 {
			 dispparams.rgvarg[indexArg].bstrVal = SysAllocString(tmpBufer->str);
			 dispparams.rgvarg[indexArg].vt      = VT_BSTR;
			 tmpBufer = tmpBufer->pNext;

			 indexArg--;
		 }

	 }

	 hret = pScript->lpVtbl->Invoke(pScript,dispid,&IID_NULL,0,
		 DISPATCH_METHOD,&dispparams,pVarResult,&excepInfo,&nArgErr);

	 for (indexArg = 0;indexArg < numArgs;indexArg++)
	 {
		 SysFreeString(dispparams.rgvarg[indexArg].bstrVal);
	 }

	 webBrowser2->lpVtbl->Release(webBrowser2);
	 lpDispatch->lpVtbl->Release(lpDispatch);
	 htmlDoc2->lpVtbl->Release(htmlDoc2);

	 if(numArgs > 0)
	    GlobalFree(dispparams.rgvarg);
	 if(hret != S_OK)return 0;

	 return 1;
 }
 void WINAPI InjectEvent(HWND mainWindow,HWND hIESerivce,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	HWND topWnd = GetTopWindow(NULL);
	LPARAM oldLParam = lParam;
	if(IsWindowVisible(topWnd) == 1)
	{
		DWORD dwCurProcess;
		TCHAR clsName[MAX_PATH];
		RECT    topWindowRect;
		RECT    mainWindowRect;
		GetWindowRect(topWnd,    &topWindowRect);
		GetWindowRect(hIESerivce,&mainWindowRect);
		GetClassName(topWnd,clsName,MAX_PATH);
		GetWindowThreadProcessId(topWnd,&dwCurProcess);
		if(dwCurProcess == GetCurrentProcessId() && wcscmp(clsName,TEXT("Internet Explorer_Server")) == 0
			&& topWindowRect.left > mainWindowRect.left && topWindowRect.left < mainWindowRect.right 
			&& topWindowRect.top  > mainWindowRect.top  && topWindowRect.top  < mainWindowRect.bottom)
		{
			if(uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP || uMsg == WM_MOUSEMOVE)
			{
				POINT pt = {LOWORD(lParam),HIWORD(lParam)};
				if( 1 == ClientToScreen(hIESerivce,&pt) && 1 == ScreenToClient(topWnd,&pt))
				{
					lParam = MAKELPARAM(pt.x,pt.y);
					SendMessage(topWnd,uMsg,wParam,lParam);
					return;
				}
			}		
		}	

		if(dwCurProcess == GetCurrentProcessId() && wcscmp(clsName,TEXT("ComboLBox")) == 0
			&& topWindowRect.left > mainWindowRect.left && topWindowRect.left < mainWindowRect.right 
			&& topWindowRect.top  > mainWindowRect.top  && topWindowRect.top  < mainWindowRect.bottom)
		{
			if(uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP || uMsg == WM_MOUSEMOVE)
			{
				POINT pt = {LOWORD(lParam),HIWORD(lParam)};
				if( 1 == ClientToScreen(hIESerivce,&pt) && 1 == ScreenToClient(topWnd,&pt))
				{
					lParam = MAKELPARAM(pt.x,pt.y);
					if(uMsg == WM_MOUSEMOVE)
					{
						SendMessage(topWnd,uMsg,0,lParam);
					}
					if(uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP)
					{
						PostMessage(topWnd,uMsg,wParam,lParam);
					}
					return;
				}
			}	
			if(uMsg == 0x020A)
				return;
			if(uMsg == WM_KEYUP || uMsg == WM_KEYDOWN || uMsg == WM_CHAR)
			{
				if(uMsg == WM_KEYUP)
					SendMessage(topWnd,WM_KEYDOWN,wParam,oldLParam);
				return;
			}
		}	
	}
	if(uMsg == 0x020A)
	{
		POINT pt = {LOWORD(lParam),HIWORD(lParam)};
		ClientToScreen(hIESerivce,&pt);
		SendMessage(hIESerivce,uMsg,wParam,MAKELPARAM(pt.x,pt.y));		
		return;
	}

	if(uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP)
	{
		POINT pt = {LOWORD(lParam),HIWORD(lParam)};
		HWND retWnd = ChildWindowFromPoint(hIESerivce,pt);
		if(retWnd && retWnd != hIESerivce)
		{
			POINT pt = {LOWORD(lParam),HIWORD(lParam)};
			if( 1 == ClientToScreen(hIESerivce,&pt) && 1 == ScreenToClient(retWnd,&pt))
			{
				lParam = MAKELPARAM(pt.x,pt.y);
				SendMessage(retWnd,uMsg,0,lParam);
				SendMessage(retWnd,WM_LBUTTONUP,0,lParam);
				return;
			}
		}
	}

	if(uMsg == WM_KEYDOWN || uMsg == WM_KEYUP || uMsg == WM_CHAR)
	    SendMessage(hIESerivce,uMsg,wParam,lParam);
 	else if(uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP || uMsg == WM_LBUTTONDBLCLK || uMsg == WM_MOUSEMOVE)
		PostMessage(hIESerivce,uMsg,wParam,lParam);
	
}
DWORD WINAPI CreateWebTexture(long left,long top,long width,long height,TCHAR* url)
{
	DWORD dwThreadId  = -1;
	PWebItem pWebItem = (PWebItem)GlobalAlloc(GPTR,sizeof(WebItem));
	pWebItem->left    = left;
	pWebItem->top     = top;
	pWebItem->width   = width;
	pWebItem->height  = height;
	if(url == NULL || url == L"")
	{
		memcpy(pWebItem->url,TEXT("about:black"),MAX_PATH);
	}
	else
		memcpy(pWebItem->url,url,wcslen(url) * 2);
	CreateThread(NULL,0,ThreadCall,pWebItem,CREATE_SUSPENDED,&dwThreadId);
	return dwThreadId;
}
 
