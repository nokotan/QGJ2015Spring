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

// main�֐�
PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT PSOutput;
	float2 TexturePos;

	TexturePos.x = (floor(PSInput.TexCoords0.x * GraphSize.x) + .5f) * GraphSize.z;
	TexturePos.y = (floor(PSInput.TexCoords0.y * GraphSize.y) + .5f) * GraphSize.w;

	// �e�N�X�`���J���[�̓ǂݍ���
	PSOutput.Color = tex2D(SourceImage, TexturePos);

	// �o�̓p�����[�^��Ԃ�
	return PSOutput;
}