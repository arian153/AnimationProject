TextureCube cube_map_texture;
SamplerState sample_type
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

//global
cbuffer MatrixBuffer
{
    matrix world;
    matrix view;
    matrix proj;
};


//defs
struct VSIn
{
    float4 pos_local : POSITION;
};

struct VSOut
{
    float4 pos_hclip : SV_POSITION;
    float3 pos_local : POSITION;
};

//vertex shader
VSOut VertexShaderEntry(VSIn input)
{
    input.pos_local.w = 1.0f;

    VSOut output;
    output.pos_hclip = mul(input.pos_local, world);
    output.pos_hclip = mul(output.pos_hclip, view);
    output.pos_hclip = mul(output.pos_hclip, proj).xyww;
    output.pos_local = input.pos_local;
    return output;
}

//pixel shader
float4 PixelShaderEntry(VSOut input) : SV_TARGET
{
    return cube_map_texture.Sample(sample_type, input.pos_local);
}

RasterizerState NoCullRS
{
    CullMode = None;
};

DepthStencilState LessEqualDSS
{
    DepthFunc = LESS_EQUAL;
};