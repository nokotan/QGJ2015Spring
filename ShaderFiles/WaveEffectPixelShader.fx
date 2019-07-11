// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float2 TexCoords0		: TEXCOORD0;		// �e�N�X�`�����W
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color			: COLOR0;
};


// C++ ���Őݒ肷��e�N�X�`���̒�`	
sampler SourceImage : register(s0);				// �f�B�t���[�Y�}�b�v�e�N�X�`��
float4 GraphSize	: register(c22);
float4 Parameter	: register(c23);			// x:�U��, y:����*2*PI(), z:�U����, w:�g���̋t��*2*PI()

// main�֐�
PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT PSOutput;
	float2 TexturePos;

	TexturePos.x = PSInput.TexCoords0.x;
	TexturePos.y = PSInput.TexCoords0.y + Parameter.x * sin(Parameter.y * Parameter.z - PSInput.TexCoords0.x * GraphSize.x * Parameter.w) * GraphSize.w;

	// �e�N�X�`���J���[�̓ǂݍ���
	PSOutput.Color = tex2D(SourceImage, TexturePos);

	// �o�̓p�����[�^��Ԃ�
	return PSOutput;
}