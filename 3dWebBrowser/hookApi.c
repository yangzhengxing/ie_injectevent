#include "flash.h"
#include "wmp.h"
#include "detours.h"
#include "browser.h"
extern PGetValFromThreadId      g_pGetFromThreadFun;
extern PSetDwordToThreadId      g_pSetToThreadFun;

HRESULT STDMETHODCALLTYPE MyCreateInstanceWMP(IClassFactory * This,IUnknown *pUnkOuter,REFIID riid,void **ppvObject)
{
	if(g_pOldCreateInstanceWMP)
	{
		HRESULT ret  = g_pOldCreateInstanceWMP(This,pUnkOuter,riid,ppvObject);

		if(0 == g_pGetFromThreadFun(GetCurrentThreadId(),WEBPAGE_IS_VALID_INDEX))
			return ret;

		if(*ppvObject)
		{
			IWMPPlayer4* wmpPlayer4 = NULL;
			((IUnknown*)(*ppvObject))->lpVtbl->QueryInterface(*ppvObject,&IID_IWMPPlayer4,&wmpPlayer4);
			if(wmpPlayer4)
			{
				wmpPlayer4->lpVtbl->put_windowlessVideo(wmpPlayer4,VARIANT_TRUE);
			}
		}		 
		return ret;
	}
	else
		return S_FALSE;
}
HRESULT STDMETHODCALLTYPE MyCreateInstanceFlash(IClassFactory * This,IUnknown *pUnkOuter,REFIID riid,void **ppvObject)
{
	if(g_pOldCreateInstaceFlash)
	{
		HRESULT ret = g_pOldCreateInstaceFlash(This,pUnkOuter,riid,ppvObject);

		if(0 == g_pGetFromThreadFun(GetCurrentThreadId(),WEBPAGE_IS_VALID_INDEX))
			return ret;

		if(*ppvObject)
			((IShockwaveFlash*)(*ppvObject))->lpVtbl->put_WMode(*ppvObject,L"Transparent");

		return ret;
	}
	else
		return S_FALSE;
}

HRESULT STDMETHODCALLTYPE MyCoGetClassObjectFromURL(REFCLSID rclsid,LPCWSTR szCodeURL,DWORD dwFileVersionMS,DWORD dwFileVersionLS,LPCWSTR szContentType,LPBINDCTX pBindCtx,DWORD dwClsContext,LPVOID pvReserved,REFIID riid,VOID **ppv)
{
	if(g_pOldCoGetClassObjectFromURL)
	{
		HRESULT ret = g_pOldCoGetClassObjectFromURL(rclsid,szCodeURL,dwFileVersionMS,dwFileVersionLS,szContentType,pBindCtx,dwClsContext,pvReserved,riid,ppv);

		if(0 == g_pGetFromThreadFun(GetCurrentThreadId(),WEBPAGE_IS_VALID_INDEX))
			return ret;

		if(ret == S_OK && !g_pOldCreateInstaceFlash && !memcmp(rclsid,&CLSID_ShockwaveFlash,sizeof(CLSID)))
		{
			g_pOldCreateInstaceFlash = (*((IClassFactory**)ppv))->lpVtbl->CreateInstance;
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach((PVOID*)&g_pOldCreateInstaceFlash,MyCreateInstanceFlash);
			DetourTransactionCommit();
		}
		else if(ret == S_OK && !g_pOldCreateInstanceWMP && !memcmp(rclsid,&CLSID_WindowsMediaPlayer,sizeof(CLSID)))
		{
			g_pOldCreateInstanceWMP = (*((IClassFactory**)ppv))->lpVtbl->CreateInstance;
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach((PVOID*)&g_pOldCreateInstanceWMP,MyCreateInstanceWMP);
			DetourTransactionCommit();	
		}
		else if(memcmp(rclsid,&CLSID_ShockwaveFlash,sizeof(CLSID)) != 0 && memcmp(rclsid,&CLSID_WindowsMediaPlayer,sizeof(CLSID)) != 0)
		{
			DWORD dwCurrThreadId = GetCurrentThreadId();
			HWND mainWnd = (HWND)g_pGetFromThreadFun(dwCurrThreadId,WEBPAGE_GET_MAINWINDOW);
			DoPageAction(mainWnd,WEBPAGE_STOP);
			PopupIeWindows(mainWnd);
		}
		return ret;
	}
	else
		return S_FALSE;
}

BOOL    WINAPI MyRedrawWindow(HWND hWnd,CONST RECT *lprcUpdate,HRGN hrgnUpdate,UINT flags)
{
	BOOL BRet = FALSE;
	if(g_pOldRedrawWindow)
		BRet = g_pOldRedrawWindow(hWnd,lprcUpdate,hrgnUpdate,flags);

	if(0 != g_pGetFromThreadFun(GetCurrentThreadId(),WEBPAGE_IS_VALID_INDEX))
	{
		if(lprcUpdate != NULL)
			g_pSetToThreadFun(GetCurrentThreadId(),(DWORD)lprcUpdate,WEBPAGE_SET_DIRTY_RECT);
	}

	return BRet;
}