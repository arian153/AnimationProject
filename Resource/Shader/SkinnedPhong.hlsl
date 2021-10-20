Texture2D shader_texture[5];
SamplerState sample_type;

#include "LightProcessing.hlsl"
#include "TextureProcessing.hlsl"

cbuffer MatrixBuffer
{
    matrix world;
    matrix view;
    matrix proj;
};

cbuffer SkinnedBuffer
{
    matrix bone_transform[127];
    int bone_count;
};

cbuffer TextureBuffer
{
    int diff_type;
    int spec_type;
    int norm_type;
    float gamma;
};

cbuffer CameraBuffer
{
    float3 cam_pos;
};

cbuffer LightBuffer
{
    DynamicLight dynamic_light[16];
    int light_count;
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
    float4 weight : WEIGHTS;
    int4 bone_indices : BONEINDICES;
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

    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    weights[0] = input.weight.x;
    weights[1] = input.weight.y;
    weights[2] = input.weight.z;
    weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

    float3 pos_local = float3(0.0f, 0.0f, 0.0f);
    float3 normal_local = float3(0.0f, 0.0f, 0.0f);
    float3 tangent_local = float3(0.0f, 0.0f, 0.0f);
    float3 binormal_local = float3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; ++i)
    {
        pos_local      += weights[i] * mul(input.position, bone_transform[input.bone_indices[i]]).xyz;
        normal_local   += weights[i] * mul(input.normal,   (float3x3)bone_transform[input.bone_indices[i]]);
        tangent_local  += weights[i] * mul(input.tangent,  (float3x3)bone_transform[input.bone_indices[i]]);
        binormal_local += weights[i] * mul(input.binormal, (float3x3)bone_transform[input.bone_indices[i]]);
    }

    output.position = mul(float4(pos_local, 1.0f), world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, proj);
    output.tex = input.tex;

    float3x3 inv_trans_world = (float3x3)world;

    output.normal = mul(normal_local, inv_trans_world);
    output.normal = normalize(output.normal);

    output.tangent = mul(tangent_local, inv_trans_world);
    output.tangent = normalize(output.tangent);

    output.binormal = mul(binormal_local, inv_trans_world);
    output.binormal = normalize(output.binormal);

    output.pos_world = (float3)mul(pos_local, world);

    return output;
}

float4 PixelShaderEntry(VSOut input) : SV_TARGET
{
    //process material
    Material material;
    material.ambient = ambient;
    material.diffuse = ProcessDiffuse(input.tex, diff_type, 0, 1, 2, diffuse, gamma);
    material.specular = ProcessSpecular(input.tex, spec_type, 3, specular);
    material.reflect = reflect_;

    //process normal
    float3 tangent_world = normalize(input.tangent);
    float3 binormal_world = normalize(input.binormal);
    float3 normal_world = normalize(input.normal);
    float3 normal_vec = ProcessNormal(input.tex, norm_type, 4, tangent_world, binormal_world, normal_world);

    //process eye vector
    float3 to_eye_world = normalize(cam_pos - input.pos_world);

    //process lighting
    float4 ambient_light = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse_light = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular_light = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 A, D, S;

    for (int i = 0; i < light_count; ++i)
    {
        if (dynamic_light[i].type == 0)
        {
            CalculateAmbient(material, dynamic_light[i], normal_vec, A);
            ambient_light += A;
        }
        else if (dynamic_light[i].type == 1)
        {
            CalculateDirectional(material, dynamic_light[i], normal_vec, to_eye_world, A, D, S);
            ambient_light += A;
            diffuse_light += D;
            specular_light += S;

        }
        else if (dynamic_light[i].type == 2)
        {
            CalculatePoint(material, dynamic_light[i], input.pos_world, normal_vec, to_eye_world, A, D, S);

            ambient_light += A;
            diffuse_light += D;
            specular_light += S;

        }
        else if (dynamic_light[i].type == 3)
        {
            CalculateSpot(material, dynamic_light[i], input.pos_world, normal_vec, to_eye_world, A, D, S);

            ambient_light += A;
            diffuse_light += D;
            specular_light += S;
        }
        else if (dynamic_light[i].type == 4)
        {
            CalculateCapsule(material, dynamic_light[i], input.pos_world, normal_vec, to_eye_world, A, D, S);

            ambient_light += A;
            diffuse_light += D;
            specular_light += S;
        }

    }

    float4 final_color = ambient_light + diffuse_light + specular_light;
    // take alpha from diffuse material
    final_color.a = material.diffuse.a;

    //do post process.

    return final_color;
}
