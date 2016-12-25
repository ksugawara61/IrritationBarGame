/* IplImageEditor.cpp */
#include "IplImageEditor.h"

/**
 * �@�\�F�s�N�`���{�b�N�X�\�t�֐�
 * �����F	img	�摜
 *			box		�s�N�`���{�b�N�X
 *
 * �߂�l�F	�Ȃ�
 *
 * �@�\�����F�s�N�`���{�b�N�X�ɉ摜��\��t��
 */
void IplImageEditor::setPictureBox(IplImage *img, System::Windows::Forms::PictureBox^ box)
{
	// Bitmap�ɒ���img->imageData��ǂݍ��܂���ƁC�G���[�ɂȂ邽�߃R�s�[�f�[�^��n��
	IntPtr ip(new unsigned char[img->widthStep * img->height]);
	memcpy(ip.ToPointer(), img->imageData, img->widthStep * img->height);
	Bitmap^ bmp = gcnew Bitmap(img->width, img->height, img->widthStep,
		System::Drawing::Imaging::PixelFormat::Format24bppRgb, ip);

	// PictureBox��Bitmap���Z�b�g
	box->Image = bmp;
}

/**
 * �@�\�FRGB�\�t�֐�
 * �����F	img	�\�t�摜
 *			pt	�s�N�Z�����W
 *			rgb	RGB�l
 *
 * �߂�l�F	�Ȃ�
 *
 * �@�\�����F�摜��RGB��\�t
 */
void IplImageEditor::setPixelValue(IplImage *img, CvPoint pt, RGBTRIPLE rgb)
{
	((uchar *)(img->imageData + img->widthStep * pt.y))[pt.x * 3 + 2] = rgb.rgbtRed;
	((uchar *)(img->imageData + img->widthStep * pt.y))[pt.x * 3 + 1] = rgb.rgbtGreen;
	((uchar *)(img->imageData + img->widthStep * pt.y))[pt.x * 3 + 0] = rgb.rgbtBlue;
}

/**
 * �@�\�FRGB�Q�Ɗ֐�
 * �����F	img	�Q�Ɖ摜
 *			pt	�s�N�Z�����W
 *			rgb	�Q��RGB�i�[
 *
 * �߂�l�F	�Ȃ�
 *
 * �@�\�����FRGB�l���Q��
 */
void IplImageEditor::getPixelValue(IplImage *img, CvPoint pt, RGBTRIPLE *rgb)
{
	rgb->rgbtRed = ((uchar *)(img->imageData + img->widthStep * pt.y))[pt.x * 3 + 2];
	rgb->rgbtGreen = ((uchar *)(img->imageData + img->widthStep * pt.y))[pt.x * 3 + 1];
	rgb->rgbtBlue = ((uchar *)(img->imageData + img->widthStep * pt.y))[pt.x * 3 + 0];
}