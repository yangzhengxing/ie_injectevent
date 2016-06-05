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
	//����url�������
	virtual void Browser(const char* url) = 0;

	//��ҳ�Ƿ��б仯
	virtual bool IsNeedUpdate() = 0;

	//��ҳҳ���Ƶ�ָ��HDC��
	virtual void DrawToTexture( HDC hdc ) = 0;

	//������Ϣ��webpage
	virtual void SendMessage( UINT nMsg, WPARAM wparam, LPARAM lparam ) = 0;

	//ҳ���һЩ����
	//����
	virtual void Back(void) = 0;

	//ǰ��
	virtual void Front(void) = 0;

	// �����Ƿ��ڲ������µ�ҳ��
	virtual void EnablePopNewWindow(BOOL BEnable) = 0;

	//���þֲ���ҳ
	virtual void SetPartPage(long scrollx,long scrolly,long width,long height,long startx = 0,long starty = 0) = 0;
};
// js����3d
typedef std::wstring (*PJS_CALL_3D)(long webIndex,const std::wstring& funName,const std::wstring& paramArray);

typedef std::wstring (*PJS_CALL_3DEX)(const std::vector<std::wstring>& paramArray);

// ���µ�ҳ��
typedef void (*PEVENT_NEWWINDOW)(const std::wstring& url);

class IWebManager
{
public:
	//��һ��webҳ��,������������
	virtual long CreateWebPage( int w, int h, const char* url = NULL ) = 0;

	//����ָ��ҳ��
	virtual void DestroyWebPage(long index) = 0;

	//����������ȡ��webҳ��
	virtual IWebPage* GetWebPage(long index) = 0;

	// 3d�ͻ��˵���js
	virtual void D3D_CALL_JS(long webIndex,const std::wstring& funName,const std::wstring& params) = 0;

	// 3d�ͻ��˵���js
	virtual void D3D_CALL_JSEX(const std::wstring& toWeb/*to web*/,const std::wstring& param1,const std::wstring& param2/*fromWeb*/,const std::wstring& param3) = 0;

	// ����js����3d�Ļص�
	virtual void Set_JS_CALL_D3D(PJS_CALL_3D js_call3dFun) = 0;

	// �����µ�ҳ��Ļص�
	virtual void Set_EventNewWindow(PEVENT_NEWWINDOW eventNewWindow) = 0;

	// ����js����3d�Ļص�
	virtual void Set_JS_CALL_D3DEX(PJS_CALL_3DEX js_call3dFun) = 0;
};

WEB_BROWSER_API extern IWebManager* GetWebManager();
