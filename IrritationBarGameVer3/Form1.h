#pragma once

namespace IrritationBarGameVer3 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Form1 の概要
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: ここにコンストラクター コードを追加します
			//
		}

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureGame;
	protected: 
	private: System::Windows::Forms::PictureBox^  pictureFusion;
	private: System::Windows::Forms::PictureBox^  picturePosition;
	private: System::Windows::Forms::Button^  buttonStart;
	private: System::Windows::Forms::Button^  buttonClear;
	private: System::Windows::Forms::Button^  buttonEnd;

	private:
		/// <summary>
		/// 必要なデザイナー変数です。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->pictureGame = (gcnew System::Windows::Forms::PictureBox());
			this->pictureFusion = (gcnew System::Windows::Forms::PictureBox());
			this->picturePosition = (gcnew System::Windows::Forms::PictureBox());
			this->buttonStart = (gcnew System::Windows::Forms::Button());
			this->buttonClear = (gcnew System::Windows::Forms::Button());
			this->buttonEnd = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureGame))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureFusion))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->picturePosition))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureGame
			// 
			this->pictureGame->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureGame->Location = System::Drawing::Point(10, 8);
			this->pictureGame->Name = L"pictureGame";
			this->pictureGame->Size = System::Drawing::Size(1100, 230);
			this->pictureGame->TabIndex = 0;
			this->pictureGame->TabStop = false;
			// 
			// pictureFusion
			// 
			this->pictureFusion->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureFusion->Location = System::Drawing::Point(470, 244);
			this->pictureFusion->Name = L"pictureFusion";
			this->pictureFusion->Size = System::Drawing::Size(640, 480);
			this->pictureFusion->TabIndex = 1;
			this->pictureFusion->TabStop = false;
			// 
			// picturePosition
			// 
			this->picturePosition->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->picturePosition->Location = System::Drawing::Point(10, 244);
			this->picturePosition->Name = L"picturePosition";
			this->picturePosition->Size = System::Drawing::Size(300, 300);
			this->picturePosition->TabIndex = 2;
			this->picturePosition->TabStop = false;
			// 
			// buttonStart
			// 
			this->buttonStart->Location = System::Drawing::Point(10, 605);
			this->buttonStart->Name = L"buttonStart";
			this->buttonStart->Size = System::Drawing::Size(100, 25);
			this->buttonStart->TabIndex = 3;
			this->buttonStart->Text = L"start";
			this->buttonStart->UseVisualStyleBackColor = true;
			this->buttonStart->Click += gcnew System::EventHandler(this, &Form1::buttonStart_Click);
			// 
			// buttonClear
			// 
			this->buttonClear->Location = System::Drawing::Point(116, 605);
			this->buttonClear->Name = L"buttonClear";
			this->buttonClear->Size = System::Drawing::Size(100, 25);
			this->buttonClear->TabIndex = 4;
			this->buttonClear->Text = L"clear";
			this->buttonClear->UseVisualStyleBackColor = true;
			this->buttonClear->Click += gcnew System::EventHandler(this, &Form1::buttonClear_Click);
			// 
			// buttonEnd
			// 
			this->buttonEnd->Location = System::Drawing::Point(222, 605);
			this->buttonEnd->Name = L"buttonEnd";
			this->buttonEnd->Size = System::Drawing::Size(100, 25);
			this->buttonEnd->TabIndex = 5;
			this->buttonEnd->Text = L"end";
			this->buttonEnd->UseVisualStyleBackColor = true;
			this->buttonEnd->Click += gcnew System::EventHandler(this, &Form1::buttonEnd_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1350, 730);
			this->Controls->Add(this->buttonEnd);
			this->Controls->Add(this->buttonClear);
			this->Controls->Add(this->buttonStart);
			this->Controls->Add(this->picturePosition);
			this->Controls->Add(this->pictureFusion);
			this->Controls->Add(this->pictureGame);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureGame))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureFusion))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->picturePosition))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e);
	private: System::Void Form1_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e);
	private: System::Void buttonStart_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void buttonClear_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void buttonEnd_Click(System::Object^  sender, System::EventArgs^  e);

			 bool initPictureBox(void);
			 bool judgeGame(int pt_x, int pt_y);
};
}

