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

extern PSetDirty                g_pSetDirtyFun;
extern PGetValFromThreadId      g_pGetFromThreadFun;
extern PSetDwordToThreadId      g_pSetToThreadFun;

extern DWORD InitializeWindowEvents(HWND hwnd);
extern BOOL  DestroyWindowEvents(HWND hwnd, DWORD dwAdviseCookie);
extern long WINAPI ScrollIeWindow(HWND hwnd,long scrollx,long scrolly);
extern void  SetDocumentToAdvise(HWND hwnd);


void ClientToHostWindow(long * CX,long * CY) {	}
void CommandStateChange(long Command,VARIANT_BOOL Enable) {}

void DocumentComplete(IDispatch *pDisp, VARIANT *URL) {
	DWORD dwCurrThreadId = GetCurrentThreadId();
	HWND  mainWindow     = (HWND)g_pGetFromThreadFun(dwCurrThreadId,WEBPAGE_GET_MAINWINDOW);
	HWND  hIEService     = GethDocObjectView(mainWindow);
	DWORD dwAviseWindow  = g_pGetFromThreadFun(dwCurrThreadId,WEBPAGE_GET_WINDOWADVISE);
#ifndef _DEBUG
	DisableDebug(mainWindow);
#endif
	DestroyWindowEvents((HWND)g_pGetFromThreadFun(dwCurrThreadId,WEBPAGE_GET_MAINWINDOW),dwAviseWindow);
	SetDocumentToAdvise(mainWindow);
	dwAviseWindow = InitializeWindowEvents(mainWindow);
	g_pSetToThreadFun(dwCurrThreadId,dwAviseWindow,WEBPAGE_SET_WINDOWADVISE);
	//g_pSetDirtyFun(dwCurrThreadId,2);
}
void BeforeNavigate2(IDispatch *pDisp, VARIANT * url,VARIANT * Flags,VARIANT * TargetFrameName, VARIANT * PostData,VARIANT * Headers,  VARIANT_BOOL * Cancel) 
{
	DWORD dwCurrThread  = GetCurrentThreadId();
	//g_pSetDirtyFun(dwCurrThread,0);
}
/* -------------------------------------------------------------------------------- */
void DownloadBegin(VOID) {
	//g_pSetDirtyFun(GetCurrentThreadId(),0);
}
void DownloadComplete(VOID) {}
void FileDownload(VARIANT_BOOL * ActiveDocument, VARIANT_BOOL * Cancel) {}
void NavigateComplete2(IDispatch *pDisp, VARIANT *URL) {}
void NavigateError(IDispatch *pDisp,VARIANT *URL,VARIANT *TargetFrameName,VARIANT *StatusCode,VARIANT_BOOL *Cancel) {}
void NewWindow2(IDispatch ** ppDisp,VARIANT_BOOL * Cancel) {}
void OnFullScreen(VARIANT_BOOL FullScreen) {}
void OnMenuBar(VARIANT_BOOL MenuBar) {}
void OnQuit(VOID) {}
void OnStatusBar(VARIANT_BOOL StatusBar) {}
void OnTheaterMode(VARIANT_BOOL TheaterMode) {}
void OnToolBar(VARIANT_BOOL ToolBar) {}
void OnVisible(VARIANT_BOOL Visible) {}
void PrintTemplateInstantiation(IDispatch *pDisp) {}
void PrintTemplateTeardown(IDispatch *pDisp) {}
void PrivacyImpactedStateChange(VARIANT_BOOL PrivacyImpacted) {}
void ProgressChange(long Progress, long ProgressMax) {}
void PropertyChange(BSTR szProperty) {}
void SetSecureLockIcon(long SecureLockIcon) {}
void StatusTextChange(BSTR Text) {}
void TitleChange(BSTR Text) {}
void WindowClosing(VARIANT_BOOL IsChildWindow, VARIANT_BOOL *Cancel) {}
void WindowSetHeight(long Height) {}
void WindowSetLeft(long Left) {}
void WindowSetResizable(VARIANT_BOOL Resizable) {}
void WindowSetTop(long Top) {}
void WindowSetWidth(long Width) {}
HRESULT STDMETHODCALLTYPE WebEvents_QueryInterface(DWebBrowserEvents2 FAR* This, REFIID riid, void ** ppvObject);
HRESULT STDMETHODCALLTYPE WebEvents_AddRef(DWebBrowserEvents2 FAR* This);
HRESULT STDMETHODCALLTYPE WebEvents_Release(DWebBrowserEvents2 FAR* This);
HRESULT STDMETHODCALLTYPE WebEvents_GetTypeInfoCount(DWebBrowserEvents2 FAR* This, unsigned int FAR*  pctinfo );
HRESULT STDMETHODCALLTYPE WebEvents_GetTypeInfo(DWebBrowserEvents2 FAR* This, unsigned int  iTInfo, LCID  lcid, ITypeInfo FAR* FAR* ppTInfo );
HRESULT STDMETHODCALLTYPE WebEvents_GetIDsOfNames(DWebBrowserEvents2 FAR* This, REFIID  riid, OLECHAR FAR* FAR*  rgszNames, unsigned int cNames, LCID lcid, DISPID FAR* rgDispId);
HRESULT STDMETHODCALLTYPE WebEvents_Invoke(DWebBrowserEvents2 FAR* This, DISPID  dispIdMember, REFIID  riid, LCID  lcid, WORD  wFlags, DISPPARAMS FAR*  pDispParams, VARIANT FAR*  pVarResult, EXCEPINFO FAR*  pExcepInfo, unsigned int FAR*  puArgErr );
typedef struct tag_DWebBrowserEvents2Ex {
	DWebBrowserEvents2 WebEventsObj;
	LONG refCount;    
} _DWebBrowserEvents2Ex;
DWebBrowserEvents2Vtbl MyDWebBrowserEvents2Vtbl = {
	WebEvents_QueryInterface,
	WebEvents_AddRef,
	WebEvents_Release,
	WebEvents_GetTypeInfoCount,
	WebEvents_GetTypeInfo,
	WebEvents_GetIDsOfNames,
	WebEvents_Invoke
};
HRESULT STDMETHODCALLTYPE WebEvents_QueryInterface(DWebBrowserEvents2 FAR* This, REFIID riid, void ** ppvObject) {
	*ppvObject = NULL;
	if ( IsEqualGUID(riid, &IID_IUnknown) )
		*ppvObject = (void *) This;
	else if ( IsEqualGUID(riid, &IID_IDispatch) )
		*ppvObject = (void *) This;
	else if ( IsEqualGUID(riid, &DIID_DWebBrowserEvents2) )
		*ppvObject = (void *) This;
	if (*ppvObject)	{
		This->lpVtbl->AddRef(This);
		return S_OK;
	}
	else return E_NOINTERFACE;
}
HRESULT STDMETHODCALLTYPE WebEvents_AddRef(DWebBrowserEvents2 FAR* This) {
	return 0;
}
HRESULT STDMETHODCALLTYPE WebEvents_Release(DWebBrowserEvents2 FAR* This) {
	return 0;
}
HRESULT STDMETHODCALLTYPE WebEvents_GetTypeInfoCount(DWebBrowserEvents2 FAR* This, unsigned int FAR*  pctinfo ) {
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE WebEvents_GetTypeInfo(DWebBrowserEvents2 FAR* This, unsigned int  iTInfo, LCID  lcid, ITypeInfo FAR* FAR* ppTInfo ) {
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE WebEvents_GetIDsOfNames(DWebBrowserEvents2 FAR* This, REFIID  riid, OLECHAR FAR* FAR*  rgszNames, unsigned int cNames, LCID lcid, DISPID FAR* rgDispId) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE WebEvents_Invoke(DWebBrowserEvents2 FAR* This, DISPID  dispIdMember, REFIID  riid, LCID  lcid, WORD  wFlags, DISPPARAMS FAR*  pDispParams, VARIANT FAR*  pVarResult, EXCEPINFO FAR*  pExcepInfo, unsigned int FAR*  puArgErr ) {
	switch (dispIdMember) {
	case DISPID_BEFORENAVIGATE2:
		BeforeNavigate2( pDispParams->rgvarg[6].pdispVal,    
			pDispParams->rgvarg[5].pvarVal,     
			pDispParams->rgvarg[4].pvarVal,     
			pDispParams->rgvarg[3].pvarVal,     
			pDispParams->rgvarg[2].pvarVal,     
			pDispParams->rgvarg[1].pvarVal,     
			pDispParams->rgvarg[0].pboolVal);   
		break;
	case DISPID_CLIENTTOHOSTWINDOW:
		ClientToHostWindow(pDispParams->rgvarg[1].plVal, pDispParams->rgvarg[0].plVal);
		break;
	case DISPID_COMMANDSTATECHANGE:
		CommandStateChange(pDispParams->rgvarg[1].lVal, pDispParams->rgvarg[0].boolVal);
		break;
	case DISPID_DOCUMENTCOMPLETE:
		DocumentComplete(pDispParams->rgvarg[1].pdispVal, pDispParams->rgvarg[0].pvarVal);
		break;
	case DISPID_DOWNLOADBEGIN:
		DownloadBegin();
		break;
	case DISPID_DOWNLOADCOMPLETE:
		DownloadComplete();
		break;
	case DISPID_FILEDOWNLOAD:
		FileDownload(pDispParams->rgvarg[1].pboolVal, pDispParams->rgvarg[0].pboolVal);
		break;
	case DISPID_NAVIGATECOMPLETE2:
		NavigateComplete2(pDispParams->rgvarg[1].pdispVal, pDispParams->rgvarg[0].pvarVal);
		break;
	case DISPID_NAVIGATEERROR:
		NavigateError(pDispParams->rgvarg[4].pdispVal, pDispParams->rgvarg[3].pvarVal, pDispParams->rgvarg[2].pvarVal, pDispParams->rgvarg[1].pvarVal, pDispParams->rgvarg[0].pboolVal);
		break;
	case DISPID_NEWWINDOW2:
		NewWindow2(pDispParams->rgvarg[1].ppdispVal, pDispParams->rgvarg[0].pboolVal);
		break;
	case DISPID_NEWWINDOW3:
		{
			*(pDispParams->rgvarg[3].pboolVal) = (VARIANT_BOOL)0xffff;	
			if(wcscmp(L"about:blank",pDispParams->rgvarg[0].bstrVal) != 0)
			{
				StrBuffer* urlBuf = GlobalAlloc(GPTR,sizeof(StrBuffer));
				urlBuf->pNext     = NULL;
				urlBuf->str       = pDispParams->rgvarg[0].bstrVal;
				g_pSetToThreadFun(GetCurrentThreadId(),(DWORD)urlBuf,WEBPAGE_CALL_NEWWINDOW);
				GlobalFree(urlBuf);

				if(g_pGetFromThreadFun(GetCurrentThreadId(),WEBPAGE_GET_NEWWINDOW) == 1)
					DisplayHTMLPage((HWND)g_pGetFromThreadFun(GetCurrentThreadId(),WEBPAGE_GET_MAINWINDOW),pDispParams->rgvarg[0].bstrVal);
			}
		}
		break;
	case DISPID_ONFULLSCREEN:
		OnFullScreen(pDispParams->rgvarg[0].boolVal);
		break;
	case DISPID_ONMENUBAR:
		OnMenuBar(pDispParams->rgvarg[0].boolVal);
		break;
	case DISPID_ONQUIT:
		OnQuit();
		break;
	case DISPID_ONSTATUSBAR:
		OnStatusBar(pDispParams->rgvarg[0].boolVal);
		break;
	case DISPID_ONTHEATERMODE:
		OnTheaterMode(pDispParams->rgvarg[0].boolVal);
		break;
	case DISPID_ONTOOLBAR:
		OnToolBar(pDispParams->rgvarg[0].boolVal);
		break;
	case DISPID_ONVISIBLE:
		OnVisible(pDispParams->rgvarg[0].boolVal);
		break;
	case DISPID_PRINTTEMPLATEINSTANTIATION:
		PrintTemplateInstantiation(pDispParams->rgvarg[0].pdispVal);
		break;
	case DISPID_PRINTTEMPLATETEARDOWN:
		PrintTemplateTeardown(pDispParams->rgvarg[0].pdispVal);
		break;
	case DISPID_PRIVACYIMPACTEDSTATECHANGE:
		PrivacyImpactedStateChange(pDispParams->rgvarg[0].boolVal);
		break;
	case DISPID_PROGRESSCHANGE:
		ProgressChange(pDispParams->rgvarg[1].lVal, pDispParams->rgvarg[0].lVal);
		break;
	case DISPID_PROPERTYCHANGE:
		PropertyChange(pDispParams->rgvarg[0].bstrVal);
		break;
	case DISPID_SETSECURELOCKICON:
		SetSecureLockIcon(pDispParams->rgvarg[0].lVal);
		break;
	case DISPID_STATUSTEXTCHANGE:
		StatusTextChange(pDispParams->rgvarg[0].bstrVal);
		break;
	case DISPID_TITLECHANGE:
		TitleChange(pDispParams->rgvarg[0].bstrVal);
		break;
	case DISPID_WINDOWCLOSING:
		WindowClosing(pDispParams->rgvarg[1].boolVal, pDispParams->rgvarg[0].pboolVal);
		break;
	case DISPID_WINDOWSETHEIGHT:
		WindowSetHeight(pDispParams->rgvarg[0].lVal);
		break;
	case DISPID_WINDOWSETLEFT:
		WindowSetLeft(pDispParams->rgvarg[0].lVal);
		break;
	case DISPID_WINDOWSETRESIZABLE:
		WindowSetResizable(pDispParams->rgvarg[0].boolVal);
		break;
	case DISPID_WINDOWSETTOP:
		WindowSetTop(pDispParams->rgvarg[0].lVal);
		break;
	case DISPID_WINDOWSETWIDTH:
		WindowSetWidth(pDispParams->rgvarg[0].lVal);
		break;
	default:
		return DISP_E_MEMBERNOTFOUND;
	} 
	return S_OK;
}
DWORD InitializeWebEvents(HWND hwnd) {

	_DWebBrowserEvents2Ex      *WebEventsEx;
	IOleObject	               *browserObject;
	HRESULT 	               hret;
	IConnectionPoint           *pConnectionPoint;
	IConnectionPointContainer  *pConnectionPointContainer;
	DWORD                       dwAdviseCookie = 0;
	_DWebBrowserEvents2Ex      *lastWebEvents;
	browserObject = *((IOleObject **)GetWindowLong(hwnd, GWL_USERDATA));
	if(browserObject == NULL)return 0;
	lastWebEvents = (_DWebBrowserEvents2Ex*)g_pGetFromThreadFun(GetCurrentThreadId(),WEBPAGE_GET_COOKIEBUFFER);
	if(lastWebEvents)
	{
		GlobalFree(lastWebEvents);
		lastWebEvents = NULL;
	}
	lastWebEvents = NULL;
	if ( !(WebEventsEx = (_DWebBrowserEvents2Ex *) GlobalAlloc(GMEM_FIXED, sizeof(_DWebBrowserEvents2Ex) ) ) ){
		return 0;
	}
	g_pSetToThreadFun(GetCurrentThreadId(),(DWORD)WebEventsEx,WEBPAGE_SET_COOKIEBUFFER);
	WebEventsEx->WebEventsObj.lpVtbl = &MyDWebBrowserEvents2Vtbl;
	WebEventsEx->refCount = 0;
	hret = browserObject->lpVtbl->QueryInterface(browserObject,&IID_IConnectionPointContainer, (void**)(&pConnectionPointContainer));
	browserObject->lpVtbl->Release(browserObject);
	if (hret == S_OK) {
		hret = pConnectionPointContainer->lpVtbl->FindConnectionPoint(pConnectionPointContainer,&DIID_DWebBrowserEvents2, &pConnectionPoint);
		if (hret == S_OK) {
			hret = pConnectionPoint->lpVtbl->Advise(pConnectionPoint, (IUnknown *) WebEventsEx, &dwAdviseCookie);
			pConnectionPoint->lpVtbl->Release(pConnectionPoint);
		}
		pConnectionPointContainer->lpVtbl->Release(pConnectionPointContainer);
	}
	if (dwAdviseCookie == 0) {
		GlobalFree(WebEventsEx);
		return 0;
	}
	return dwAdviseCookie;
}
BOOL DestroyWebEvents(HWND hwnd, DWORD dwAdviseCookie) {
	IOleObject	               *browserObject;
	HRESULT 	               hret;
	IConnectionPoint           *pConnectionPoint;
	IConnectionPointContainer  *pConnectionPointContainer;
	_DWebBrowserEvents2Ex      *lastWebEvents;
	browserObject = *((IOleObject **)GetWindowLong(hwnd, GWL_USERDATA));
	if(browserObject == NULL)return 0;
	hret = browserObject->lpVtbl->QueryInterface(browserObject,&IID_IConnectionPointContainer, (void**)(&pConnectionPointContainer));
	browserObject->lpVtbl->Release(browserObject);
	if (hret == S_OK) {
		hret = pConnectionPointContainer->lpVtbl->FindConnectionPoint(pConnectionPointContainer,&DIID_DWebBrowserEvents2, &pConnectionPoint);
		if (hret == S_OK) {
			hret = pConnectionPoint->lpVtbl->Unadvise(pConnectionPoint,dwAdviseCookie);
			pConnectionPoint->lpVtbl->Release(pConnectionPoint);
		}
		pConnectionPointContainer->lpVtbl->Release(pConnectionPointContainer);
	}
	lastWebEvents = (_DWebBrowserEvents2Ex*)g_pGetFromThreadFun(GetCurrentThreadId(),WEBPAGE_GET_COOKIEBUFFER);
	if(lastWebEvents)
	{
		GlobalFree(lastWebEvents);
		lastWebEvents = NULL;
	}
	return (hret == S_OK);
}
HRESULT STDMETHODCALLTYPE WindowEvents_QueryInterface(HTMLWindowEvents2 FAR* This, REFIID riid, void ** ppvObject);
HRESULT STDMETHODCALLTYPE WindowEvents_AddRef(HTMLWindowEvents2 FAR* This);
HRESULT STDMETHODCALLTYPE WindowEvents_Release(HTMLWindowEvents2 FAR* This);
HRESULT STDMETHODCALLTYPE WindowEvents_GetTypeInfoCount(HTMLWindowEvents2 FAR* This, unsigned int FAR*  pctinfo );
HRESULT STDMETHODCALLTYPE WindowEvents_GetTypeInfo(HTMLWindowEvents2 FAR* This, unsigned int  iTInfo, LCID  lcid, ITypeInfo FAR* FAR* ppTInfo );
HRESULT STDMETHODCALLTYPE WindowEvents_GetIDsOfNames(HTMLWindowEvents2 FAR* This, REFIID  riid, OLECHAR FAR* FAR*  rgszNames, unsigned int cNames, LCID lcid, DISPID FAR* rgDispId);
HRESULT STDMETHODCALLTYPE WindowEvents_Invoke(HTMLWindowEvents2 FAR* This, DISPID  dispIdMember, REFIID  riid, LCID  lcid, WORD  wFlags, DISPPARAMS FAR*  pDispParams, VARIANT FAR*  pVarResult, EXCEPINFO FAR*  pExcepInfo, unsigned int FAR*  puArgErr );
typedef struct tag_HTMLWindowEvents2Ex {
	HTMLWindowEvents2 WindowEventsObj;
	LONG refCount;    
} _HTMLWindowEvents2Ex;
HTMLWindowEvents2Vtbl MyHTMLWindowEvents2Vtbl = {
	WindowEvents_QueryInterface,
	WindowEvents_AddRef,
	WindowEvents_Release,
	WindowEvents_GetTypeInfoCount,
	WindowEvents_GetTypeInfo,
	WindowEvents_GetIDsOfNames,
	WindowEvents_Invoke
};


DWORD InitializeWindowEvents(HWND hwnd) {
	_HTMLWindowEvents2Ex      *WindowEventsEx;
	IOleObject	                *browserObject;
	IWebBrowser2                *webBrowser2;
	HRESULT 	                hret;
	IConnectionPoint            *pConnectionPoint;
	IConnectionPointContainer   *pConnectionPointContainer;
	DWORD                       dwAdviseCookie = 0;
	LPDISPATCH		            lpDispatch;
	IHTMLDocument2	            *htmlDoc2;
	IHTMLWindow2                *pHtmlWindow2;
	_HTMLWindowEvents2Ex        *lastWindowEvents;
	browserObject = *((IOleObject **)GetWindowLong(hwnd, GWL_USERDATA));
	hret = browserObject->lpVtbl->QueryInterface(browserObject, &IID_IWebBrowser2, (void**)&webBrowser2);
	if(hret != S_OK)
		return 0;
	browserObject->lpVtbl->Release(browserObject);
	if(webBrowser2 == NULL)return -1;
	hret = webBrowser2->lpVtbl->get_Document(webBrowser2, &lpDispatch);
	if(hret != S_OK)
		return 0;
	webBrowser2->lpVtbl->Release(webBrowser2);
	if(lpDispatch == NULL)return -1;
	hret = lpDispatch->lpVtbl->QueryInterface(lpDispatch, &IID_IHTMLDocument2, (void**)&htmlDoc2);
	if(hret != S_OK)
		return 0;
	lpDispatch->lpVtbl->Release(lpDispatch);
	htmlDoc2->lpVtbl->get_parentWindow(htmlDoc2,&pHtmlWindow2);
	htmlDoc2->lpVtbl->Release(htmlDoc2);
	lastWindowEvents = (_HTMLWindowEvents2Ex*)g_pGetFromThreadFun(GetCurrentThreadId(),WEBPAGE_GET_WINDOWBUFFER);
	if(lastWindowEvents)
	{
		GlobalFree(lastWindowEvents);
		lastWindowEvents = NULL;
	}
	if ( !(WindowEventsEx = (_HTMLWindowEvents2Ex *) GlobalAlloc(GMEM_FIXED, sizeof(_HTMLWindowEvents2Ex) ) ) ){
		return 0;
	}
	g_pSetToThreadFun(GetCurrentThreadId(),(DWORD)lastWindowEvents,WEBPAGE_SET_WINDOWBUFFER);
	WindowEventsEx->WindowEventsObj.lpVtbl = &MyHTMLWindowEvents2Vtbl;
	WindowEventsEx->refCount = 0;
	if(pHtmlWindow2 == NULL)return -1;
	hret = pHtmlWindow2->lpVtbl->QueryInterface(pHtmlWindow2,&IID_IConnectionPointContainer, (void**)(&pConnectionPointContainer));
	pHtmlWindow2->lpVtbl->Release(pHtmlWindow2);
	if (hret == S_OK) {
		hret = pConnectionPointContainer->lpVtbl->FindConnectionPoint(pConnectionPointContainer,&DIID_HTMLWindowEvents2, &pConnectionPoint);
		if (hret == S_OK) {
			hret = pConnectionPoint->lpVtbl->Advise(pConnectionPoint, (IUnknown *) WindowEventsEx, &dwAdviseCookie);
			pConnectionPoint->lpVtbl->Release(pConnectionPoint);
		}
		pConnectionPointContainer->lpVtbl->Release(pConnectionPointContainer);
	}
	if (dwAdviseCookie == 0) {
		GlobalFree(WindowEventsEx);
		return 0;
	}
	return dwAdviseCookie;
}
BOOL DestroyWindowEvents(HWND hwnd, DWORD dwAdviseCookie) {
	IOleObject	           *browserObject;
	HRESULT 	           hret;
	IConnectionPoint           *pConnectionPoint;
	IConnectionPointContainer  *pConnectionPointContainer;
	_HTMLWindowEvents2Ex*       lastWindowEvents;
	browserObject = *((IOleObject **)GetWindowLong(hwnd, GWL_USERDATA));
	hret = browserObject->lpVtbl->QueryInterface(browserObject,&IID_IConnectionPointContainer, (void**)(&pConnectionPointContainer));
	browserObject->lpVtbl->Release(browserObject);
	if (hret == S_OK) {
		hret = pConnectionPointContainer->lpVtbl->FindConnectionPoint(pConnectionPointContainer,&DIID_HTMLWindowEvents2, &pConnectionPoint);
		if (hret == S_OK) {
			hret = pConnectionPoint->lpVtbl->Unadvise(pConnectionPoint,dwAdviseCookie);
			pConnectionPoint->lpVtbl->Release(pConnectionPoint);
		}
		pConnectionPointContainer->lpVtbl->Release(pConnectionPointContainer);
	}
	lastWindowEvents = (_HTMLWindowEvents2Ex*)g_pGetFromThreadFun(GetCurrentThreadId(),WEBPAGE_GET_WINDOWBUFFER);
	if(lastWindowEvents)
	{
		GlobalFree(lastWindowEvents);
		lastWindowEvents = NULL;
	}
	return (hret == S_OK);
}

HRESULT STDMETHODCALLTYPE WindowEvents_QueryInterface(HTMLWindowEvents2 FAR* This, REFIID riid, void ** ppvObject) {
	*ppvObject = NULL;
	if ( IsEqualGUID(riid, &IID_IUnknown) )
		*ppvObject = (void *) This;
	else if ( IsEqualGUID(riid, &IID_IDispatch) )
		*ppvObject = (void *) This;
	else if ( IsEqualGUID(riid, &DIID_HTMLWindowEvents2) )
		*ppvObject = (void *) This;
	if (*ppvObject)	{
		This->lpVtbl->AddRef(This);
		return S_OK;
	}
	else return E_NOINTERFACE;
}
HRESULT STDMETHODCALLTYPE WindowEvents_AddRef(HTMLWindowEvents2 FAR* This) {
	return InterlockedIncrement( &((_HTMLWindowEvents2Ex *) This)->refCount );
}
HRESULT STDMETHODCALLTYPE WindowEvents_Release(HTMLWindowEvents2 FAR* This) {
	return InterlockedDecrement( &((_HTMLWindowEvents2Ex *) This)->refCount );
}
HRESULT STDMETHODCALLTYPE WindowEvents_GetTypeInfoCount(HTMLWindowEvents2 FAR* This, unsigned int FAR*  pctinfo ) {
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE WindowEvents_GetTypeInfo(HTMLWindowEvents2 FAR* This, unsigned int  iTInfo, LCID  lcid, ITypeInfo FAR* FAR* ppTInfo ) {
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE WindowEvents_GetIDsOfNames(HTMLWindowEvents2 FAR* This, REFIID  riid, OLECHAR FAR* FAR*  rgszNames, unsigned int cNames, LCID lcid, DISPID FAR* rgDispId) {
	return S_OK;
}
HRESULT STDMETHODCALLTYPE WindowEvents_Invoke(HTMLWindowEvents2 FAR* This, DISPID  dispIdMember, REFIID  riid, LCID  lcid, WORD  wFlags, DISPPARAMS FAR*  pDispParams, VARIANT FAR*  pVarResult, EXCEPINFO FAR*  pExcepInfo, unsigned int FAR*  puArgErr ) {
	switch (dispIdMember)
	{
	case DISPID_HTMLWINDOWEVENTS2_ONSCROLL:
		g_pSetToThreadFun(GetCurrentThreadId(),0,WEBPAGE_SET_DIRTY_RECT);
		g_pSetDirtyFun(GetCurrentThreadId(),1);
		break;
	} 
	return S_OK;
}