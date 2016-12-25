/* IplImageEditor.cpp */
#include "IplImageEditor.h"

/**
 * 機能：ピクチャボックス貼付関数
 * 引数：	img	画像
 *			box		ピクチャボックス
 *
 * 戻り値：	なし
 *
 * 機能説明：ピクチャボックスに画像を貼り付け
 */
void IplImageEditor::setPictureBox(IplImage *img, System::Windows::Forms::PictureBox^ box)
{
	// Bitmapに直接img->imageDataを読み込ませると，エラーになるためコピーデータを渡す
	IntPtr ip(new unsigned char[img->widthStep * img->height]);
	memcpy(ip.ToPointer(), img->imageData, img->widthStep * img->height);
	Bitmap^ bmp = gcnew Bitmap(img->width, img->height, img->widthStep,
		System::Drawing::Imaging::PixelFormat::Format24bppRgb, ip);

	// PictureBoxにBitmapをセット
	box->Image = bmp;
}

/**
 * 機能：RGB貼付関数
 * 引数：	img	貼付画像
 *			pt	ピクセル座標
 *			rgb	RGB値
 *
 * 戻り値：	なし
 *
 * 機能説明：画像にRGBを貼付
 */
void IplImageEditor::setPixelValue(IplImage *img, CvPoint pt, RGBTRIPLE rgb)
{
	((uchar *)(img->imageData + img->widthStep * pt.y))[pt.x * 3 + 2] = rgb.rgbtRed;
	((uchar *)(img->imageData + img->widthStep * pt.y))[pt.x * 3 + 1] = rgb.rgbtGreen;
	((uchar *)(img->imageData + img->widthStep * pt.y))[pt.x * 3 + 0] = rgb.rgbtBlue;
}

/**
 * 機能：RGB参照関数
 * 引数：	img	参照画像
 *			pt	ピクセル座標
 *			rgb	参照RGB格納
 *
 * 戻り値：	なし
 *
 * 機能説明：RGB値を参照
 */
void IplImageEditor::getPixelValue(IplImage *img, CvPoint pt, RGBTRIPLE *rgb)
{
	rgb->rgbtRed = ((uchar *)(img->imageData + img->widthStep * pt.y))[pt.x * 3 + 2];
	rgb->rgbtGreen = ((uchar *)(img->imageData + img->widthStep * pt.y))[pt.x * 3 + 1];
	rgb->rgbtBlue = ((uchar *)(img->imageData + img->widthStep * pt.y))[pt.x * 3 + 0];
}