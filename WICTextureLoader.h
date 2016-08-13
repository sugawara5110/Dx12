//--------------------------------------------------------------------------------------
// File: WICTextureLoader.h
//
// Function for loading a WIC image and creating a Direct3D 11 runtime texture for it
// (auto-generating mipmaps if possible)
//
// Note: Assumes application has already called CoInitializeEx
//
// Warning: CreateWICTexture* functions are not thread-safe if given a d3dContext instance for
//          auto-gen mipmap support.
//
// Note these functions are useful for images created as simple 2D textures. For
// more complex resources, DDSTextureLoader is an excellent light-weight runtime loader.
// For a full-featured DDS file reader, writer, and texture processing pipeline see
// the 'Texconv' sample and the 'DirectXTex' library.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=248926
// http://go.microsoft.com/fwlink/?LinkId=248929
//--------------------------------------------------------------------------------------

//custom http://zerogram.info
//D3D12対応　Upload用中間リソースを受け取る引数追加
//GPU転送終了まで中間リソースを保持しておく

#ifdef _MSC_VER
#pragma once
#endif

#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP) && (_WIN32_WINNT <= _WIN32_WINNT_WIN8)
#error WIC is not supported on Windows Phone 8.0
#endif

#include <d3d12.h>

#pragma warning(push)
#pragma warning(disable : 4005)
#include <stdint.h>
#pragma warning(pop)

#if defined(_MSC_VER) && (_MSC_VER<1610) && !defined(_In_reads_)
#define _In_reads_(exp)
#define _Out_writes_(exp)
#define _In_reads_bytes_(exp)
#endif

#ifndef _Use_decl_annotations_
#define _Use_decl_annotations_
#endif


namespace DirectX
{

    HRESULT CreateWICTextureFromMemory( _In_ ID3D12Device* d3dDevice,
                                        _In_ ID3D12GraphicsCommandList* cmdList,
                                        _In_reads_bytes_(wicDataSize) const uint8_t* wicData,
                                        _In_ size_t wicDataSize,
										_Out_ ID3D12Resource** texture,
										_Out_ ID3D12Resource** upload,
										_Out_ D3D12_SHADER_RESOURCE_VIEW_DESC* view,
										_In_ size_t maxsize = 0
                                      );

    HRESULT CreateWICTextureFromFile(	_In_ ID3D12Device* d3dDevice,
										_In_ ID3D12GraphicsCommandList* cmdList,
										_In_z_ const wchar_t* szFileName,
										_Out_ ID3D12Resource** texture,
										_Out_ ID3D12Resource** upload,
										_Out_ D3D12_SHADER_RESOURCE_VIEW_DESC* view,
										_In_ size_t maxsize = 0
                                    );

 

    HRESULT CreateWICTextureFromMemoryEx( _In_ ID3D12Device* d3dDevice,
											_In_ ID3D12GraphicsCommandList* cmdList,
											_In_reads_bytes_(wicDataSize) const uint8_t* wicData,
											_In_ size_t wicDataSize,
											_In_ size_t maxsize,
											_In_ bool forceSRGB,
											_Out_ ID3D12Resource** texture,
											_Out_ ID3D12Resource** upload,
											_Out_ D3D12_SHADER_RESOURCE_VIEW_DESC* view
										);

    HRESULT CreateWICTextureFromFileEx( _In_ ID3D12Device* d3dDevice,
                                        _In_ ID3D12GraphicsCommandList* cmdList,
                                        _In_z_ const wchar_t* szFileName,
                                        _In_ size_t maxsize,
                                        _In_ bool forceSRGB,
										_Out_ ID3D12Resource** texture,
										_Out_ ID3D12Resource** upload,
										_Out_ D3D12_SHADER_RESOURCE_VIEW_DESC* view
                                    );
}
