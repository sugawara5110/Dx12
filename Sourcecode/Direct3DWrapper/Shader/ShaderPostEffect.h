///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           ShaderPostEffect.hlsl                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

char *ShaderPostEffect =
"RWTexture2D<float4> gInput : register(u0);\n"
"RWTexture2D<float4> gOutput : register(u1);\n"

//モザイク大きさ
"cbuffer mosaic  : register(b0)\n"
"{\n"
"    float4 g_mosaicSize;\n"//x
"};\n"

//Dispatch(APP側)(X1, Y1, Z1)numthreads(CS側)(X, Y, Z)
//x,y,z,x1,y1,z1 は～番目
//X,Y,Z,X1,Y1,Z1 はMax値
//SV_GroupThreadID:    x, y, z
//SV_GroupID:          x1,y1,z1
//SV_DispatchThreadID: x1*X+x, y1*Y+y, z1*Z+z
//SV_GroupIndex      : z*X*Y+y*X+x
//SV_GroupIndex uint その他uint3

"[numthreads(32, 8, 1)]\n"
"void MosaicCS(int2 dtid : SV_DispatchThreadID)\n"
"{\n"
"   float size1 = 1 / g_mosaicSize.x;\n"
"   int size2 = g_mosaicSize.x;\n"
"   int x = dtid.x * size1;\n"
"   int y = dtid.y * size1;\n"
"	gOutput[dtid] = gInput[int2(x * size2, y * size2)];\n"
"}\n";

