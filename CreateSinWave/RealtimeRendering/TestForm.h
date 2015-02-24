#pragma once

namespace RealtimeRendering {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	using namespace SlimDX::XAudio2;
	using namespace SlimDX::Multimedia;

	/// <summary>
	/// TestForm の概要
	/// </summary>
	public ref class TestForm : public System::Windows::Forms::Form
	{
	public:
		TestForm(void)
		{
			InitializeComponent();
			//
			//TODO: ここにコンストラクター コードを追加します
			//

			//XAudio2の初期化
		}

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~TestForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	protected:

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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(98, 121);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &TestForm::button1_Click);
			// 
			// TestForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 261);
			this->Controls->Add(this->button1);
			this->Name = L"TestForm";
			this->Text = L"TestForm";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &TestForm::TestForm_FormClosing);
			this->Load += gcnew System::EventHandler(this, &TestForm::TestForm_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	public:
		XAudio2 ^device;
		MasteringVoice ^xMVoice;
		WaveFormat ^format;
		AudioBuffer ^xBuf;
		IO::MemoryStream ^mStream;
		SourceVoice ^xSrcVoice;
	private: 
		System::Void button1_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void TestForm_Load(System::Object^  sender, System::EventArgs^  e);
		System::Void TestForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e);
	};
}
#include <Windows.h>

#define PI		3.1415926535f

using namespace RealtimeRendering;
using namespace std;