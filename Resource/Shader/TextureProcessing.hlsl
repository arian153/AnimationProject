float4 AlphaMapping(float2 tex, int diffuse1_id, int diffuse2_id, int alpha_map_id)
{
    float4 diffuse1 = shader_texture[diffuse1_id].Sample(sample_type, tex);
    float4 diffuse2 = shader_texture[diffuse2_id].Sample(sample_type, tex);
    float4 alpha = shader_texture[alpha_map_id].Sample(sample_type, tex);

    float4 blend_color = (alpha * diffuse1) + ((1.0f - alpha) * diffuse2);
    return saturate(blend_color);
}

float3 NormalMapping(float2 tex, float3 t, float3 b, float3 n, int normal_map_id)
{
    float4 normal_map = shader_texture[normal_map_id].Sample(sample_type, tex);
    normal_map = (normal_map * 2.0f) - 1.0f;
    float3 normal = (normal_map.x * t) + (normal_map.y * b) + (normal_map.z * n);
    return normalize(normal);
}

float4 SpecularMapping(float2 tex, int specular_map_id)
{
    return shader_texture[specular_map_id].Sample(sample_type, tex);
}

float4 LightMapping(float2 tex, int diffuse_id, int light_map_id)
{
    float4 diffuse = shader_texture[diffuse_id].Sample(sample_type, tex);
    float4 light_map = shader_texture[light_map_id].Sample(sample_type, tex);
    return diffuse * light_map;
}

float4 BlendTexture(float2 tex, int diffuse1_id, int diffuse2_id, float gamma)
{
    float4 diffuse1 = shader_texture[diffuse1_id].Sample(sample_type, tex);
    float4 diffuse2 = shader_texture[diffuse2_id].Sample(sample_type, tex);
    return diffuse1 * diffuse2 * gamma;
}

float4 ProcessDiffuse(float2 tex, int type, int diff0, int diff1, int diff2, float4 mat, float gamma)
{
    float4 diffuse_texture = mat;
    if (type == 1)
    {
        // sample simple diffuse texture.
        diffuse_texture = shader_texture[diff0].Sample(sample_type, tex);
    }
    else if (type == 2)
    {
        // alpha mapping
        diffuse_texture = AlphaMapping(tex, diff0, diff1, diff2);
    }
    else if (type == 3)
    {
        // light mapping
        diffuse_texture = LightMapping(tex, diff0, diff1);
    }
    else if (type == 4)
    {
        // multiple texture blending
        diffuse_texture = BlendTexture(tex, diff0, diff1, gamma);
    }
    else if (type == 5)
    {
        diffuse_texture = shader_texture[diff0].Sample(sample_type, tex) * mat;
    }
    else
    {
        //use diffuse color
        diffuse_texture = mat;
    }

    return diffuse_texture;
}

float4 ProcessSpecular(float2 tex, int type, int spec0, float4 mat)
{
    float4 specular_texture = float4(0.0f, 0.0f, 0.0f, 0.0f);
    if (type == 1)
    {
        //specular mapping
        specular_texture = SpecularMapping(tex, spec0);
    }
    else
    {
        //use specular color
        specular_texture = mat;
    }

    return specular_texture;
}

float3 ProcessNormal(float2 tex, int type, int norm0, float3 t, float3 b, float3 n)
{
    float3 normal = float3(0.0f, 0.0f, 0.0f);
    if (type == 1)
    {
        normal = NormalMapping(tex, t, b, n, norm0);
    }
    else
    {
        normal = n;
    }
    return normal;
}
