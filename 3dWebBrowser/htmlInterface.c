#include <windows.h>
#include <exdisp.h>		
#include <mshtml.h>		
#include <mshtmhst.h>	
#include <crtdbg.h>		
#include <shlobj.h>
#include <exdispid.h>
#include <MsHtmdid.h>
#include "browser.h"
extern PSetDirty                g_pSetDirtyFun;
extern PGetValFromThreadId      g_pGetFromThreadFun;
extern PSetDwordToThreadId      g_pSetToThreadFun;

void  DisableDebug(HWND hwnd);
DWORD InitializeWindowEvents(HWND hwnd);
BOOL  DestroyWindowEvents(HWND hwnd, DWORD dwAdviseCookie);
HRESULT STDMETHODCALLTYPE Storage_QueryInterface(IStorage FAR* This, REFIID riid, LPVOID FAR* ppvObj);
HRESULT STDMETHODCALLTYPE Storage_AddRef(IStorage FAR* This);
HRESULT STDMETHODCALLTYPE Storage_Release(IStorage FAR* This);
HRESULT STDMETHODCALLTYPE Storage_CreateStream(IStorage FAR* This, const WCHAR *pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStream **ppstm);
HRESULT STDMETHODCALLTYPE Storage_OpenStream(IStorage FAR* This, const WCHAR * pwcsName, void *reserved1, DWORD grfMode, DWORD reserved2, IStream **ppstm);
HRESULT STDMETHODCALLTYPE Storage_CreateStorage(IStorage FAR* This, const WCHAR *pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStorage **ppstg);
HRESULT STDMETHODCALLTYPE Storage_OpenStorage(IStorage FAR* This, const WCHAR * pwcsName, IStorage * pstgPriority, DWORD grfMode, SNB snbExclude, DWORD reserved, IStorage **ppstg);
HRESULT STDMETHODCALLTYPE Storage_CopyTo(IStorage FAR* This, DWORD ciidExclude, IID const *rgiidExclude, SNB snbExclude,IStorage *pstgDest);
HRESULT STDMETHODCALLTYPE Storage_MoveElementTo(IStorage FAR* This, const OLECHAR *pwcsName,IStorage * pstgDest, const OLECHAR *pwcsNewName, DWORD grfFlags);
HRESULT STDMETHODCALLTYPE Storage_Commit(IStorage FAR* This, DWORD grfCommitFlags);
HRESULT STDMETHODCALLTYPE Storage_Revert(IStorage FAR* This);
HRESULT STDMETHODCALLTYPE Storage_EnumElements(IStorage FAR* This, DWORD reserved1, void * reserved2, DWORD reserved3, IEnumSTATSTG ** ppenum);
HRESULT STDMETHODCALLTYPE Storage_DestroyElement(IStorage FAR* This, const OLECHAR *pwcsName);
HRESULT STDMETHODCALLTYPE Storage_RenameElement(IStorage FAR* This, const WCHAR *pwcsOldName, const WCHAR *pwcsNewName);
HRESULT STDMETHODCALLTYPE Storage_SetElementTimes(IStorage FAR* This, const WCHAR *pwcsName, FILETIME const *pctime, FILETIME const *patime, FILETIME const *pmtime);
HRESULT STDMETHODCALLTYPE Storage_SetClass(IStorage FAR* This, REFCLSID clsid);
HRESULT STDMETHODCALLTYPE Storage_SetStateBits(IStorage FAR* This, DWORD grfStateBits, DWORD grfMask);
HRESULT STDMETHODCALLTYPE Storage_Stat(IStorage FAR* This, STATSTG * pstatstg, DWORD grfStatFlag);
static const IStorageVtbl MyIStorageTable = {Storage_QueryInterface,
Storage_AddRef,
Storage_Release,
Storage_CreateStream,
Storage_OpenStream,
Storage_CreateStorage,
Storage_OpenStorage,
Storage_CopyTo,
Storage_MoveElementTo,
Storage_Commit,
Storage_Revert,
Storage_EnumElements,
Storage_DestroyElement,
Storage_RenameElement,
Storage_SetElementTimes,
Storage_SetClass,
Storage_SetStateBits,
Storage_Stat};
const IStorage			MyIStorage = { (IStorageVtbl *)&MyIStorageTable };
HRESULT STDMETHODCALLTYPE Frame_QueryInterface(IOleInPlaceFrame FAR* This, REFIID riid, LPVOID FAR* ppvObj);
HRESULT STDMETHODCALLTYPE Frame_AddRef(IOleInPlaceFrame FAR* This);
HRESULT STDMETHODCALLTYPE Frame_Release(IOleInPlaceFrame FAR* This);
HRESULT STDMETHODCALLTYPE Frame_GetWindow(IOleInPlaceFrame FAR* This, HWND FAR* lphwnd);
HRESULT STDMETHODCALLTYPE Frame_ContextSensitiveHelp(IOleInPlaceFrame FAR* This, BOOL fEnterMode);
HRESULT STDMETHODCALLTYPE Frame_GetBorder(IOleInPlaceFrame FAR* This, LPRECT lprectBorder);
HRESULT STDMETHODCALLTYPE Frame_RequestBorderSpace(IOleInPlaceFrame FAR* This, LPCBORDERWIDTHS pborderwidths);
HRESULT STDMETHODCALLTYPE Frame_SetBorderSpace(IOleInPlaceFrame FAR* This, LPCBORDERWIDTHS pborderwidths);
HRESULT STDMETHODCALLTYPE Frame_SetActiveObject(IOleInPlaceFrame FAR* This, IOleInPlaceActiveObject *pActiveObject, LPCOLESTR pszObjName);
HRESULT STDMETHODCALLTYPE Frame_InsertMenus(IOleInPlaceFrame FAR* This, HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths);
HRESULT STDMETHODCALLTYPE Frame_SetMenu(IOleInPlaceFrame FAR* This, HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject);
HRESULT STDMETHODCALLTYPE Frame_RemoveMenus(IOleInPlaceFrame FAR* This, HMENU hmenuShared);
HRESULT STDMETHODCALLTYPE Frame_SetStatusText(IOleInPlaceFrame FAR* This, LPCOLESTR pszStatusText);
HRESULT STDMETHODCALLTYPE Frame_EnableModeless(IOleInPlaceFrame FAR* This, BOOL fEnable);
HRESULT STDMETHODCALLTYPE Frame_TranslateAccelerator(IOleInPlaceFrame FAR* This, LPMSG lpmsg, WORD wID);
IOleInPlaceFrameVtbl MyIOleInPlaceFrameTable = {Frame_QueryInterface,
Frame_AddRef,
Frame_Release,
Frame_GetWindow,
Frame_ContextSensitiveHelp,
Frame_GetBorder,
Frame_RequestBorderSpace,
Frame_SetBorderSpace,
Frame_SetActiveObject,
Frame_InsertMenus,
Frame_SetMenu,
Frame_RemoveMenus,
Frame_SetStatusText,
Frame_EnableModeless,
Frame_TranslateAccelerator};
typedef struct {
IOleInPlaceFrame	frame;		
HWND				window;
} _IOleInPlaceFrameEx;
HRESULT STDMETHODCALLTYPE Site_QueryInterface(IOleClientSite FAR* This, REFIID riid, void ** ppvObject);
HRESULT STDMETHODCALLTYPE Site_AddRef(IOleClientSite FAR* This);
HRESULT STDMETHODCALLTYPE Site_Release(IOleClientSite FAR* This);
HRESULT STDMETHODCALLTYPE Site_SaveObject(IOleClientSite FAR* This);
HRESULT STDMETHODCALLTYPE Site_GetMoniker(IOleClientSite FAR* This, DWORD dwAssign, DWORD dwWhichMoniker, IMoniker ** ppmk);
HRESULT STDMETHODCALLTYPE Site_GetContainer(IOleClientSite FAR* This, LPOLECONTAINER FAR* ppContainer);
HRESULT STDMETHODCALLTYPE Site_ShowObject(IOleClientSite FAR* This);
HRESULT STDMETHODCALLTYPE Site_OnShowWindow(IOleClientSite FAR* This, BOOL fShow);
HRESULT STDMETHODCALLTYPE Site_RequestNewObjectLayout(IOleClientSite FAR* This);
IOleClientSiteVtbl MyIOleClientSiteTable = {Site_QueryInterface,
Site_AddRef,
Site_Release,
Site_SaveObject,
Site_GetMoniker,
Site_GetContainer,
Site_ShowObject,
Site_OnShowWindow,
Site_RequestNewObjectLayout};
HRESULT STDMETHODCALLTYPE UI_QueryInterface(IDocHostUIHandler FAR* This, REFIID riid, void ** ppvObject);
HRESULT STDMETHODCALLTYPE UI_AddRef(IDocHostUIHandler FAR* This);
HRESULT STDMETHODCALLTYPE UI_Release(IDocHostUIHandler FAR* This);
HRESULT STDMETHODCALLTYPE UI_ShowContextMenu(IDocHostUIHandler FAR* This, DWORD dwID, POINT __RPC_FAR *ppt, IUnknown __RPC_FAR *pcmdtReserved, IDispatch __RPC_FAR *pdispReserved);
HRESULT STDMETHODCALLTYPE UI_GetHostInfo(IDocHostUIHandler FAR* This, DOCHOSTUIINFO __RPC_FAR *pInfo);
HRESULT STDMETHODCALLTYPE UI_ShowUI(IDocHostUIHandler FAR* This, DWORD dwID, IOleInPlaceActiveObject __RPC_FAR *pActiveObject, IOleCommandTarget __RPC_FAR *pCommandTarget, IOleInPlaceFrame __RPC_FAR *pFrame, IOleInPlaceUIWindow __RPC_FAR *pDoc);
HRESULT STDMETHODCALLTYPE UI_HideUI(IDocHostUIHandler FAR* This);
HRESULT STDMETHODCALLTYPE UI_UpdateUI(IDocHostUIHandler FAR* This);
HRESULT STDMETHODCALLTYPE UI_EnableModeless(IDocHostUIHandler FAR* This, BOOL fEnable);
HRESULT STDMETHODCALLTYPE UI_OnDocWindowActivate(IDocHostUIHandler FAR* This, BOOL fActivate);
HRESULT STDMETHODCALLTYPE UI_OnFrameWindowActivate(IDocHostUIHandler FAR* This, BOOL fActivate);
HRESULT STDMETHODCALLTYPE UI_ResizeBorder(IDocHostUIHandler FAR* This, LPCRECT prcBorder, IOleInPlaceUIWindow __RPC_FAR *pUIWindow, BOOL fRameWindow);
HRESULT STDMETHODCALLTYPE UI_TranslateAccelerator(IDocHostUIHandler FAR* This, LPMSG lpMsg, const GUID __RPC_FAR *pguidCmdGroup, DWORD nCmdID);
HRESULT STDMETHODCALLTYPE UI_GetOptionKeyPath(IDocHostUIHandler FAR* This, LPOLESTR __RPC_FAR *pchKey, DWORD dw);
HRESULT STDMETHODCALLTYPE UI_GetDropTarget(IDocHostUIHandler FAR* This, IDropTarget __RPC_FAR *pDropTarget, IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget);
HRESULT STDMETHODCALLTYPE UI_GetExternal(IDocHostUIHandler FAR* This, IDispatch __RPC_FAR *__RPC_FAR *ppDispatch);
HRESULT STDMETHODCALLTYPE UI_TranslateUrl(IDocHostUIHandler FAR* This, DWORD dwTranslate, OLECHAR __RPC_FAR *pchURLIn, OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut);
HRESULT STDMETHODCALLTYPE UI_FilterDataObject(IDocHostUIHandler FAR* This, IDataObject __RPC_FAR *pDO, IDataObject __RPC_FAR *__RPC_FAR *ppDORet);
IDocHostUIHandlerVtbl MyIDocHostUIHandlerTable =  {UI_QueryInterface,
UI_AddRef,
UI_Release,
UI_ShowContextMenu,
UI_GetHostInfo,
UI_ShowUI,
UI_HideUI,
UI_UpdateUI,
UI_EnableModeless,
UI_OnDocWindowActivate,
UI_OnFrameWindowActivate,
UI_ResizeBorder,
UI_TranslateAccelerator,
UI_GetOptionKeyPath,
UI_GetDropTarget,
UI_GetExternal,
UI_TranslateUrl,
UI_FilterDataObject};
HRESULT STDMETHODCALLTYPE IDocHostShow_QueryInterface(IDocHostShowUI * This,REFIID riid,void **ppvObject);
ULONG   STDMETHODCALLTYPE IDocHostShow_AddRef(IDocHostShowUI * This);
ULONG   STDMETHODCALLTYPE IDocHostShow_Release(IDocHostShowUI * This);
HRESULT STDMETHODCALLTYPE IDocHostShow_ShowMessage(IDocHostShowUI * This,HWND hwnd,LPOLESTR lpstrText,LPOLESTR lpstrCaption,
									   DWORD dwType,LPOLESTR lpstrHelpFile,DWORD dwHelpContext,LRESULT *plResult);
HRESULT STDMETHODCALLTYPE IDocHostShow_ShowHelp(IDocHostShowUI * This,HWND hwnd,LPOLESTR pszHelpFile,
									UINT uCommand,DWORD dwData,POINT ptMouse,IDispatch *pDispatchObjectHit);
IDocHostShowUIVtbl MyIDocHostShowUIVbl = {
IDocHostShow_QueryInterface,
IDocHostShow_AddRef,
IDocHostShow_Release,
IDocHostShow_ShowMessage,
IDocHostShow_ShowHelp};
HRESULT STDMETHODCALLTYPE InPlace_QueryInterface(IOleInPlaceSite FAR* This, REFIID riid, void ** ppvObject);
HRESULT STDMETHODCALLTYPE InPlace_AddRef(IOleInPlaceSite FAR* This);
HRESULT STDMETHODCALLTYPE InPlace_Release(IOleInPlaceSite FAR* This);
HRESULT STDMETHODCALLTYPE InPlace_GetWindow(IOleInPlaceSite FAR* This, HWND FAR* lphwnd);
HRESULT STDMETHODCALLTYPE InPlace_ContextSensitiveHelp(IOleInPlaceSite FAR* This, BOOL fEnterMode);
HRESULT STDMETHODCALLTYPE InPlace_CanInPlaceActivate(IOleInPlaceSite FAR* This);
HRESULT STDMETHODCALLTYPE InPlace_OnInPlaceActivate(IOleInPlaceSite FAR* This);
HRESULT STDMETHODCALLTYPE InPlace_OnUIActivate(IOleInPlaceSite FAR* This);
HRESULT STDMETHODCALLTYPE InPlace_GetWindowContext(IOleInPlaceSite FAR* This, LPOLEINPLACEFRAME FAR* lplpFrame,LPOLEINPLACEUIWINDOW FAR* lplpDoc,LPRECT lprcPosRect,LPRECT lprcClipRect,LPOLEINPLACEFRAMEINFO lpFrameInfo);
HRESULT STDMETHODCALLTYPE InPlace_Scroll(IOleInPlaceSite FAR* This, SIZE scrollExtent);
HRESULT STDMETHODCALLTYPE InPlace_OnUIDeactivate(IOleInPlaceSite FAR* This, BOOL fUndoable);
HRESULT STDMETHODCALLTYPE InPlace_OnInPlaceDeactivate(IOleInPlaceSite FAR* This);
HRESULT STDMETHODCALLTYPE InPlace_DiscardUndoState(IOleInPlaceSite FAR* This);
HRESULT STDMETHODCALLTYPE InPlace_DeactivateAndUndo(IOleInPlaceSite FAR* This);
HRESULT STDMETHODCALLTYPE InPlace_OnPosRectChange(IOleInPlaceSite FAR* This, LPCRECT lprcPosRect);
IOleInPlaceSiteVtbl MyIOleInPlaceSiteTable =  {InPlace_QueryInterface,
InPlace_AddRef,
InPlace_Release,
InPlace_GetWindow,
InPlace_ContextSensitiveHelp,
InPlace_CanInPlaceActivate,
InPlace_OnInPlaceActivate,
InPlace_OnUIActivate,
InPlace_GetWindowContext,
InPlace_Scroll,
InPlace_OnUIDeactivate,
InPlace_OnInPlaceDeactivate,
InPlace_DiscardUndoState,
InPlace_DeactivateAndUndo,
InPlace_OnPosRectChange};
HRESULT STDMETHODCALLTYPE Advise_QueryInterface(IAdviseSink * This,REFIID riid,void **ppvObject);
ULONG   STDMETHODCALLTYPE Advise_AddRef(IAdviseSink * This);
ULONG   STDMETHODCALLTYPE Advise_Release(IAdviseSink * This);
void    STDMETHODCALLTYPE Advise_OnDataChange(IAdviseSink * This,FORMATETC *pFormatetc,STGMEDIUM *pStgmed);
void    STDMETHODCALLTYPE Advise_OnViewChange(IAdviseSink * This,DWORD dwAspect,LONG lindex);
void    STDMETHODCALLTYPE Advise_OnRename(IAdviseSink * This,IMoniker *pmk);
void    STDMETHODCALLTYPE Advise_OnSave(IAdviseSink * This);
void    STDMETHODCALLTYPE Advise_OnClose(IAdviseSink * This);
IAdviseSinkVtbl MyAdviseSinkVbl = {
Advise_QueryInterface,
Advise_AddRef,
Advise_Release,
Advise_OnDataChange,
Advise_OnViewChange,
Advise_OnRename,
Advise_OnSave,
Advise_OnClose};
HRESULT STDMETHODCALLTYPE Dispatch_QueryInterface(IDispatch * This,REFIID riid,void **ppvObject);
ULONG   STDMETHODCALLTYPE Dispatch_AddRef(IDispatch * This);
ULONG   STDMETHODCALLTYPE Dispatch_Release(IDispatch * This);
HRESULT STDMETHODCALLTYPE Dispatch_GetTypeInfoCount(IDispatch * This,UINT *pctinfo);
HRESULT STDMETHODCALLTYPE Dispatch_GetTypeInfo(IDispatch * This,UINT iTInfo,LCID lcid,ITypeInfo **ppTInfo);
HRESULT STDMETHODCALLTYPE Dispatch_GetIDsOfNames(IDispatch * This,REFIID riid,LPOLESTR *rgszNames,UINT cNames,LCID lcid,DISPID *rgDispId);
HRESULT STDMETHODCALLTYPE Dispatch_Invoke(IDispatch * This,DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,
DISPPARAMS *pDispParams,VARIANT *pVarResult,EXCEPINFO *pExcepInfo,UINT *puArgErr);
IDispatchVtbl MyDispatchVbl = {
Dispatch_QueryInterface,
Dispatch_AddRef,
Dispatch_Release,
Dispatch_GetTypeInfoCount,
Dispatch_GetTypeInfo,
Dispatch_GetIDsOfNames,
Dispatch_Invoke};

typedef struct {
	IOleInPlaceSite			inplace;	
	_IOleInPlaceFrameEx		frame;		
} _IOleInPlaceSiteEx;
typedef struct {
	IDocHostUIHandler		ui;	
	IDocHostShowUI          showUI;
} _IDocHostUIHandlerEx;
typedef struct {
	IOleClientSite			client;		
	_IOleInPlaceSiteEx		inplace;	
	_IDocHostUIHandlerEx	uiex;			 
	IAdviseSink             adviseSink;
	IDispatch               dispatchJscript;
} _IOleClientSiteEx;

#define NOTIMPLEMENTED _ASSERT(0); return(E_NOTIMPL)
HRESULT STDMETHODCALLTYPE UI_QueryInterface(IDocHostUIHandler FAR* This, REFIID riid, LPVOID FAR* ppvObj)
{
	return(Site_QueryInterface((IOleClientSite *)((char *)This - sizeof(IOleClientSite) - sizeof(_IOleInPlaceSiteEx)), riid, ppvObj));
}

HRESULT STDMETHODCALLTYPE UI_AddRef(IDocHostUIHandler FAR* This)
{
	return(1);
}

HRESULT STDMETHODCALLTYPE UI_Release(IDocHostUIHandler FAR* This)
{
	return(1);
}

HRESULT STDMETHODCALLTYPE UI_ShowContextMenu(IDocHostUIHandler FAR* This, DWORD dwID, POINT __RPC_FAR *ppt, IUnknown __RPC_FAR *pcmdtReserved, IDispatch __RPC_FAR *pdispReserved)
{

	return(S_OK);
}
HRESULT STDMETHODCALLTYPE IDocHostShow_QueryInterface(IDocHostShowUI * This,REFIID riid,void **ppvObject)
{
	return(Site_QueryInterface((IOleClientSite *)((char *)This - sizeof(IOleClientSite) - 
		sizeof(_IOleInPlaceSiteEx) - sizeof(IDocHostUIHandler)), riid, ppvObject));
}
ULONG   STDMETHODCALLTYPE IDocHostShow_AddRef(IDocHostShowUI * This)
{
	return 1;
}
ULONG   STDMETHODCALLTYPE IDocHostShow_Release(IDocHostShowUI * This)
{
	return 1;
}
HRESULT STDMETHODCALLTYPE IDocHostShow_ShowMessage(IDocHostShowUI * This,HWND hwnd,LPOLESTR lpstrText,LPOLESTR lpstrCaption,
	DWORD dwType,LPOLESTR lpstrHelpFile,DWORD dwHelpContext,LRESULT *plResult)
{
	MessageBoxW(NULL,lpstrText,lpstrCaption,MB_OK);
	return S_OK;
}
HRESULT STDMETHODCALLTYPE IDocHostShow_ShowHelp(IDocHostShowUI * This,HWND hwnd,LPOLESTR pszHelpFile,
	UINT uCommand,DWORD dwData,POINT ptMouse,IDispatch *pDispatchObjectHit)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE UI_GetHostInfo(IDocHostUIHandler FAR* This, DOCHOSTUIINFO __RPC_FAR *pInfo)
{
	pInfo->cbSize = sizeof(DOCHOSTUIINFO);
	pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER/*DOCHOSTUIFLAG_FLAT_SCROLLBAR*/;
	pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE UI_ShowUI(IDocHostUIHandler FAR* This, DWORD dwID, IOleInPlaceActiveObject __RPC_FAR *pActiveObject, IOleCommandTarget __RPC_FAR *pCommandTarget, IOleInPlaceFrame __RPC_FAR *pFrame, IOleInPlaceUIWindow __RPC_FAR *pDoc)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE UI_HideUI(IDocHostUIHandler FAR* This)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE UI_UpdateUI(IDocHostUIHandler FAR* This)
{	
	return(S_OK);
}
HRESULT STDMETHODCALLTYPE UI_EnableModeless(IDocHostUIHandler FAR* This, BOOL fEnable)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE UI_OnDocWindowActivate(IDocHostUIHandler FAR* This, BOOL fActivate)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE UI_OnFrameWindowActivate(IDocHostUIHandler FAR* This, BOOL fActivate)
{
	return(S_OK);
}
HRESULT STDMETHODCALLTYPE UI_ResizeBorder(IDocHostUIHandler FAR* This, LPCRECT prcBorder, IOleInPlaceUIWindow __RPC_FAR *pUIWindow, BOOL fRameWindow)
{
	return(S_OK);
}
HRESULT STDMETHODCALLTYPE UI_TranslateAccelerator(IDocHostUIHandler FAR* This, LPMSG lpMsg, const GUID __RPC_FAR *pguidCmdGroup, DWORD nCmdID)
{

	return(S_FALSE);
}
HRESULT STDMETHODCALLTYPE UI_GetOptionKeyPath(IDocHostUIHandler FAR* This, LPOLESTR __RPC_FAR *pchKey, DWORD dw)
{

	return(S_FALSE);
}
HRESULT STDMETHODCALLTYPE UI_GetDropTarget(IDocHostUIHandler FAR* This, IDropTarget __RPC_FAR *pDropTarget, IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget)
{
	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE UI_GetExternal(IDocHostUIHandler FAR* This, IDispatch __RPC_FAR *__RPC_FAR *ppDispatch)
{
	_IOleClientSiteEx* pOleCltSiteEx  = (_IOleClientSiteEx*)((char *)This - sizeof(IOleClientSite) 
		- sizeof(_IOleInPlaceSiteEx));
	*ppDispatch = &(pOleCltSiteEx->dispatchJscript);
	return S_OK;
}
HRESULT STDMETHODCALLTYPE UI_TranslateUrl(IDocHostUIHandler *This, DWORD dwTranslate, OLECHAR *pchURLIn, OLECHAR **ppchURLOut)
{
	return(E_NOTIMPL);
}
HRESULT STDMETHODCALLTYPE UI_FilterDataObject(IDocHostUIHandler FAR* This, IDataObject __RPC_FAR *pDO, IDataObject __RPC_FAR *__RPC_FAR *ppDORet)
{
	*ppDORet = 0;
	return(S_FALSE);
}

HRESULT STDMETHODCALLTYPE Storage_QueryInterface(IStorage FAR* This, REFIID riid, LPVOID FAR* ppvObj)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Storage_AddRef(IStorage FAR* This)
{
	return(1);
}

HRESULT STDMETHODCALLTYPE Storage_Release(IStorage FAR* This)
{
	return(1);
}

HRESULT STDMETHODCALLTYPE Storage_CreateStream(IStorage FAR* This, const WCHAR *pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStream **ppstm)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Storage_OpenStream(IStorage FAR* This, const WCHAR * pwcsName, void *reserved1, DWORD grfMode, DWORD reserved2, IStream **ppstm)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Storage_CreateStorage(IStorage FAR* This, const WCHAR *pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStorage **ppstg)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Storage_OpenStorage(IStorage FAR* This, const WCHAR * pwcsName, IStorage * pstgPriority, DWORD grfMode, SNB snbExclude, DWORD reserved, IStorage **ppstg)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Storage_CopyTo(IStorage FAR* This, DWORD ciidExclude, IID const *rgiidExclude, SNB snbExclude,IStorage *pstgDest)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Storage_MoveElementTo(IStorage FAR* This, const OLECHAR *pwcsName,IStorage * pstgDest, const OLECHAR *pwcsNewName, DWORD grfFlags)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Storage_Commit(IStorage FAR* This, DWORD grfCommitFlags)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Storage_Revert(IStorage FAR* This)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Storage_EnumElements(IStorage FAR* This, DWORD reserved1, void * reserved2, DWORD reserved3, IEnumSTATSTG ** ppenum)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Storage_DestroyElement(IStorage FAR* This, const OLECHAR *pwcsName)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Storage_RenameElement(IStorage FAR* This, const WCHAR *pwcsOldName, const WCHAR *pwcsNewName)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Storage_SetElementTimes(IStorage FAR* This, const WCHAR *pwcsName, FILETIME const *pctime, FILETIME const *patime, FILETIME const *pmtime)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Storage_SetClass(IStorage FAR* This, REFCLSID clsid)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE Storage_SetStateBits(IStorage FAR* This, DWORD grfStateBits, DWORD grfMask)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Storage_Stat(IStorage FAR* This, STATSTG * pstatstg, DWORD grfStatFlag)
{
	NOTIMPLEMENTED;
}



HRESULT STDMETHODCALLTYPE Site_QueryInterface(IOleClientSite FAR* This, REFIID riid, void ** ppvObject)
{

	if (!memcmp(riid, &IID_IUnknown, sizeof(GUID)) || !memcmp(riid, &IID_IOleClientSite, sizeof(GUID)))
		*ppvObject = &((_IOleClientSiteEx *)This)->client;
	else if (!memcmp(riid, &IID_IOleInPlaceSite, sizeof(GUID)))
		*ppvObject = &((_IOleClientSiteEx *)This)->inplace;
	else if (!memcmp(riid, &IID_IDocHostUIHandler, sizeof(GUID)))
		*ppvObject = &((_IOleClientSiteEx *)This)->uiex.ui;
	else if(!memcmp(riid,&IID_IAdviseSink,sizeof(GUID)))
		*ppvObject = &((_IOleClientSiteEx*)This)->adviseSink;
	else if(!memcmp(riid,&IID_IDocHostShowUI,sizeof(GUID)))
		*ppvObject = &((_IOleClientSiteEx*)This)->uiex.showUI;
	else
	{
		*ppvObject = 0;
		return(E_NOINTERFACE);
	}

	return(S_OK);
}

HRESULT STDMETHODCALLTYPE Site_AddRef(IOleClientSite FAR* This)
{
	return(1);
}

HRESULT STDMETHODCALLTYPE Site_Release(IOleClientSite FAR* This)
{
	return(1);
}

HRESULT STDMETHODCALLTYPE Site_SaveObject(IOleClientSite FAR* This)
{
	//NOTIMPLEMENTED;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Site_GetMoniker(IOleClientSite FAR* This, DWORD dwAssign, DWORD dwWhichMoniker, IMoniker ** ppmk)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Site_GetContainer(IOleClientSite FAR* This, LPOLECONTAINER FAR* ppContainer)
{
	*ppContainer = 0;
	return(E_NOINTERFACE);
}

HRESULT STDMETHODCALLTYPE Site_ShowObject(IOleClientSite FAR* This)
{
	return(NOERROR);
}

HRESULT STDMETHODCALLTYPE Site_OnShowWindow(IOleClientSite FAR* This, BOOL fShow)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Site_RequestNewObjectLayout(IOleClientSite FAR* This)
{
	NOTIMPLEMENTED;
}
HRESULT STDMETHODCALLTYPE InPlace_QueryInterface(IOleInPlaceSite FAR* This, REFIID riid, LPVOID FAR* ppvObj)
{
	return(Site_QueryInterface((IOleClientSite *)((char *)This - sizeof(IOleClientSite)), riid, ppvObj));
}

HRESULT STDMETHODCALLTYPE InPlace_AddRef(IOleInPlaceSite FAR* This)
{
	return(1);
}

HRESULT STDMETHODCALLTYPE InPlace_Release(IOleInPlaceSite FAR* This)
{
	return(1);
}

HRESULT STDMETHODCALLTYPE InPlace_GetWindow(IOleInPlaceSite FAR* This, HWND FAR* lphwnd)
{
	*lphwnd = ((_IOleInPlaceSiteEx FAR*)This)->frame.window;
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE InPlace_ContextSensitiveHelp(IOleInPlaceSite FAR* This, BOOL fEnterMode)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE InPlace_CanInPlaceActivate(IOleInPlaceSite FAR* This)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE InPlace_OnInPlaceActivate(IOleInPlaceSite FAR* This)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE InPlace_OnUIActivate(IOleInPlaceSite FAR* This)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE InPlace_GetWindowContext(IOleInPlaceSite FAR* This, LPOLEINPLACEFRAME FAR* lplpFrame, LPOLEINPLACEUIWINDOW FAR* lplpDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	*lplpFrame = (LPOLEINPLACEFRAME)&((_IOleInPlaceSiteEx *)This)->frame;
	*lplpDoc = 0;
	lpFrameInfo->fMDIApp = FALSE;
	lpFrameInfo->hwndFrame = ((_IOleInPlaceFrameEx *)*lplpFrame)->window;
	lpFrameInfo->haccel = 0;
	lpFrameInfo->cAccelEntries = 0;
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE InPlace_Scroll(IOleInPlaceSite FAR* This, SIZE scrollExtent)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE InPlace_OnUIDeactivate(IOleInPlaceSite FAR* This, BOOL fUndoable)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE InPlace_OnInPlaceDeactivate(IOleInPlaceSite FAR* This)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE InPlace_DiscardUndoState(IOleInPlaceSite FAR* This)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE InPlace_DeactivateAndUndo(IOleInPlaceSite FAR* This)
{
	NOTIMPLEMENTED;
}
HRESULT STDMETHODCALLTYPE InPlace_OnPosRectChange(IOleInPlaceSite FAR* This, LPCRECT lprcPosRect)
{
	IOleObject			*browserObject;
	IOleInPlaceObject	*inplace;
	browserObject = *((IOleObject **)((char *)This - sizeof(IOleObject *) - sizeof(IOleClientSite)));
	if(browserObject == NULL)return S_FALSE;
	if (!browserObject->lpVtbl->QueryInterface(browserObject, &IID_IOleInPlaceObject, (void**)&inplace))
	{
		inplace->lpVtbl->SetObjectRects(inplace, lprcPosRect, lprcPosRect);
	}

	return(S_OK);
}
HRESULT STDMETHODCALLTYPE Frame_QueryInterface(IOleInPlaceFrame FAR* This, REFIID riid, LPVOID FAR* ppvObj)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Frame_AddRef(IOleInPlaceFrame FAR* This)
{
	return(1);
}

HRESULT STDMETHODCALLTYPE Frame_Release(IOleInPlaceFrame FAR* This)
{
	return(1);
}

HRESULT STDMETHODCALLTYPE Frame_GetWindow(IOleInPlaceFrame FAR* This, HWND FAR* lphwnd)
{
	*lphwnd = ((_IOleInPlaceFrameEx *)This)->window;
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE Frame_ContextSensitiveHelp(IOleInPlaceFrame FAR* This, BOOL fEnterMode)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Frame_GetBorder(IOleInPlaceFrame FAR* This, LPRECT lprectBorder)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Frame_RequestBorderSpace(IOleInPlaceFrame FAR* This, LPCBORDERWIDTHS pborderwidths)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Frame_SetBorderSpace(IOleInPlaceFrame FAR* This, LPCBORDERWIDTHS pborderwidths)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Frame_SetActiveObject(IOleInPlaceFrame FAR* This, IOleInPlaceActiveObject *pActiveObject, LPCOLESTR pszObjName)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE Frame_InsertMenus(IOleInPlaceFrame FAR* This, HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Frame_SetMenu(IOleInPlaceFrame FAR* This, HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE Frame_RemoveMenus(IOleInPlaceFrame FAR* This, HMENU hmenuShared)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Frame_SetStatusText(IOleInPlaceFrame FAR* This, LPCOLESTR pszStatusText)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE Frame_EnableModeless(IOleInPlaceFrame FAR* This, BOOL fEnable)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE Frame_TranslateAccelerator(IOleInPlaceFrame FAR* This, LPMSG lpmsg, WORD wID)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Advise_QueryInterface(IAdviseSink * This,REFIID riid,void **ppvObj)
{
	return(Site_QueryInterface((IOleClientSite *)((char *)This - sizeof(IOleClientSite) -
		sizeof(_IOleInPlaceSiteEx) - sizeof(_IDocHostUIHandlerEx)), riid, ppvObj));
}
ULONG  STDMETHODCALLTYPE Advise_AddRef(IAdviseSink * This)
{
	return 0;
}
ULONG  STDMETHODCALLTYPE Advise_Release(IAdviseSink * This)
{
	return 0;
}
void   STDMETHODCALLTYPE Advise_OnDataChange(IAdviseSink * This,FORMATETC *pFormatetc,STGMEDIUM *pStgmed)
{

}
void   STDMETHODCALLTYPE Advise_OnViewChange(IAdviseSink * This,DWORD dwAspect,LONG lindex)
{
	g_pSetDirtyFun(GetCurrentThreadId(),1);
}
void   STDMETHODCALLTYPE Advise_OnRename(IAdviseSink * This,IMoniker *pmk)
{

}
void   STDMETHODCALLTYPE Advise_OnSave(IAdviseSink * This)
{

}
void   STDMETHODCALLTYPE Advise_OnClose(IAdviseSink * This)
{

}


void SetDocumentToAdvise(HWND hwnd)
{
	IOleObject             *browserObject;
	IWebBrowser2           *webBrowser2;
	LPDISPATCH             lpDispatch;
	HRESULT                hret;
	_IOleClientSiteEx      *_iOleClientSiteEx;
	IHTMLDocument2         *htmlDoc2;
	IViewObject            *viewObj;
	char                   *ptr;
	browserObject = *((IOleObject**)GetWindowLong(hwnd,GWL_USERDATA));
	if(browserObject == NULL)return;
	ptr           = (char*)GetWindowLong(hwnd,GWL_USERDATA);
	_iOleClientSiteEx = (_IOleClientSiteEx *)(ptr + sizeof(IOleObject *));
	hret = browserObject->lpVtbl->QueryInterface(browserObject,&IID_IWebBrowser2,(void**)&webBrowser2);
	browserObject->lpVtbl->Release(browserObject);
	if(hret != S_OK)return;	
	if(webBrowser2 == NULL)return;
	hret = webBrowser2->lpVtbl->get_Document(webBrowser2,&lpDispatch);
	webBrowser2->lpVtbl->Release(webBrowser2);
	if(hret != S_OK)return;
	if(lpDispatch == NULL)return;
	hret = lpDispatch->lpVtbl->QueryInterface(lpDispatch,&IID_IHTMLDocument2,(void**)&htmlDoc2);
	lpDispatch->lpVtbl->Release(lpDispatch);
	if(hret != S_OK)return;
	if(htmlDoc2 == NULL)return;
	hret = htmlDoc2->lpVtbl->QueryInterface(htmlDoc2,&IID_IViewObject,(void**)&viewObj);
	htmlDoc2->lpVtbl->Release(htmlDoc2);
	if(hret != S_OK)return;
	if(viewObj == NULL)return;
	hret = viewObj->lpVtbl->SetAdvise(viewObj,DVASPECT_CONTENT,0,&_iOleClientSiteEx->adviseSink);
	if(hret != S_OK)
		return;
}

HRESULT STDMETHODCALLTYPE Dispatch_QueryInterface(IDispatch * This,REFIID riid,void **ppvObject)
{
	_IOleClientSiteEx* pClientSiteEx = (_IOleClientSiteEx *)((char *)This - 
		sizeof(IOleClientSite) -
		sizeof(_IOleInPlaceSiteEx) - 
		sizeof(_IDocHostUIHandlerEx) - 
		sizeof(IAdviseSink));

	if(memcmp(riid,&IID_IDispatch,sizeof(GUID)) == 0)
	{
		*ppvObject = &(pClientSiteEx->dispatchJscript);
		return S_OK;
	}
	else
	{
		*ppvObject = NULL;
	}
	return E_NOINTERFACE;
}
ULONG   STDMETHODCALLTYPE Dispatch_AddRef(IDispatch * This)
{
	return 1;
}
ULONG   STDMETHODCALLTYPE Dispatch_Release(IDispatch * This)
{
	return 1;
}
HRESULT STDMETHODCALLTYPE Dispatch_GetTypeInfoCount(IDispatch * This,UINT *pctinfo)
{
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE Dispatch_GetTypeInfo(IDispatch * This,UINT iTInfo,LCID lcid,ITypeInfo **ppTInfo)
{
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE Dispatch_GetIDsOfNames(IDispatch * This,REFIID riid,LPOLESTR *rgszNames,UINT cNames,LCID lcid,DISPID *rgDispId)
{
	HRESULT hr = NOERROR;
	UINT     i = 0;
	for (; i < cNames;i++)
	{
		DWORD dwJsId = g_pGetFromThreadFun((DWORD)rgszNames[i],WEBPAGE_GET_JSID);
		if(dwJsId != -1)
			rgDispId[i] = dwJsId;

		else if(wcscmp(WEBRENDER_REGISTER_CALL,rgszNames[i]) == 0)
			rgDispId[i] = WEBRENDER_REGISTER_CALL_ID;

		else if(wcscmp(WEBRENDER_JS_CALL ,rgszNames[i]) == 0)
			rgDispId[i] = WEBRENDER_JS_CALL_ID;

		else if(wcscmp(WEBRENDER_JS_CALL_ME,rgszNames[i]) == 0)
			rgDispId[i] = WEBRENDER_JS_CALLME_ID;

		else
		{
			hr = ResultFromScode(DISP_E_UNKNOWNNAME);
			rgDispId[i] = DISPID_UNKNOWN;
		}
	}
	return hr;
}
HRESULT STDMETHODCALLTYPE Dispatch_Invoke(IDispatch * This,DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,
	DISPPARAMS *pDispParams,VARIANT *pVarResult,EXCEPINFO *pExcepInfo,UINT *puArgErr)
{

	// 	if(DISPID_MSG_IsOurCustomBrowser == dispIdMember)
	// 	{
	// 		if(wFlags & DISPATCH_PROPERTYGET)
	// 		{
	// 			VariantInit(pVarResult);
	// 			V_VT(pVarResult)   = VT_BOOL;
	// 			V_BOOL(pVarResult) =  VARIANT_TRUE;
	// 		}
	// 		if(wFlags & DISPATCH_METHOD)
	// 		{
	// 			VariantInit(pVarResult);
	// 			V_VT(pVarResult)   = VT_BOOL;
	// 			V_BOOL(pVarResult) = VARIANT_TRUE;
	// 		}
	// 	}
	// 	else if(DISPID_MSG_SetJsId == dispIdMember)
	// 	{
	// 		if(wFlags & DISPATCH_PROPERTYGET)
	// 		{
	// 			if(pVarResult != NULL)
	// 			{
	// 				VariantInit(pVarResult);
	// 				V_VT(pVarResult) = VT_BOOL;
	// 				V_BOOL(pVarResult) = VARIANT_TRUE;
	// 			}
	// 		}
	// 		if(wFlags & DISPATCH_METHOD)
	// 		{
	// 
	// 		}
	// 	}
	wchar_t* dispIdFunName = (wchar_t*)g_pGetFromThreadFun(dispIdMember,WEBPAGE_GET_JSNAME);
	if(wcscmp(dispIdFunName,L"") != 0)
	{
		StrBuffer* pFunName,*params;

		pFunName            = (StrBuffer*)GlobalAlloc(GPTR,sizeof(StrBuffer));
		pFunName->str       = (wchar_t*)dispIdFunName;
		pFunName->pNext     = GlobalAlloc(GPTR,sizeof(StrBuffer));

		params              = (StrBuffer*)(pFunName->pNext);
		params->str         = pDispParams->rgvarg[0].bstrVal;
		params->pNext       = NULL;
		g_pSetToThreadFun(GetCurrentThreadId(),(DWORD)pFunName,WEBPAGE_JS_CALL_3D);

		if(pVarResult)
		{
			VariantInit(pVarResult);
			V_VT(pVarResult)   = VT_BOOL;
			V_BOOL(pVarResult) = VARIANT_TRUE;
		}
		GlobalFree(params);
		GlobalFree(pFunName);
	}
	else if(dispIdMember == WEBRENDER_REGISTER_CALL_ID)
	{
		StrBuffer* regVal;

		if(pDispParams->cArgs != 1)
			return S_OK;

		regVal = (StrBuffer*)GlobalAlloc(GPTR,sizeof(StrBuffer));
		regVal->str       = pDispParams->rgvarg[0].bstrVal;
		regVal->pNext     = NULL;

		g_pSetToThreadFun(GetCurrentThreadId(),(DWORD)regVal,WEBRENDER_REGISTER_CALL_ID);

		GlobalFree(regVal);
	}

	else if(dispIdMember == WEBRENDER_JS_CALLME_ID)
	{
		if(pDispParams->cArgs != 2)
			return S_OK;

		if(wcsicmp(pDispParams->rgvarg[1].bstrVal,WEBRENDER_JS_TRACE) == 0)
		{
			g_pSetToThreadFun(GetCurrentThreadId(),(DWORD)pDispParams->rgvarg[0].bstrVal,WEBRENDER_TRACE_LOG);
		}
	}

	else if(dispIdMember == WEBRENDER_JS_CALL_ID)
	{
		long numArgs        = pDispParams->cArgs;
		long argIndex       = numArgs - 1;
		wchar_t* wstrUnique = (wchar_t*)g_pGetFromThreadFun(GetCurrentThreadId(),WEBRENDER_GET_REGISTER);
		if(!wstrUnique || wcscmp(wstrUnique,L"") == 0) // 没有注册就return 
			return S_OK;

		if(numArgs == 0)
		{
			StrBuffer* pFirstbuf = (StrBuffer*)GlobalAlloc(GPTR,sizeof(StrBuffer));
			pFirstbuf->str       = wstrUnique;
			pFirstbuf->pNext     = NULL;

			g_pSetToThreadFun(GetCurrentThreadId(),(DWORD)0,WEBRENDER_JS_CALL_ID);

			GlobalFree(pFirstbuf);
		}
		else
		{
			// 构造参数(js传入的参数)
			StrBuffer* pFirstbuf = (StrBuffer*)GlobalAlloc(GPTR,sizeof(StrBuffer));
			StrBuffer* pTmpBuf   = pFirstbuf;
			pFirstbuf->str       = pDispParams->rgvarg[argIndex].bstrVal;			

			for (argIndex = argIndex - 1;argIndex >= 0;argIndex--)
			{
				pTmpBuf->pNext  = (StrBuffer*)GlobalAlloc(GPTR,sizeof(StrBuffer));
				pTmpBuf         = (StrBuffer*)(pTmpBuf->pNext);

				pTmpBuf->str    = pDispParams->rgvarg[argIndex].bstrVal;
			}

			// 构造自己的页面的唯一标示的字符串 
			pTmpBuf->pNext      = (StrBuffer*)GlobalAlloc(GPTR,sizeof(StrBuffer));
			pTmpBuf             = (StrBuffer*)(pTmpBuf->pNext);

			pTmpBuf->str        = wstrUnique;
			pTmpBuf->pNext      = NULL;

			// js调用3d
			g_pSetToThreadFun(GetCurrentThreadId(),(DWORD)pFirstbuf,WEBRENDER_JS_CALL_ID);

			// 释放内存
			pTmpBuf = pFirstbuf;
			while(pTmpBuf)
			{
				StrBuffer* pDelBuf = pTmpBuf;
				pTmpBuf = (StrBuffer*)(pTmpBuf->pNext);

				GlobalFree(pDelBuf);
			}
		}
	}
	return S_OK;
}

void UnEmbedBrowserObject(HWND hwnd)
{
	IOleObject	**browserHandle;
	IOleObject	*browserObject;
	char        *ptr;
	if ((browserHandle = (IOleObject **)GetWindowLong(hwnd, GWL_USERDATA)))
	{
		browserObject = *browserHandle;
		browserObject->lpVtbl->Close(browserObject, OLECLOSE_SAVEIFDIRTY );
		browserObject->lpVtbl->Release(browserObject);
		SetWindowLong(hwnd, GWL_USERDATA, 0);
	}
	ptr = (char*)g_pGetFromThreadFun(GetCurrentThreadId(),WEBPAGE_GET_OLEOBJBUFFER);
	if(ptr)
	{
		GlobalFree(ptr);
		ptr = NULL;
	}
}

long EmbedBrowserObject(HWND hwnd)
{
	IOleObject			*browserObject;
	IWebBrowser2		*webBrowser2;
	RECT				rect;
	char				*ptr;
	_IOleClientSiteEx	*_iOleClientSiteEx;
	if (!(ptr = (char *)GlobalAlloc(GMEM_FIXED, sizeof(_IOleClientSiteEx) + sizeof(IOleObject *))))
		return(-1);
	g_pSetToThreadFun(GetCurrentThreadId(),(DWORD)ptr,WEBPAGE_SET_OLEOBJBUFFER);
	_iOleClientSiteEx = (_IOleClientSiteEx *)(ptr + sizeof(IOleObject *));
	_iOleClientSiteEx->client.lpVtbl              = &MyIOleClientSiteTable;
	_iOleClientSiteEx->inplace.inplace.lpVtbl     = &MyIOleInPlaceSiteTable;
	_iOleClientSiteEx->inplace.frame.frame.lpVtbl = &MyIOleInPlaceFrameTable;
	_iOleClientSiteEx->inplace.frame.window       = hwnd;
	_iOleClientSiteEx->uiex.ui.lpVtbl             = &MyIDocHostUIHandlerTable;
	_iOleClientSiteEx->uiex.showUI.lpVtbl         = &MyIDocHostShowUIVbl;
	_iOleClientSiteEx->adviseSink.lpVtbl          = &MyAdviseSinkVbl;	
	_iOleClientSiteEx->dispatchJscript.lpVtbl     = &MyDispatchVbl;
	if (!OleCreate(&CLSID_WebBrowser, &IID_IOleObject, OLERENDER_DRAW, 0, (IOleClientSite *)_iOleClientSiteEx, (IStorage *)&MyIStorage, (void**)&browserObject))
	{
		*((IOleObject **)ptr) = browserObject;
		SetWindowLong(hwnd, GWL_USERDATA, (LONG)ptr);
		browserObject->lpVtbl->SetHostNames(browserObject, L"My Host Name", 0);
		GetClientRect(hwnd, &rect);
		if (!OleSetContainedObject((struct IUnknown *)browserObject, TRUE) &&
			!browserObject->lpVtbl->DoVerb(browserObject, OLEIVERB_SHOW, NULL, (IOleClientSite *)_iOleClientSiteEx, -1, hwnd, &rect) &&
			!browserObject->lpVtbl->QueryInterface(browserObject, &IID_IWebBrowser2, (void**)&webBrowser2)
			)
		{
			webBrowser2->lpVtbl->put_Silent(webBrowser2,VARIANT_TRUE);
			webBrowser2->lpVtbl->put_Left(webBrowser2, 0);
			webBrowser2->lpVtbl->put_Top(webBrowser2, 0);
			webBrowser2->lpVtbl->put_Width(webBrowser2, rect.right);
			webBrowser2->lpVtbl->put_Height(webBrowser2, rect.bottom);
			webBrowser2->lpVtbl->Release(webBrowser2);
			return(0);
		}
		UnEmbedBrowserObject(hwnd);
		return(-3);
	}
	GlobalFree(ptr);
	return(-2);
}