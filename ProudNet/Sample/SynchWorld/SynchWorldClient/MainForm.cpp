#include "stdafx.h"
#include "resource.h"
#include "mainform.h"
#include "LocalHero.h"
#include "ConnectForm.h"
#include "ErrorForm.h"
#include "world.h"

#ifdef _WIN64
//#define DEBUG_VS   // Uncomment this line to debug vertex shaders
//#define DEBUG_PS   // Uncomment this line to debug pixel shaders
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


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
ID3DXFont*              g_pFont = NULL;         // Font for drawing text
ID3DXSprite*            g_pTextSprite = NULL;   // Sprite for batching draw text calls
ID3DXEffect*            g_pEffect = NULL;       // D3DX effect interface
CDXUTMesh               g_Mesh,g_heroMesh,g_circleMesh;                 // Mesh objects						
D3DXMATRIXA16           g_mView;
CModelViewerCamera      g_Camera;               // A model viewing camera
CDXUTDialogResourceManager g_DialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg         g_SettingsDlg;          // Device settings dialog
CDXUTDialog             g_HUD;                  // dialog for standard controls

LPCWSTR GetRandomUserName();

// Distance between camera and object for rendering
// 렌더링에 쓰일 카메라의 피사체와의 거리
const float CameraDistance = 80;
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
	DXUTCreateWindow( L"ProudNet World Synchronization" );
	DXUTCreateDevice( D3DADAPTER_DEFAULT, true, 640, 480, IsDeviceAcceptable, ModifyDeviceSettings );

	// Pass control to DXUT for handling the message pump and 
	// dispatching render calls. DXUT will call your FrameMove 
	// and FrameRender callback when there is idle time between handling window messages.
	DXUTMainLoop();

	// Perform any application-level cleanup here. Direct3D device resources are released within the
	// appropriate callback functions and therefore don't require any cleanup code here.

	return DXUTGetExitCode();
}


//--------------------------------------------------------------------------------------
// Initialize the app
//--------------------------------------------------------------------------------------
void InitApp()
{
	// Initialize dialogs
	g_SettingsDlg.Init( &g_DialogResourceManager );
	g_HUD.Init( &g_DialogResourceManager );
	g_World->m_connectForm.Init( &g_DialogResourceManager );
	g_World->m_errorForm.Init( &g_DialogResourceManager );
	g_World->m_mainForm.Init( &g_DialogResourceManager );

	int iY = 10;
	g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22 );
	g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22 );
	g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 35, iY += 24, 125, 22, VK_F2 );

	// 클라이언트에서 사용될 UI들을 초기화한다.
	// 자세한 것은 DirectX SDK CustomUI sample을 참고할 것.
	// Initialize UI that using at client.
	// Please refer DirectX SDK CustomUI sample for more detail.
	g_World->m_connectForm.SetCallback( ConnectForm_OnGuiEvent );
	g_World->m_connectForm.SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );
	g_World->m_connectForm.SetFont( 2, L"Courier New", 16, FW_NORMAL );
	g_World->m_connectForm.SetLocation(180, 180);

	g_World->m_errorForm.SetCallback( ErrorForm_OnGuiEvent );
	g_World->m_errorForm.SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );
	g_World->m_errorForm.SetFont( 2, L"Courier New", 16, FW_NORMAL );
	g_World->m_errorForm.SetLocation(180, 180);

	g_World->m_mainForm.SetCallback( MainForm_OnGuiEvent );
	g_World->m_mainForm.SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );
	g_World->m_mainForm.SetFont( 2, L"Courier New", 16, FW_NORMAL );
	g_World->m_mainForm.SetLocation(0, 0);

	CStringW addressString;
	addressString.LoadStringW(IDS_STRING115);
	// Static
	g_World->m_connectForm.AddStatic(IDC_SERVER_ADDR_STATIC, addressString, 0, 0, 200, 20);
	g_World->m_connectForm.GetControl(IDC_SERVER_ADDR_STATIC)->SetTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	g_World->m_connectForm.GetControl(IDC_SERVER_ADDR_STATIC)->GetElement(0)->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;

	CStringW userString;
	userString.LoadStringW(IDS_STRING114);
	g_World->m_connectForm.AddStatic(IDC_USER_NAME_STATIC, userString, 0, 50, 200, 20);
	g_World->m_connectForm.GetControl(IDC_USER_NAME_STATIC)->SetTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	g_World->m_connectForm.GetControl(IDC_USER_NAME_STATIC)->GetElement(0)->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;

	g_World->m_errorForm.AddStatic(IDC_STATIC, L"<Error String>", 0, 0, 257, 48);
	g_World->m_errorForm.GetControl(IDC_STATIC)->SetTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	g_World->m_errorForm.GetControl(IDC_STATIC)->GetElement(0)->dwTextFormat = DT_CENTER | DT_VCENTER | DT_WORDBREAK;

	CStringW logString;
	logString.LoadStringW(IDS_STRING116);
	g_World->m_mainForm.AddStatic(IDC_STATIC_LOG, logString, 20, 0, 100, 20);
	g_World->m_mainForm.GetControl(IDC_STATIC_LOG)->SetTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	g_World->m_mainForm.GetControl(IDC_STATIC_LOG)->GetElement(0)->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;

	g_World->m_mainForm.AddStatic(IDC_STATIC_CLIENTCOUNT, L"<Client Count>", 420, 0, 150, 20);
	g_World->m_mainForm.GetControl(IDC_STATIC_CLIENTCOUNT)->SetTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	g_World->m_mainForm.GetControl(IDC_STATIC_CLIENTCOUNT)->GetElement(0)->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;

	// 원하는 클라이언트 카운트
	// Wanted client count
	CStringW wantCountString;
	wantCountString.LoadStringW(IDS_STRING104);
	g_World->m_mainForm.AddStatic(IDC_STATIC_DESIRED_CLIENTCOUNT, wantCountString, 420, 50, 150, 20);
	g_World->m_mainForm.GetControl(IDC_STATIC_DESIRED_CLIENTCOUNT)->SetTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	g_World->m_mainForm.GetControl(IDC_STATIC_DESIRED_CLIENTCOUNT)->GetElement(0)->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;

	// 제어중인 클라이언트 카운트
	// Controlling client count
	CStringW controllingClientString;
	controllingClientString.LoadStringW(IDS_STRING105);
	g_World->m_mainForm.AddStatic(IDC_STATIC_SELECTED_CLIENT, controllingClientString, 420, 50, 80, 50);
	g_World->m_mainForm.GetControl(IDC_STATIC_SELECTED_CLIENT)->SetTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	g_World->m_mainForm.GetControl(IDC_STATIC_SELECTED_CLIENT)->GetElement(0)->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;

	// Buttons
	g_World->m_connectForm.AddButton( IDC_OK, L"OK", 40, 100, 80, 35, VK_RETURN );
	g_World->m_connectForm.AddButton( IDC_CANCEL, L"Cancel", 120, 100, 80, 35, VK_ESCAPE );

	g_World->m_errorForm.AddButton( IDC_OK, L"OK", 78, 62, 102, 37, VK_RETURN);

	// ����
	// Apply
	CStringW applyString;
	applyString.LoadStringW(IDS_STRING111);
	g_World->m_mainForm.AddButton( IDC_APPLY, applyString, 78, 62, 80, 35, VK_RETURN);

	// Edit box
	g_World->m_connectForm.AddEditBox( IDC_SERVER_ADDR, L"localhost", 0, 15, 200, 30);
	g_World->m_connectForm.AddEditBox( IDC_USER_NAME, GetRandomUserName(), 0, 65, 200, 30);

	g_World->m_mainForm.AddEditBox( IDC_DESIRED_CLIENTCOUNT, L"1", 420, 30, 50, 30);

	// List box
	g_World->m_mainForm.AddListBox( IDC_LOG, 20, 20, 400, 100, 0);
	g_World->m_mainForm.AddListBox( IDC_CLIENT_LIST, 20, 20, 50, 50, 0);

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
	if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
		AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
		D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
		return false;

	// Must support pixel shader 1.1
	if( pCaps->PixelShaderVersion < D3DPS_VERSION( 1, 1 ) )
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
	if( (pCaps->DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		pCaps->VertexShaderVersion < D3DVS_VERSION(1,1) )
	{
		pDeviceSettings->BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	// Debugging vertex shaders requires either REF or software vertex processing 
	// and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
	if( pDeviceSettings->DeviceType != D3DDEVTYPE_REF )
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
	if( s_bFirstTime )
	{
		s_bFirstTime = false;
		if( pDeviceSettings->DeviceType == D3DDEVTYPE_REF )
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
	V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"SynchWorldClient.fx" ) );
	V_RETURN( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags,
	                                    NULL, &g_pEffect, NULL ) );		
	g_Mesh.Create( pd3dDevice, L"misc\\cell.x" );
	g_heroMesh.Create( pd3dDevice, L"airplane\\Airplane.X" );
	g_circleMesh.Create( pd3dDevice, L"circle\\circle.x" );

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
	if( !(pMesh->GetFVF() & D3DFVF_NORMAL) )
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
	if( rgdwAdjacency == NULL )
		return E_OUTOFMEMORY;
	V( pMesh->GenerateAdjacency(1e-6f,rgdwAdjacency) );
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

	if( g_pFont )
		V_RETURN( g_pFont->OnResetDevice() );
	if( g_pEffect )
		V_RETURN( g_pEffect->OnResetDevice() );

	// Create a sprite to help batch calls when drawing many lines of text
	V_RETURN( D3DXCreateSprite( pd3dDevice, &g_pTextSprite ) );

	// Setup the camera's projection parameters
	float fAspectRatio =pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
	g_Camera.SetProjParams( D3DX_PI/4, fAspectRatio, 0.1f, 1000.0f );
	
	g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );

	g_HUD.SetLocation( pBackBufferSurfaceDesc->Width-170, 0 );
	g_HUD.SetSize( 170, 170 );

	g_World->m_connectForm.SetLocation(180, 180);
	g_World->m_errorForm.SetLocation(180, 180);
	g_World->m_mainForm.SetLocation(0, 0);

	g_World->m_connectForm.SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
	g_World->m_errorForm.SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
	g_World->m_mainForm.SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );

	g_World->m_mainForm.GetControl(IDC_STATIC_LOG)->SetLocation(20, pBackBufferSurfaceDesc->Height - 150);
	g_World->m_mainForm.GetControl(IDC_LOG)->SetLocation(20, pBackBufferSurfaceDesc->Height - 130);
	g_World->m_mainForm.GetControl(IDC_STATIC_CLIENTCOUNT)->SetLocation(450, pBackBufferSurfaceDesc->Height - 150);
	g_World->m_mainForm.GetControl(IDC_STATIC_DESIRED_CLIENTCOUNT)->SetLocation(450, pBackBufferSurfaceDesc->Height - 130);
	g_World->m_mainForm.GetControl(IDC_DESIRED_CLIENTCOUNT)->SetLocation(450, pBackBufferSurfaceDesc->Height - 110);
	g_World->m_mainForm.GetControl(IDC_APPLY)->SetLocation(500, pBackBufferSurfaceDesc->Height - 110);
	g_World->m_mainForm.GetControl(IDC_STATIC_SELECTED_CLIENT)->SetLocation(450, pBackBufferSurfaceDesc->Height - 70);
	g_World->m_mainForm.GetControl(IDC_CLIENT_LIST)->SetLocation(520, pBackBufferSurfaceDesc->Height - 70);

	g_Mesh.RestoreDeviceObjects( pd3dDevice );
	g_circleMesh.RestoreDeviceObjects( pd3dDevice );
	g_heroMesh.RestoreDeviceObjects( pd3dDevice );

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
#ifdef _DEBUG
	// 만약 lost focus 상태이면 CPU 점유를 줄이기 위해 프레임수를 낮춘다.
	// If it is lost focus state then lower frame number to reduce CPU occupation rate.
	if (!DXUTIsActive())
		Sleep(10);
#endif
	g_World->FrameMove(fElapsedTime);
	D3DXVECTOR3 eyePos = g_World->m_cameraPosition.GetFollowerPosition();
	eyePos.y += CameraDistance;
	eyePos.z -= CameraDistance;

	D3DXVECTOR3 atPos = g_World->m_cameraPosition.GetFollowerPosition();

	D3DXMatrixLookAtLH(&g_mView, &eyePos, &atPos, &D3DXVECTOR3(0, 1, 0));
}

void RenderMesh( D3DXMATRIX mWorld, CDXUTMesh &mesh , D3DXCOLOR diffuse,bool btrace)
{
	D3DXMATRIX mProj = *g_Camera.GetProjMatrix();
	D3DXMATRIX mView = g_mView;

	D3DXMATRIX mWorldViewProjection = mWorld * mView * mProj;

	if(btrace)
	{
		D3DXVECTOR3 vector1;
		D3DXVec3TransformCoord(&vector1,&D3DXVECTOR3(0,0,0),&mWorldViewProjection);

		//ATLTRACE("x:%f,y:%f,z:%f\n",vector1.x,vector1.y,vector1.z);
	}

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

// 지정한 3D 공간에 텍스트를 표시한다.
// Indicate text at specific 3D area.
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
		D3DXMATRIX mWorld;

		// ����� �׸���, ���μ��η� �޿� �ø���.
		// Draw background but stretch width and hight.
		D3DXMatrixScaling(&mWorld, 200, 1, 200);

		RenderMesh(mWorld, g_Mesh);

		g_World->Render();

		RenderText();
		V( g_HUD.OnRender( fElapsedTime ) );
#ifdef ENABLE_SAMPLE_UI
		V( g_SampleUI.OnRender( fElapsedTime ) );
#endif
		g_World->RenderForm( fElapsedTime );

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
	if(!g_World)
		return 0;

	*pbNoFurtherProcessing = g_World->FormMsgProc( hWnd, uMsg, wParam, lParam );
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
	g_Mesh.InvalidateDeviceObjects();

	if( g_pFont )
		g_pFont->OnLostDevice();
	if( g_pEffect )
		g_pEffect->OnLostDevice();

	g_Mesh.InvalidateDeviceObjects();
	g_circleMesh.InvalidateDeviceObjects();
	g_heroMesh.InvalidateDeviceObjects();

	SAFE_RELEASE(g_pTextSprite);
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice( void* pUserContext )
{
	g_DialogResourceManager.OnDestroyDevice();
	g_SettingsDlg.OnDestroyDevice();
	g_Mesh.Destroy();
	g_circleMesh.Destroy();
	g_heroMesh.Destroy();

	SAFE_RELEASE(g_pEffect);
	SAFE_RELEASE(g_pFont);
}

void CALLBACK MainForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch (nControlID)
	{
	case IDC_APPLY:
	{
		// 입력한 클라 갯수의 값을 얻는다.
		// Get value of entered client number.
		int desiredCount = _wtoi(g_World->m_mainForm.GetEditBox(IDC_DESIRED_CLIENTCOUNT)->GetText());
		desiredCount = max(1, desiredCount);
		CStringW newText;
		newText.Format(L"%d", desiredCount);
		g_World->m_mainForm.GetEditBox(IDC_DESIRED_CLIENTCOUNT)->SetText(newText);

		g_World->m_desiredClientCount = desiredCount;
	}
	break;
	case IDC_CLIENT_LIST:
		// 선택한 항목에 대응하는 클라이언트를 선택한다.
		// Select client that opposite selected item.
		CDXUTListBox *listCtl =  g_World->m_mainForm.GetListBox(IDC_CLIENT_LIST);
		if (listCtl->GetSelectedItem())
		{
			CClient* client = (CClient*)listCtl->GetSelectedItem()->pData;
			g_World->m_clientUnderUserControl = g_World->FindClientByValue(client);
			ASSERT(g_World->m_clientUnderUserControl >= 0);
		}
		break;
	}
}

LPCWSTR SampleNames[] =
{
	L"Edward",
	L"Edwin",
	L"Enoch",
	L"Eugene",
	L"Evelyn",
	L"Ferdinand",
	L"Frances",
	L"Frederick",
	L"Gabriel",
	L"Geoffrey",
	L"George",
	L"Gilbert",
	L"Gregory",
	L"Harold",
	L"Henry",
	L"Herbert",
	L"Harace",
	L"Humphrey",
	L"Issac",
	L"Jacob",
	L"Jerome",
	L"John",
	L"Kenneth",
	L"Lawrence",
	L"Leonard",
	L"Leslie",
	L"Lewi",
	L"Martin",
	L"Matthew",
	L"Nicholas",
	L"Noel",
	L"Oliver",
	L"Oscar",
	L"Oswald",
	L"Owen",
	L"Patricia",
	L"Paul",
	L"Peter",
	L"Philip",
	L"Richard",
	L"Robert",
	L"Roland",
	L"Samuel",
	L"Sabastian",
	L"Theodore",
	L"Thomas",
	L"Vincent",
	L"Vivian",
	L"Wallace",
	L"William",
	L"Agatha",
	L"Agnes",
	L"Angela",
	L"Aileen",
	L"Alice",
	L"Amy",
	L"Beatrice",
	L"Bridget",
	L"Catherine",
	L"Cecil",
	L"Cordelia",
	L"Dorothy",
	L"Elizabeth",
	L"Edith",
	L"Emery",
};

// 랜덤으로 캐릭터 이름을 만든다. 테스트를 위함.
// Create character name randomly. Test purpose.
LPCWSTR GetRandomUserName()
{
	int index = CRandom::StaticGetInt() % (sizeof(SampleNames)/sizeof(SampleNames[0]));
	return SampleNames[index];
}

