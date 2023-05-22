#pragma once

#include "DXUTForm.h"

INT MainForm();
void RenderMesh( D3DXMATRIX mWorld, CDXUTMesh &mesh , D3DXCOLOR diffuse = D3DXCOLOR(1, 1, 1, 1),bool btrace = false);
void RenderSignPost( D3DXVECTOR3 pos3D, LPCWSTR text );

extern CDXUTMesh          g_heroMesh, g_circleMesh;

void CALLBACK MainForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
