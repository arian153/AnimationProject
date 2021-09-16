Texture2D shader_texture[5];
SamplerState sample_type;

#include "TextureProcessing.hlsl"

//global
cbuffer MatrixBuffer
{
    matrix view;
    matrix proj;
};

cbuffer TextureBuffer
{
    int diff_type;
    int spec_type;
    int norm_type;
    float gamma;
};

//defs
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD;
    float4x4 world : WORLD;
    float4 ambient : COLOR0;
    float4 diffuse : COLOR1;
    float4 specular : COLOR2;
    float4 reflect : COLOR3;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
    float4 ambient : COLOR0;
    float4 diffuse : COLOR1;
    float4 specular : COLOR2;
    float4 reflect : COLOR3;
};

//vertex shader
PixelInputType VertexShaderEntry(VertexInputType input)
{
    input.position.w = 1.0f;

    PixelInputType output;
    output.position = mul(input.position, input.world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, proj);
    output.tex      = input.tex;
    output.ambient = input.ambient;
    output.diffuse = input.diffuse;
    output.specular = input.specular;
    output.reflect = input.reflect;
    return output;
}

//pixel shader
float4 PixelShaderEntry(PixelInputType input) : SV_TARGET
{
    float4 diffuse_texture = ProcessDiffuse(input.tex, diff_type, 0, 1, 2, input.diffuse, gamma);
    return diffuse_texture;
}