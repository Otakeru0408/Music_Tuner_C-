//=========================================
// FishEye.fx
//=========================================

// 入力画像
Texture2D SceneTexture : register(t0);

// サンプラー
SamplerState SceneSampler : register(s0)
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

// 魚眼の強さ
cbuffer FishEyeParam : register(b0)
{
    float Strength;
    float3 Padding;
};

// 頂点シェーダーから渡されるデータ
struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD0;
};

//--------------------------------------
// ピクセルシェーダー
//--------------------------------------
float4 PS_Main(PS_INPUT input) : SV_TARGET
{
    
    // 元のUV
    float2 uv = input.UV;

    // 中心を(0,0)へ移動
    float2 p = uv - 0.5;

    // 中心からの距離
    float r = length(p);

    // 中心ならそのまま
    if (r < 0.0001)
    {
        return SceneTexture.Sample(SceneSampler, uv);
    }

    // 魚眼変換
    float newR = atan(r * Strength) / atan(Strength);

    // 新しい座標
    float2 newUV = normalize(p) * newR + 0.5;

    // 範囲外なら黒
    if (newUV.x < 0.0 || newUV.x > 1.0 ||
       newUV.y < 0.0 || newUV.y > 1.0)
    {
        return float4(0, 0, 0, 1);
    }

    // 色取得
    return SceneTexture.Sample(SceneSampler, newUV);
}