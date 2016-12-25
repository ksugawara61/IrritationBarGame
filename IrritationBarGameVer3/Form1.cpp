/* Form1.cpp */
// �w�b�_�t�@�C��
#include "stdafx.h"
#include "Form1.h"
#include <string>
#include "KinectFusion.h"
#include "IplImageEditor.h"

// ���C�u�����t�@�C��
#pragma comment(lib, "WinMM.Lib")

// ���O���
using namespace IrritationBarGameVer3;

// �}�N���̒�`
#define	GAME_IMAGE_PATH	"./item/kinectfusiondemo.bmp"
#define	POSITION_IMAGE_PATH	"./item/position.bmp"
#define	INITIALIZE_IMAGE_PATH "./item/initialize.bmp"
#define	GOAL_IMAGE_PATH		"./item/goal.png"
#define	SUCEED_MUSIC_PATH	"./item/fanfare.wav"
#define	FAILED_MUSIC_PATH	"./item/failed.wav"
#define	WIDTH	640
#define	HEIGHT	480
#define POSITION_X	50	// �����ʒu x (�e�X�g�p�ʒu1000)
#define	POSITION_Y	195	// �����ʒu y (�e�X�g�p�ʒu30)
#define	CENTER		150

// �O���[�o���ϐ�
bool			flag;			// �Q�[���J�n�t���O
bool			goal;
IplImage		*game_img;		// �Q�[���摜
IplImage		*position_img;
IplImage		*fusion_img;	// KinectFusion�摜
KinectFusion	*fusion;			// KinectFusion�C���X�^���X
IplImageEditor	editor;			// �摜�ҏW�C���X�^���X
CvPoint			pt;
RGBTRIPLE		rgb;

/**
 * �E�B���h�E�N����
 */
System::Void Form1::Form1_Load(System::Object^  sender, System::EventArgs^  e)
{
	// �ϐ��̏�����
	fusion_img = cvCreateImage(cvSize(WIDTH, HEIGHT),
		IPL_DEPTH_8U,
		1);
	rgb.rgbtRed = 255;
	rgb.rgbtGreen = 1;
	rgb.rgbtBlue = 92;

	flag = false;
	goal = false;
	
	// �s�N�`���{�b�N�X�̏�����
	if (!initPictureBox()) {
		System::Windows::Forms::MessageBox::Show("error - cannot initialize picturebox.");
	}
}

/**
 * �E�B���h�E�I����
 */
System::Void Form1::Form1_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
{
	flag = false;

	delete fusion;
}

/**
 * �X�^�[�g�{�^���N���b�N��
 */
System::Void Form1::buttonStart_Click(System::Object^  sender, System::EventArgs^  e)
{
	// �Q�[�����N�����Ă��邩
	if (flag) {
		System::Windows::Forms::MessageBox::Show("error - game is running.");
		return;
	}

	cv::Mat		temp;	// KinectFusion�摜�ꎞ�i�[
	
	// [�N���A]�{�^���Ƀt�H�[�J�X���Z�b�g
	buttonClear->Focus();

	System::Windows::Forms::MessageBox::Show("�Q�[�����J�n���܂��D");

	// KinectFusion�̏�����
	fusion = new KinectFusion;
	if (!fusion->initialize()) {
		System::Windows::Forms::MessageBox::Show("error - cannot initialize kinectfusion.");
		return;
	}

	// �Q�[���J�n
	flag = true;
	while(flag) {
		// �s�N�`���{�b�N�X��KinectFusion�摜��\��t��
		editor.setPictureBox(fusion_img, pictureFusion);

		// KinectFusion�̏���
		if (!fusion->process(temp, FALSE)) {
			System::Windows::Forms::MessageBox::Show("�L�l�N�g�𓮂��������ł��I�I\nKinect�̉摜�����������čĊJ���܂��D");
			
			delete fusion;

			// KinectFusion�̏�����
			fusion = new KinectFusion;
			if (!fusion->initialize()) {
				System::Windows::Forms::MessageBox::Show("error - cannot initialize kinectfusion.");
				return;
			}

			continue;
		}
		*fusion_img = temp;
		
		// �O�Ղ̎擾
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

		// �s�N�`���{�b�N�X�ɃQ�[���摜��\��t��
		for (int y = -2; y < 2; y++) {
			for (int x = -2; x < 2; x++) {
				CvPoint temp = cvPoint(pt.x + x, pt.y + y);
				editor.setPixelValue(game_img, temp, rgb);
			}
		}
		editor.setPictureBox(game_img, pictureGame);

		// �t�@�C���̓ǂݍ���
		position_img = cvLoadImage(POSITION_IMAGE_PATH);
		if (position_img == 0) {
			System::Windows::Forms::MessageBox::Show("error - cannot load position image.");
			break;
		}

		// �s�N�`���{�b�N�X�Ɉʒu�摜��\��t��
		for (int y = -5; y < 5; y++) {
			for (int x = -5; x < 5; x++) {
				CvPoint temp = cvPoint(CENTER + x + (mat.m41 * -500), CENTER + y + (mat.m42 * -500));
				editor.setPixelValue(position_img, temp, rgb);
			}
		}
		editor.setPictureBox(position_img, picturePosition);

		// �C�x���g�̊Ď�
		Application::DoEvents();
	}

	flag = false;

	/*// �{�N�Z����Ԃ̏�����
	if (!fusion->resetKinectFusion()) {x
		System::Windows::Forms::MessageBox::Show("error - cannot reset kinectfusion.");
	}*/

	delete fusion;

	// �s�N�`���{�b�N�X�̏�����
	if (!initPictureBox()) {
		System::Windows::Forms::MessageBox::Show("error - cannot initialize picturebox.");
	}
}

/**
 * �N���A�{�^���N���b�N��
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

	/*// �{�N�Z����Ԃ̏�����
	if (!fusion->resetKinectFusion()) {
		System::Windows::Forms::MessageBox::Show("error - cannot reset kinectfusion.");
	}*/
}

/**
 * �G���h�{�^���N���b�N��
 */
System::Void Form1::buttonEnd_Click(System::Object^  sender, System::EventArgs^  e)
{
	flag = false;

	this->Close();
}

/**
 * �@�\�F�摜������
 */
bool Form1::initPictureBox(void)
{
	// �Q�[���摜�t�@�C���̓ǂݍ���
	game_img = cvLoadImage(GAME_IMAGE_PATH);
	if (game_img == 0) {
		System::Windows::Forms::MessageBox::Show("error - cannot load game image.");
		return false;
	}

	// �����ʒu�ݒ�
	pt = cvPoint(POSITION_X, POSITION_Y);

	// �s�N�`���{�b�N�X�ɃQ�[���摜��\��t��
	for (int y = -3; y < 3; y++) {
		for (int x = -3; x < 3; x++) {
			CvPoint temp = cvPoint(pt.x + x, pt.y + y);
			editor.setPixelValue(game_img, temp, rgb);
		}
	}
	editor.setPictureBox(game_img, pictureGame);

	
	// �ʒu�摜�t�@�C���̓ǂݍ���
	position_img = cvLoadImage(POSITION_IMAGE_PATH);
	if (position_img == 0) {
		System::Windows::Forms::MessageBox::Show("error - cannot load position image.");
		return false;
	}

	// �s�N�`���{�b�N�X�Ɉʒu�摜��\��t��
	for (int y = -5; y < 5; y++) {
		for (int x = -5; x < 5; x++) {
			CvPoint temp = cvPoint(CENTER + x, CENTER + y);
			editor.setPixelValue(position_img, temp, rgb);
		}
	}
	editor.setPictureBox(position_img, picturePosition);

	// KinectFusion�摜�̏�����
	if (goal) {
		fusion_img = cvLoadImage(GOAL_IMAGE_PATH);
	}
	else {
		fusion_img = cvLoadImage(INITIALIZE_IMAGE_PATH);
	}
	if (fusion_img == 0) {
		System::Windows::Forms::MessageBox::Show("error - cannot load initialize image.");
	}

	// �s�N�`���{�b�N�X��KinectFusion�摜��\��t��
	editor.setPictureBox(fusion_img, pictureFusion);

	return true;
}

/**
 * �@�\�F�Q�[�����f�֐�
 * �@�\�����F���K���ɍ�����̂Ńo�O�����邩��
 */
 bool Form1::judgeGame(int pt_x, int pt_y)
 {
	RGBTRIPLE	judge;	// ���۔���p

	// ���۔���
	for (int y = -2; y < 2; y++) {
		for (int x = -2; x < 2; x++) {
			CvPoint temp = cvPoint(pt_x + x, pt_y + y);

			editor.getPixelValue(game_img, temp, &judge);
			if (judge.rgbtRed != 255 && judge.rgbtGreen != 255 && judge.rgbtBlue != 255) {
				// ����
				if (judge.rgbtRed == 237 && judge.rgbtGreen == 28 && judge.rgbtBlue == 36) {
					PlaySound(TEXT("./item/fanfare.wav"), NULL, (SND_ASYNC|SND_FILENAME));
					System::Windows::Forms::MessageBox::Show("�S�[�����߂łƂ��I");
					goal = true;
					return true;
				}
				// ���s
				else if (judge.rgbtRed == 0 && judge.rgbtGreen == 0 && judge.rgbtBlue == 0) {
					PlaySound(TEXT("./item/failed.wav"), NULL, (SND_ASYNC|SND_FILENAME));
					System::Windows::Forms::MessageBox::Show("���s!");
					goal = false;
					return true;
				}
			}
		}
	}

	return false;
 }