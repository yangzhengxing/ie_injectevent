#include "C3DWebPage.h"
#include "detours.h"
#include "detoured.h"

extern PCreateInstance          g_pOldCreateInstaceFlash;
extern PCreateInstance          g_pOldCreateInstanceWMP;
extern PCoGetClassObjectFromURL g_pOldCoGetClassObjectFromURL;
extern "C"
inline void WINAPI g_SetDirty(DWORD dwThreadId,long dirty)
{
	C3DWebManager* pWebManger = (C3DWebManager*)GetWebManager();
	C3DWebPage*    pWebPage   = pWebManger->FindWebPage(dwThreadId);
	if(pWebPage)
		pWebPage->SetWebItem_Dirty(dirty);
}
extern "C"
inline DWORD WINAPI g_GetValFromThreadId(DWORD dwThreadId,long wndIndex)
{
	C3DWebManager* pWebManger = (C3DWebManager*)GetWebManager();
	C3DWebPage*    pWebPage   = pWebManger->FindWebPage(dwThreadId);
	if(wndIndex == WEBPAGE_GET_JSNAME)
	{
		return 
			(DWORD)(pWebManger->FindFunName(dwThreadId));
	}
	else if(wndIndex == WEBPAGE_GET_JSID)
	{
		wchar_t* funName = (wchar_t*)dwThreadId;
		std::wstring wFunName = funName;
		return 
			(DWORD)(pWebManger->FindFunId(wFunName)); 
	}
	else if(wndIndex == WEBPAGE_IS_VALID_INDEX)
	{
		if(pWebPage == NULL)
			return 0;
		else
			return 1;
	}
	else if(pWebPage)
	{
		PIEWebItem pWebItem = pWebPage->GetWebItem();
		if(wndIndex == WEBPAGE_GET_MAINWINDOW)
			return (DWORD)pWebItem->mainWnd;
		else if(wndIndex == WEBPAGE_GET_IESERVICEWINDOW)
			return (DWORD)pWebItem->hIEService;
		else if(wndIndex == WEBPAGE_GET_COOKIEADVISE)
			return pWebItem->dwAdviseCookie;
		else if(wndIndex == WEBPAGE_GET_WINDOWADVISE)
			return pWebItem->dwAviseWindow;
		else if(wndIndex == WEBPAGE_GET_NEED_POPUPIE)
			return pWebItem->needPopupIe;
		else if(wndIndex == WEBPAGE_GET_COOKIEBUFFER)
			return (DWORD)pWebItem->dwAdviseCookieBuf;
		else if(wndIndex == WEBPAGE_GET_WINDOWBUFFER)
			return (DWORD)pWebItem->dwAdviseWindowBuf;
		else if(wndIndex == WEBPAGE_GET_OLEOBJBUFFER)
			return (DWORD)pWebItem->dwOleObjBuf;
		else if(wndIndex == WEBPAGE_GET_DYNAMIC_UPDATE)
			return (DWORD)pWebItem->needWebDynmaicUpdate;
		else if(wndIndex == WEBPAGE_GET_IESERVICE)
			return (DWORD)pWebItem->hIEService;
		else if(wndIndex == WEBPAGE_GET_SCROLLX)
			return (DWORD)pWebItem->scrollx;
		else if(wndIndex == WEBPAGE_GET_SCROLLY)
			return (DWORD)pWebItem->scrolly;
		else if(wndIndex == WEBRENDER_GET_REGISTER)
		{
			const std::wstring& wstrUnique = pWebPage->GetWebUniqueStr();
			return (DWORD)(wstrUnique.c_str());
		}
		else if(wndIndex == WEBPAGE_GET_NEWWINDOW)
		{
			return (DWORD)pWebItem->isNeedNewWindow;
		}
		else if(wndIndex == WEBPAGE_IS_PARTWINDOW)
		{
			return pWebPage->IsPartWindow();
		}
		else if(wndIndex == WEBPAGE_IS_DIRTY)
		{
			return pWebItem->isDirty;
		}
	}
	return WEBPAGE_INVALIDE_VALUE;
}
extern "C"
inline void  WINAPI g_SetDwordToThreadId(DWORD dwThreadId,DWORD dwValue,long adviseIndex)
{
	C3DWebManager* pWebManger = (C3DWebManager*)GetWebManager();
	C3DWebPage*    pWebPage   = pWebManger->FindWebPage(dwThreadId);
	if(pWebPage)
	{
		if(adviseIndex == WEBPAGE_SET_MAINWINDOW)
			pWebPage->SetWebItem_MainWnd((HWND)dwValue);
		else if(adviseIndex == WEBPAGE_SET_IESERVICEWINDOW)
			pWebPage->SetWebItem_IEService((HWND)dwValue);
		else if(adviseIndex == WEBPAGE_SET_COOKIEADVISE)
			pWebPage->SetWebItem_AdviseCookie(dwValue);
		else if(adviseIndex == WEBPAGE_SET_WINDOWADVISE)
			pWebPage->SetWebItem_AdviseWindow(dwValue);
		else if(adviseIndex == WEBPAGE_SET_CHILDWINDOW)
			pWebPage->SetWebItem_ChildWnd((HWND)dwValue);
		else if(adviseIndex == WEBPAGE_SET_NEED_POPUPIE)
			pWebPage->SetWebItem_NeedPopIE(dwValue);
		else if(adviseIndex == WEBPAGE_SET_COOKIEBUFFER)
			pWebPage->SetWebItem_AdviseCookieBuf((LPVOID)dwValue);
		else if(adviseIndex == WEBPAGE_SET_WINDOWBUFFER)
			pWebPage->SetWebItem_AdviseWindowBuf((LPVOID)dwValue);
		else if(adviseIndex == WEBPAGE_SET_OLEOBJBUFFER)
			pWebPage->SetWebItem_OleObjBuf((LPVOID)dwValue);
		else if(adviseIndex == WEBPAGE_SET_DYNAMIC_UPDATE)
			pWebPage->SetWebItem_WebDynamicUpdate(dwValue);
		else if(adviseIndex == WEBPAGE_SET_DIRTY_RECT)
			pWebPage->SetWebItem_DirtyRect((RECT*)dwValue);
		else if(adviseIndex == WEBPAGE_SET_IESERVICE)
			pWebPage->SetWebItem_IEService((HWND)dwValue);
		else if(adviseIndex == WEBPAGE_RESET_DIRTYRECT)
			pWebPage->ResetDirtyRect();
		else if(adviseIndex == WEBPAGE_SET_SCROLLX)
			pWebPage->SetWebItem_ScrollX(dwValue);
		else if(adviseIndex == WEBPAGE_SET_SCROLLY)
			pWebPage->SetWebItem_ScrollY(dwValue);
		else if(adviseIndex == WEBPAGE_CALL_NEWWINDOW)
		{
			PEVENT_NEWWINDOW newWindowCall = pWebManger->GetNewWindowCall();

			if(newWindowCall && dwValue)
			{
				StrBuffer* pUrlBuf = (StrBuffer*)dwValue;
				newWindowCall(pUrlBuf->str);
			}
		}
		else if(adviseIndex == WEBPAGE_JS_CALL_3D)
		{
			StrBuffer*   pFunName   = (StrBuffer*)dwValue;
			StrBuffer*   pStrArray  = (StrBuffer*)(pFunName->pNext);
			std::wstring paramArray = L"";

			if(pStrArray)
				paramArray = pStrArray->str;

			PJS_CALL_3D jsCall3D = pWebManger->Get_JS_CALL_D3DFun();
			
			if(jsCall3D)
			{
				wchar_t wstrMsg[2048] = {'\0'};
				char strMsg[2048]  = {'\0'};
				wsprintf(wstrMsg,L"[js call 3d] --> funName = %s,param = %s",pFunName->str,paramArray.c_str());

				WideCharToMultiByte(CP_ACP, 0, wstrMsg, -1, strMsg, 2048,NULL,NULL);
				pWebManger->OutputLineString(strMsg,2048);

				jsCall3D(dwThreadId,pFunName->str,paramArray);
			}
		}

		else if(adviseIndex == WEBRENDER_REGISTER_CALL_ID)
		{
			StrBuffer* webUnique = (StrBuffer*)dwValue;

			pWebPage->SetWebUniqueStr(webUnique->str);
		}

		else if(adviseIndex == WEBRENDER_TRACE_LOG)
		{
			wchar_t* wstr = (wchar_t*)dwValue;

			pWebManger->OutputTraceLog(wstr);
		}

		else if(adviseIndex == WEBRENDER_JS_CALL_ID)
		{
			PJS_CALL_3DEX jsCall3DEx = pWebManger->Get_JS_CALL_D3DFunEx();

			if(jsCall3DEx)
			{
				std::vector<std::wstring> paramArray;
				
				StrBuffer* pFirstBuf = (StrBuffer*)dwValue;			
				while(pFirstBuf)
				{
					paramArray.push_back(pFirstBuf->str);

					pFirstBuf = (StrBuffer*)(pFirstBuf->pNext);
				}

				jsCall3DEx(paramArray);
			}	
		}
	}
}
extern C3DWebManager* g_pWebPage;

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
{
	if (dwReason == DLL_PROCESS_ATTACH) 
	{
		s_hDll = hinst;
		DisableThreadLibraryCalls(hinst);
		g_pOldCoGetClassObjectFromURL = (PCoGetClassObjectFromURL)DetourFindFunction("UrlMon.dll", "CoGetClassObjectFromURL");
		g_pOldRedrawWindow            = (PRedrawWindow)DetourFindFunction("User32.dll","RedrawWindow");
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach((PVOID*)&g_pOldCoGetClassObjectFromURL, MyCoGetClassObjectFromURL);	
		DetourAttach((PVOID*)&g_pOldRedrawWindow,MyRedrawWindow);
		DetourTransactionCommit();
		long ret = InitGlobaFuns(g_SetDirty,g_GetValFromThreadId,g_SetDwordToThreadId);
		if(ret == 0)
			return FALSE;
	}
	if ( dwReason == DLL_PROCESS_DETACH )
	{
		C3DWebManager* webManager = (C3DWebManager*)GetWebManager(); 
		webManager->ClearAll();

		if(g_pWebPage)
		{
			delete g_pWebPage;
			g_pWebPage = NULL;
		}
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach((PVOID*)&g_pOldCreateInstaceFlash,     MyCreateInstanceFlash);
		DetourDetach((PVOID*)&g_pOldCreateInstanceWMP,      MyCreateInstanceWMP);
		DetourDetach((PVOID*)&g_pOldCoGetClassObjectFromURL,MyCoGetClassObjectFromURL);
		DetourDetach((PVOID*)&g_pOldRedrawWindow,           MyRedrawWindow);
		DetourTransactionCommit();

	}
	return TRUE;
}
