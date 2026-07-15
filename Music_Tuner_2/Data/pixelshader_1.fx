Texture2D SceneTexture : register(t0);

SamplerState SceneSampler : register(s0)
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 Diffuse : COLOR0;
    float4 Specular : COLOR1;
    float2 TexCoord0 : TEXCOORD0;
    float2 TexCoord1 : TEXCOORD1;
};

// 定数バッファ
cbuffer FishEyeParam : register(b0)
{
    float Strength;
    float Zoom;
    float2 Padding;
};

float4 PS_Main(PS_INPUT input) : SV_TARGET
{
    float2 uv = input.TexCoord0;

    // 中心座標
    float2 center = float2(0.5f, 0.5f);

    // 中心からのベクトル
    float2 d = uv - center;

    // 半径
    float r = length(d);

    // 中心はそのまま 最大値は0.5fだね
    if (r < 0.00001f)
    {
        return SceneTexture.Sample(SceneSampler, uv) * input.Diffuse;
    }
    
    //★後ほど削除する
    float strength = 2.0f;
    float zoom = 0.7f;

    // 魚眼変換
    //そのピクセルの距離を変換してる
    float newR = atan(r * Strength) / atan(Strength);

    // UV変換
    //変換した距離をuv座標に直してる
    float2 newUV = center + normalize(d) * newR * Zoom;
    
    //newUV = center + (center - newUV) * zoom;

    // 範囲外は黒
    if (newUV.x < 0.0f || newUV.x > 1.0f ||
        newUV.y < 0.0f || newUV.y > 1.0f)
    {
        return float4(0, 0, 0, 1);
    }
    
    return SceneTexture.Sample(SceneSampler, newUV) * input.Diffuse;
}