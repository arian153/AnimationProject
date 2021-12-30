Texture2D shader_texture[5];
SamplerState sample_type;

#include "TextureProcessing.hlsl"
cbuffer MatrixBuffer
{
    matrix world;
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

cbuffer MaterialBuffer
{
    float4 ambient : COLOR0;
    float4 diffuse : COLOR1;
    float4 specular : COLOR2;
    float4 reflect_ : COLOR3;
}

struct VSIn
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 pos_world : TEXCOORD1;
};


VSOut VertexShaderEntry(VSIn input)
{
    VSOut output;
    input.position.w = 1.0f;

    output.position = mul(input.position, input.world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, proj);
    output.tex = input.tex;

    float3x3 inv_trans_world = (float3x3)input.world;

    output.normal = mul(input.normal, inv_trans_world);
    output.normal = normalize(output.normal);

    output.tangent = mul(input.tangent, inv_trans_world);
    output.tangent = normalize(output.tangent);

    output.binormal = mul(input.binormal, inv_trans_world);
    output.binormal = normalize(output.binormal);

    output.pos_world = (float3)mul(input.position, input.world);


    return output;
}

struct PS_GBUFFER_OUT
{
    float4 pos_out  : SV_TARGET0;
    float4 norm_out : SV_TARGET1;
    float4 ambi_out : SV_TARGET2;
    float4 diff_out : SV_TARGET3;
    float4 spec_out : SV_TARGET4;
    float4 refl_out : SV_TARGET5;
};

PS_GBUFFER_OUT PixelShaderEntry(VSOut input)
{
    PS_GBUFFER_OUT output;

    //process material
    output.ambi_out = ambient;
    output.diff_out = ProcessDiffuse(input.tex, diff_type, 0, 1, 2, diffuse, gamma);
    output.spec_out = ProcessSpecular(input.tex, spec_type, 3, specular);
    output.refl_out = reflect;

    //process normal
    float3 tangent_world = normalize(input.tangent);
    float3 binormal_world = normalize(input.binormal);
    float3 normal_world = normalize(input.normal);
    float3 normal_vec = ProcessNormal(input.tex, norm_type, 4, tangent_world, binormal_world, normal_world);

    output.pos_out;
    output.norm_out = normal_vec;

    return output;
}
