#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "../3dWebBrowser/IWebRender.h"


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


long webIndex;
long webIndex1;
long webIndex2;
long webIndex3;
long webIndex4;
long webIndex5;
long webIndex6;
long webIndex7;
long webIndex8;
long webIndex9;
// #include  "commctrl.h"  
// #pragma  comment(lib,"comctl32.lib") 
#define VertexFormat D3DFVF_XYZ | D3DFVF_TEX1
#define LocalPtInWeb(value,min,max,demsion)\
	(long)((value - min) * demsion / (max - min));
IWebPage*               g_pWebPage     = NULL;
IWebPage*               g_pWebPage1    = NULL;
IWebPage*               g_pWebPage2    = NULL;
IWebPage*               g_pWebPage3    = NULL;
IWebPage*               g_pWebPage4    = NULL;
IWebPage*               g_pWebPage5    = NULL;
IWebPage*               g_pWebPage6    = NULL;
IWebPage*               g_pWebPage7    = NULL;
IWebPage*               g_pWebPage8    = NULL;
IWebPage*               g_pWebPage9    = NULL;
LPDIRECT3D9             g_pD3D         = NULL; 
LPDIRECT3DDEVICE9       g_pd3dDevice   = NULL; 
IDirect3DTexture9*      g_pTexture     = NULL;
IDirect3DVertexBuffer9* g_pVertexBuf   = NULL;
IDirect3DTexture9*      g_pTexture1    = NULL;
IDirect3DVertexBuffer9* g_pVertexBuf1  = NULL;
IDirect3DTexture9*      g_pTexture2    = NULL;
IDirect3DVertexBuffer9* g_pVertexBuf2  = NULL;
IDirect3DTexture9*      g_pTexture3    = NULL;
IDirect3DVertexBuffer9* g_pVertexBuf3  = NULL;
IDirect3DTexture9*      g_pTexture4    = NULL;
IDirect3DVertexBuffer9* g_pVertexBuf4  = NULL;
IDirect3DTexture9*      g_pTexture5    = NULL;
IDirect3DVertexBuffer9* g_pVertexBuf5  = NULL;
IDirect3DTexture9*      g_pTexture6    = NULL;
IDirect3DVertexBuffer9* g_pVertexBuf6  = NULL;
IDirect3DTexture9*      g_pTexture7    = NULL;
IDirect3DVertexBuffer9* g_pVertexBuf7  = NULL;
IDirect3DTexture9*      g_pTexture8    = NULL;
IDirect3DVertexBuffer9* g_pVertexBuf8  = NULL;
IDirect3DTexture9*      g_pTexture9    = NULL;
IDirect3DVertexBuffer9* g_pVertexBuf9  = NULL;
D3DXVECTOR3             g_pt0          = D3DXVECTOR3(-6.f, 6.f, 0.f);
D3DXVECTOR3             g_pt1          = D3DXVECTOR3( 6.f, 6.f, 0.f);
D3DXVECTOR3             g_pt2          = D3DXVECTOR3(-6.f,-6.f, 0.f);
D3DXVECTOR3             g_pt3          = D3DXVECTOR3( 6.f,-6.f, 0.f);
HWND                    g_hWnd;
bool                    g_bIsCapture   = false;
D3DXMATRIX              g_matWorld;
HANDLE                  g_handel;
typedef struct tagWEBVERTEX
{
	D3DXVECTOR3 pos;
	float       tu0,tv0;
}WEBVERTEX,*LPWEBVERTEX;
#define WIDTH 600
#define HEIGHT 768
HRESULT InitD3D( HWND hWnd )
{
	if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return E_FAIL;
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	D3DDISPLAYMODE d3ddm;
	g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );
	d3dpp.Windowed               = TRUE;
	d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat       = d3ddm.Format;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice ) ) )
	{
		return E_FAIL;
	}
	HRESULT hr = g_pd3dDevice->CreateTexture(308,292,1,0,D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED,&g_pTexture,NULL);
	hr = g_pd3dDevice->CreateTexture(WIDTH,HEIGHT,1,0,D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED,&g_pTexture1,NULL);
	hr = g_pd3dDevice->CreateTexture(WIDTH,HEIGHT,1,0,D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED,&g_pTexture2,NULL);
	hr = g_pd3dDevice->CreateTexture(WIDTH,HEIGHT,1,0,D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED,&g_pTexture3,NULL);
	hr = g_pd3dDevice->CreateTexture(WIDTH,HEIGHT,1,0,D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED,&g_pTexture4,NULL);

	hr = g_pd3dDevice->CreateTexture(WIDTH,HEIGHT,1,0,D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED,&g_pTexture5,NULL);
	hr = g_pd3dDevice->CreateTexture(WIDTH,HEIGHT,1,0,D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED,&g_pTexture6,NULL);
	hr = g_pd3dDevice->CreateTexture(WIDTH,HEIGHT,1,0,D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED,&g_pTexture7,NULL);
	hr = g_pd3dDevice->CreateTexture(WIDTH,HEIGHT,1,0,D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED,&g_pTexture8,NULL);
	hr = g_pd3dDevice->CreateTexture(WIDTH,HEIGHT,1,0,D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED,&g_pTexture9,NULL);

	hr = g_pd3dDevice->CreateVertexBuffer(6 * sizeof(WEBVERTEX),0,VertexFormat,D3DPOOL_MANAGED,
		&g_pVertexBuf,NULL);
	hr = g_pd3dDevice->CreateVertexBuffer(6 * sizeof(WEBVERTEX),0,VertexFormat,D3DPOOL_MANAGED,
		&g_pVertexBuf1,NULL);
	hr = g_pd3dDevice->CreateVertexBuffer(6 * sizeof(WEBVERTEX),0,VertexFormat,D3DPOOL_MANAGED,
		&g_pVertexBuf2,NULL);
	hr = g_pd3dDevice->CreateVertexBuffer(6 * sizeof(WEBVERTEX),0,VertexFormat,D3DPOOL_MANAGED,
		&g_pVertexBuf3,NULL);
	hr = g_pd3dDevice->CreateVertexBuffer(6 * sizeof(WEBVERTEX),0,VertexFormat,D3DPOOL_MANAGED,
		&g_pVertexBuf4,NULL);

	hr = g_pd3dDevice->CreateVertexBuffer(6 * sizeof(WEBVERTEX),0,VertexFormat,D3DPOOL_MANAGED,
		&g_pVertexBuf5,NULL);
	hr = g_pd3dDevice->CreateVertexBuffer(6 * sizeof(WEBVERTEX),0,VertexFormat,D3DPOOL_MANAGED,
		&g_pVertexBuf6,NULL);
	hr = g_pd3dDevice->CreateVertexBuffer(6 * sizeof(WEBVERTEX),0,VertexFormat,D3DPOOL_MANAGED,
		&g_pVertexBuf7,NULL);
	hr = g_pd3dDevice->CreateVertexBuffer(6 * sizeof(WEBVERTEX),0,VertexFormat,D3DPOOL_MANAGED,
		&g_pVertexBuf8,NULL);
	hr = g_pd3dDevice->CreateVertexBuffer(6 * sizeof(WEBVERTEX),0,VertexFormat,D3DPOOL_MANAGED,
		&g_pVertexBuf9,NULL);
	if( FAILED( hr ))
	{
		return E_FAIL;
	}
	WEBVERTEX* pVerData = NULL;
	g_pVertexBuf->Lock(0,0,(void**)&pVerData,0);

	pVerData[0].pos   = g_pt0;
	pVerData[0].tu0   = 0.f;
	pVerData[0].tv0   = 0.f;

	pVerData[1].pos   = g_pt2;
	pVerData[1].tu0   = 0.f;
	pVerData[1].tv0   = 1.f;

	pVerData[2].pos   = g_pt1;
	pVerData[2].tu0   = 1.f;
	pVerData[2].tv0   = 0.f;

	pVerData[3].pos   = g_pt1;
	pVerData[3].tu0   = 1.f;
	pVerData[3].tv0   = 0.f;


	pVerData[4].pos   = g_pt2;
	pVerData[4].tu0   = 0.f;
	pVerData[4].tv0   = 1.f;

	pVerData[5].pos   = g_pt3;
	pVerData[5].tu0   = 1.f;
	pVerData[5].tv0   = 1.f;

	g_pVertexBuf->Unlock();

	g_pt0.x = -8.f; g_pt1.x = 8.f;  g_pt2.x = -8.f; g_pt3.x = 8.f;
	g_pt0.y = 8.f;  g_pt1.y = 8.f;  g_pt2.y = -8.f; g_pt3.y =-8.f;
	g_pt0.z = 0.8f;  g_pt1.z = 0.8f;  g_pt2.z = 0.8f;  g_pt3.z = 0.8f;
	g_pVertexBuf1->Lock(0,0,(void**)&pVerData,0);

	pVerData[0].pos   = g_pt0;
	pVerData[0].tu0   = 0.f;
	pVerData[0].tv0   = 0.f;

	pVerData[1].pos   = g_pt2;
	pVerData[1].tu0   = 0.f;
	pVerData[1].tv0   = 1.f;

	pVerData[2].pos   = g_pt1;
	pVerData[2].tu0   = 1.f;
	pVerData[2].tv0   = 0.f;

	pVerData[3].pos   = g_pt1;
	pVerData[3].tu0   = 1.f;
	pVerData[3].tv0   = 0.f;


	pVerData[4].pos   = g_pt2;
	pVerData[4].tu0   = 0.f;
	pVerData[4].tv0   = 1.f;

	pVerData[5].pos   = g_pt3;
	pVerData[5].tu0   = 1.f;
	pVerData[5].tv0   = 1.f;

	g_pVertexBuf1->Unlock();

	g_pt0.x = -8.f; g_pt1.x = 8.f;  g_pt2.x = -8.f; g_pt3.x = 8.f;
	g_pt0.y = 8.f;  g_pt1.y = 8.f;  g_pt2.y = -8.f; g_pt3.y =-8.f;
	g_pt0.z = 0.7f;  g_pt1.z = 0.7f;  g_pt2.z = 0.7f;  g_pt3.z = 0.7f;
	g_pVertexBuf2->Lock(0,0,(void**)&pVerData,0);

	pVerData[0].pos   = g_pt0;
	pVerData[0].tu0   = 0.f;
	pVerData[0].tv0   = 0.f;

	pVerData[1].pos   = g_pt2;
	pVerData[1].tu0   = 0.f;
	pVerData[1].tv0   = 1.f;

	pVerData[2].pos   = g_pt1;
	pVerData[2].tu0   = 1.f;
	pVerData[2].tv0   = 0.f;

	pVerData[3].pos   = g_pt1;
	pVerData[3].tu0   = 1.f;
	pVerData[3].tv0   = 0.f;


	pVerData[4].pos   = g_pt2;
	pVerData[4].tu0   = 0.f;
	pVerData[4].tv0   = 1.f;

	pVerData[5].pos   = g_pt3;
	pVerData[5].tu0   = 1.f;
	pVerData[5].tv0   = 1.f;

	g_pVertexBuf2->Unlock();

	g_pt0.x = -8.f; g_pt1.x = 8.f;  g_pt2.x = -8.f; g_pt3.x = 8.f;
	g_pt0.y = 8.f;  g_pt1.y = 8.f;  g_pt2.y = -8.f; g_pt3.y =-8.f;
	g_pt0.z = 0.5f;  g_pt1.z = 0.5f;  g_pt2.z = 0.5f;  g_pt3.z = 0.5f;
	g_pVertexBuf3->Lock(0,0,(void**)&pVerData,0);

	pVerData[0].pos   = g_pt0;
	pVerData[0].tu0   = 0.f;
	pVerData[0].tv0   = 0.f;

	pVerData[1].pos   = g_pt2;
	pVerData[1].tu0   = 0.f;
	pVerData[1].tv0   = 1.f;

	pVerData[2].pos   = g_pt1;
	pVerData[2].tu0   = 1.f;
	pVerData[2].tv0   = 0.f;

	pVerData[3].pos   = g_pt1;
	pVerData[3].tu0   = 1.f;
	pVerData[3].tv0   = 0.f;


	pVerData[4].pos   = g_pt2;
	pVerData[4].tu0   = 0.f;
	pVerData[4].tv0   = 1.f;

	pVerData[5].pos   = g_pt3;
	pVerData[5].tu0   = 1.f;
	pVerData[5].tv0   = 1.f;

	g_pVertexBuf3->Unlock();

	g_pt0.x = -8.f; g_pt1.x = 8.f;  g_pt2.x = -8.f; g_pt3.x = 8.f;
	g_pt0.y = 8.f;  g_pt1.y = 8.f;  g_pt2.y = -8.f; g_pt3.y =-8.f;
	g_pt0.z = 0.2f;  g_pt1.z = 0.2f;  g_pt2.z = 0.2f;  g_pt3.z = 0.2f;
	g_pVertexBuf4->Lock(0,0,(void**)&pVerData,0);

	pVerData[0].pos   = g_pt0;
	pVerData[0].tu0   = 0.f;
	pVerData[0].tv0   = 0.f;

	pVerData[1].pos   = g_pt2;
	pVerData[1].tu0   = 0.f;
	pVerData[1].tv0   = 1.f;

	pVerData[2].pos   = g_pt1;
	pVerData[2].tu0   = 1.f;
	pVerData[2].tv0   = 0.f;

	pVerData[3].pos   = g_pt1;
	pVerData[3].tu0   = 1.f;
	pVerData[3].tv0   = 0.f;


	pVerData[4].pos   = g_pt2;
	pVerData[4].tu0   = 0.f;
	pVerData[4].tv0   = 1.f;

	pVerData[5].pos   = g_pt3;
	pVerData[5].tu0   = 1.f;
	pVerData[5].tv0   = 1.f;

	g_pVertexBuf4->Unlock();

	g_pt0.x = -8.f; g_pt1.x = 8.f;  g_pt2.x = -8.f; g_pt3.x = 8.f;
	g_pt0.y = 8.f;  g_pt1.y = 8.f;  g_pt2.y = -8.f; g_pt3.y =-8.f;
	g_pt0.z = 0.0f;  g_pt1.z = 0.0f;  g_pt2.z = 0.0f;  g_pt3.z = 0.0f;
	g_pVertexBuf5->Lock(0,0,(void**)&pVerData,0);

	pVerData[0].pos   = g_pt0;
	pVerData[0].tu0   = 0.f;
	pVerData[0].tv0   = 0.f;

	pVerData[1].pos   = g_pt2;
	pVerData[1].tu0   = 0.f;
	pVerData[1].tv0   = 1.f;

	pVerData[2].pos   = g_pt1;
	pVerData[2].tu0   = 1.f;
	pVerData[2].tv0   = 0.f;

	pVerData[3].pos   = g_pt1;
	pVerData[3].tu0   = 1.f;
	pVerData[3].tv0   = 0.f;


	pVerData[4].pos   = g_pt2;
	pVerData[4].tu0   = 0.f;
	pVerData[4].tv0   = 1.f;

	pVerData[5].pos   = g_pt3;
	pVerData[5].tu0   = 1.f;
	pVerData[5].tv0   = 1.f;

	g_pVertexBuf5->Unlock();

	g_pt0.x = -8.f; g_pt1.x = 8.f;  g_pt2.x = -8.f; g_pt3.x = 8.f;
	g_pt0.y = 8.f;  g_pt1.y = 8.f;  g_pt2.y = -8.f; g_pt3.y =-8.f;
	g_pt0.z = -0.3f;  g_pt1.z = -0.3f;  g_pt2.z = -0.3f;  g_pt3.z = -0.3f;
	g_pVertexBuf6->Lock(0,0,(void**)&pVerData,0);

	pVerData[0].pos   = g_pt0;
	pVerData[0].tu0   = 0.f;
	pVerData[0].tv0   = 0.f;

	pVerData[1].pos   = g_pt2;
	pVerData[1].tu0   = 0.f;
	pVerData[1].tv0   = 1.f;

	pVerData[2].pos   = g_pt1;
	pVerData[2].tu0   = 1.f;
	pVerData[2].tv0   = 0.f;

	pVerData[3].pos   = g_pt1;
	pVerData[3].tu0   = 1.f;
	pVerData[3].tv0   = 0.f;


	pVerData[4].pos   = g_pt2;
	pVerData[4].tu0   = 0.f;
	pVerData[4].tv0   = 1.f;

	pVerData[5].pos   = g_pt3;
	pVerData[5].tu0   = 1.f;
	pVerData[5].tv0   = 1.f;

	g_pVertexBuf6->Unlock();


	g_pt0.x = -8.f; g_pt1.x = 8.f;  g_pt2.x = -8.f; g_pt3.x = 8.f;
	g_pt0.y = 8.f;  g_pt1.y = 8.f;  g_pt2.y = -8.f; g_pt3.y =-8.f;
	g_pt0.z = -0.6f;  g_pt1.z = -0.6f;  g_pt2.z = -0.6f;  g_pt3.z = -0.6f;
	g_pVertexBuf7->Lock(0,0,(void**)&pVerData,0);

	pVerData[0].pos   = g_pt0;
	pVerData[0].tu0   = 0.f;
	pVerData[0].tv0   = 0.f;

	pVerData[1].pos   = g_pt2;
	pVerData[1].tu0   = 0.f;
	pVerData[1].tv0   = 1.f;

	pVerData[2].pos   = g_pt1;
	pVerData[2].tu0   = 1.f;
	pVerData[2].tv0   = 0.f;

	pVerData[3].pos   = g_pt1;
	pVerData[3].tu0   = 1.f;
	pVerData[3].tv0   = 0.f;


	pVerData[4].pos   = g_pt2;
	pVerData[4].tu0   = 0.f;
	pVerData[4].tv0   = 1.f;

	pVerData[5].pos   = g_pt3;
	pVerData[5].tu0   = 1.f;
	pVerData[5].tv0   = 1.f;

	g_pVertexBuf7->Unlock();

	g_pt0.x = -8.f; g_pt1.x = 8.f;  g_pt2.x = -8.f; g_pt3.x = 8.f;
	g_pt0.y = 8.f;  g_pt1.y = 8.f;  g_pt2.y = -8.f; g_pt3.y =-8.f;
	g_pt0.z = -0.8f;  g_pt1.z = -0.8f;  g_pt2.z = -0.8f;  g_pt3.z = -0.8f;
	g_pVertexBuf8->Lock(0,0,(void**)&pVerData,0);

	pVerData[0].pos   = g_pt0;
	pVerData[0].tu0   = 0.f;
	pVerData[0].tv0   = 0.f;

	pVerData[1].pos   = g_pt2;
	pVerData[1].tu0   = 0.f;
	pVerData[1].tv0   = 1.f;

	pVerData[2].pos   = g_pt1;
	pVerData[2].tu0   = 1.f;
	pVerData[2].tv0   = 0.f;

	pVerData[3].pos   = g_pt1;
	pVerData[3].tu0   = 1.f;
	pVerData[3].tv0   = 0.f;


	pVerData[4].pos   = g_pt2;
	pVerData[4].tu0   = 0.f;
	pVerData[4].tv0   = 1.f;

	pVerData[5].pos   = g_pt3;
	pVerData[5].tu0   = 1.f;
	pVerData[5].tv0   = 1.f;

	g_pVertexBuf8->Unlock();

	g_pt0.x = -8.f; g_pt1.x = 8.f;  g_pt2.x = -8.f; g_pt3.x = 8.f;
	g_pt0.y = 8.f;  g_pt1.y = 8.f;  g_pt2.y = -8.f; g_pt3.y =-8.f;
	g_pt0.z = -1.1f;  g_pt1.z = -1.1f;  g_pt2.z = -1.1f;  g_pt3.z = -1.1f;
	g_pVertexBuf9->Lock(0,0,(void**)&pVerData,0);

	pVerData[0].pos   = g_pt0;
	pVerData[0].tu0   = 0.f;
	pVerData[0].tv0   = 0.f;

	pVerData[1].pos   = g_pt2;
	pVerData[1].tu0   = 0.f;
	pVerData[1].tv0   = 1.f;

	pVerData[2].pos   = g_pt1;
	pVerData[2].tu0   = 1.f;
	pVerData[2].tv0   = 0.f;

	pVerData[3].pos   = g_pt1;
	pVerData[3].tu0   = 1.f;
	pVerData[3].tv0   = 0.f;


	pVerData[4].pos   = g_pt2;
	pVerData[4].tu0   = 0.f;
	pVerData[4].tv0   = 1.f;

	pVerData[5].pos   = g_pt3;
	pVerData[5].tu0   = 1.f;
	pVerData[5].tv0   = 1.f;

	g_pVertexBuf9->Unlock();

	g_pt0.x = -6.f;g_pt1.x =6.f;   g_pt2.z = -6.f;g_pt3.x = 6.f;
	g_pt0.y = 6.f; g_pt1.y = 6.f;  g_pt2.y = -6.f;g_pt3.y =-6.f;
	g_pt0.z = 0.f; g_pt1.z = 0.f;  g_pt2.z = 0.f; g_pt3.z = 0.f;
	D3DXVECTOR3 vEyePt( -0.0f, 0.0f,-18.5f );
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

	D3DXMATRIX matMatrix;
	D3DXMatrixIdentity(&matMatrix);
	g_pd3dDevice->SetTransform(D3DTS_WORLD,&matMatrix);
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 500.0f );
	g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	// 
	g_pd3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR );
	g_pd3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR );
	D3DXMatrixIdentity(&g_matWorld);
	return S_OK;
}
BOOL PtInPlane(const D3DXVECTOR3& vecOrg,const D3DXVECTOR3& vecDir,
			   const D3DXVECTOR3& pt0,const D3DXVECTOR3& pt1,const D3DXVECTOR3& pt2,const D3DXVECTOR3& pt3,
			   D3DXVECTOR3&outPptTri)
{
	BOOL hasInterset0 = D3DXIntersectTri(&pt0,&pt1,&pt2,&vecOrg,&vecDir,NULL,NULL,NULL);
	BOOL hasInterset1 = D3DXIntersectTri(&pt1,&pt2,&pt3,&vecOrg,&vecDir,NULL,NULL,NULL);
	if(hasInterset0 == FALSE && hasInterset1 == FALSE)
		return FALSE;
	D3DXVECTOR3 endPt = vecOrg + 1000.f * vecDir;
	D3DXPLANE plane;
	D3DXPlaneFromPoints(&plane,&pt0,&pt1,&pt2);
	if(NULL == D3DXPlaneIntersectLine(&outPptTri,&plane,&vecOrg,&endPt))
		return FALSE;
	return TRUE;
}
void GetMousePtInSceen(POINT ptInClt,D3DXVECTOR3&vPickRayDir,D3DXVECTOR3&vPickRayOrig)
{
	D3DXMATRIX matProj;
	g_pd3dDevice->GetTransform(D3DTS_PROJECTION,&matProj);
	RECT rc;
	GetClientRect(g_hWnd,&rc);
	D3DXVECTOR3 v;
	v.x =  ( ( ( 2.0f * ptInClt.x ) / (rc.right - rc.left) ) - 1 ) / matProj._11;
	v.y = -( ( ( 2.0f * ptInClt.y ) / (rc.bottom - rc.top) ) - 1 ) / matProj._22;
	v.z =  1.0f;
	D3DXMATRIX matViewMatrix,matWorldMatrix;
	g_pd3dDevice->GetTransform(D3DTS_VIEW,&matViewMatrix);
	g_pd3dDevice->GetTransform(D3DTS_WORLD,&matWorldMatrix);
	D3DXMATRIX mWorldView = matWorldMatrix * matViewMatrix;
	D3DXMATRIX m;
	D3DXMatrixInverse( &m, NULL, &mWorldView );
	vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
	vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
	vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
	vPickRayOrig.x = m._41;
	vPickRayOrig.y = m._42;
	vPickRayOrig.z = m._43;
}
VOID Cleanup()
{
// 	GetWebManager()->DestroyWebPage(webIndex);
// 	GetWebManager()->DestroyWebPage(webIndex1);
// 	GetWebManager()->DestroyWebPage(webIndex2);
// 	GetWebManager()->DestroyWebPage(webIndex3);
// 	GetWebManager()->DestroyWebPage(webIndex4);

	if( g_pd3dDevice != NULL) 
		g_pd3dDevice->Release();
	if( g_pD3D != NULL)
		g_pD3D->Release();
	if( g_pTexture != NULL)
		g_pTexture->Release();
	if( g_pVertexBuf != NULL)
		g_pVertexBuf->Release();

	if( g_pTexture1 != NULL)
		g_pTexture1->Release();
	if( g_pVertexBuf1 != NULL)
		g_pVertexBuf1->Release();

	if( g_pTexture2 != NULL)
		g_pTexture2->Release();
	if( g_pVertexBuf2 != NULL)
		g_pVertexBuf2->Release();

	if( g_pTexture3 != NULL)
		g_pTexture3->Release();
	if( g_pVertexBuf3 != NULL)
		g_pVertexBuf3->Release();

	if( g_pTexture4 != NULL)
		g_pTexture4->Release();
	if( g_pVertexBuf4 != NULL)
		g_pVertexBuf4->Release();

	if( g_pTexture5 != NULL)
		g_pTexture5->Release();
	if( g_pVertexBuf5 != NULL)
		g_pVertexBuf5->Release();

	if( g_pTexture6 != NULL)
		g_pTexture6->Release();
	if( g_pVertexBuf6 != NULL)
		g_pVertexBuf6->Release();

	if( g_pTexture7 != NULL)
		g_pTexture7->Release();
	if( g_pVertexBuf7 != NULL)
		g_pVertexBuf7->Release();

	if( g_pTexture8 != NULL)
		g_pTexture8->Release();
	if( g_pVertexBuf8 != NULL)
		g_pVertexBuf8->Release();

	if( g_pTexture9 != NULL)
		g_pTexture9->Release();
	if( g_pVertexBuf9 != NULL)
		g_pVertexBuf9->Release();

}
VOID Render()
{
	if( NULL == g_pd3dDevice )
		return;
	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
	if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
	{
		IDirect3DSurface9* lpSurface = NULL;
		if(g_pWebPage)
		{			
			if(SUCCEEDED(g_pTexture->GetSurfaceLevel(0,&lpSurface)))
			{
				HDC dstHDC;
				if( SUCCEEDED(lpSurface->GetDC(&dstHDC)) )
				{
					if(g_pWebPage && g_pWebPage->IsNeedUpdate())
						g_pWebPage->DrawToTexture(dstHDC);
					lpSurface->ReleaseDC(dstHDC);
				}
				lpSurface->Release();
			}
		}
		
 		if(SUCCEEDED(g_pTexture1->GetSurfaceLevel(0,&lpSurface)))
 		{
 			HDC dstHDC;
 			if( SUCCEEDED(lpSurface->GetDC(&dstHDC)) )
 			{
 				if(g_pWebPage1 && g_pWebPage1->IsNeedUpdate())
 					g_pWebPage1->DrawToTexture(dstHDC);
 				lpSurface->ReleaseDC(dstHDC);
 			}
 			lpSurface->Release();
 		}
		if(SUCCEEDED(g_pTexture2->GetSurfaceLevel(0,&lpSurface)))
		{
			HDC dstHDC;
			if( SUCCEEDED(lpSurface->GetDC(&dstHDC)) )
			{
				if(g_pWebPage2 && g_pWebPage2->IsNeedUpdate())
					g_pWebPage2->DrawToTexture(dstHDC);
				lpSurface->ReleaseDC(dstHDC);
			}
			lpSurface->Release();
		}
		if(SUCCEEDED(g_pTexture3->GetSurfaceLevel(0,&lpSurface)))
		{
			HDC dstHDC;
			if( SUCCEEDED(lpSurface->GetDC(&dstHDC)) )
			{
				if(g_pWebPage3)
					g_pWebPage3->DrawToTexture(dstHDC);
				lpSurface->ReleaseDC(dstHDC);
			}
			lpSurface->Release();
		}
		if(SUCCEEDED(g_pTexture4->GetSurfaceLevel(0,&lpSurface)))
		{
			HDC dstHDC;
			if( SUCCEEDED(lpSurface->GetDC(&dstHDC)) )
			{
				if(g_pWebPage4)
					g_pWebPage4->DrawToTexture(dstHDC);
				lpSurface->ReleaseDC(dstHDC);
			}
			lpSurface->Release();
		}
		if(SUCCEEDED(g_pTexture5->GetSurfaceLevel(0,&lpSurface)))
		{
			HDC dstHDC;
			if( SUCCEEDED(lpSurface->GetDC(&dstHDC)) )
			{
				if(g_pWebPage5)
					g_pWebPage5->DrawToTexture(dstHDC);
				lpSurface->ReleaseDC(dstHDC);
			}
			lpSurface->Release();
		}
		if(SUCCEEDED(g_pTexture6->GetSurfaceLevel(0,&lpSurface)))
		{
			HDC dstHDC;
			if( SUCCEEDED(lpSurface->GetDC(&dstHDC)) )
			{
				if(g_pWebPage6)
					g_pWebPage6->DrawToTexture(dstHDC);
				lpSurface->ReleaseDC(dstHDC);
			}
			lpSurface->Release();
		}
		if(SUCCEEDED(g_pTexture7->GetSurfaceLevel(0,&lpSurface)))
		{
			HDC dstHDC;
			if( SUCCEEDED(lpSurface->GetDC(&dstHDC)) )
			{
				if(g_pWebPage7)
					g_pWebPage7->DrawToTexture(dstHDC);
				lpSurface->ReleaseDC(dstHDC);
			}
			lpSurface->Release();
		}
		if(SUCCEEDED(g_pTexture8->GetSurfaceLevel(0,&lpSurface)))
		{
			HDC dstHDC;
			if( SUCCEEDED(lpSurface->GetDC(&dstHDC)) )
			{
				if(g_pWebPage8)
					g_pWebPage8->DrawToTexture(dstHDC);
				lpSurface->ReleaseDC(dstHDC);
			}
			lpSurface->Release();
		}
		if(SUCCEEDED(g_pTexture9->GetSurfaceLevel(0,&lpSurface)))
		{
			HDC dstHDC;
			if( SUCCEEDED(lpSurface->GetDC(&dstHDC)) )
			{
				if(g_pWebPage9)
					g_pWebPage9->DrawToTexture(dstHDC);
				lpSurface->ReleaseDC(dstHDC);
			}
			lpSurface->Release();
		}
 		g_pd3dDevice->SetTransform(D3DTS_WORLD,&g_matWorld);
 		g_pd3dDevice->SetStreamSource(0,g_pVertexBuf,0,sizeof(WEBVERTEX));
 		g_pd3dDevice->SetFVF(VertexFormat);
 		g_pd3dDevice->SetTexture(0,g_pTexture);
 		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

 		g_pd3dDevice->SetStreamSource(0,g_pVertexBuf1,0,sizeof(WEBVERTEX));
 		g_pd3dDevice->SetFVF(VertexFormat);
 		g_pd3dDevice->SetTexture(0,g_pTexture1);
 		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

		g_pd3dDevice->SetStreamSource(0,g_pVertexBuf2,0,sizeof(WEBVERTEX));
		g_pd3dDevice->SetFVF(VertexFormat);
		g_pd3dDevice->SetTexture(0,g_pTexture2);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

		g_pd3dDevice->SetStreamSource(0,g_pVertexBuf3,0,sizeof(WEBVERTEX));
		g_pd3dDevice->SetFVF(VertexFormat);
		g_pd3dDevice->SetTexture(0,g_pTexture3);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

		g_pd3dDevice->SetStreamSource(0,g_pVertexBuf4,0,sizeof(WEBVERTEX));
		g_pd3dDevice->SetFVF(VertexFormat);
		g_pd3dDevice->SetTexture(0,g_pTexture4);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

		g_pd3dDevice->SetStreamSource(0,g_pVertexBuf5,0,sizeof(WEBVERTEX));
		g_pd3dDevice->SetFVF(VertexFormat);
		g_pd3dDevice->SetTexture(0,g_pTexture5);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

		g_pd3dDevice->SetStreamSource(0,g_pVertexBuf6,0,sizeof(WEBVERTEX));
		g_pd3dDevice->SetFVF(VertexFormat);
		g_pd3dDevice->SetTexture(0,g_pTexture6);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

		g_pd3dDevice->SetStreamSource(0,g_pVertexBuf7,0,sizeof(WEBVERTEX));
		g_pd3dDevice->SetFVF(VertexFormat);
		g_pd3dDevice->SetTexture(0,g_pTexture7);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

		g_pd3dDevice->SetStreamSource(0,g_pVertexBuf8,0,sizeof(WEBVERTEX));
		g_pd3dDevice->SetFVF(VertexFormat);
		g_pd3dDevice->SetTexture(0,g_pTexture8);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

		g_pd3dDevice->SetStreamSource(0,g_pVertexBuf9,0,sizeof(WEBVERTEX));
		g_pd3dDevice->SetFVF(VertexFormat);
		g_pd3dDevice->SetTexture(0,g_pTexture9);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

		g_pd3dDevice->EndScene();
	}
	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage( 0 );
		return 0;
	case 0x020A:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDBLCLK:
	case WM_NCHITTEST:
	case WM_NCMOUSEMOVE:
	case WM_MOUSEHOVER:
		{
			POINT pt;
			D3DXVECTOR3 vecDir,vecOrg,outPt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			if(msg == 0x020A || msg == WM_NCHITTEST || msg == WM_MOUSEHOVER)
			{
				GetCursorPos(&pt);
				ScreenToClient(hWnd,&pt);
			}
			if(msg == WM_MOUSEHOVER)
			{
				//OutputDebugStringA("WM_MOUSEHOVER.\n");
			}
			GetMousePtInSceen(pt,vecDir,vecOrg);
			BOOL bPtInPlane = PtInPlane(vecOrg,vecDir,g_pt0,g_pt1,g_pt2,g_pt3,outPt);
			if(bPtInPlane == TRUE)
			{
				long x = LocalPtInWeb(outPt.x,g_pt0.x,g_pt1.x,308);
				long y = LocalPtInWeb(-outPt.y,g_pt2.y,g_pt0.y,292);
				LPARAM tmplparam = MAKELPARAM(x,y);
				if(g_pWebPage)
				{
					if(msg == WM_MOUSEHOVER)
						g_pWebPage->SendMessage(WM_NCHITTEST,wParam,tmplparam);
					else
					g_pWebPage->SendMessage(msg,wParam,tmplparam);
				}
				if(msg == WM_LBUTTONDOWN)
				{
					OutputDebugStringA("click");
				}
				g_bIsCapture = true;
			}			
			else
				g_bIsCapture = false;

			
		}
		break;
	case WM_CHAR:
	case WM_KEYDOWN:
	case WM_KEYUP:
		{
			if(wParam == VK_F5)
			{
				IWebManager* pWebManager = GetWebManager();
				if(pWebManager)
				{
					//g_pWebPage = pWebManager->GetWebPage(webIndex);
					//VARIANT pVarRlt;
					//g_pWebPage->CallJScript(L"Elcss",L"you pressed f5.",&pVarRlt);
					//SysFreeString(pVarRlt.bstrVal);
				}
			}
			else if(wParam == VK_F6 && msg == WM_KEYDOWN)
			{
				IWebManager* pWebManager = GetWebManager();
				if(pWebManager)
				{
					g_pWebPage = pWebManager->GetWebPage(webIndex);
					g_pWebPage->Back();
				}
				
			}
			else if(wParam == VK_F7)
			{
				IWebManager* pWebManager = GetWebManager();
				if(pWebManager)
				{
					g_pWebPage = pWebManager->GetWebPage(webIndex);
					g_pWebPage->Front();
				}
				
			}
			else if(wParam == VK_F8)
			{
				IWebManager* pWebManager = GetWebManager();
				if(g_pWebPage4 == NULL)
				{
					webIndex4 = pWebManager->CreateWebPage(WIDTH,HEIGHT,"http://www.renren.com");
					g_pWebPage4 = pWebManager->GetWebPage(webIndex4);
				}
			}
			else if(wParam == VK_F4)
			{
				int randInt = rand() % 4 + 1;
				IWebManager* pWebManager = GetWebManager();
				if(randInt == 1 && g_pWebPage1)
				{
					pWebManager->DestroyWebPage(webIndex1);
				    g_pWebPage1 = NULL;
				}
				if(randInt == 2 && g_pWebPage2)
				{
					pWebManager->DestroyWebPage(webIndex2);
					g_pWebPage2 = NULL;
				}
				if(randInt == 3 && g_pWebPage3)
				{
					pWebManager->DestroyWebPage(webIndex3);
					g_pWebPage3 = NULL;
				}
				if(randInt == 4 && g_pWebPage4)
				{
					pWebManager->DestroyWebPage(webIndex4);
					g_pWebPage4 = NULL;
				}
			}
			if(g_bIsCapture == true)
			{
				if(g_pWebPage)
				   g_pWebPage->SendMessage(msg,wParam,lParam);
			}
			if(wParam == VK_LEFT)
			{
				D3DXMATRIX matTmpWorld;
				D3DXMatrixRotationY(&matTmpWorld,0.1f);
				D3DXMatrixMultiply(&g_matWorld,&g_matWorld,&matTmpWorld);
			}
			else if(wParam == VK_RIGHT)
			{
				D3DXMATRIX matTmpWorld;
				D3DXMatrixRotationY(&matTmpWorld,-0.1f);
				D3DXMatrixMultiply(&g_matWorld,&g_matWorld,&matTmpWorld);
			}
			else if(wParam == VK_UP)
			{
				D3DXMATRIX matTmpWorld;
				D3DXMatrixRotationX(&matTmpWorld,-0.1f);
				D3DXMatrixMultiply(&g_matWorld,&g_matWorld,&matTmpWorld);
			}
			else if(wParam == VK_DOWN)
			{
				D3DXMATRIX matTmpWorld;
				D3DXMatrixRotationX(&matTmpWorld,0.1f);
				D3DXMatrixMultiply(&g_matWorld,&g_matWorld,&matTmpWorld);
			}
		}
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}
std::wstring myJsCall3D(long webIndex,const std::wstring& funName,const std::wstring& paramArray)
{

	return L"";
}

void MYEVENT_NEWWINDOW(const std::wstring& url)
{
	ShellExecuteW(NULL,L"open",L"iexplore",url.c_str(),NULL,SW_SHOWNORMAL);
}

INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{


	IWebManager* pWebManager = GetWebManager();
	//pWebManager->Set_JS_CALL_D3D(myJsCall3D);
	//pWebManager->Set_EventNewWindow(MYEVENT_NEWWINDOW);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	webIndex = pWebManager->CreateWebPage(WIDTH,HEIGHT,"http://2010.sina.com.cn/");
	webIndex1= pWebManager->CreateWebPage(WIDTH,HEIGHT,"http://www.17173.com");
	webIndex2= pWebManager->CreateWebPage(WIDTH,HEIGHT,"http://www.sina.com");
	webIndex3= pWebManager->CreateWebPage(WIDTH,HEIGHT,"http://www.hao123.cn");
	webIndex4= pWebManager->CreateWebPage(WIDTH,HEIGHT,"http://www.taobao.com");
	webIndex5= pWebManager->CreateWebPage(WIDTH,HEIGHT,"http://www.hao123.cn");
	webIndex6= pWebManager->CreateWebPage(WIDTH,HEIGHT,"http://www.youku.com");
	webIndex7= pWebManager->CreateWebPage(WIDTH,HEIGHT,"http://www.2345.com");
	webIndex8= pWebManager->CreateWebPage(WIDTH,HEIGHT,"http://www.tianya.com");
	webIndex9= pWebManager->CreateWebPage(WIDTH,HEIGHT,"http://www.gameres.com");
	g_pWebPage    = pWebManager->GetWebPage(webIndex);
  	g_pWebPage->SetPartPage(5,433,308,292);
  	g_pWebPage->EnablePopNewWindow(FALSE);

	g_pWebPage1    = pWebManager->GetWebPage(webIndex1);
   	g_pWebPage1->SetPartPage(5,433,308,292);
   	g_pWebPage1->EnablePopNewWindow(FALSE);
	

	g_pWebPage2    = pWebManager->GetWebPage(webIndex2);
 	g_pWebPage2->SetPartPage(5,433,308,292);
 	g_pWebPage2->EnablePopNewWindow(FALSE);

	g_pWebPage3    = pWebManager->GetWebPage(webIndex3);
 	g_pWebPage3->SetPartPage(5,433,308,292);
 	g_pWebPage3->EnablePopNewWindow(FALSE);


	g_pWebPage4    = pWebManager->GetWebPage(webIndex4);
 	g_pWebPage4->SetPartPage(5,433,308,292);
 	g_pWebPage4->EnablePopNewWindow(FALSE);

	g_pWebPage5    = pWebManager->GetWebPage(webIndex5);
	g_pWebPage5->SetPartPage(5,433,308,292);
	g_pWebPage5->EnablePopNewWindow(FALSE);

	g_pWebPage6    = pWebManager->GetWebPage(webIndex6);
	g_pWebPage6->SetPartPage(5,433,308,292);
	g_pWebPage6->EnablePopNewWindow(FALSE);

	g_pWebPage7    = pWebManager->GetWebPage(webIndex7);
	g_pWebPage7->SetPartPage(5,433,308,292);
	g_pWebPage7->EnablePopNewWindow(FALSE);

	g_pWebPage8    = pWebManager->GetWebPage(webIndex8);
	g_pWebPage8->SetPartPage(5,433,308,292);
	g_pWebPage8->EnablePopNewWindow(FALSE);

	g_pWebPage9    = pWebManager->GetWebPage(webIndex9);
	g_pWebPage9->SetPartPage(5,433,308,292);
	g_pWebPage9->EnablePopNewWindow(FALSE);

	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, 
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"D3D Tutorial", NULL };
	RegisterClassEx( &wc );
	HWND hWnd = CreateWindow( L"D3D Tutorial", L"3dä¯ÀÀÆ÷ÑÝÊ¾",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 100, 100, WIDTH, HEIGHT,
		NULL, NULL, wc.hInstance, NULL );
	g_hWnd = hWnd;
	if( SUCCEEDED( InitD3D( hWnd ) ) )
	{ 
		ShowWindow( hWnd, SW_SHOWDEFAULT );
		UpdateWindow( hWnd );
		SendMessage(hWnd,WM_MOUSEHOVER,0,0);
		MSG msg = {0};
		while(true)
		{
			if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				if(msg.message == WM_QUIT)
					break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				Render();
				Sleep(500);
			}
		}
	}
	pWebManager->DestroyWebPage(webIndex);
  	pWebManager->DestroyWebPage(webIndex1);
  	pWebManager->DestroyWebPage(webIndex2);
  	pWebManager->DestroyWebPage(webIndex3);
  	pWebManager->DestroyWebPage(webIndex4);
	pWebManager->DestroyWebPage(webIndex5);
	pWebManager->DestroyWebPage(webIndex6);
	pWebManager->DestroyWebPage(webIndex7);
	pWebManager->DestroyWebPage(webIndex8);
	pWebManager->DestroyWebPage(webIndex9);

	
	//char* ptr = (char*)GlobalAlloc(GPTR,200);
	UnregisterClass( L"D3D Tutorial", wc.hInstance );
	_CrtDumpMemoryLeaks();
	
	return 0;
}