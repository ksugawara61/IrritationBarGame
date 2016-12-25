/* KinectFusion.cpp */
#include "KinectFusion.h"

/**
 * 機能：コンストラクタ
 */
KinectFusion::KinectFusion(void) : kinect(0), volume(0), frame(0), shaded(0)
{
}

/**
 * 機能：デストラクタ
 */
KinectFusion::~KinectFusion(void)
{
	// Kinect終了処理
	if (kinect != 0) {
		kinect->NuiShutdown();	// Kinectインスタンスの動作停止
		kinect->Release();		// Kinectインスタンスの解放
	}
}

/**
 * 機能：初期化関数
 * 引数：なし
 *
 * 戻り値：	true	成功
 *			false	失敗
 *
 * 機能説明：Kinectの初期化を行う
 */
bool KinectFusion::initialize(void)
{
	// 接続Kinect数の確認
	int num = 0;
	NuiGetSensorCount(&num);
	if (num == 0) {
		cout << "error - cannnot find Kinect" << endl;
		return false;
	}

	// Kinectインスタンスの生成
	NuiCreateSensorByIndex(0, &kinect);

	// Kinectの状態を取得
	result = kinect->NuiStatus();
	if (result != S_OK) {
		cout << "error - cannot use Kinect" << endl;
		return false;
	}

	// Kinectの初期化（深度カメラ利用の設定）
	result = kinect->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH);
	if (result != S_OK) {
		cout <<"error - cannot initialize kinect" << endl;
		return false;
	}

	// 深度カメラの初期化（解像度を640x480に設定）
	result = kinect->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH, NUI_IMAGE_RESOLUTION_640x480,
		0, 2, 0, &h_thread);
	if (result != S_OK) {
		cout <<"error - cannot open image stream of depth sensor" << endl;
		return false;
	}

	// 640x480解像度の画像サイズを取得
	NuiImageResolutionToSize(NUI_IMAGE_RESOLUTION_640x480, width, height);

	// DepthFloaImageインスタンスの生成
	result = NuiFusionCreateImageFrame(NUI_FUSION_IMAGE_TYPE_FLOAT, width, height,
		nullptr, &frame);
	if (result != S_OK) {
		cout <<"error - cannot create depth float image instance" << endl;
		return false;
	}

	// ポイントクラウドインスタンスの生成
	result = NuiFusionCreateImageFrame(NUI_FUSION_IMAGE_TYPE_POINT_CLOUD, width, height,
		nullptr, &pointcloud);
	if (result != S_OK) {
		cout << "error - cannot create point cloud instance" << endl;
		return false;
	}

	// 陰影画像用インスタンスの生成
	result = NuiFusionCreateImageFrame(NUI_FUSION_IMAGE_TYPE_COLOR, width, height,
		nullptr, &shaded);
	if (result != S_OK) {
		cout << "error - cannot create color instance" << endl;
		return false;
	}

	// ボクセル空間のパラメータの設定
	NUI_FUSION_RECONSTRUCTION_PARAMETERS reconstruction;
	reconstruction.voxelsPerMeter = 128;
	reconstruction.voxelCountX = 512;
	reconstruction.voxelCountY = 512;
	reconstruction.voxelCountZ = 512;

	// 初期化用行列の初期化
	reset.M11 = 1; reset.M12 = 0; reset.M13 = 0; reset.M14 = 0;
	reset.M21 = 0; reset.M22 = 1; reset.M23 = 0; reset.M24 = 0;
	reset.M31 = 0; reset.M32 = 0; reset.M33 = 1; reset.M34 = 0;
	reset.M41 = 0; reset.M42 = 0; reset.M43 = 0; reset.M44 = 1;

	// Reconstrucion Volumeインスタンスの生成
	result = NuiFusionCreateReconstruction(
		&reconstruction, NUI_FUSION_RECONSTRUCTION_PROCESSOR_TYPE_AMP,  // CPU or AMP
		-1, &reset, &volume);
	if (result != S_OK) {
		cout <<"error - cannot create reconstruction volume instance" << endl;
		return false;
	}

	// Reconstruction Volumeインスタンスの初期化
	result = volume->ResetReconstruction(&reset, nullptr);
	if (result != S_OK) {
		cout <<"error - cannot initialize reconstruction volume instance" << endl;
		return false;
	}

	return true;
}

/**
 * 機能：処理関数
 * 引数：	image	画像
 *			mode	nearモード
 *
 * 戻り値：	true	成功
 *			false	失敗
 *
 * 機能説明：KinectFusionの処理を行う
 */
bool KinectFusion::process(cv::Mat &image, BOOL mode)
{
	// 深度フレームの取得（待機時間INFINITE）
	NUI_IMAGE_FRAME depth_frame = {0};
	result = kinect->NuiImageStreamGetNextFrame(h_thread, INFINITE, &depth_frame);
	if (result != S_OK) {
		System::Windows::Forms::MessageBox::Show("0");
		cout << "error - cannot get depth frame" << endl;
		return false;
	}

	// 拡張深度データの取得
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

	// フレームデータの解放
	result = kinect->NuiImageStreamReleaseFrame(h_thread, &depth_frame);
	if (result != S_OK) {
		System::Windows::Forms::MessageBox::Show("3");
		cout << "error - cannot release depth frame" << endl;
		return false;
	}

	// DepthImagePixelからDepthFloatFrameへ変換
	result = NuiFusionDepthToDepthFloatFrame((NUI_DEPTH_IMAGE_PIXEL *)depth_data.pBits,
		width, height, frame, NUI_FUSION_DEFAULT_MINIMUM_DEPTH,
		NUI_FUSION_DEFAULT_MAXIMUM_DEPTH, FALSE);
	if (result != S_OK) {
		System::Windows::Forms::MessageBox::Show("4");
		cout << "error - cannot convert depth float frame" << endl;
		return false;
	}

	// 軌跡を取得
	result = volume->GetCurrentWorldToCameraTransform(&transform);
	if (result != S_OK) {
		System::Windows::Forms::MessageBox::Show("5");
		cout << "error - cannot get trajectory" << endl;
		return false;
	}

	// 深度情報の統合画像の生成
	result = volume->ProcessFrame(frame, NUI_FUSION_DEFAULT_ALIGN_ITERATION_COUNT,
		NUI_FUSION_DEFAULT_INTEGRATION_WEIGHT, &transform);
	if (result != S_OK) {
		cout << "error - cannot create a integrated image" << endl;
		return false;
	}

	// ポイントクラウドの取得
	result = volume->CalculatePointCloud(pointcloud, &transform);
	if (result != S_OK) {
		System::Windows::Forms::MessageBox::Show("7");
		cout << "error - cannot get point cloud" << endl;
		return false;
	}

	// ポイントクラウドの陰影付
	result = NuiFusionShadePointCloud(pointcloud, &transform, nullptr,
		shaded, nullptr);
	if (result != S_OK) {
		System::Windows::Forms::MessageBox::Show("8");
		cout << "error - cannot create shaded image" << endl;
		return false;
	}

	// Bitmapへ書き出す
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
 * 機能：軌跡取得関数
 * 引数：なし
 *
 * 戻り値：軌跡格納行列
 *
 * 機能説明：軌跡の行列を取得する
 */
matrix KinectFusion::getTrajectory(void)
{
	matrix temp;

	// radianからdegreeへ変換
	// ※degreeの変換式に問題があり角度の値にゴミが入る場合あり
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
 * 機能：リセット関数
 * 引数：なし
 *
 * 戻り値：	true	成功
 *			false	失敗
 *
 * 機能説明：ボクセル空間を初期化する
 */
bool KinectFusion::resetKinectFusion(void)
{
	// Reconstruction Volumeインスタンスの初期化
	result = volume->ResetReconstruction(&reset, nullptr);
	if (result != S_OK) {
		cout <<"error - cannot initialize reconstruction volume instance" << endl;
		return false;
	}

	return true;
}
