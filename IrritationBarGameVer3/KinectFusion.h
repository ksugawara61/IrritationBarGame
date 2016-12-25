/* KinectFusion.h */
#ifndef	KINECTFUSION_H
#define	KINECTFUSION_H

// インクルードファイル
#include <math.h>
#include <iostream>
#include <Windows.h>
#include <NuiApi.h>
#include <NuiKinectFusionApi.h>
#include <opencv2\opencv.hpp>

// 名前空間
using namespace std;

// ライブラリファイル
#pragma comment(lib, "Kinect10.lib")
#pragma comment(lib, "KinectFusion180_64.lib")

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

/* マクロの定義 */
#define	PI	3.14159265358979	// 円周率

/* 構造体の宣言 */
struct matrix {
	double m11;
	double m12;
	double m13;
	double m14;

	double m21;
	double m22;
	double m23;
	double m24;

	double m31;
	double m32;
	double m33;
	double m34;

	double m41;
	double m42;
	double m43;
	double m44;
};

class KinectFusion	// KinectFusionクラス
{
public:
	KinectFusion(void);
	~KinectFusion(void);
	bool	initialize(void);
	bool	process(cv::Mat &image, BOOL mode);
	matrix	getTrajectory(void);
	bool	resetKinectFusion(void);

private:
	INuiSensor	*kinect;	// Kinect
	HANDLE		h_thread;	// ハンドラ
	HRESULT		result;		// エラーチェック
	DWORD		width;		// 画像幅
	DWORD		height;		// 画像高さ

	INuiFusionReconstruction	*volume;	// ボクセル空間

	NUI_FUSION_IMAGE_FRAME		*frame;		// DepthFloatFrame変換用
	NUI_FUSION_IMAGE_FRAME		*pointcloud;	// ポイントクラウド用
	NUI_FUSION_IMAGE_FRAME		*shaded;	// 陰影画像用

	INuiFrameTexture			*texture;
	NUI_LOCKED_RECT				lockedrect;

	Matrix4		reset;		// 初期化用行列
	Matrix4		transform;	// 軌跡格納用行列
};

#endif