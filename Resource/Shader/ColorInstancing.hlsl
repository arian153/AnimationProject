//global
cbuffer MatrixBuffer
{
    matrix view;
    matrix proj;
};

//defs
struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
    float4x4 world : WORLD;
    float4 ambient : COLOR1;
    float4 diffuse : COLOR2;
    float4 specular : COLOR3;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

//vertex shader
PixelInputType VertexShaderEntry(VertexInputType input)
{
    input.position.w = 1.0f;

    PixelInputType output;
    output.position = mul(input.position, input.world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, proj);
    output.color = input.color * input.diffuse;

    return output;
}

//pixel shader
float4 PixelShaderEntry(PixelInputType input) : SV_TARGET
{
    return input.color;
}
