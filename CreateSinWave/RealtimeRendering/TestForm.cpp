#include "TestForm.h"


//	XAudio2の初期化処理
System::Void TestForm::TestForm_Load(System::Object^  sender, System::EventArgs^  e)
{
	
}
System::Void TestForm::TestForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
{
}
System::Void TestForm::button1_Click(System::Object^  sender, System::EventArgs^  e)
{
	//XAudio2の初期化
	XAudio2 ^device = gcnew XAudio2();
	MasteringVoice ^xMVoice = gcnew MasteringVoice(device);

	//	波形フォーマットの生成
	WaveFormat ^format = gcnew WaveFormat();
	format->FormatTag = static_cast<WaveFormatTag>(WAVE_FORMAT_PCM);		//	フォーマット形式:PCM
	format->Channels = 1;													//	チャネル数:モノラル
	format->BitsPerSample = 16;												//	16 bit/sample
	format->SamplesPerSecond = 44100;										//	サンプリングレート
	format->BlockAlignment = format->BitsPerSample / 8 * format->Channels;				//	1ブロックあたりのbyte数
	format->AverageBytesPerSecond = format->SamplesPerSecond * format->BlockAlignment;	//	1秒あたりのバッファ数

	//	Sin波生成(double -> short)
	float a = 0.5;				//	振幅(0, 1]
	float freq = 880.0;			//	周波数[Hz]
	float time = 1.0;			//	時間[sec]
	array<short>^ waveData = gcnew array<short>((int)(format->AverageBytesPerSecond * time));	//	1秒分のバッファ
	for (int i = 0; i < waveData->Length / 2; i++)
	{
		waveData[i] = (short)(SHRT_MAX * a * Math::Sin(i * PI * freq / (double)format->SamplesPerSecond));
	}
	//	short -> byte
	array<unsigned char>^ byteArray = gcnew array<unsigned char>(waveData->Length * format->BitsPerSample / 8);	//	16bit音源なので2倍
	Buffer::BlockCopy(waveData, 0, byteArray, 0, byteArray->Length);

	//	バッファの作成
	AudioBuffer ^xBuf = gcnew AudioBuffer();
	IO::MemoryStream ^mStream = gcnew IO::MemoryStream(byteArray);
	xBuf->AudioData = mStream;
	xBuf->AudioBytes = (int)mStream->Length;
	xBuf->Flags = BufferFlags::EndOfStream;

	//	Source Voice の作成
	SourceVoice ^xSrcVoice = gcnew SourceVoice(device, format);
	xSrcVoice->SubmitSourceBuffer(xBuf);

	//	再生
	xSrcVoice->Start();
	MessageBox::Show("終了します．");

	//	破棄
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