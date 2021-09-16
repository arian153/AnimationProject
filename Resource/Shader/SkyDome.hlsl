Texture2D sky_dome_texture;
SamplerState sample_type
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

//global
cbuffer MatrixBuffer
{
    matrix world
    matrix view;
    matrix proj;
};


//defs
struct VSIn
{
    float4 pos_local : POSITION;
    float2 tex : TEXCOORD;
};

struct VSOut
{
    float4 pos_hclip : SV_POSITION;
    float2 tex : TEXCOORD;
};

//vertex shader
VSOut VertexShaderEntry(VSIn input)
{
    input.position.w = 1.0f;

    VSOut output;
    output.pos_hclip = mul(input.pos_local, input.world);
    output.pos_hclip = mul(output.pos_hclip, view);
    output.pos_hclip = mul(output.pos_hclip, proj).xyww;
    output.tex = input.tex;
    return output;
}

//pixel shader
float4 PixelShaderEntry(VSOut input) : SV_TARGET
{
    return sky_dome_texture.Sample(sample_type, input.tex);
}

RasterizerState NoCullRS
{
    CullMode = None;
};

DepthStencilState LessEqualDSS
{
    DepthFunc = LESS_EQUAL;
};