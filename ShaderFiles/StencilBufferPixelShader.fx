// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 Diffuse			: COLOR0;
	float4 Specular			: COLOR1;
	float2 TexCoords0		: TEXCOORD0;		// テクスチャ座標
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color			: COLOR0;
};


// C++ 側で設定するテクスチャの定義	
sampler SourceImage : register(s0); // = sampler_state {
//	MagFilter = Linear;
// };	// ディフューズマップテクスチャ


// main関数
PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT PSOutput;
	float4 TextureDiffuseColor;

	// テクスチャカラーの読み込み
	TextureDiffuseColor = tex2D(SourceImage, PSInput.TexCoords0);

	if (TextureDiffuseColor.a == 0.f) {
		PSOutput.Color = PSInput.Specular;
	} else {
		PSOutput.Color = PSInput.Diffuse;
	}

	// 出力パラメータを返す
	return PSOutput;
}