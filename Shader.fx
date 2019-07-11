sampler SourseImage : register(s0);
float4 GraphSize : register(c22);
float4 Parameter : register(c23);
float4 main(float2 TexPos : TEXCOORD0) : COLOR0{
	const float DoublePI = 3.1415926536f * 2;
	TexPos.y += Parameter.x * sin(DoublePI * (Parameter.y * Parameter.z - TexPos.x * GraphSize.x * Parameter.w)) * GraphSize.w; 
	return tex2D(SourseImage, TexPos);
}
