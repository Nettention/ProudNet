//--------------------------------------------------------------------------------------
// File: CustomUI.fx
//
// The effect file for the CustomUI sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
float	 g_fTime;					// App's time in seconds
float4x4 g_mWorld;					// World matrix for object
float4x4 g_mWorldViewProjection;	// World * View * Projection matrix
texture  g_txScene;
float4	 g_vDiffuse;			// ���� ��

sampler  g_samScene =
sampler_state
{
    Texture = <g_txScene>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Point;
};


void VertScene( float4 Pos : POSITION,
                float2 Tex : TEXCOORD0,
                out float4 oPos : POSITION,
                out float4 oD0 : COLOR,
                out float2 oTex : TEXCOORD0)
{
    oPos = mul( Pos, g_mWorldViewProjection );
    oTex = Tex;
    oD0 = g_vDiffuse;
}


float4 PixScene( float4 oD0 : COLOR ,float2 Tex : TEXCOORD0 ) : COLOR0
{
    return tex2D( g_samScene, Tex ) * oD0;
}


//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique RenderScene
{
    pass P0
    {
		ALPHABLENDENABLE=TRUE;

		SRCBLEND=SRCALPHA;
		DESTBLEND=INVSRCALPHA;
		
		ZWriteEnable=true;		
        VertexShader = compile vs_2_0 VertScene();
        PixelShader = compile ps_2_0 PixScene();
    }
}
