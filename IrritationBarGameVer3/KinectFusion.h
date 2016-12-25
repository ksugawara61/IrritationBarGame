/* KinectFusion.h */
#ifndef	KINECTFUSION_H
#define	KINECTFUSION_H

// �C���N���[�h�t�@�C��
#include <math.h>
#include <iostream>
#include <Windows.h>
#include <NuiApi.h>
#include <NuiKinectFusionApi.h>
#include <opencv2\opencv.hpp>

// ���O���
using namespace std;

// ���C�u�����t�@�C��
#pragma comment(lib, "Kinect10.lib")
#pragma comment(lib, "KinectFusion180_64.lib")

#ifdef _DEBUG
    //Debug���[�h�̏ꍇ
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
    //Release���[�h�̏ꍇ
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

/* �}�N���̒�` */
#define	PI	3.14159265358979	// �~����

/* �\���̂̐錾 */
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

class KinectFusion	// KinectFusion�N���X
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
	HANDLE		h_thread;	// �n���h��
	HRESULT		result;		// �G���[�`�F�b�N
	DWORD		width;		// �摜��
	DWORD		height;		// �摜����

	INuiFusionReconstruction	*volume;	// �{�N�Z�����

	NUI_FUSION_IMAGE_FRAME		*frame;		// DepthFloatFrame�ϊ��p
	NUI_FUSION_IMAGE_FRAME		*pointcloud;	// �|�C���g�N���E�h�p
	NUI_FUSION_IMAGE_FRAME		*shaded;	// �A�e�摜�p

	INuiFrameTexture			*texture;
	NUI_LOCKED_RECT				lockedrect;

	Matrix4		reset;		// �������p�s��
	Matrix4		transform;	// �O�Պi�[�p�s��
};

#endif