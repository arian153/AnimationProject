Texture2D shader_texture[6];
SamplerState sample_type;

#include "LightProcessing.hlsl"
#include "TextureProcessing.hlsl"

cbuffer CameraBuffer
{
    float3 cam_pos;
};

cbuffer LightBuffer
{
    DynamicLight dynamic_light[16];
    int light_count;
};


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

  
    return output;
}

float4 PixelShaderEntry(VSOut input) : SV_TARGET
{



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
