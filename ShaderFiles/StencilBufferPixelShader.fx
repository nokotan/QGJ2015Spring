// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 Diffuse			: COLOR0;
	float4 Specular			: COLOR1;
	float2 TexCoords0		: TEXCOORD0;		// �e�N�X�`�����W
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color			: COLOR0;
};


// C++ ���Őݒ肷��e�N�X�`���̒�`	
sampler SourceImage : register(s0); // = sampler_state {
//	MagFilter = Linear;
// };	// �f�B�t���[�Y�}�b�v�e�N�X�`��


// main�֐�
PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT PSOutput;
	float4 TextureDiffuseColor;

	// �e�N�X�`���J���[�̓ǂݍ���
	TextureDiffuseColor = tex2D(SourceImage, PSInput.TexCoords0);

	if (TextureDiffuseColor.a == 0.f) {
		PSOutput.Color = PSInput.Specular;
	} else {
		PSOutput.Color = PSInput.Diffuse;
	}

	// �o�̓p�����[�^��Ԃ�
	return PSOutput;
}