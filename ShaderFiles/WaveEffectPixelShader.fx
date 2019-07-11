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
float4 Parameter	: register(c23);			// x:振幅, y:時刻*2*PI(), z:振動数, w:波長の逆数*2*PI()

// main関数
PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT PSOutput;
	float2 TexturePos;

	TexturePos.x = PSInput.TexCoords0.x;
	TexturePos.y = PSInput.TexCoords0.y + Parameter.x * sin(Parameter.y * Parameter.z - PSInput.TexCoords0.x * GraphSize.x * Parameter.w) * GraphSize.w;

	// テクスチャカラーの読み込み
	PSOutput.Color = tex2D(SourceImage, TexturePos);

	// 出力パラメータを返す
	return PSOutput;
}