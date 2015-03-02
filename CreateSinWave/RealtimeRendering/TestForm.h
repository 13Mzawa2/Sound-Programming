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
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  waveGraphics;
	private: System::Windows::Forms::VScrollBar^  vScrollBarMax;
	private: System::Windows::Forms::VScrollBar^  vScrollBar10Hz;
	private: System::Windows::Forms::VScrollBar^  vScrollBar16Hz;
	private: System::Windows::Forms::VScrollBar^  vScrollBar25Hz;
	private: System::Windows::Forms::VScrollBar^  vScrollBar40Hz;
	private: System::Windows::Forms::VScrollBar^  vScrollBar63Hz;
	private: System::Windows::Forms::VScrollBar^  vScrollBar100Hz;
	private: System::Windows::Forms::VScrollBar^  vScrollBar158Hz;
	private: System::Windows::Forms::VScrollBar^  vScrollBar251Hz;
	private: System::Windows::Forms::VScrollBar^  vScrollBar398Hz;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label6;

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
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->waveGraphics = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->vScrollBarMax = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar10Hz = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar16Hz = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar25Hz = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar40Hz = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar63Hz = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar100Hz = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar158Hz = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar251Hz = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar398Hz = (gcnew System::Windows::Forms::VScrollBar());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->waveGraphics))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(12, 12);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Start";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &TestForm::button1_Click);
			// 
			// waveGraphics
			// 
			chartArea1->Name = L"ChartArea1";
			this->waveGraphics->ChartAreas->Add(chartArea1);
			legend1->Name = L"Legend1";
			this->waveGraphics->Legends->Add(legend1);
			this->waveGraphics->Location = System::Drawing::Point(12, 144);
			this->waveGraphics->Name = L"waveGraphics";
			series1->ChartArea = L"ChartArea1";
			series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series1->Legend = L"Legend1";
			series1->Name = L"wavedata";
			series1->YValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::Double;
			this->waveGraphics->Series->Add(series1);
			this->waveGraphics->Size = System::Drawing::Size(464, 164);
			this->waveGraphics->TabIndex = 1;
			this->waveGraphics->Text = L"WaveGraphics";
			// 
			// vScrollBarMax
			// 
			this->vScrollBarMax->AccessibleRole = System::Windows::Forms::AccessibleRole::None;
			this->vScrollBarMax->Location = System::Drawing::Point(130, 12);
			this->vScrollBarMax->Maximum = 109;
			this->vScrollBarMax->Name = L"vScrollBarMax";
			this->vScrollBarMax->Size = System::Drawing::Size(14, 105);
			this->vScrollBarMax->TabIndex = 2;
			this->vScrollBarMax->Value = 100;
			this->vScrollBarMax->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &TestForm::vScrollBarMax_Scroll);
			// 
			// vScrollBar10Hz
			// 
			this->vScrollBar10Hz->Location = System::Drawing::Point(176, 12);
			this->vScrollBar10Hz->Maximum = 109;
			this->vScrollBar10Hz->Name = L"vScrollBar10Hz";
			this->vScrollBar10Hz->Size = System::Drawing::Size(14, 105);
			this->vScrollBar10Hz->TabIndex = 3;
			this->vScrollBar10Hz->Value = 100;
			this->vScrollBar10Hz->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &TestForm::vScrollBar10Hz_Scroll);
			// 
			// vScrollBar16Hz
			// 
			this->vScrollBar16Hz->Location = System::Drawing::Point(190, 12);
			this->vScrollBar16Hz->Maximum = 109;
			this->vScrollBar16Hz->Name = L"vScrollBar16Hz";
			this->vScrollBar16Hz->Size = System::Drawing::Size(14, 105);
			this->vScrollBar16Hz->TabIndex = 4;
			this->vScrollBar16Hz->Value = 100;
			this->vScrollBar16Hz->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &TestForm::vScrollBar16Hz_Scroll);
			// 
			// vScrollBar25Hz
			// 
			this->vScrollBar25Hz->Location = System::Drawing::Point(204, 12);
			this->vScrollBar25Hz->Maximum = 109;
			this->vScrollBar25Hz->Name = L"vScrollBar25Hz";
			this->vScrollBar25Hz->Size = System::Drawing::Size(14, 105);
			this->vScrollBar25Hz->TabIndex = 5;
			this->vScrollBar25Hz->Value = 100;
			this->vScrollBar25Hz->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &TestForm::vScrollBar25Hz_Scroll);
			// 
			// vScrollBar40Hz
			// 
			this->vScrollBar40Hz->Location = System::Drawing::Point(218, 12);
			this->vScrollBar40Hz->Maximum = 109;
			this->vScrollBar40Hz->Name = L"vScrollBar40Hz";
			this->vScrollBar40Hz->Size = System::Drawing::Size(14, 105);
			this->vScrollBar40Hz->TabIndex = 6;
			this->vScrollBar40Hz->Value = 100;
			this->vScrollBar40Hz->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &TestForm::vScrollBar40Hz_Scroll);
			// 
			// vScrollBar63Hz
			// 
			this->vScrollBar63Hz->Location = System::Drawing::Point(232, 12);
			this->vScrollBar63Hz->Maximum = 109;
			this->vScrollBar63Hz->Name = L"vScrollBar63Hz";
			this->vScrollBar63Hz->Size = System::Drawing::Size(14, 105);
			this->vScrollBar63Hz->TabIndex = 7;
			this->vScrollBar63Hz->Value = 100;
			this->vScrollBar63Hz->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &TestForm::vScrollBar63Hz_Scroll);
			// 
			// vScrollBar100Hz
			// 
			this->vScrollBar100Hz->Location = System::Drawing::Point(246, 12);
			this->vScrollBar100Hz->Maximum = 109;
			this->vScrollBar100Hz->Name = L"vScrollBar100Hz";
			this->vScrollBar100Hz->Size = System::Drawing::Size(14, 105);
			this->vScrollBar100Hz->TabIndex = 8;
			this->vScrollBar100Hz->Value = 100;
			this->vScrollBar100Hz->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &TestForm::vScrollBar100Hz_Scroll);
			// 
			// vScrollBar158Hz
			// 
			this->vScrollBar158Hz->Location = System::Drawing::Point(260, 12);
			this->vScrollBar158Hz->Maximum = 109;
			this->vScrollBar158Hz->Name = L"vScrollBar158Hz";
			this->vScrollBar158Hz->Size = System::Drawing::Size(14, 105);
			this->vScrollBar158Hz->TabIndex = 9;
			this->vScrollBar158Hz->Value = 100;
			this->vScrollBar158Hz->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &TestForm::vScrollBar158Hz_Scroll);
			// 
			// vScrollBar251Hz
			// 
			this->vScrollBar251Hz->Location = System::Drawing::Point(274, 12);
			this->vScrollBar251Hz->Maximum = 109;
			this->vScrollBar251Hz->Name = L"vScrollBar251Hz";
			this->vScrollBar251Hz->Size = System::Drawing::Size(14, 105);
			this->vScrollBar251Hz->TabIndex = 10;
			this->vScrollBar251Hz->Value = 100;
			this->vScrollBar251Hz->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &TestForm::vScrollBar251Hz_Scroll);
			// 
			// vScrollBar398Hz
			// 
			this->vScrollBar398Hz->Location = System::Drawing::Point(288, 12);
			this->vScrollBar398Hz->Maximum = 109;
			this->vScrollBar398Hz->Name = L"vScrollBar398Hz";
			this->vScrollBar398Hz->Size = System::Drawing::Size(14, 105);
			this->vScrollBar398Hz->TabIndex = 11;
			this->vScrollBar398Hz->Value = 100;
			this->vScrollBar398Hz->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &TestForm::vScrollBar398Hz_Scroll);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(174, 129);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(17, 12);
			this->label1->TabIndex = 12;
			this->label1->Text = L"10";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(257, 129);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(23, 12);
			this->label2->TabIndex = 13;
			this->label2->Text = L"100";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(286, 129);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(23, 12);
			this->label3->TabIndex = 14;
			this->label3->Text = L"400";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(116, 129);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(43, 12);
			this->label4->TabIndex = 15;
			this->label4->Text = L"MaxVol";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(202, 129);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(17, 12);
			this->label5->TabIndex = 16;
			this->label5->Text = L"40";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(366, 14);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(60, 19);
			this->textBox1->TabIndex = 17;
			this->textBox1->Text = L"1.0";
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &TestForm::textBox1_TextChanged);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(317, 17);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(30, 12);
			this->label6->TabIndex = 18;
			this->label6->Text = L"Time";
			// 
			// TestForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(488, 320);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->vScrollBar398Hz);
			this->Controls->Add(this->vScrollBar251Hz);
			this->Controls->Add(this->vScrollBar158Hz);
			this->Controls->Add(this->vScrollBar100Hz);
			this->Controls->Add(this->vScrollBar63Hz);
			this->Controls->Add(this->vScrollBar40Hz);
			this->Controls->Add(this->vScrollBar25Hz);
			this->Controls->Add(this->vScrollBar16Hz);
			this->Controls->Add(this->vScrollBar10Hz);
			this->Controls->Add(this->vScrollBarMax);
			this->Controls->Add(this->waveGraphics);
			this->Controls->Add(this->button1);
			this->Name = L"TestForm";
			this->Text = L"TestForm";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &TestForm::TestForm_FormClosing);
			this->Load += gcnew System::EventHandler(this, &TestForm::TestForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->waveGraphics))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public:
		float a = 1.0;				//	振幅(0, 1]
		array<float> ^power;	//	周波数毎のパワー[dB]
		float time = 1.0;				//	時間[sec]
		float th;				//	前回終了時の位相[rad]
		XAudio2 ^device;
		MasteringVoice ^xMVoice;
		WaveFormat ^format;
		AudioBuffer ^xBuf;
		IO::MemoryStream ^mStream;
		SourceVoice ^xSrcVoice;
		array<short>^ waveData;
		array<unsigned char>^ byteArray;
	private: 
		System::Void button1_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void TestForm_Load(System::Object^  sender, System::EventArgs^  e);
		System::Void TestForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e);
		System::Void drawWaveGraphics(array<short>^ wavedata);
		System::Void createWaveData(array<short>^ wavedata);
		System::Void vScrollBarMax_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e);
		System::Void vScrollBar10Hz_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e);
		System::Void vScrollBar16Hz_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e);
		System::Void vScrollBar25Hz_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e);
		System::Void vScrollBar40Hz_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e);
		System::Void vScrollBar63Hz_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e);
		System::Void vScrollBar100Hz_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e);
		System::Void vScrollBar158Hz_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e);
		System::Void vScrollBar251Hz_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e);
		System::Void vScrollBar398Hz_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e);
		System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e);
	};
}
#include <Windows.h>

#define PI		3.1415926535f

using namespace RealtimeRendering;
using namespace std;


const float freq[9] = {10.0, 15.8, 25.1, 39.8, 63.1, 100, 158, 251, 880};				//	周波数[Hz]