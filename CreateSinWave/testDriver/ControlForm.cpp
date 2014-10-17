#include "ControlForm.h"
#include "Wave.h"

using namespace testDriver;
using namespace System::Threading;
using namespace System::IO::Ports;
using namespace System::Runtime::InteropServices;

Synthesizer synth1;

//-------------------------------------------------------------------------------------------------
//			�J�n��
//-------------------------------------------------------------------------------------------------
System::Void ControlForm::ControlForm_Load(System::Object^  sender, System::EventArgs^  e)
{
	//���X�g�{�b�N�X������
	COMportList->Items->Clear();

	array<System::String^>^ serialPortsList = nullptr;

	//�|�[�g���擾
	//��O�̔��������鏈���ɑ΂��Ă�try-catch�\�����g��
	try
	{
		serialPortsList = SerialPort::GetPortNames();
	}
	//��O�����i�V���A���|�[�g�����Ɖ�ł��܂���ł����j
	catch (Win32Exception^ ex)
	{
		MessageBox::Show(ex->Message, "�G���[",
			MessageBoxButtons::OK, MessageBoxIcon::Error);
	}

	COMportList->Items->AddRange(serialPortsList);
}

//-------------------------------------------------------------------------------------------------
//			�I����
//-------------------------------------------------------------------------------------------------
System::Void ControlForm::ControlForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
{
	if (MessageBox::Show("�A�v���P�[�V�������I�����܂����H",
		"�m�F",
		MessageBoxButtons::YesNo,
		MessageBoxIcon::Question) == ::DialogResult::No)
		e->Cancel = true;
	else
		serialPort1->Close();
}
//-------------------------------------------------------------------------------------------------
//			�|�[�g����
//-------------------------------------------------------------------------------------------------
System::Void ControlForm::button1_Click(System::Object^  sender, System::EventArgs^  e)
{
	//	�|�[�g�ڑ���Ԃł͐ؒf
	if (serialPort1->IsOpen)
	{
		serialPort1->Close();
		button1->Text = "Connect";
		return;
	}

	//�ݒ�ύX
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
		MessageBox::Show(ex->Message + "\n���߁F�|�[�g����ݒ肵�Ă��Ȃ��\��������܂��D",
			"�G���[", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}
	//	�|�[�g�ڑ�
	serialPort1->Open();
	button1->Text = "Disconnect";

	return;
}
//-------------------------------------------------------------------------------------------------
//			��M���̏���
//-------------------------------------------------------------------------------------------------
System::Void ControlForm::serialPort1_DataReceived(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e)
{
	static int i = 0;
	if (buttonPlay->Text == "Play") return;			//	�X�^���o�C��Ԃł͋N�����Ȃ�
	//	�n�[�h�E�F�A���̑��M�v��1��ɂ�1byte����
	if (serialPort1->ReadChar() == 'R')
	{
		if (i >= synth1.waveData.size()) i = 0;		//	���[�v
		try
		{
			serialPort1->Write(data, i, 1);
		}
		catch (::Exception^ ex)
		{
			//�f�[�^���������߂܂���ł���
			MessageBox::Show(ex->Message, "�G���[",
				MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		i++;
	}
}
//-------------------------------------------------------------------------------------------------
//			WAV�t�@�C���Ǎ�
//-------------------------------------------------------------------------------------------------
System::Void ControlForm::buttonOpen_Click(System::Object^  sender, System::EventArgs^  e)
{
	buttonOpen->Enabled = false;
	openFileDialog1->Filter = "�I�[�f�B�I�t�@�C��(*.wav) | *.wav";
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