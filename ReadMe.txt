=============================================

	DXライブラリ拡張パッケージ

=============================================



１．はじめに

　DXライブラリ単体では扱いにくい、入力やリソース管理、シーン制御などのゲーム作成に役立つ基盤と、
C#やVBで使用できる機能の一部分（System.Drawing.Rectangle と Delegate）を再現したライブラリです。





２．導入方法

　（１）（Visual Studio ディレクトリ）\Templates\ProjectTemplates フォルダに、
	DXLibExtendedPackage.zip をコピーします。

　（２）プロジェクトを作るとき、Visual C++ のところに、DXLibExtendedPackage が追加されているので、
	これを選びます。

　（３）プロジェクトが作成されたら、プロジェクトのプロパティを開き、追加のインクルードパスと
	追加のライブラリディレクトリに、DXライブラリの追加すべきファイルがある場所を入力します。


※ （３）以外のプロパティの設定はすでに済ませてあります。（マルチバイト文字列、ランタイムライブラリなど）

※ Debug ビルド時には、コンソールウィンドウを表示するように設定しています。不要な場合は、プロパティの
　 [リンカー] -> [システム] のサブシステムを「Windows」に変更してください。





３．クラスリファレンス

　（１）Main.h, Main.cpp

	・void AddWork(std::function<void()>), void AddWorkRef(std::function<void()>&)
	｜
	｜　非同期で行いたい処理をこの関数に渡します。
	｜
	｜　毎フレームが始まるときに登録された関数を実行し、疑似的に非同期処理を行います。
	｜　一度実行された関数は自動的に削除されます。
	｜　
	｜　使用例）
	｜	// グローバル関数に、void Something() があると仮定
	｜	AddWork(Something);
	｜
	｜	// class SomeBody にメンバ関数 void Cry() と、SomeBody のインスタンス　Foo があると仮定
	｜	AddWork(std::bind(&SomeBody::Cry, std::ref(Foo)));
	｜
	｜	// ラムダ式を追加してみる。
	｜	AddWork([]{ printf("This is a pen.\n"); });



　（２）InputManager.h

	※ Singleton パターンを実装: インスタンス化ができません。InputMgr（または、InputManager::Instance()）
	　 を使用して、メンバにアクセスしてください。

	・bool KeyPressed(int keycode)
	｜
	｜　特定のキー入力状態を取得します。
	｜
	｜　パッド入力は、デフォルトでは
	｜	・１キー	→　[Z]
	｜	・２キー	→  [X]
	｜	・十字ボタン	→  [方向キー]
	｜　に変換され、この関数でその状態を取得できます。
	｜
	｜　【引数】	CheckHitKey に用いるキー定数
	｜　【戻り値】	押されていれば true, それ以外は false


	・bool KeyPressedOnce(int kencode)
	｜
	｜　特定のキーが押された瞬間かどうかを取得します。
	｜　（そのほかは KeyPressed と同様）


	※ このほかにも、エミュレート用の関数があります。(Emulate がつく関数、_Actual がつく関数)



　（３）GameResourseManager.h

	※ Singleton パターンを実装: インスタンス化ができません。GameResourseMgr
	　（または、GameResourseManager::Instance()）を使用して、メンバにアクセスしてください。

	・画像の初期化
	｜
	｜　GameResourseManager.cpp 上部に、画像と音楽の初期化に必要な情報を入力するところがあります。
	｜　また、GameResourseManager.h 上部に、データにアクセスするための列挙体を宣言するところがあります。
	｜
	｜　なので、読み込む必要がある数だけ列挙体の値を追加し、さらに、その順番の通りに、
	｜　画像と音楽の初期化情報を追加してください。
	｜
	｜　使用例）
	｜	// GameResourseManager.h 上部
	｜	enumdef(GameImages,
	｜		Player,
	｜		Enemy
	｜	)
	｜
	｜	enumdef(GameMusics,
	｜		GM_Opening,
	｜		GM_Game
	｜
	｜	// GameResourseManager.cpp 上部
	｜	static GraphData ImageFileNames[] {
	｜		"Player.png",　// プレイヤーの画像名を指定
	｜		{ "Enemy.png", 4, 1, 4, 64, 64 }　// 敵の画像名と、画像分割情報を指定
	｜	}
	｜
	｜	static std::string MusicFileNames[] {
	｜		"Opening.wav",
	｜		"Game.wav"
	｜	}


	・int GetGameImageHandle(GameImages), int *GetGameImageHandleArray(GameImages)
	　int GetGameMusicHandle(GameMusics)
	｜
	｜　指定した画像のハンドルを取得します。
	｜
	｜　【引数】	GameImages(GameMusics) 列挙体の値
	｜　【戻り値】	画像（音楽）ハンドル（GetGameImageHandle, GetGameMusicHandle）
	｜		画像ハンドルを保有する配列へのポインタ（GetGameImageHandleArray）
	｜
	｜　画像の初期化時に、画像のファイル名だけを指定したものには前者を、
	｜　分割情報も指定した画像ハンドルにアクセスするには後者を用います。
	｜
	｜　使用例）
	｜	// 画像は、画像の初期化の例のように読み込むと仮定
	｜	int grHandle1 = GameResourseMgr.GetGameImageHandle(Player);
	｜	int grHandle2 = GameResourseMgr.GetGameImageHandleArray(Enemy)[0];



　（４）SceneBace.h

	・void Scene::OnSceneChange(SceneType)
	｜
	｜　シーンを切り替えます。
	｜