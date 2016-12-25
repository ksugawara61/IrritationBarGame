/* IplImageEditor.h */
#ifndef	IPLIMAGEEDITOR_H
#define	IPLIMAGEEDITOR_H

// インクルードファイル
#include <Windows.h>
#include <opencv2\opencv.hpp>

// 名前空間
using namespace System;
using namespace System::Drawing;

// ライブラリファイル
#ifdef _DEBUG
    //Debugモードの場合
    #pragma comment(lib,"opencv_core245d.lib")
    #pragma comment(lib,"opencv_imgproc245d.lib")
    #pragma comment(lib,"opencv_highgui245d.lib")
    #pragma comment(lib,"opencv_objdetect245d.lib")
    #pragma comment(lib,"opencv_contrib245d.lib")
    #pragma comment(lib,"opencv_features2d245d.lib")
    #pragma comment(lib,"opencv_flann245d.lib")
    #pragma comment(lib,"opencv_gpu245d.lib")
    //#pragma comment(lib,"opencv_haartraining_engined.lib")
    #pragma comment(lib,"opencv_legacy245d.lib")
    #pragma comment(lib,"opencv_ts245d.lib")
    #pragma comment(lib,"opencv_video245d.lib")
#else
    //Releaseモードの場合
    #pragma comment(lib,"opencv_core245.lib")
    #pragma comment(lib,"opencv_imgproc245.lib")
    #pragma comment(lib,"opencv_highgui245.lib")
    #pragma comment(lib,"opencv_objdetect245.lib")
    #pragma comment(lib,"opencv_contrib245.lib")
    #pragma comment(lib,"opencv_features2d245.lib")
    #pragma comment(lib,"opencv_flann245.lib")
    #pragma comment(lib,"opencv_gpu245.lib")
    //#pragma comment(lib,"opencv_haartraining_engined.lib")
    #pragma comment(lib,"opencv_legacy245.lib")
    #pragma comment(lib,"opencv_ts245.lib")
    #pragma comment(lib,"opencv_video245.lib")
#endif

class IplImageEditor	// IplImage編集クラス
{
public:
	void setPictureBox(IplImage *img, System::Windows::Forms::PictureBox^ box);
	void setPixelValue(IplImage *img, CvPoint pt, RGBTRIPLE rgb);
	void getPixelValue(IplImage *img, CvPoint pt, RGBTRIPLE *rgb);
};

#endif