=============================================

	DX���C�u�����g���p�b�P�[�W

=============================================



�P�D�͂��߂�

�@DX���C�u�����P�̂ł͈����ɂ����A���͂⃊�\�[�X�Ǘ��A�V�[������Ȃǂ̃Q�[���쐬�ɖ𗧂�ՂƁA
C#��VB�Ŏg�p�ł���@�\�̈ꕔ���iSystem.Drawing.Rectangle �� Delegate�j���Č��������C�u�����ł��B





�Q�D�������@

�@�i�P�j�iVisual Studio �f�B���N�g���j\Templates\ProjectTemplates �t�H���_�ɁA
	DXLibExtendedPackage.zip ���R�s�[���܂��B

�@�i�Q�j�v���W�F�N�g�����Ƃ��AVisual C++ �̂Ƃ���ɁADXLibExtendedPackage ���ǉ�����Ă���̂ŁA
	�����I�т܂��B

�@�i�R�j�v���W�F�N�g���쐬���ꂽ��A�v���W�F�N�g�̃v���p�e�B���J���A�ǉ��̃C���N���[�h�p�X��
	�ǉ��̃��C�u�����f�B���N�g���ɁADX���C�u�����̒ǉ����ׂ��t�@�C��������ꏊ����͂��܂��B


�� �i�R�j�ȊO�̃v���p�e�B�̐ݒ�͂��łɍς܂��Ă���܂��B�i�}���`�o�C�g������A�����^�C�����C�u�����Ȃǁj

�� Debug �r���h���ɂ́A�R���\�[���E�B���h�E��\������悤�ɐݒ肵�Ă��܂��B�s�v�ȏꍇ�́A�v���p�e�B��
�@ [�����J�[] -> [�V�X�e��] �̃T�u�V�X�e�����uWindows�v�ɕύX���Ă��������B





�R�D�N���X���t�@�����X

�@�i�P�jMain.h, Main.cpp

	�Evoid AddWork(std::function<void()>), void AddWorkRef(std::function<void()>&)
	�b
	�b�@�񓯊��ōs���������������̊֐��ɓn���܂��B
	�b
	�b�@���t���[�����n�܂�Ƃ��ɓo�^���ꂽ�֐������s���A�^���I�ɔ񓯊��������s���܂��B
	�b�@��x���s���ꂽ�֐��͎����I�ɍ폜����܂��B
	�b�@
	�b�@�g�p��j
	�b	// �O���[�o���֐��ɁAvoid Something() ������Ɖ���
	�b	AddWork(Something);
	�b
	�b	// class SomeBody �Ƀ����o�֐� void Cry() �ƁASomeBody �̃C���X�^���X�@Foo ������Ɖ���
	�b	AddWork(std::bind(&SomeBody::Cry, std::ref(Foo)));
	�b
	�b	// �����_����ǉ����Ă݂�B
	�b	AddWork([]{ printf("This is a pen.\n"); });



�@�i�Q�jInputManager.h

	�� Singleton �p�^�[��������: �C���X�^���X�����ł��܂���BInputMgr�i�܂��́AInputManager::Instance()�j
	�@ ���g�p���āA�����o�ɃA�N�Z�X���Ă��������B

	�Ebool KeyPressed(int keycode)
	�b
	�b�@����̃L�[���͏�Ԃ��擾���܂��B
	�b
	�b�@�p�b�h���͂́A�f�t�H���g�ł�
	�b	�E�P�L�[	���@[Z]
	�b	�E�Q�L�[	��  [X]
	�b	�E�\���{�^��	��  [�����L�[]
	�b�@�ɕϊ�����A���̊֐��ł��̏�Ԃ��擾�ł��܂��B
	�b
	�b�@�y�����z	CheckHitKey �ɗp����L�[�萔
	�b�@�y�߂�l�z	������Ă���� true, ����ȊO�� false


	�Ebool KeyPressedOnce(int kencode)
	�b
	�b�@����̃L�[�������ꂽ�u�Ԃ��ǂ������擾���܂��B
	�b�@�i���̂ق��� KeyPressed �Ɠ��l�j


	�� ���̂ق��ɂ��A�G�~�����[�g�p�̊֐�������܂��B(Emulate �����֐��A_Actual �����֐�)



�@�i�R�jGameResourseManager.h

	�� Singleton �p�^�[��������: �C���X�^���X�����ł��܂���BGameResourseMgr
	�@�i�܂��́AGameResourseManager::Instance()�j���g�p���āA�����o�ɃA�N�Z�X���Ă��������B

	�E�摜�̏�����
	�b
	�b�@GameResourseManager.cpp �㕔�ɁA�摜�Ɖ��y�̏������ɕK�v�ȏ�����͂���Ƃ��낪����܂��B
	�b�@�܂��AGameResourseManager.h �㕔�ɁA�f�[�^�ɃA�N�Z�X���邽�߂̗񋓑̂�錾����Ƃ��낪����܂��B
	�b
	�b�@�Ȃ̂ŁA�ǂݍ��ޕK�v�����鐔�����񋓑̂̒l��ǉ����A����ɁA���̏��Ԃ̒ʂ�ɁA
	�b�@�摜�Ɖ��y�̏���������ǉ����Ă��������B
	�b
	�b�@�g�p��j
	�b	// GameResourseManager.h �㕔
	�b	enumdef(GameImages,
	�b		Player,
	�b		Enemy
	�b	)
	�b
	�b	enumdef(GameMusics,
	�b		GM_Opening,
	�b		GM_Game
	�b
	�b	// GameResourseManager.cpp �㕔
	�b	static GraphData ImageFileNames[] {
	�b		"Player.png",�@// �v���C���[�̉摜�����w��
	�b		{ "Enemy.png", 4, 1, 4, 64, 64 }�@// �G�̉摜���ƁA�摜���������w��
	�b	}
	�b
	�b	static std::string MusicFileNames[] {
	�b		"Opening.wav",
	�b		"Game.wav"
	�b	}


	�Eint GetGameImageHandle(GameImages), int *GetGameImageHandleArray(GameImages)
	�@int GetGameMusicHandle(GameMusics)
	�b
	�b�@�w�肵���摜�̃n���h�����擾���܂��B
	�b
	�b�@�y�����z	GameImages(GameMusics) �񋓑̂̒l
	�b�@�y�߂�l�z	�摜�i���y�j�n���h���iGetGameImageHandle, GetGameMusicHandle�j
	�b		�摜�n���h����ۗL����z��ւ̃|�C���^�iGetGameImageHandleArray�j
	�b
	�b�@�摜�̏��������ɁA�摜�̃t�@�C�����������w�肵�����̂ɂ͑O�҂��A
	�b�@���������w�肵���摜�n���h���ɃA�N�Z�X����ɂ͌�҂�p���܂��B
	�b
	�b�@�g�p��j
	�b	// �摜�́A�摜�̏������̗�̂悤�ɓǂݍ��ނƉ���
	�b	int grHandle1 = GameResourseMgr.GetGameImageHandle(Player);
	�b	int grHandle2 = GameResourseMgr.GetGameImageHandleArray(Enemy)[0];



�@�i�S�jSceneBace.h

	�Evoid Scene::OnSceneChange(SceneType)
	�b
	�b�@�V�[����؂�ւ��܂��B
	�b