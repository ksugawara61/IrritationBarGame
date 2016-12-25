/* KinectFusion.cpp */
#include "KinectFusion.h"

/**
 * �@�\�F�R���X�g���N�^
 */
KinectFusion::KinectFusion(void) : kinect(0), volume(0), frame(0), shaded(0)
{
}

/**
 * �@�\�F�f�X�g���N�^
 */
KinectFusion::~KinectFusion(void)
{
	// Kinect�I������
	if (kinect != 0) {
		kinect->NuiShutdown();	// Kinect�C���X�^���X�̓����~
		kinect->Release();		// Kinect�C���X�^���X�̉��
	}
}

/**
 * �@�\�F�������֐�
 * �����F�Ȃ�
 *
 * �߂�l�F	true	����
 *			false	���s
 *
 * �@�\�����FKinect�̏��������s��
 */
bool KinectFusion::initialize(void)
{
	// �ڑ�Kinect���̊m�F
	int num = 0;
	NuiGetSensorCount(&num);
	if (num == 0) {
		cout << "error - cannnot find Kinect" << endl;
		return false;
	}

	// Kinect�C���X�^���X�̐���
	NuiCreateSensorByIndex(0, &kinect);

	// Kinect�̏�Ԃ��擾
	result = kinect->NuiStatus();
	if (result != S_OK) {
		cout << "error - cannot use Kinect" << endl;
		return false;
	}

	// Kinect�̏������i�[�x�J�������p�̐ݒ�j
	result = kinect->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH);
	if (result != S_OK) {
		cout <<"error - cannot initialize kinect" << endl;
		return false;
	}

	// �[�x�J�����̏������i�𑜓x��640x480�ɐݒ�j
	result = kinect->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH, NUI_IMAGE_RESOLUTION_640x480,
		0, 2, 0, &h_thread);
	if (result != S_OK) {
		cout <<"error - cannot open image stream of depth sensor" << endl;
		return false;
	}

	// 640x480�𑜓x�̉摜�T�C�Y���擾
	NuiImageResolutionToSize(NUI_IMAGE_RESOLUTION_640x480, width, height);

	// DepthFloaImage�C���X�^���X�̐���
	result = NuiFusionCreateImageFrame(NUI_FUSION_IMAGE_TYPE_FLOAT, width, height,
		nullptr, &frame);
	if (result != S_OK) {
		cout <<"error - cannot create depth float image instance" << endl;
		return false;
	}

	// �|�C���g�N���E�h�C���X�^���X�̐���
	result = NuiFusionCreateImageFrame(NUI_FUSION_IMAGE_TYPE_POINT_CLOUD, width, height,
		nullptr, &pointcloud);
	if (result != S_OK) {
		cout << "error - cannot create point cloud instance" << endl;
		return false;
	}

	// �A�e�摜�p�C���X�^���X�̐���
	result = NuiFusionCreateImageFrame(NUI_FUSION_IMAGE_TYPE_COLOR, width, height,
		nullptr, &shaded);
	if (result != S_OK) {
		cout << "error - cannot create color instance" << endl;
		return false;
	}

	// �{�N�Z����Ԃ̃p�����[�^�̐ݒ�
	NUI_FUSION_RECONSTRUCTION_PARAMETERS reconstruction;
	reconstruction.voxelsPerMeter = 128;
	reconstruction.voxelCountX = 512;
	reconstruction.voxelCountY = 512;
	reconstruction.voxelCountZ = 512;

	// �������p�s��̏�����
	reset.M11 = 1; reset.M12 = 0; reset.M13 = 0; reset.M14 = 0;
	reset.M21 = 0; reset.M22 = 1; reset.M23 = 0; reset.M24 = 0;
	reset.M31 = 0; reset.M32 = 0; reset.M33 = 1; reset.M34 = 0;
	reset.M41 = 0; reset.M42 = 0; reset.M43 = 0; reset.M44 = 1;

	// Reconstrucion Volume�C���X�^���X�̐���
	result = NuiFusionCreateReconstruction(
		&reconstruction, NUI_FUSION_RECONSTRUCTION_PROCESSOR_TYPE_AMP,  // CPU or AMP
		-1, &reset, &volume);
	if (result != S_OK) {
		cout <<"error - cannot create reconstruction volume instance" << endl;
		return false;
	}

	// Reconstruction Volume�C���X�^���X�̏�����
	result = volume->ResetReconstruction(&reset, nullptr);
	if (result != S_OK) {
		cout <<"error - cannot initialize reconstruction volume instance" << endl;
		return false;
	}

	return true;
}

/**
 * �@�\�F�����֐�
 * �����F	image	�摜
 *			mode	near���[�h
 *
 * �߂�l�F	true	����
 *			false	���s
 *
 * �@�\�����FKinectFusion�̏������s��
 */
bool KinectFusion::process(cv::Mat &image, BOOL mode)
{
	// �[�x�t���[���̎擾�i�ҋ@����INFINITE�j
	NUI_IMAGE_FRAME depth_frame = {0};
	result = kinect->NuiImageStreamGetNextFrame(h_thread, INFINITE, &depth_frame);
	if (result != S_OK) {
		System::Windows::Forms::MessageBox::Show("0");
		cout << "error - cannot get depth frame" << endl;
		return false;
	}

	// �g���[�x�f�[�^�̎擾
	INuiFrameTexture *frame_texture = 0;
	result = kinect->NuiImageFrameGetDepthImagePixelFrameTexture(h_thread,
		&depth_frame, &mode, &frame_texture);
	if (result != S_OK) {
		System::Windows::Forms::MessageBox::Show("1");
		cout << "error - cannot get depth image pixel frame texture" << endl;
		return false;
	}

	NUI_LOCKED_RECT depth_data = {0};
	result = frame_texture->LockRect(0, &depth_data, 0, 0);
	if (result != S_OK) {
		System::Windows::Forms::MessageBox::Show("2");
		cout << "error - cannot get depth data" << endl;
		return false;
	}

	// �t���[���f�[�^�̉��
	result = kinect->NuiImageStreamReleaseFrame(h_thread, &depth_frame);
	if (result != S_OK) {
		System::Windows::Forms::MessageBox::Show("3");
		cout << "error - cannot release depth frame" << endl;
		return false;
	}

	// DepthImagePixel����DepthFloatFrame�֕ϊ�
	result = NuiFusionDepthToDepthFloatFrame((NUI_DEPTH_IMAGE_PIXEL *)depth_data.pBits,
		width, height, frame, NUI_FUSION_DEFAULT_MINIMUM_DEPTH,
		NUI_FUSION_DEFAULT_MAXIMUM_DEPTH, FALSE);
	if (result != S_OK) {
		System::Windows::Forms::MessageBox::Show("4");
		cout << "error - cannot convert depth float frame" << endl;
		return false;
	}

	// �O�Ղ��擾
	result = volume->GetCurrentWorldToCameraTransform(&transform);
	if (result != S_OK) {
		System::Windows::Forms::MessageBox::Show("5");
		cout << "error - cannot get trajectory" << endl;
		return false;
	}

	// �[�x���̓����摜�̐���
	result = volume->ProcessFrame(frame, NUI_FUSION_DEFAULT_ALIGN_ITERATION_COUNT,
		NUI_FUSION_DEFAULT_INTEGRATION_WEIGHT, &transform);
	if (result != S_OK) {
		cout << "error - cannot create a integrated image" << endl;
		return false;
	}

	// �|�C���g�N���E�h�̎擾
	result = volume->CalculatePointCloud(pointcloud, &transform);
	if (result != S_OK) {
		System::Windows::Forms::MessageBox::Show("7");
		cout << "error - cannot get point cloud" << endl;
		return false;
	}

	// �|�C���g�N���E�h�̉A�e�t
	result = NuiFusionShadePointCloud(pointcloud, &transform, nullptr,
		shaded, nullptr);
	if (result != S_OK) {
		System::Windows::Forms::MessageBox::Show("8");
		cout << "error - cannot create shaded image" << endl;
		return false;
	}

	// Bitmap�֏����o��
	texture = shaded->pFrameTexture;
	result = texture->LockRect(0, &lockedrect, nullptr, 0);
	if (result != S_OK) {
		System::Windows::Forms::MessageBox::Show("9");
		cout << "error - cannot write bitmap image" << endl;
		return false;
	}

	image = cv::Mat(height, width, CV_8UC4, lockedrect.pBits);

	return true;
}

/**
 * �@�\�F�O�Վ擾�֐�
 * �����F�Ȃ�
 *
 * �߂�l�F�O�Պi�[�s��
 *
 * �@�\�����F�O�Ղ̍s����擾����
 */
matrix KinectFusion::getTrajectory(void)
{
	matrix temp;

	// radian����degree�֕ϊ�
	// ��degree�̕ϊ����ɖ�肪����p�x�̒l�ɃS�~������ꍇ����
	temp.m11 = acos(static_cast<double>(transform.M11)) * 180.0 / PI;
	temp.m21 = asin(static_cast<double>(transform.M21) * -1.0) * 180.0 / PI;
	temp.m31 = asin(static_cast<double>(transform.M31)) * 180.0 / PI;
	temp.m41 = static_cast<double>(transform.M41);

	temp.m12 = asin(static_cast<double>(transform.M12)) * 180.0 / PI;
	temp.m22 = acos(static_cast<double>(transform.M22)) * 180.0 / PI;
	temp.m32 = asin(static_cast<double>(transform.M32) * -1.0) * 180.0 / PI;
	temp.m42 = static_cast<double>(transform.M42);

	temp.m13 = asin(static_cast<double>(transform.M13) * -1.0) * 180.0 / PI;
	temp.m23 = asin(static_cast<double>(transform.M23)) * 180.0 / PI;
	temp.m33 = acos(static_cast<double>(transform.M33)) * 180.0 / PI;
	temp.m43 = static_cast<double>(transform.M43);

	temp.m14 = static_cast<double>(transform.M14);
	temp.m24 = static_cast<double>(transform.M24);
	temp.m34 = static_cast<double>(transform.M34);
	temp.m44 = static_cast<double>(transform.M44);

	return temp;
}

/**
 * �@�\�F���Z�b�g�֐�
 * �����F�Ȃ�
 *
 * �߂�l�F	true	����
 *			false	���s
 *
 * �@�\�����F�{�N�Z����Ԃ�����������
 */
bool KinectFusion::resetKinectFusion(void)
{
	// Reconstruction Volume�C���X�^���X�̏�����
	result = volume->ResetReconstruction(&reset, nullptr);
	if (result != S_OK) {
		cout <<"error - cannot initialize reconstruction volume instance" << endl;
		return false;
	}

	return true;
}
