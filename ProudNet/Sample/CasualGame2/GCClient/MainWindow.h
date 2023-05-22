#pragma once

#include "DXUTForm.h"


INT MainForm();
void RenderMesh( D3DXMATRIX mWorld, CDXUTMesh &mesh , D3DXCOLOR diffuse = D3DXCOLOR(1, 1, 1, 1));

/**
\~korean
지정한 3D 공간에 텍스트를 표시합니다.

\~english
Mark text at specific 3D area.

\~chinese
在指定的3D空间显示文本。

\~japanese
指定した3D空間にテキストを表示します。

\~
*/
void RenderSignPost( D3DXVECTOR3 pos3D, LPCWSTR text );

enum
{
	WM_LAUNCH_LOBBY_CLIENT = WM_USER + 1,
	WM_LAUNCH_BATTLE_CLIENT,
	
};

/**
\~korean
메쉬 객체들

\~english
Mesh Objects

\~chinese
Mesh对象

\~japanese
メッシュオブジェクト

\~
*/
extern CDXUTMesh    g_terrainMesh,g_HeroMesh,g_synchAreaMesh,g_BulletMesh,g_cylinderMesh,g_BombMesh,
					g_GoodItemMesh,g_BadItemMesh,g_ClearItemMesh,g_ActiveItemMesh,g_RapidItemMesh,
					g_GameAreaMesh;   


extern D3DXMATRIXA16           g_mView;
