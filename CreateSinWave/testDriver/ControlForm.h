#pragma once

namespace testDriver {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// ControlForm の概要
	/// </summary>
	public ref class ControlForm : public System::Windows::Forms::Form
	{
	public:
		ControlForm(void)
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
		~ControlForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::IO::Ports::SerialPort^  serialPort1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::ComboBox^  COMportList;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  buttonOpen;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  buttonPlay;

	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	protected:
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// 必要なデザイナー変数です。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->COMportList = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->buttonOpen = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->buttonPlay = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->SuspendLayout();
			// 
			// serialPort1
			// 
			this->serialPort1->BaudRate = 115200;
			this->serialPort1->DataReceived += gcnew System::IO::Ports::SerialDataReceivedEventHandler(this, &ControlForm::serialPort1_DataReceived);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(187, 38);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Connect";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &ControlForm::button1_Click);
			// 
			// COMportList
			// 
			this->COMportList->FormattingEnabled = true;
			this->COMportList->Location = System::Drawing::Point(92, 40);
			this->COMportList->Name = L"COMportList";
			this->COMportList->Size = System::Drawing::Size(75, 20);
			this->COMportList->TabIndex = 1;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 43);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(55, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"COM Port";
			// 
			// buttonOpen
			// 
			this->buttonOpen->Location = System::Drawing::Point(12, 80);
			this->buttonOpen->Name = L"buttonOpen";
			this->buttonOpen->Size = System::Drawing::Size(75, 23);
			this->buttonOpen->TabIndex = 3;
			this->buttonOpen->Text = L"Load WAV";
			this->buttonOpen->UseVisualStyleBackColor = true;
			this->buttonOpen->Click += gcnew System::EventHandler(this, &ControlForm::buttonOpen_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(115, 85);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(42, 12);
			this->label2->TabIndex = 4;
			this->label2->Text = L"No File";
			// 
			// buttonPlay
			// 
			this->buttonPlay->Location = System::Drawing::Point(187, 80);
			this->buttonPlay->Name = L"buttonPlay";
			this->buttonPlay->Size = System::Drawing::Size(75, 23);
			this->buttonPlay->TabIndex = 5;
			this->buttonPlay->Text = L"Play";
			this->buttonPlay->UseVisualStyleBackColor = true;
			this->buttonPlay->Click += gcnew System::EventHandler(this, &ControlForm::buttonPlay_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// ControlForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 261);
			this->Controls->Add(this->buttonPlay);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->buttonOpen);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->COMportList);
			this->Controls->Add(this->button1);
			this->Name = L"ControlForm";
			this->Text = L"ControlForm";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &ControlForm::ControlForm_FormClosing);
			this->Load += gcnew System::EventHandler(this, &ControlForm::ControlForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:
		array<unsigned char>^ data;
	private: System::Void ControlForm_Load(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ControlForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e);
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void buttonOpen_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void serialPort1_DataReceived(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e);
private: System::Void buttonPlay_Click(System::Object^  sender, System::EventArgs^  e);
};
}
