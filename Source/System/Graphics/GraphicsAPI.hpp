#pragma once

#if defined(CS460_DIRECTX11)
//classes
#define RendererAPI RendererDX11
//buffer
#define MeshBufferAPI MeshBufferDX11
#define DeferredBufferAPI DeferredBufferDX11
#define RenderTextureBufferAPI RenderTextureBufferDX11
#define TextTextureBufferAPI TextTextureBufferDX11
#define TextLayoutBufferAPI TextLayoutBufferDX11
#define TextFormatBufferAPI TextFormatBufferDX11
#define InstanceBufferAPI InstanceBufferDX11
#define DynamicMeshBufferAPI DynamicMeshBufferDX11

//buffer2
#define ConstantBufferAPI ConstantBufferDX11
#define IndexBufferAPI IndexBufferDX11
#define VertexBufferAPI VertexBufferDX11
#define VertexAttributeAPI VertexAttributeDX11
#define VertexLayoutAPI VertexLayoutDX11

//others
#define TextureAPI TextureDX11
#define TextureArrayAPI TextureArrayDX11
#define TextureAtlasAPI TextureAtlasDX11
#define ColorVertexAPI ColorVertexDX11
#define VertexAPI VertexDX11
#define TextureVertexAPI TextureVertexDX11
#define NormalVertexAPI NormalVertexDX11

//shader
#define ShaderManagerAPI ShaderManagerDX11
#define ShaderProgramAPI ShaderProgramDX11

//includes
#define IncludeRendererAPI "../../DX11/RendererDX11.hpp"
#define IncludeMeshBufferAPI "../../DX11/Buffer/MeshBufferDX11.hpp"
#define IncludeDeferredBufferAPI "../../DX11/Buffer/DeferredBufferDX11.hpp"
#define IncludeRenderTextureBufferAPI "../../DX11/Buffer/RenderTextureBufferDX11.hpp"
#define IncludeTextTextureBufferAPI "../../DX11/Buffer/TextTextureBufferDX11.hpp"
#define IncludeTextLayoutBufferAPI "../../DX11/Buffer/TextLayoutBufferDX11.hpp"
#define IncludeTextFormatBufferAPI "../../DX11/Buffer/TextFormatBufferDX11.hpp"
#define IncludeDynamicMeshBufferAPI "../../DX11/Buffer/DynamicMeshBufferDX11.hpp"

//buffer2
#define IncludeConstantBufferAPI "../../DX11/Buffer2/ConstantBufferDX11.hpp"
#define IncludeIndexBufferAPI  "../../DX11/Buffer2/IndexBufferDX11.hpp"
#define IncludeInstanceBufferAPI "../../DX11/Buffer2/InstanceBufferDX11.hpp"
#define IncludeVertexBufferAPI "../../DX11/Buffer2/VertexBufferDX11.hpp"
#define IncludeVertexLayoutAPI "../../DX11/Buffer2/VertexLayoutDX11.hpp"

#define IncludeMeshBufferAPI "../../DX11/Buffer/MeshBufferDX11.hpp"
#define IncludeDeferredBufferAPI "../../DX11/Buffer/DeferredBufferDX11.hpp"
#define IncludeRenderTextureBufferAPI "../../DX11/Buffer/RenderTextureBufferDX11.hpp"
#define IncludeTextTextureBufferAPI "../../DX11/Buffer/TextTextureBufferDX11.hpp"
#define IncludeTextLayoutBufferAPI "../../DX11/Buffer/TextLayoutBufferDX11.hpp"
#define IncludeTextFormatBufferAPI "../../DX11/Buffer/TextFormatBufferDX11.hpp"
#define IncludeDynamicMeshBufferAPI "../../DX11/Buffer/DynamicMeshBufferDX11.hpp"

#define IncludeTextureAPI "../../DX11/Texture/TextureDX11.hpp"
#define IncludeTextureArrayAPI "../../DX11/Texture/TextureArrayDX11.hpp"
#define IncludeTextureAtlasAPI "../../DX11/Texture/TextureAtlasDX11.hpp"
#define IncludeColorVertexAPI "../../DX11/Vertex/ColorVertexDX11.hpp"
#define IncludeVertexAPI "../../DX11/Vertex/VertexDX11.hpp"
#define IncludeTextureVertexAPI "../../DX11/Vertex/TextureVertexDX11.hpp"
#define IncludeNormalVertexAPI "../../DX11/Vertex/NormalVertexDX11.hpp"

#define IncludeShaderManagerAPI "../../DX11/Shader/ShaderManagerDX11.hpp"
#define IncludeShaderProgramAPI "../../DX11/Shader/ShaderProgramDX11.hpp"

#endif
