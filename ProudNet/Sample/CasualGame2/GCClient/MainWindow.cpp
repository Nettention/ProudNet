#include "StdAfx.h"
#include "MainWindow.h"
#include "resource.h"
#include "battleform.h"
#include "ConnectForm.h"
#include "ErrorForm.h"
#include "world.h"
#include "LobbyChannelForm.h"
#include "NewHeroForm.h"
#include "NewRoomForm.h"
#include "SelectChannelForm.h"
#include "SelectHeroForm.h"
#include "BattleForm.h"
#include "BattleWaitingForm.h"
#include "ErrorForm.h"
#include "JoinForm.h"
#include "BattleClient.h"




//#define DEBUG_VS   // Uncomment this line to debug vertex shaders
//#define DEBUG_PS   // Uncomment this line to debug pixel shaders

/*
32bit, 64bit 에서 링크해야 하는 lib 가 다르기 때문에.
32bit 는 _WIN32 만 정의.
64bit 에서는 _WIN32 _WIN64 모두 정의 됨.
따라서 _WIN32 가 조건문에 먼저 나올경우 32bit 64bit 에 대한 구분이 되지 않음.
*/
#ifdef _WIN64
#pragma comment(lib,"../../../../directx/lib/x64/d3d9.lib")
#pragma comment(lib,"../../../../directx/lib/x64/d3dx9.lib")
#pragma comment(lib,"../../../../directx/lib/x64/d3dx10.lib")
#pragma comment(lib,"../../../../directx/lib/x64/dxerr.lib")
#pragma comment(lib,"../../../../directx/lib/x64/dxguid.lib")
#elif _WIN32
#pragma comment(lib,"../../../../directx/lib/x86/d3d9.lib")
#pragma comment(lib,"../../../../directx/lib/x86/d3dx9.lib")
#pragma comment(lib,"../../../../directx/lib/x86/d3dx10.lib")
#pragma comment(lib,"../../../../directx/lib/x86/dxerr.lib")
#pragma comment(lib,"../../../../directx/lib/x86/dxguid.lib")
#endif

#pragma comment(lib,"imm32.lib")
#pragma comment(lib,"version.lib")

#if defined(DEBUG) || defined(_DEBUG)
#ifndef V
#define V(x)           { hr = x; if( FAILED(hr) ) { DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#else
#ifndef V
#define V(x)           { hr = x; }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return hr; } }
#endif
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
ID3DXFont*              g_pFont = NULL;         // Font for drawing text
ID3DXSprite*            g_pTextSprite = NULL;   // Sprite for batching draw text calls
ID3DXEffect*            g_pEffect = NULL;       // D3DX effect interface

// 메쉬 객체들
CDXUTMesh               g_terrainMesh,g_HeroMesh,g_synchAreaMesh,g_BulletMesh,g_cylinderMesh,g_BombMesh,
						g_GoodItemMesh,g_BadItemMesh,g_ClearItemMesh,g_RapidItemMesh,
						g_ActiveItemMesh,g_GameAreaMesh;   

D3DXMATRIXA16           g_mView;
CModelViewerCamera      g_Camera;               // A model viewing camera
CDXUTDialogResourceManager g_DialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg         g_SettingsDlg;          // Device settings dialog
CDXUTDialog             g_HUD;                  // dialog for standard controls


//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
bool    CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
bool    CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext );
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void    CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
void    CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
void    CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void    CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void    CALLBACK OnLostDevice( void* pUserContext );
void    CALLBACK OnDestroyDevice( void* pUserContext );

void    InitApp();
HRESULT LoadMesh( IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh );
void    RenderText();



//--------------------------------------------------------------------------------------
// Initialize the app
//--------------------------------------------------------------------------------------
void InitApp()
{
	// Initialize dialogs
	g_SettingsDlg.Init( &g_DialogResourceManager );
	g_HUD.Init( &g_DialogResourceManager );
	CWorld::GetSharedPtr()->m_connectForm.Init( &g_DialogResourceManager );
	CWorld::GetSharedPtr()->m_selectHeroForm.Init(&g_DialogResourceManager);
	CWorld::GetSharedPtr()->m_newHeroForm.Init(&g_DialogResourceManager);
	CWorld::GetSharedPtr()->m_selectChannelForm.Init(&g_DialogResourceManager);
	CWorld::GetSharedPtr()->m_lobbyChannelForm.Init(&g_DialogResourceManager);
	CWorld::GetSharedPtr()->m_newRoomForm.Init(&g_DialogResourceManager);
	CWorld::GetSharedPtr()->m_battleWaitingForm.Init(&g_DialogResourceManager);
	CWorld::GetSharedPtr()->m_battleForm.Init(&g_DialogResourceManager);
	CWorld::GetSharedPtr()->m_errorForm.Init(&g_DialogResourceManager);
	CWorld::GetSharedPtr()->m_joinForm.Init(&g_DialogResourceManager);

	int iY = 10;
	g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22 );
	g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22 );
	g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 35, iY += 24, 125, 22, VK_F2 );

	// 클라이언트에서 사용될 UI들을 초기화합니다.
	// 자세한 것은 DirectX SDK CustomUI sample을 참고할 것.
	// Initialize UI that using at client.
	// Please refer DirectX SDK CustomUI sample for more detail.
	InitConnectForm();
	InitLobbyChannelForm();
	InitNewHeroForm();
	InitNewRoomForm();
	InitSelectChannelForm();
	InitSelectHeroForm();
	InitBattleWaitingForm();
	InitBattleForm();
	InitErrorForm();
	InitJoinForm();
}


//--------------------------------------------------------------------------------------
// Called during device initialization, this code checks the device for some
// minimum set of capabilities, and rejects those that don't pass by returning false.
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
                                  D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
	// Skip backbuffer formats that don't support alpha blending
	IDirect3D9* pD3D = DXUTGetD3DObject();
	if ( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
	                                      AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
	                                      D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
		return false;

	// Must support pixel shader 1.1
	if ( pCaps->PixelShaderVersion < D3DPS_VERSION( 1, 1 ) )
		return false;

	return true;
}


//--------------------------------------------------------------------------------------
// This callback function is called immediately before a device is created to allow the
// application to modify the device settings. The supplied pDeviceSettings parameter
// contains the settings that the framework has selected for the new device, and the
// application can make any desired changes directly to this structure.  Note however that
// DXUT will not correct invalid device settings so care must be taken
// to return valid device settings, otherwise IDirect3D9::CreateDevice() will fail.
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext )
{
	// If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW
	// then switch to SWVP.
	if ( (pCaps->DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
	        pCaps->VertexShaderVersion < D3DVS_VERSION(1, 1) )
	{
		pDeviceSettings->BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	// Debugging vertex shaders requires either REF or software vertex processing
	// and debugging pixel shaders requires REF.
#ifdef DEBUG_VS
	if ( pDeviceSettings->DeviceType != D3DDEVTYPE_REF )
	{
		pDeviceSettings->BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
		pDeviceSettings->BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
		pDeviceSettings->BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
#endif
#ifdef DEBUG_PS
	pDeviceSettings->DeviceType = D3DDEVTYPE_REF;
#endif
	// For the first device created if its a REF device, optionally display a warning dialog box
	static bool s_bFirstTime = true;
	if ( s_bFirstTime )
	{
		s_bFirstTime = false;
		if ( pDeviceSettings->DeviceType == D3DDEVTYPE_REF )
			DXUTDisplaySwitchingToREFWarning();
	}

	return true;
}

//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been
// created, which will happen during application initialization and windowed/full screen
// toggles. This is the best location to create D3DPOOL_MANAGED resources since these
// resources need to be reloaded whenever the device is destroyed. Resources created
// here should be released in the OnDestroyDevice callback.
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	HRESULT hr;


	V_RETURN( g_DialogResourceManager.OnCreateDevice( pd3dDevice ) );
	V_RETURN( g_SettingsDlg.OnCreateDevice( pd3dDevice ) );
	// Initialize the font
	V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
	                          OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
	                          L"Arial", &g_pFont ) );

	// Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the
	// shader debugger. Debugging vertex shaders requires either REF or software vertex
	// processing, and debugging pixel shaders requires REF.  The
	// D3DXSHADER_FORCE_*_SOFTWARE_NOOPT flag improves the debug experience in the
	// shader debugger.  It enables source level debugging, prevents instruction
	// reordering, prevents dead code elimination, and forces the compiler to compile
	// against the next higher available software target, which ensures that the
	// unoptimized shaders do not exceed the shader model limitations.  Setting these
	// flags will cause slower rendering since the shaders will be unoptimized and
	// forced into software.  See the DirectX documentation for more information about
	// using the shader debugger.
	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows
	// the shaders to be optimized and to run exactly the way they will run in
	// the release configuration of this program.
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif
	WCHAR str[10000];
	V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"GCClient.fx" ) );
	V_RETURN( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags,
	                                    NULL, &g_pEffect, NULL ) );

	g_terrainMesh.Create( pd3dDevice, L"terrain\\terrain.x" );
	g_HeroMesh.Create(pd3dDevice,L"gunner\\gunner.x");
	g_synchAreaMesh.Create(pd3dDevice, L"circle\\circle.x");
	g_BulletMesh.Create(pd3dDevice, L"circle\\3Dbullet1.X");
	g_cylinderMesh.Create(pd3dDevice, L"cylinder\\cylinder.X");
	g_BombMesh.Create(pd3dDevice,L"circle\\star.x");
	g_GoodItemMesh.Create(pd3dDevice,L"Item\\Good.x");
	g_BadItemMesh.Create(pd3dDevice,L"Item\\Bad.x");
	g_ClearItemMesh.Create(pd3dDevice,L"Item\\Clear.x");
	g_RapidItemMesh.Create(pd3dDevice,L"Item\\Rapid.x");
	g_ActiveItemMesh.Create(pd3dDevice,L"Item\\Active.x");
	g_GameAreaMesh.Create(pd3dDevice,L"Item\\GameArea.x");
	pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	pd3dDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	return S_OK;
}


//--------------------------------------------------------------------------------------
// This function loads the mesh and ensures the mesh has normals; it also optimizes the
// mesh for the graphics card's vertex cache, which improves performance by organizing
// the internal triangle list for less cache misses.
//--------------------------------------------------------------------------------------
HRESULT LoadMesh( IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh )
{
	ID3DXMesh* pMesh = NULL;
	WCHAR str[MAX_PATH];
	HRESULT hr;

	// Load the mesh with D3DX and get back a ID3DXMesh*.  For this
	// sample we'll ignore the X file's embedded materials since we know
	// exactly the model we're loading.  See the mesh samples such as
	// "OptimizedMesh" for a more generic mesh loading example.
	V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, strFileName ) );

	V_RETURN( D3DXLoadMeshFromX(str, D3DXMESH_MANAGED, pd3dDevice, NULL, NULL, NULL, NULL, &pMesh) );

	DWORD *rgdwAdjacency = NULL;

	// Make sure there are normals which are required for lighting
	if ( !(pMesh->GetFVF() & D3DFVF_NORMAL) )
	{
		ID3DXMesh* pTempMesh;
		V( pMesh->CloneMeshFVF( pMesh->GetOptions(),
		                        pMesh->GetFVF() | D3DFVF_NORMAL,
		                        pd3dDevice, &pTempMesh ) );
		V( D3DXComputeNormals( pTempMesh, NULL ) );

		SAFE_RELEASE( pMesh );
		pMesh = pTempMesh;
	}

	// Optimize the mesh for this graphics card's vertex cache
	// so when rendering the mesh's triangle list the vertices will
	// cache hit more often so it won't have to re-execute the vertex shader
	// on those vertices so it will improve perf.
	rgdwAdjacency = new DWORD[pMesh->GetNumFaces() * 3];
	if ( rgdwAdjacency == NULL )
		return E_OUTOFMEMORY;
	V( pMesh->GenerateAdjacency(1e-6f, rgdwAdjacency) );
	V( pMesh->OptimizeInplace(D3DXMESHOPT_VERTEXCACHE, rgdwAdjacency, NULL, NULL, NULL) );
	delete []rgdwAdjacency;

	*ppMesh = pMesh;

	return S_OK;
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been
// reset, which will happen after a lost device scenario. This is the best location to
// create D3DPOOL_DEFAULT resources since these resources need to be reloaded whenever
// the device is lost. Resources created here should be released in the OnLostDevice
// callback.
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice,
                                const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	HRESULT hr;

	V_RETURN( g_DialogResourceManager.OnResetDevice() );
	V_RETURN( g_SettingsDlg.OnResetDevice() );

	if ( g_pFont )
		V_RETURN( g_pFont->OnResetDevice() );
	if ( g_pEffect )
		V_RETURN( g_pEffect->OnResetDevice() );

	// Create a sprite to help batch calls when drawing many lines of text
	V_RETURN( D3DXCreateSprite( pd3dDevice, &g_pTextSprite ) );

	// Setup the camera's projection parameters
	float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
	g_Camera.SetProjParams( D3DX_PI /4, fAspectRatio, 0.5f, 1000.0f );
	g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
	

	g_HUD.SetLocation( pBackBufferSurfaceDesc->Width - 170, 0 );
	g_HUD.SetSize( 170, 170 );

	ResetBattleForm(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	ResetBattleWaitingForm(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	ResetConnectForm(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	ResetJoinForm(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	ResetErrorForm(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	ResetLobbyChannelForm(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	ResetNewHeroForm(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	ResetNewRoomForm(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	ResetSelectChannelForm(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	ResetSelectHeroForm(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);

	g_terrainMesh.RestoreDeviceObjects( pd3dDevice );
	g_HeroMesh.RestoreDeviceObjects(pd3dDevice);
	g_synchAreaMesh.RestoreDeviceObjects(pd3dDevice);
	g_BulletMesh.RestoreDeviceObjects(pd3dDevice);
	g_cylinderMesh.RestoreDeviceObjects(pd3dDevice);
	g_BombMesh.RestoreDeviceObjects(pd3dDevice);
	g_GoodItemMesh.RestoreDeviceObjects(pd3dDevice);
	g_BadItemMesh.RestoreDeviceObjects(pd3dDevice);
	g_ClearItemMesh.RestoreDeviceObjects(pd3dDevice);
	g_RapidItemMesh.RestoreDeviceObjects(pd3dDevice);
	g_ActiveItemMesh.RestoreDeviceObjects(pd3dDevice);
	g_GameAreaMesh.RestoreDeviceObjects(pd3dDevice);
	return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
#ifdef _DEBUG
	// 만약 lost focus 상태이면 CPU 점유를 줄이기 위해 프레임수를 50fps 정도로 낮춘다.
	// 만약 lost focus 상태이면 CPU 점유를 줄이기 위해 프레임수를 낮춘다.
	if (!DXUTIsActive())
		Sleep(20); 
#endif

	CWorld::GetSharedPtr()->FrameMove(fElapsedTime);
	
	CWorld::GetSharedPtr()->SetCamera();	
}

void RenderMesh( D3DXMATRIX mWorld, CDXUTMesh &mesh , D3DXCOLOR diffuse )
{
	D3DXMATRIX mProj = *g_Camera.GetProjMatrix();
	D3DXMATRIX mView = g_mView;

	D3DXMATRIX mWorldViewProjection = mWorld * mView * mProj;

	HRESULT hr;
	// Update the effect's variables.  Instead of using strings, it would
	// be more efficient to cache a handle to the parameter by calling
	// ID3DXEffect::GetParameterByName
	V( g_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) );
	V( g_pEffect->SetMatrix( "g_mWorld", &mWorld ) );
	V( g_pEffect->SetVector("g_vDiffuse", (D3DXVECTOR4*)&diffuse) );
	//V( g_pEffect->SetFloat( "g_fTime", (float)fTime ) );

	UINT cPasses;
	g_pEffect->SetTechnique( "RenderScene" );
	g_pEffect->Begin( &cPasses, 0 );
	ID3DXMesh *pMesh = mesh.GetMesh();
	for ( UINT p = 0; p < cPasses; ++p )
	{
		g_pEffect->BeginPass( p );
		for ( UINT m = 0; m < mesh.m_dwNumMaterials; ++m )
		{
			g_pEffect->SetTexture( "g_txScene", mesh.m_pTextures[m] );
			g_pEffect->CommitChanges();
			pMesh->DrawSubset( m );
		}
		g_pEffect->EndPass();
	}
	g_pEffect->End();
}

D3DXMATRIX IdentityMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

// Mark text at specific 3D area.
// 지정한 3D 공간에 텍스트를 표시합니다.
void RenderSignPost( D3DXVECTOR3 pos3D, LPCWSTR text )
{

	D3DXMATRIX mProj = *g_Camera.GetProjMatrix();
	D3DXMATRIX mView = g_mView;

	D3DXVECTOR3 pos2D;
	D3DVIEWPORT9 viewport;
	DXUTGetD3DDevice()->GetViewport(&viewport);
	D3DXVec3Project(&pos2D, &pos3D, &viewport, &mProj, &mView, &IdentityMatrix);

	CDXUTTextHelper txtHelper( g_pFont, g_pTextSprite, 15 );

	// Output statistics
	txtHelper.Begin();
	txtHelper.SetInsertionPos( (int)pos2D.x, (int)pos2D.y );
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
	txtHelper.DrawTextLine( text);
	txtHelper.End();

}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	if ( g_SettingsDlg.IsActive() )
	{
		g_SettingsDlg.OnRender( fElapsedTime );
		return;
	}

	HRESULT hr;

	// Clear the render target and the zbuffer
	V( pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0) );

	// Render the scene
	if ( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{


		CWorld::GetSharedPtr()->RenderForm(fElapsedTime);

		RenderText();
		V( g_HUD.OnRender( fElapsedTime ) );

		CWorld::GetSharedPtr()->RenderForm( fElapsedTime );

		
		if (CWorld::GetSharedPtr()->m_FormState == CWorld::BattleForm)
		{
			CWorld::GetSharedPtr()->Render();

		}

		V( pd3dDevice->EndScene() );
	}
}


//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for
// efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText()
{
	// The helper object simply helps keep track of text position, and color
	// and then it calls pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_NOCLIP, m_clr );
	// If NULL is passed in as the sprite object, then it will work however the
	// pFont->DrawText() will not be batched together.  Batching calls will improves performance.
	CDXUTTextHelper txtHelper( g_pFont, g_pTextSprite, 15 );

	// Output statistics
	txtHelper.Begin();
	txtHelper.SetInsertionPos( 5, 5 );
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
	txtHelper.DrawTextLine( DXUTGetFrameStats() );
	txtHelper.DrawTextLine( DXUTGetDeviceStats() );

	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );

	// Draw help
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	txtHelper.DrawTextLine( L"Press ESC to quit" );
	txtHelper.End();
}


//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
	// Always allow dialog resource manager calls to handle global messages
	// so GUI state is updated correctly
	*pbNoFurtherProcessing = CDXUTIMEEditBox::StaticMsgProc( uMsg, wParam, lParam );
	if ( *pbNoFurtherProcessing )
		return 0;
	*pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
	if ( *pbNoFurtherProcessing )
		return 0;

	if ( g_SettingsDlg.IsActive() )
	{
		g_SettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
		return 0;
	}

	// Give the dialogs a chance to handle the message first
	*pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
	if ( *pbNoFurtherProcessing )
		return 0;
	*pbNoFurtherProcessing = CWorld::GetSharedPtr()->FormMsgProc( hWnd, uMsg, wParam, lParam );
	if ( *pbNoFurtherProcessing )
		return 0;

	// Pass all remaining windows messages to camera so it can respond to user input
	g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );

	return 0;
}


//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has
// entered a lost state and before IDirect3DDevice9::Reset is called. Resources created
// in the OnResetDevice callback should be released here, which generally includes all
// D3DPOOL_DEFAULT resources. See the "Lost Devices" section of the documentation for
// information about lost devices.
//--------------------------------------------------------------------------------------
void CALLBACK OnLostDevice( void* pUserContext )
{
	g_DialogResourceManager.OnLostDevice();
	g_SettingsDlg.OnLostDevice();
	g_terrainMesh.InvalidateDeviceObjects();

	if ( g_pFont )
		g_pFont->OnLostDevice();
	if ( g_pEffect )
		g_pEffect->OnLostDevice();

	g_terrainMesh.InvalidateDeviceObjects();
	g_HeroMesh.InvalidateDeviceObjects();
	g_synchAreaMesh.InvalidateDeviceObjects();
	g_BulletMesh.InvalidateDeviceObjects();
	g_cylinderMesh.InvalidateDeviceObjects();
	g_BombMesh.InvalidateDeviceObjects();
	g_GoodItemMesh.InvalidateDeviceObjects();
	g_BadItemMesh.InvalidateDeviceObjects();
	g_ClearItemMesh.InvalidateDeviceObjects();
	g_RapidItemMesh.InvalidateDeviceObjects();
	g_ActiveItemMesh.InvalidateDeviceObjects();
	g_GameAreaMesh.InvalidateDeviceObjects();
	SAFE_RELEASE(g_pTextSprite);
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice( void* pUserContext )
{
	g_DialogResourceManager.OnDestroyDevice();
	g_SettingsDlg.OnDestroyDevice();
	g_terrainMesh.Destroy();
	g_HeroMesh.Destroy();
	g_synchAreaMesh.Destroy();
	g_BulletMesh.Destroy();
	g_cylinderMesh.Destroy();
	g_BombMesh.Destroy();
	g_GoodItemMesh.Destroy();
	g_BadItemMesh.Destroy();
	g_ClearItemMesh.Destroy();
	g_RapidItemMesh.Destroy();
	g_ActiveItemMesh.Destroy();
	g_GameAreaMesh.Destroy();
	
	SAFE_RELEASE(g_pEffect);
	SAFE_RELEASE(g_pFont);

}

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
INT MainForm()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Set the callback functions. These functions allow DXUT to notify
	// the application about device changes, user input, and windows messages.  The
	// callbacks are optional so you need only set callbacks for events you're interested
	// in. However, if you don't handle the device reset/lost callbacks then the sample
	// framework won't be able to reset your device since the application must first
	// release all device resources before resetting.  Likewise, if you don't handle the
	// device created/destroyed callbacks then DXUT won't be able to
	// recreate your device resources.
	DXUTSetCallbackDeviceCreated( OnCreateDevice );
	DXUTSetCallbackDeviceReset( OnResetDevice );
	DXUTSetCallbackDeviceLost( OnLostDevice );
	DXUTSetCallbackDeviceDestroyed( OnDestroyDevice );
	DXUTSetCallbackMsgProc( MsgProc );
	DXUTSetCallbackKeyboard( KeyboardProc );
	DXUTSetCallbackFrameRender( OnFrameRender );
	DXUTSetCallbackFrameMove( OnFrameMove );

	// Show the cursor and clip it when in full screen
	DXUTSetCursorSettings( true, true );

	InitApp();
	
	// Initialize DXUT and create the desired Win32 window and Direct3D
	// device for the application. Calling each of these functions is optional, but they
	// allow you to set several options which control the behavior of the framework.
	DXUTInit( true, true, true ); // Parse the command line, handle the default hotkeys, and show msgboxes
	DXUTCreateWindow( L"ProudNet Simple Casual Game Sample2" );
	DXUTCreateDevice( D3DADAPTER_DEFAULT, true, 640, 480, IsDeviceAcceptable, ModifyDeviceSettings );

	// Pass control to DXUT for handling the message pump and
	// dispatching render calls. DXUT will call your FrameMove
	// and FrameRender callback when there is idle time between handling window messages.
	DXUTMainLoop();

	// Perform any application-level cleanup here. Direct3D device resources are released within the
	// appropriate callback functions and therefore don't require any cleanup code here.

	return DXUTGetExitCode();
}
