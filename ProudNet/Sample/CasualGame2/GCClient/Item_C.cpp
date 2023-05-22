#include "StdAfx.h"
#include "Item_C.h"
#include "MainWindow.h"

CItem_C::CItem_C(void)
{
}

CItem_C::~CItem_C(void)
{
}
// Does only drawing.
//그리는 역할만 합니다.
void CItem_C::Render()
{
	D3DXMATRIX moveTM, rotTM, scaleTM;

	D3DXMatrixScaling(&scaleTM, 10, 1, 10);

	
	D3DXMatrixRotationY(&rotTM, -D3DX_PI / 180*90 );
	D3DXMatrixTranslation(&moveTM, m_position.x, m_position.y, m_position.z);
		
	switch(m_type)
	{
	case LIFEINCREASE:
		{
			RenderMesh(scaleTM*moveTM,g_GoodItemMesh,D3DXCOLOR(1,1,1,1));
			RenderSignPost(m_position + D3DXVECTOR3(0,10,0),L"LIFE + 1");
		}
		break;
	case LIFEDECREASE:
		{
			RenderMesh(scaleTM*moveTM,g_BadItemMesh,D3DXCOLOR(1,1,1,1));
			RenderSignPost(m_position + D3DXVECTOR3(0,10,0),L"LIFE --");
		}
		break;
	case DISAPPEARBULLET:
		{
			RenderMesh(scaleTM*moveTM,g_ClearItemMesh,D3DXCOLOR(1,1,1,1));
			RenderSignPost(m_position + D3DXVECTOR3(0,10,0),L"Bullet Clear");
		}
		break;
	case RAPID:
		{
			RenderMesh(scaleTM*moveTM,g_RapidItemMesh,D3DXCOLOR(1,1,1,1));
			RenderSignPost(m_position + D3DXVECTOR3(0,10,0),L"Bullet Rapid");
		}
		break;
	}
}
