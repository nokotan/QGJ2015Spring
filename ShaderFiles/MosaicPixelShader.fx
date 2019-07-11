// ピクセルシェーダーの入力
struct PS_INPUT
{
	float2 TexCoords0		: TEXCOORD0;		// テクスチャ座標
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color			: COLOR0;
};


// C++ 側で設定するテクスチャの定義	
sampler SourceImage : register(s0);				// ディフューズマップテクスチャ
float4 GraphSize	: register(c22);

// main関数
PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT PSOutput;
	float2 TexturePos;

	TexturePos.x = (floor(PSInput.TexCoords0.x * GraphSize.x) + .5f) * GraphSize.z;
	TexturePos.y = (floor(PSInput.TexCoords0.y * GraphSize.y) + .5f) * GraphSize.w;

	// テクスチャカラーの読み込み
	PSOutput.Color = tex2D(SourceImage, TexturePos);

	// 出力パラメータを返す
	return PSOutput;
}