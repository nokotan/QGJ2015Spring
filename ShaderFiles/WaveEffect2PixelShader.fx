// C++ 側で設定するテクスチャの定義	
sampler SourceImage : register(s0);				// ディフューズマップテクスチャ
sampler DeltaImage	: register(s1);				// ディフューズマップテクスチャ
float4  WaveParam	: register(c22);
float4  NormParam	: register(c23);
float4  DecayParam	: register(c24);
float4  ScreenSize	: register(c25);

// main関数
float4 main(float2 TexCoords : TEXCOORD0) : COLOR0 {

	float4 PSOutput;

	// CPU 処理版 ----------------------------------------
	// float2 DeltaPos = tex2D(DeltaImage, TexCoords0).xy;
	//
	// // テクスチャカラーの読み込み
	// PSOutput = tex2D(SourceImage, TexCoords0 + DeltaPos);

	// GPU 処理版 ----------------------------------------
	// 距離ベクトル
	float2 DistancePos = (TexCoords - ScreenSize.xy) * ScreenSize.zw;
	// 距離ベクトルの大きさ
	float Distance = sqrt(DistancePos.x * DistancePos.x + DistancePos.y * DistancePos.y);
	// 位相パラメータ
	float LocalFlame = (WaveParam.x * WaveParam.y - Distance * WaveParam.z);

	if (LocalFlame < 0 || LocalFlame > WaveParam.w) {
		PSOutput = tex2D(SourceImage, TexCoords);
	} else {
		// 距離減衰項
		float Decay = max(DecayParam.x - Distance, 0.f) * DecayParam.y;

		if (Decay <= .001f) {
			PSOutput = tex2D(SourceImage, TexCoords);
		} else {
			// 変位
			float2 Delta = Decay * DecayParam.zw;
			// 変位ベクトル
			float2 DeltaPos = DistancePos / Distance * -sin(LocalFlame) * Delta;

			PSOutput = tex2D(SourceImage, TexCoords + DeltaPos);
			PSOutput.rgb += dot(normalize(float3(DeltaPos, Delta.x)), NormParam.xyz) * Decay * NormParam.w;
		}
	}

	// 出力パラメータを返す
	return PSOutput;
}