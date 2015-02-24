#include "TestForm.h"


//	XAudio2�̏���������
System::Void TestForm::TestForm_Load(System::Object^  sender, System::EventArgs^  e)
{
	
}
System::Void TestForm::TestForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
{
}
System::Void TestForm::button1_Click(System::Object^  sender, System::EventArgs^  e)
{
	//XAudio2�̏�����
	XAudio2 ^device = gcnew XAudio2();
	MasteringVoice ^xMVoice = gcnew MasteringVoice(device);

	//	�g�`�t�H�[�}�b�g�̐���
	WaveFormat ^format = gcnew WaveFormat();
	format->FormatTag = static_cast<WaveFormatTag>(WAVE_FORMAT_PCM);		//	�t�H�[�}�b�g�`��:PCM
	format->Channels = 1;													//	�`���l����:���m����
	format->BitsPerSample = 16;												//	16 bit/sample
	format->SamplesPerSecond = 44100;										//	�T���v�����O���[�g
	format->BlockAlignment = format->BitsPerSample / 8 * format->Channels;				//	1�u���b�N�������byte��
	format->AverageBytesPerSecond = format->SamplesPerSecond * format->BlockAlignment;	//	1�b������̃o�b�t�@��

	//	Sin�g����(double -> short)
	float a = 0.5;				//	�U��(0, 1]
	float freq = 880.0;			//	���g��[Hz]
	float time = 1.0;			//	����[sec]
	array<short>^ waveData = gcnew array<short>((int)(format->AverageBytesPerSecond * time));	//	1�b���̃o�b�t�@
	for (int i = 0; i < waveData->Length / 2; i++)
	{
		waveData[i] = (short)(SHRT_MAX * a * Math::Sin(i * PI * freq / (double)format->SamplesPerSecond));
	}
	//	short -> byte
	array<unsigned char>^ byteArray = gcnew array<unsigned char>(waveData->Length * format->BitsPerSample / 8);	//	16bit�����Ȃ̂�2�{
	Buffer::BlockCopy(waveData, 0, byteArray, 0, byteArray->Length);

	//	�o�b�t�@�̍쐬
	AudioBuffer ^xBuf = gcnew AudioBuffer();
	IO::MemoryStream ^mStream = gcnew IO::MemoryStream(byteArray);
	xBuf->AudioData = mStream;
	xBuf->AudioBytes = (int)mStream->Length;
	xBuf->Flags = BufferFlags::EndOfStream;

	//	Source Voice �̍쐬
	SourceVoice ^xSrcVoice = gcnew SourceVoice(device, format);
	xSrcVoice->SubmitSourceBuffer(xBuf);

	//	�Đ�
	xSrcVoice->Start();
	MessageBox::Show("�I�����܂��D");

	//	�j��
	xSrcVoice->Stop();
	//xBuf->Dispose();
	//xSrcVoice->Dispose();
	//mStream->Dispose();
	//xMVoice->Dispose();
	//device->Dispose();
	delete xBuf;
	delete xSrcVoice;
	delete mStream;
	delete xMVoice;
	delete device;
}