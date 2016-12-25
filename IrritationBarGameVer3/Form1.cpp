/* Form1.cpp */
// ヘッダファイル
#include "stdafx.h"
#include "Form1.h"
#include <string>
#include "KinectFusion.h"
#include "IplImageEditor.h"

// ライブラリファイル
#pragma comment(lib, "WinMM.Lib")

// 名前空間
using namespace IrritationBarGameVer3;

// マクロの定義
#define	GAME_IMAGE_PATH	"./item/kinectfusiondemo.bmp"
#define	POSITION_IMAGE_PATH	"./item/position.bmp"
#define	INITIALIZE_IMAGE_PATH "./item/initialize.bmp"
#define	GOAL_IMAGE_PATH		"./item/goal.png"
#define	SUCEED_MUSIC_PATH	"./item/fanfare.wav"
#define	FAILED_MUSIC_PATH	"./item/failed.wav"
#define	WIDTH	640
#define	HEIGHT	480
#define POSITION_X	50	// 初期位置 x (テスト用位置1000)
#define	POSITION_Y	195	// 初期位置 y (テスト用位置30)
#define	CENTER		150

// グローバル変数
bool			flag;			// ゲーム開始フラグ
bool			goal;
IplImage		*game_img;		// ゲーム画像
IplImage		*position_img;
IplImage		*fusion_img;	// KinectFusion画像
KinectFusion	*fusion;			// KinectFusionインスタンス
IplImageEditor	editor;			// 画像編集インスタンス
CvPoint			pt;
RGBTRIPLE		rgb;

/**
 * ウィンドウ起動時
 */
System::Void Form1::Form1_Load(System::Object^  sender, System::EventArgs^  e)
{
	// 変数の初期化
	fusion_img = cvCreateImage(cvSize(WIDTH, HEIGHT),
		IPL_DEPTH_8U,
		1);
	rgb.rgbtRed = 255;
	rgb.rgbtGreen = 1;
	rgb.rgbtBlue = 92;

	flag = false;
	goal = false;
	
	// ピクチャボックスの初期化
	if (!initPictureBox()) {
		System::Windows::Forms::MessageBox::Show("error - cannot initialize picturebox.");
	}
}

/**
 * ウィンドウ終了時
 */
System::Void Form1::Form1_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
{
	flag = false;

	delete fusion;
}

/**
 * スタートボタンクリック時
 */
System::Void Form1::buttonStart_Click(System::Object^  sender, System::EventArgs^  e)
{
	// ゲームが起動しているか
	if (flag) {
		System::Windows::Forms::MessageBox::Show("error - game is running.");
		return;
	}

	cv::Mat		temp;	// KinectFusion画像一時格納
	
	// [クリア]ボタンにフォーカスをセット
	buttonClear->Focus();

	System::Windows::Forms::MessageBox::Show("ゲームを開始します．");

	// KinectFusionの初期化
	fusion = new KinectFusion;
	if (!fusion->initialize()) {
		System::Windows::Forms::MessageBox::Show("error - cannot initialize kinectfusion.");
		return;
	}

	// ゲーム開始
	flag = true;
	while(flag) {
		// ピクチャボックスにKinectFusion画像を貼り付け
		editor.setPictureBox(fusion_img, pictureFusion);

		// KinectFusionの処理
		if (!fusion->process(temp, FALSE)) {
			System::Windows::Forms::MessageBox::Show("キネクトを動かしすぎです！！\nKinectの画像を初期化して再開します．");
			
			delete fusion;

			// KinectFusionの初期化
			fusion = new KinectFusion;
			if (!fusion->initialize()) {
				System::Windows::Forms::MessageBox::Show("error - cannot initialize kinectfusion.");
				return;
			}

			continue;
		}
		*fusion_img = temp;
		
		// 軌跡の取得
		matrix mat = fusion->getTrajectory();
		if (mat.m41 > 0.2) {
			mat.m41 = 0.2;
		} else if (mat.m41 < -0.2) {
			mat.m41 = -0.2;
		}
		if (mat.m42 > 0.2) {
			mat.m42 = 0.2;
		} else if (mat.m42 < -0.2) {
			mat.m42 = -0.2;
		}

		pt.x += mat.m41 * -100 / 3;
		pt.y += mat.m42 * -100 / 3;

		if (judgeGame(pt.x, pt.y)) {
			break;
		}

		// ピクチャボックスにゲーム画像を貼り付け
		for (int y = -2; y < 2; y++) {
			for (int x = -2; x < 2; x++) {
				CvPoint temp = cvPoint(pt.x + x, pt.y + y);
				editor.setPixelValue(game_img, temp, rgb);
			}
		}
		editor.setPictureBox(game_img, pictureGame);

		// ファイルの読み込み
		position_img = cvLoadImage(POSITION_IMAGE_PATH);
		if (position_img == 0) {
			System::Windows::Forms::MessageBox::Show("error - cannot load position image.");
			break;
		}

		// ピクチャボックスに位置画像を貼り付け
		for (int y = -5; y < 5; y++) {
			for (int x = -5; x < 5; x++) {
				CvPoint temp = cvPoint(CENTER + x + (mat.m41 * -500), CENTER + y + (mat.m42 * -500));
				editor.setPixelValue(position_img, temp, rgb);
			}
		}
		editor.setPictureBox(position_img, picturePosition);

		// イベントの監視
		Application::DoEvents();
	}

	flag = false;

	/*// ボクセル空間の初期化
	if (!fusion->resetKinectFusion()) {x
		System::Windows::Forms::MessageBox::Show("error - cannot reset kinectfusion.");
	}*/

	delete fusion;

	// ピクチャボックスの初期化
	if (!initPictureBox()) {
		System::Windows::Forms::MessageBox::Show("error - cannot initialize picturebox.");
	}
}

/**
 * クリアボタンクリック時
 */
System::Void Form1::buttonClear_Click(System::Object^  sender, System::EventArgs^  e)
{
	goal = false;
	if (flag) {
		flag = false;
	}
	else {
		if (!initPictureBox()) {
			System::Windows::Forms::MessageBox::Show("error - cannot initialize picturebox.");
		}
	}

	/*// ボクセル空間の初期化
	if (!fusion->resetKinectFusion()) {
		System::Windows::Forms::MessageBox::Show("error - cannot reset kinectfusion.");
	}*/
}

/**
 * エンドボタンクリック時
 */
System::Void Form1::buttonEnd_Click(System::Object^  sender, System::EventArgs^  e)
{
	flag = false;

	this->Close();
}

/**
 * 機能：画像初期化
 */
bool Form1::initPictureBox(void)
{
	// ゲーム画像ファイルの読み込み
	game_img = cvLoadImage(GAME_IMAGE_PATH);
	if (game_img == 0) {
		System::Windows::Forms::MessageBox::Show("error - cannot load game image.");
		return false;
	}

	// 初期位置設定
	pt = cvPoint(POSITION_X, POSITION_Y);

	// ピクチャボックスにゲーム画像を貼り付け
	for (int y = -3; y < 3; y++) {
		for (int x = -3; x < 3; x++) {
			CvPoint temp = cvPoint(pt.x + x, pt.y + y);
			editor.setPixelValue(game_img, temp, rgb);
		}
	}
	editor.setPictureBox(game_img, pictureGame);

	
	// 位置画像ファイルの読み込み
	position_img = cvLoadImage(POSITION_IMAGE_PATH);
	if (position_img == 0) {
		System::Windows::Forms::MessageBox::Show("error - cannot load position image.");
		return false;
	}

	// ピクチャボックスに位置画像を貼り付け
	for (int y = -5; y < 5; y++) {
		for (int x = -5; x < 5; x++) {
			CvPoint temp = cvPoint(CENTER + x, CENTER + y);
			editor.setPixelValue(position_img, temp, rgb);
		}
	}
	editor.setPictureBox(position_img, picturePosition);

	// KinectFusion画像の初期化
	if (goal) {
		fusion_img = cvLoadImage(GOAL_IMAGE_PATH);
	}
	else {
		fusion_img = cvLoadImage(INITIALIZE_IMAGE_PATH);
	}
	if (fusion_img == 0) {
		System::Windows::Forms::MessageBox::Show("error - cannot load initialize image.");
	}

	// ピクチャボックスにKinectFusion画像を貼り付け
	editor.setPictureBox(fusion_img, pictureFusion);

	return true;
}

/**
 * 機能：ゲーム判断関数
 * 機能説明：※適当に作ったのでバグがあるかも
 */
 bool Form1::judgeGame(int pt_x, int pt_y)
 {
	RGBTRIPLE	judge;	// 成否判定用

	// 成否判定
	for (int y = -2; y < 2; y++) {
		for (int x = -2; x < 2; x++) {
			CvPoint temp = cvPoint(pt_x + x, pt_y + y);

			editor.getPixelValue(game_img, temp, &judge);
			if (judge.rgbtRed != 255 && judge.rgbtGreen != 255 && judge.rgbtBlue != 255) {
				// 成功
				if (judge.rgbtRed == 237 && judge.rgbtGreen == 28 && judge.rgbtBlue == 36) {
					PlaySound(TEXT("./item/fanfare.wav"), NULL, (SND_ASYNC|SND_FILENAME));
					System::Windows::Forms::MessageBox::Show("ゴールおめでとう！");
					goal = true;
					return true;
				}
				// 失敗
				else if (judge.rgbtRed == 0 && judge.rgbtGreen == 0 && judge.rgbtBlue == 0) {
					PlaySound(TEXT("./item/failed.wav"), NULL, (SND_ASYNC|SND_FILENAME));
					System::Windows::Forms::MessageBox::Show("失敗!");
					goal = false;
					return true;
				}
			}
		}
	}

	return false;
 }