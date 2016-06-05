#include "C3DWebPage.h"
#include <string.h>
#pragma warning(disable : 4996)

C3DWebPage::C3DWebPage(void)
{
	ZeroMemory(&webItem,sizeof(IEWebItem));
	m_rect = NULL;
	webItem.needWebDynmaicUpdate = WEBPAGE_WEBDYNMAIC_RESETUPDATE;
	webItem.isNeedNewWindow      = 1;
}

C3DWebPage::~C3DWebPage(void)
{
	ZeroMemory(&webItem,sizeof(IEWebItem));
	if(m_rect)
	{
		delete m_rect;
		m_rect = NULL;
	}
}
void C3DWebPage::Browser(const char* url )
{
	wchar_t		*buffer;
	DWORD		size;
	size = MultiByteToWideChar(CP_ACP, 0, url, -1, 0, 0);
	if (!(buffer = (wchar_t *)GlobalAlloc(GMEM_FIXED, sizeof(wchar_t) * size)))
		return;
	MultiByteToWideChar(CP_ACP, 0, url, -1, buffer, size);
	if(!IsWindow(webItem.mainWnd))return;
	::SendMessage(webItem.mainWnd,WM_BROWSER_URL,0,(LPARAM)buffer);
	GlobalFree(buffer);
}

// 页面是否要更新
bool C3DWebPage::IsNeedUpdate()
{
	if(webItem.needWebDynmaicUpdate == WEBPAGE_WEBDYNMAIC_RESETUPDATE)
		webItem.needWebDynmaicUpdate = IsWebDynamicDirty(webItem.hIEService);

	if(webItem.needWebDynmaicUpdate != WEBPAGE_WEBDYNAMIC_NOUPDATE)
		return true;

	// 整个页面绘制
	if(!m_rect)
	{
		return webItem.isDirty == 1;
	}
	else if(webItem.isDirty == 1)
	{
		if(webItem.dirtyRect.left < m_rect->right  && m_rect->left < webItem.dirtyRect.right  &&
		   webItem.dirtyRect.top  < m_rect->bottom && m_rect->top  < webItem.dirtyRect.bottom)
			return true;		
	}

	return false;
}
void C3DWebPage::DrawToTexture( HDC hdc )
{
	if(!IsWindow(webItem.mainWnd))
		return;

	PostMessage(webItem.mainWnd,WM_WEBPAGE_DRAW,0,0);

	if(webItem.scrollx || webItem.scrolly)
		PostMessage(webItem.mainWnd,WM_PAGE_SCROLL,0,0);

	if(!IsWindow(webItem.hIEService))
		return;

	if(webItem.isDirty == 1)	
	{
		if(!m_rect)
			PaintWeb(webItem.hIEService,hdc);
		else
			PaintDirtyWeb(webItem.hIEService,hdc,*m_rect);
		webItem.isDirty = 0;
 		ZeroMemory(&webItem.dirtyRect,4 * sizeof(long));
	}

	// 如果没有判断就先判断下
	if(webItem.needWebDynmaicUpdate == WEBPAGE_WEBDYNMAIC_RESETUPDATE)
		webItem.needWebDynmaicUpdate = IsWebDynamicDirty(webItem.hIEService);

	if(webItem.needWebDynmaicUpdate != WEBPAGE_WEBDYNAMIC_NOUPDATE)
		PaintWebDynamic(hdc,webItem.hIEService,webItem.needWebDynmaicUpdate);
	// 绘制完以后重置，等待下一帧的判断和绘制
	webItem.needWebDynmaicUpdate = WEBPAGE_WEBDYNMAIC_RESETUPDATE;	
}

void C3DWebPage::SendMessage( UINT nMsg, WPARAM wparam, LPARAM lparam )
{
	if(!IsWindow(webItem.hIEService))return;

	if(!m_rect)
		InjectEvent(webItem.mainWnd,webItem.hIEService,nMsg,wparam,lparam);

	if(nMsg != 0x020A && m_rect)
	{
		wparam = (nMsg == WM_MOUSEMOVE ? 0 : wparam);

		long x = LOWORD(lparam);
		long y = HIWORD(lparam);

		if(x >= m_rect->left && x <= m_rect->right && y >= m_rect->top && y <= m_rect->bottom)
			InjectEvent(webItem.mainWnd,webItem.hIEService,nMsg,wparam,lparam);
	}
}
void C3DWebPage::Back(void)
{
	if(!IsWindow(webItem.mainWnd))return;
	::SendMessage(webItem.mainWnd,WM_BROWSER_BACK,0,0);
}
void C3DWebPage::Front(void)
{
	if(!IsWindow(webItem.mainWnd))return;
	::SendMessage(webItem.mainWnd,WM_BROWSER_FRONT,0,0);
}


void C3DWebPage::EnablePopNewWindow(BOOL BEnable)
{
	webItem.isNeedNewWindow = BEnable;
}

void C3DWebPage::SetPartPage(long scrollx,long scrolly,long width,long height,long startx,long starty)
{
	webItem.scrollx = scrollx;
	webItem.scrolly = scrolly;
 	if(m_rect)
 		delete m_rect;
 
 	m_rect = new RECT;
 	m_rect->left   = startx;
 	m_rect->top    = starty;
 	m_rect->right  = width;
 	m_rect->bottom = height;
}

VARIANT* C3DWebPage::AllocFunctionRet(const VARIANT* pVarReslt)
{
	VARIANT* pFunRet = (VARIANT*)GlobalAlloc(GPTR,sizeof(VARIANT));
	VariantInit(pFunRet);
	return pFunRet;
}
StrBuffer* C3DWebPage::AllocFunAndParams(const std::wstring&funname,const std::vector<std::wstring>& paramArray)
{
	StrBuffer* pFirstArg = new StrBuffer;
	long bufsize         = (funname.size() + 1) * sizeof(wchar_t);
	pFirstArg->str       = (wchar_t*)GlobalAlloc(GPTR,bufsize);
	memcpy(pFirstArg->str,funname.c_str(),bufsize);
	pFirstArg->pNext     = NULL;

	long paramSize = (long)paramArray.size();
	if(paramSize > 0)
	{
		pFirstArg->pNext = new StrBuffer;
		StrBuffer* pTmpBuf = (StrBuffer*)(pFirstArg->pNext);
		long bufsize = (paramArray[0].size() + 1) * sizeof(wchar_t);
		pTmpBuf->str = (wchar_t*)GlobalAlloc(GPTR,bufsize);
		memcpy(pTmpBuf->str,paramArray[0].c_str(),bufsize);
		for (long index = 1;index < paramSize;index++)
		{
			pTmpBuf->pNext = new StrBuffer;

			pTmpBuf        = (StrBuffer*)(pTmpBuf->pNext);

			bufsize        = (paramArray[index].size() + 1) * sizeof(wchar_t);
			pTmpBuf->str   = (wchar_t*)GlobalAlloc(GPTR,bufsize);
			memcpy(pTmpBuf->str,paramArray[index].c_str(),bufsize);
		}
		pTmpBuf->pNext = NULL;
	}
	return pFirstArg;
}

void       C3DWebPage::FreeBufs(StrBuffer*pBuf)
{
	StrBuffer* pTmpBuf = pBuf;
	while(pTmpBuf)
	{
		StrBuffer* pDelBuf = pTmpBuf;
		pTmpBuf = (StrBuffer*)(pTmpBuf->pNext);

		GlobalFree(pDelBuf->str);
		delete pDelBuf;
	}
	pBuf = NULL;
}

void C3DWebPage::CallJScript(const std::wstring& strFunc,VARIANT* pVarResult)
{
	if(IsWindow(webItem.mainWnd) ==  FALSE)
		return;

	VARIANT* kernelReslt = NULL;
	if(pVarResult)
		kernelReslt = AllocFunctionRet(pVarResult);

	std::vector<std::wstring> paramArray;
	StrBuffer*pFunName = AllocFunAndParams(strFunc,paramArray);

	::SendMessage(webItem.mainWnd,WM_CALL_JS_ARG_0,(WPARAM)kernelReslt,(LPARAM)pFunName);

	if(pVarResult)
	{
		memcpy(pVarResult,kernelReslt,sizeof(VARIANT));
		GlobalFree(kernelReslt);
	}

	FreeBufs(pFunName);	
}
void C3DWebPage::CallJScript(const std::wstring& strFunc,const std::wstring&strArg1,VARIANT*pVarResult)
{
	if(IsWindow(webItem.mainWnd) == FALSE)
		return;

	VARIANT* kernelReslt = NULL;
	if(pVarResult)
		kernelReslt = AllocFunctionRet(pVarResult);

	std::vector<std::wstring> paramArray;
	paramArray.push_back(strArg1);
	StrBuffer*pFunName = AllocFunAndParams(strFunc,paramArray);

	::SendMessage(webItem.mainWnd,WM_CALL_JS_ARG_1,(WPARAM)kernelReslt,(LPARAM)pFunName);

	wchar_t wstrMsg[2048] = {'\0'};
	char strMsg[2048]  = {'\0'};
	wsprintf(wstrMsg,L"[3d call js] --> funName = %s,param = %s",strFunc.c_str(),strArg1.c_str());
	WideCharToMultiByte(CP_ACP, 0, wstrMsg, -1, strMsg, 2048,NULL,NULL);
	((C3DWebManager*)GetWebManager())->OutputLineString(strMsg,2048);


	if(pVarResult)
	{
		memcpy(pVarResult,kernelReslt,sizeof(VARIANT));
		GlobalFree(kernelReslt);
	}

	FreeBufs(pFunName);	
}
void C3DWebPage::CallJScript(const std::wstring& strFunc,const std::wstring&strArg1,const std::wstring&strArg2,VARIANT*pVarResult)
{
	if(IsWindow(webItem.mainWnd) == FALSE)
		return;

	VARIANT* kernelReslt = NULL;
	if(pVarResult)
		kernelReslt = AllocFunctionRet(pVarResult);

	std::vector<std::wstring> paramArray;
	paramArray.push_back(strArg1);
	paramArray.push_back(strArg2);
	StrBuffer*pFunName = AllocFunAndParams(strFunc,paramArray);

	::SendMessage(webItem.mainWnd,WM_CALL_JS_ARG_2,(WPARAM)kernelReslt,(LPARAM)pFunName);

	if(pVarResult)
	{
		memcpy(pVarResult,kernelReslt,sizeof(VARIANT));
		GlobalFree(kernelReslt);
	}

	FreeBufs(pFunName);	
}
void C3DWebPage::CallJScript(const std::wstring& strFunc,const std::wstring&strArg1,const std::wstring&strArg2,const std::wstring&strArg3,VARIANT*pVarResult)
{
	if(IsWindow(webItem.mainWnd) == FALSE)
		return;

	VARIANT* kernelReslt = NULL;
	if(pVarResult)
		kernelReslt = AllocFunctionRet(pVarResult);

	std::vector<std::wstring> paramArray;
	paramArray.push_back(strArg1);
	paramArray.push_back(strArg2);
	paramArray.push_back(strArg3);
	StrBuffer*pFunName = AllocFunAndParams(strFunc,paramArray);

	::SendMessage(webItem.mainWnd,WM_CALL_JS_ARG_3,(WPARAM)kernelReslt,(LPARAM)pFunName);

	if(pVarResult)
	{
		memcpy(pVarResult,kernelReslt,sizeof(VARIANT));
		GlobalFree(kernelReslt);
	}

	FreeBufs(pFunName);	
}
void C3DWebPage::CallJScript(const std::wstring& strFunc,const std::vector<std::wstring>& paramArray,VARIANT*pVarResult){
	if(IsWindow(webItem.mainWnd) == FALSE)
		return;

	VARIANT* kernelReslt = NULL;
	if(pVarResult)
		kernelReslt = AllocFunctionRet(pVarResult);

	StrBuffer*pFunName = AllocFunAndParams(strFunc,paramArray);

	::SendMessage(webItem.mainWnd,WM_CALL_JS_ARG_X,(WPARAM)kernelReslt,(LPARAM)pFunName);

	if(pVarResult)
	{
		memcpy(pVarResult,kernelReslt,sizeof(VARIANT));
		GlobalFree(kernelReslt);
	}

	FreeBufs(pFunName);	
}

void C3DWebPage::SetWebItem_ChildWnd(HWND childWnd)
{
	bool  isExistChildWnd = false;

	std::vector<HWND>& vecChildWnds = webItem.vecChildWnds;
	for (size_t index = 0;index < vecChildWnds.size();index++)
	{
		if(vecChildWnds[index] == childWnd)
			isExistChildWnd = true;
	}
	if(!isExistChildWnd)
	{
		webItem.vecChildWnds.push_back(childWnd);
	}
}

C3DWebManager::C3DWebManager()
{
	char fileName[MAX_PATH] = {'\0'},jsFuns[MAX_PATH] = {'\0'},logFile[MAX_PATH] = {'\0'},
		traceLog[MAX_PATH] = {'\0'};
	HMODULE hWebModule = GetModuleHandleA("WebRender.dll");
	GetModuleFileNameA(hWebModule,fileName,MAX_PATH);
	char* strFind = const_cast<char*>(strrchr(fileName,'\\'));
	*strFind = '\0';

	sprintf_s(logFile,MAX_PATH,"%s\\webrender.log",fileName);
	sprintf_s(traceLog,MAX_PATH,"%s\\webrender_trace.log",fileName);

	g_LogFile = fopen(logFile,"wb");
	g_TraceLog = fopen(traceLog,"wb");

	sprintf_s(jsFuns,MAX_PATH,"%s\\jsFuns.ini",fileName);

	FILE* file = fopen(jsFuns,"r");

	if(file)
	{
		long index = 0;
		char line[MAX_PATH]     = {'\0'};
		wchar_t wLine[MAX_PATH] = {L'\0'};
		while(fgets(line,MAX_PATH,file))
		{
			MultiByteToWideChar(CP_ACP, 0, line, -1, wLine, MAX_PATH);

			std::wstring wstrLine = wLine;
			long size = wstrLine.size();
			if(size > 0)
			{
				if(wstrLine[size - 1] == 10)
					wstrLine.erase(wstrLine.begin() + size - 1);

				m_mapIdToFunname.insert(std::make_pair(index,wstrLine));
			}

			index++;
		}
		fclose(file);
	}

	g_jsCall3DFun    = NULL;
	g_jsCall3DFunEx  = NULL;
	g_eventNewWindow = NULL;
}

long C3DWebManager::CreateWebPage(int w, int h, const char* url )// 主线程调用
{
	DWORD dwThreadId = -1;
	long left = 2048;
	std::map<DWORD,C3DWebPage*>::iterator itor = m_mapThreadToWebItem.begin();
	for(;itor != m_mapThreadToWebItem.end();itor++)
	{
		RECT tmpRect;
		HWND tmpWnd = itor->second->GetWebItem()->mainWnd;
		GetWindowRect(tmpWnd,&tmpRect);
		left += (tmpRect.right - tmpRect.left);
	}
	if(url && url != "")
	{
		wchar_t		*buffer;
		DWORD		size;
		size = MultiByteToWideChar(CP_ACP, 0, url, -1, 0, 0);
		if (!(buffer = (wchar_t *)GlobalAlloc(GMEM_FIXED, sizeof(wchar_t) * size)))
			return -1;
		MultiByteToWideChar(CP_ACP, 0, url, -1, buffer, size);
		dwThreadId = CreateWebTexture(left,0,w,h,buffer);
		GlobalFree(buffer);
	}
	else
		dwThreadId =  CreateWebTexture(left,0,w,h,NULL);
	if(dwThreadId != -1)
	{
		long sz = (long)m_mapThreadToWebItem.size();
		HANDLE* hEvent = new HANDLE[sz];
		for (long lindex = 0;lindex < sz;lindex++)
		{
			hEvent[lindex] = CreateEvent(NULL,TRUE,FALSE,NULL);
		}

		BeginPause(hEvent,sz,dwThreadId);
		C3DWebPage* pWebPage = InsertWebPage(dwThreadId);
		EndPause(hEvent,sz,dwThreadId);
		delete []hEvent;
		hEvent = NULL;

 		HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, dwThreadId );
		if(hThread != INVALID_HANDLE_VALUE && pWebPage)
		{
			PIEWebItem pWebItem = pWebPage->GetWebItem();
			ResumeThread(hThread);
			CloseHandle(hThread);
 			while(pWebItem->mainWnd != NULL)
 				Sleep(500);
		}
	}
	
	return dwThreadId;
}

std::map<DWORD,C3DWebPage*>::iterator C3DWebManager::DestroyWebPage(DWORD dwThreadId)
{
	std::map<DWORD,C3DWebPage*>::iterator itor    = m_mapThreadToWebItem.find(dwThreadId);
	std::map<DWORD,C3DWebPage*>::iterator retItor = itor;

	if(itor == m_mapThreadToWebItem.end())
		return retItor;
	DestroyWeb(dwThreadId);// 杀掉子线程
	long sz = (long)m_mapThreadToWebItem.size();
	HANDLE* hEvent = new HANDLE[sz];
	for (long lindex = 0;lindex < sz;lindex++)
	{
		hEvent[lindex] = CreateEvent(NULL,TRUE,FALSE,NULL);
	}

	BeginPause(hEvent,sz,dwThreadId);
	std::map<DWORD,C3DWebPage*>::iterator tmpitor = itor; 
	delete tmpitor->second;
	tmpitor->second = NULL;
	retItor = m_mapThreadToWebItem.erase(itor);
	EndPause(hEvent,sz,dwThreadId);

	delete []hEvent;
	hEvent = NULL;

	long left = 2048;
	RECT rc;
	itor = m_mapThreadToWebItem.begin();
	for (;itor != m_mapThreadToWebItem.end();itor++)
	{
		PIEWebItem pIeWebItem = itor->second->GetWebItem();
		GetWindowRect(pIeWebItem->mainWnd,&rc);
		MoveWindow(pIeWebItem->mainWnd,left,0,rc.right - rc.left,rc.bottom - rc.top,TRUE);
		left += (rc.right - rc.left);
	}

	return retItor;
}

void C3DWebManager::DestroyWebPage(long index)// 主线程调用
{
	std::map<DWORD,C3DWebPage*>::iterator itor = m_mapThreadToWebItem.find(index);
	if(itor == m_mapThreadToWebItem.end())
		return;
	DestroyWeb(index);// 杀掉子线程
	long sz = (long)m_mapThreadToWebItem.size();
	HANDLE* hEvent = new HANDLE[sz];
	for (long lindex = 0;lindex < sz;lindex++)
	{
		hEvent[lindex] = CreateEvent(NULL,TRUE,FALSE,NULL);
	}

	BeginPause(hEvent,sz,index);
	std::map<DWORD,C3DWebPage*>::iterator tmpitor = itor; 
	delete tmpitor->second;
	tmpitor->second = NULL;
	m_mapThreadToWebItem.erase(itor);
	EndPause(hEvent,sz,index);

	delete []hEvent;
	hEvent = NULL;

	long left = 2048;
	RECT rc;
	itor = m_mapThreadToWebItem.begin();
	for (;itor != m_mapThreadToWebItem.end();itor++)
	{
		PIEWebItem pIeWebItem = itor->second->GetWebItem();
		GetWindowRect(pIeWebItem->mainWnd,&rc);
		MoveWindow(pIeWebItem->mainWnd,left,0,rc.right - rc.left,rc.bottom - rc.top,TRUE);
		left += (rc.right - rc.left);
	}
	
}
C3DWebManager::~C3DWebManager()
{
	if(g_LogFile)
	{
		fclose(g_LogFile);
		g_LogFile = NULL;
	}

	if(g_TraceLog)
	{
		fclose(g_TraceLog);
		g_TraceLog = NULL;
	}
}

IWebPage*  C3DWebManager::GetWebPage(const std::wstring& wstrWeb)
{
	std::map<DWORD,C3DWebPage*>::iterator itor = m_mapThreadToWebItem.begin();
	for(; itor != m_mapThreadToWebItem.end();itor++)
	{
		if(wstrWeb == itor->second->GetWebUniqueStr())
			return itor->second;
	}

	return NULL;
}

IWebPage* C3DWebManager::GetWebPage(long index) // 主线程调用
{
	std::map<DWORD,C3DWebPage*>::iterator itor = m_mapThreadToWebItem.find(index);
	if(itor == m_mapThreadToWebItem.end())
		return NULL;
	return itor->second;
}

void C3DWebManager::Set_JS_CALL_D3D(PJS_CALL_3D js_call3dFun)
{
	g_jsCall3DFun = js_call3dFun;
}

void C3DWebManager::Set_JS_CALL_D3DEX(PJS_CALL_3DEX js_call3dFunEx)
{
	g_jsCall3DFunEx = js_call3dFunEx;
}

void C3DWebManager::Set_EventNewWindow(PEVENT_NEWWINDOW eventNewWindow)
{
	g_eventNewWindow = eventNewWindow;
}

void C3DWebManager::D3D_CALL_JSEX(const std::wstring& toWeb/*to web*/,const std::wstring& param1,const std::wstring& param2/*fromWeb*/,const std::wstring& param3)
{
	C3DWebPage* pWebPage = (C3DWebPage*)GetWebPage(toWeb);
	if(!pWebPage)
		return;

	pWebPage->CallJScript(WEBRENDER_JS_CALL_BACK,param1,param2,param3);

}

void C3DWebManager::D3D_CALL_JS(long webIndex,const std::wstring& funName,const std::wstring& params)
{
	wchar_t wstrMsg[2048] = {'\0'};
	char strMsg[2048]  = {'\0'};
	
	wsprintf(wstrMsg,L"[3d call js] --> funName = %s,param = %s",funName.c_str(),params.c_str());
	WideCharToMultiByte(CP_ACP, 0, wstrMsg, -1, strMsg, 2048,NULL,NULL);
	((C3DWebManager*)GetWebManager())->OutputLineString(strMsg,2048);
#ifdef _DEBUG
	MessageBoxW(NULL,L"3d call js",NULL,MB_OK);
#endif

	C3DWebPage* pWebPage = (C3DWebPage*)GetWebPage(webIndex);
	
	if(params.size() == 0)
		pWebPage->CallJScript(funName);
	else
	    pWebPage->CallJScript(funName,params);
}

long         C3DWebManager::FindFunId(std::wstring wstrFun)
{
	long wstrsz = wstrFun.size();

	if(wstrsz == 0)return -1;
	else
	{
		if(wstrFun[wstrsz - 1] == L'\0')
			wstrFun.erase(wstrFun.begin() + wstrsz - 1);
	}
	std::map<long,std::wstring>::iterator itor = m_mapIdToFunname.begin();
	for (;itor != m_mapIdToFunname.end();itor++)
	{
		if(itor->second == wstrFun)
		{
			return itor->first;
		}
	}

	return -1;
}

const wchar_t* C3DWebManager::FindFunName(long id)
{
	std::map<long,std::wstring>::iterator itor =  m_mapIdToFunname.find(id);
	if(itor == m_mapIdToFunname.end())
		return L"";
	else
		return itor->second.c_str();
}

C3DWebPage* C3DWebManager::FindWebPage(DWORD dwThreadId)// 子线程调用
{
	return (C3DWebPage*)GetWebPage(dwThreadId);
}
C3DWebPage* C3DWebManager::InsertWebPage(DWORD dwThreadId)// 子线程调用
{
	if(m_mapThreadToWebItem.find(dwThreadId) != m_mapThreadToWebItem.end())
		return NULL;
	
	C3DWebPage* pWebPage = new C3DWebPage;
	m_mapThreadToWebItem.insert(std::make_pair(dwThreadId,pWebPage));
	return pWebPage;
}
void       C3DWebManager::BeginPause(HANDLE* hEvent,long count,DWORD dwThreadId)
{	
	HANDLE hasFinish = CreateEvent(NULL,TRUE,FALSE,NULL);
	std::map<DWORD,C3DWebPage*>::iterator itor = m_mapThreadToWebItem.begin();
	
	for (long lindex = 0;
		itor != m_mapThreadToWebItem.end() && lindex < count;itor++,lindex++)
	{
		if(itor->first == dwThreadId)
			continue;
		HWND mainWnd = itor->second->GetWebItem()->mainWnd;
		if(IsWindow(mainWnd) == FALSE)continue;

		ResetEvent(hasFinish);
		ResetEvent(hEvent[lindex]);

		BOOL bRet = SendNotifyMessage(mainWnd,WM_PAUSE_THREAD,
			(WPARAM)hasFinish,(LPARAM)hEvent[lindex]);	
		if(bRet == 1)
		    WaitForSingleObject(hasFinish,INFINITE);
	}

	CloseHandle(hasFinish);
}
void       C3DWebManager::EndPause(HANDLE* hEvent,long count,DWORD dwThreadId)
{
	for (long lindex = 0;lindex < count;lindex++)
	{
		SetEvent(hEvent[lindex]);
		CloseHandle(hEvent[lindex]);
	}
}

void        C3DWebManager::OutputTraceLog(wchar_t* wstr)
{
	if(g_TraceLog)
	{
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);

		wchar_t strCurTime[500] = {L'\0'};
		wsprintf(strCurTime,L"[%4d-%02d-%02d %02d:%02d:%02d]",sysTime.wYear,
			sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,
			sysTime.wSecond);

		fwrite(strCurTime,sizeof(wchar_t),wcslen(strCurTime),g_TraceLog);
		fwrite(wstr,sizeof(wchar_t),wcslen(wstr),g_TraceLog);
		fwrite(L"\r\n",sizeof(wchar_t),2,g_TraceLog);

		fflush(g_TraceLog);
	}
}

void        C3DWebManager::OutputLineString(char* str,size_t size)
{
	if(g_LogFile)
	{
		fwrite(str,1,size,g_LogFile);
		fwrite("\r\n",1,2,g_LogFile);
		fflush(g_LogFile);
	}
}

void        C3DWebManager::ClearAll()
{
	std::map<DWORD,C3DWebPage*>::iterator itor = m_mapThreadToWebItem.begin();
	for (;itor != m_mapThreadToWebItem.end();)
	{
		itor = DestroyWebPage(itor->first);

		if(itor == m_mapThreadToWebItem.end())
			break;
	}
}
C3DWebManager* g_pWebPage = NULL;

WEB_BROWSER_API IWebManager* GetWebManager()
{
	if(g_pWebPage == NULL)
		g_pWebPage = new C3DWebManager;
	
	return g_pWebPage;
}

