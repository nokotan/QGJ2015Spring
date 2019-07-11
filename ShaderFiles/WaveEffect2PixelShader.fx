// C++ ���Őݒ肷��e�N�X�`���̒�`	
sampler SourceImage : register(s0);				// �f�B�t���[�Y�}�b�v�e�N�X�`��
sampler DeltaImage	: register(s1);				// �f�B�t���[�Y�}�b�v�e�N�X�`��
float4  WaveParam	: register(c22);
float4  NormParam	: register(c23);
float4  DecayParam	: register(c24);
float4  ScreenSize	: register(c25);

// main�֐�
float4 main(float2 TexCoords : TEXCOORD0) : COLOR0 {

	float4 PSOutput;

	// CPU ������ ----------------------------------------
	// float2 DeltaPos = tex2D(DeltaImage, TexCoords0).xy;
	//
	// // �e�N�X�`���J���[�̓ǂݍ���
	// PSOutput = tex2D(SourceImage, TexCoords0 + DeltaPos);

	// GPU ������ ----------------------------------------
	// �����x�N�g��
	float2 DistancePos = (TexCoords - ScreenSize.xy) * ScreenSize.zw;
	// �����x�N�g���̑傫��
	float Distance = sqrt(DistancePos.x * DistancePos.x + DistancePos.y * DistancePos.y);
	// �ʑ��p�����[�^
	float LocalFlame = (WaveParam.x * WaveParam.y - Distance * WaveParam.z);

	if (LocalFlame < 0 || LocalFlame > WaveParam.w) {
		PSOutput = tex2D(SourceImage, TexCoords);
	} else {
		// ����������
		float Decay = max(DecayParam.x - Distance, 0.f) * DecayParam.y;

		if (Decay <= .001f) {
			PSOutput = tex2D(SourceImage, TexCoords);
		} else {
			// �ψ�
			float2 Delta = Decay * DecayParam.zw;
			// �ψʃx�N�g��
			float2 DeltaPos = DistancePos / Distance * -sin(LocalFlame) * Delta;

			PSOutput = tex2D(SourceImage, TexCoords + DeltaPos);
			PSOutput.rgb += dot(normalize(float3(DeltaPos, Delta.x)), NormParam.xyz) * Decay * NormParam.w;
		}
	}

	// �o�̓p�����[�^��Ԃ�
	return PSOutput;
}