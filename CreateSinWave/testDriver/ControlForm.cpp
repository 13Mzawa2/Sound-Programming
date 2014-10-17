#include "ControlForm.h"
#include "Wave.h"

using namespace testDriver;
using namespace System::Threading;
using namespace System::IO::Ports;
using namespace System::Runtime::InteropServices;

Synthesizer synth1;

//-------------------------------------------------------------------------------------------------
//			開始時
//-------------------------------------------------------------------------------------------------
System::Void ControlForm::ControlForm_Load(System::Object^  sender, System::EventArgs^  e)
{
	//リストボックス初期化
	COMportList->Items->Clear();

	array<System::String^>^ serialPortsList = nullptr;

	//ポート名取得
	//例外の発生しうる処理に対してはtry-catch構文を使う
	try
	{
		serialPortsList = SerialPort::GetPortNames();
	}
	//例外処理（シリアルポート名を照会できませんでした）
	catch (Win32Exception^ ex)
	{
		MessageBox::Show(ex->Message, "エラー",
			MessageBoxButtons::OK, MessageBoxIcon::Error);
	}

	COMportList->Items->AddRange(serialPortsList);
}

//-------------------------------------------------------------------------------------------------
//			終了時
//-------------------------------------------------------------------------------------------------
System::Void ControlForm::ControlForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
{
	if (MessageBox::Show("アプリケーションを終了しますか？",
		"確認",
		MessageBoxButtons::YesNo,
		MessageBoxIcon::Question) == ::DialogResult::No)
		e->Cancel = true;
	else
		serialPort1->Close();
}
//-------------------------------------------------------------------------------------------------
//			ポート操作
//-------------------------------------------------------------------------------------------------
System::Void ControlForm::button1_Click(System::Object^  sender, System::EventArgs^  e)
{
	//	ポート接続状態では切断
	if (serialPort1->IsOpen)
	{
		serialPort1->Close();
		button1->Text = "Connect";
		return;
	}

	//設定変更
	try
	{
		serialPort1->BaudRate = 115200;
		serialPort1->PortName = COMportList->SelectedItem->ToString();
		serialPort1->Parity = Parity::None;
		serialPort1->StopBits = StopBits::Two;
		serialPort1->DataBits = 8;
		serialPort1->Handshake = Handshake::None;
	}
	catch (System::NullReferenceException^ ex)
	{
		MessageBox::Show(ex->Message + "\n注釈：ポート名を設定していない可能性があります．",
			"エラー", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}
	//	ポート接続
	serialPort1->Open();
	button1->Text = "Disconnect";

	return;
}
//-------------------------------------------------------------------------------------------------
//			受信時の処理
//-------------------------------------------------------------------------------------------------
System::Void ControlForm::serialPort1_DataReceived(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e)
{
	static int i = 0;
	if (buttonPlay->Text == "Play") return;			//	スタンバイ状態では起動しない
	//	ハードウェア側の送信要求1回につき1byte送る
	if (serialPort1->ReadChar() == 'R')
	{
		if (i >= synth1.waveData.size()) i = 0;		//	ループ
		try
		{
			serialPort1->Write(data, i, 1);
		}
		catch (::Exception^ ex)
		{
			//データを書き込めませんでした
			MessageBox::Show(ex->Message, "エラー",
				MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		i++;
	}
}
//-------------------------------------------------------------------------------------------------
//			WAVファイル読込
//-------------------------------------------------------------------------------------------------
System::Void ControlForm::buttonOpen_Click(System::Object^  sender, System::EventArgs^  e)
{
	buttonOpen->Enabled = false;
	openFileDialog1->Filter = "オーディオファイル(*.wav) | *.wav";
	if (openFileDialog1->ShowDialog() == ::DialogResult::Cancel)
	{
		buttonOpen->Enabled = true;
		return;
	}
	char* filename = (char*)Marshal::StringToHGlobalAnsi(openFileDialog1->FileName).ToPointer();
	Wave wav1(filename);
	synth1.loadWaveData(wav1, 0);
	buttonOpen->Enabled = true;
	Marshal::FreeHGlobal(IntPtr(filename));
}

System::Void ControlForm::buttonPlay_Click(System::Object^  sender, System::EventArgs^  e)
{
	if (buttonPlay->Text == "Stop")
	{
		buttonPlay->Text = "Play";
		delete data;
		return;
	}
	if (!synth1.waveData.empty())
	{
		data = gcnew array<unsigned char>(synth1.waveData.size());
		for (int i = 0; i < synth1.waveData.size(); i++)
			data[i] = (unsigned char)(synth1.waveData[i] * 0x80 + 0x80);
		buttonPlay->Text = "Stop";
	}
	else
	{
		synth1.createSinWave(0.5, 440.0, 1.0);
		data = gcnew array<unsigned char>(synth1.waveData.size());
		for (int i = 0; i < synth1.waveData.size(); i++)
			data[i] = (unsigned char)(synth1.waveData[i] * 0x80 + 0x80);
		buttonPlay->Text = "Stop";
	}
}