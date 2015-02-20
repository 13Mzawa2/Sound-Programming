#include "TestForm.h"

using namespace RealtimeRendering;
using namespace std;

IXAudio2* xaudio;
IXAudio2MasteringVoice* mastering_voice;

//	XAudio2の初期化処理
System::Void TestForm::TestForm_Load(System::Object^  sender, System::EventArgs^  e)
{
	try
	{
		XAudio2Init();
	}
	catch (const char* e)
	{
		cout << e << endl;
	}
	
}
System::Void TestForm::TestForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
{
	XAudio2CleanUp();
}
System::Void TestForm::button1_Click(System::Object^  sender, System::EventArgs^  e)
{
	try
	{
		//	波形フォーマットの生成
		WAVEFORMATEX format = { 0 };
		format.wFormatTag = WAVE_FORMAT_PCM;		//	フォーマット形式:PCM
		format.nChannels = 1;						//	チャネル数:モノラル
		format.wBitsPerSample = 16;					//	16 bit/sample
		format.nSamplesPerSec = 44100;				//	サンプリングレート
		format.nBlockAlign = format.wBitsPerSample / 8 * format.nChannels;			//	1ブロックあたりのbyte数
		format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;		//	1秒あたりのバッファ数

		//	波形生成(double)
		vector<double> datad(format.wBitsPerSample * 1);		//	1秒分のバッファ（まずはdouble型で生成）
		for (size_t i = 0; i < datad.size() / 2; i++)
		{
			datad[i] = (short)(32767 * sinf(i * PI * 440.0f / format.nSamplesPerSec));
		}
		//	double -> short　への変換
		vector<char> datac;
		setWaveDatad(datad, datac, format);

		//	Source Voice の作成
		IXAudio2SourceVoice* source_voice;
		HRESULT hr = xaudio->CreateSourceVoice(&source_voice, &format);
		if (FAILED(hr))
			throw "CreateSourceVoice";

		XAUDIO2_BUFFER buffer = { 0 };
		buffer.AudioBytes = datac.size();				//	バッファのバイト数
		buffer.pAudioData = (BYTE*)&datac[0];			//	バッファの先頭アドレス
		buffer.Flags = XAUDIO2_END_OF_STREAM;			//	tell the source voice not to expect any data after this buffer
		source_voice->SubmitSourceBuffer(&buffer);		//	Source Voice にバッファを送信

		//	再生
		source_voice->Start();
		MessageBox::Show("終了します．");

		//	Source Voice の破棄
		source_voice->Stop();
		source_voice->DestroyVoice();
	}
	catch (const char* e)
	{
		cout << e << endl;
	}

}
//	XAudio2の初期化
void XAudio2Init(void)
{
	//	Initialize XAudio2
	HRESULT hr;
	if (FAILED(hr = CoInitializeEx(NULL, COINIT_MULTITHREADED)))		//	COMの初期化
		throw "COInitializeEX";
	UINT32 flags = 0;

	if (FAILED(hr = XAudio2Create(&xaudio, flags)))
		throw "XAudio2Create";

	//	Create a mastering voice(音源)
	if (FAILED(hr = xaudio->CreateMasteringVoice(&mastering_voice)))
		throw "CreateMasteringVoice";

}
//	XAudio2の中身を空にする（XAudio2の破棄）
void XAudio2CleanUp(void)
{
	if (mastering_voice != 0)
	{
		mastering_voice->DestroyVoice();
		mastering_voice = 0;
	}
	if (xaudio != 0)
	{
		xaudio->Release();
		xaudio = 0;
	}
	CoUninitialize();
}
//	波形データをdouble型で扱うためのラッパー
void setWaveDatad(vector<double> &datad, vector<char> &datac, WAVEFORMATEX format)
{
	vector<short> buf(datad.size());
	for (int t = 0; t < datad.size(); t++)
		buf[t] = (short)(datad[t] * SHRT_MAX);

	//	解像度不明のときは16bitとして処理
	if (format.wBitsPerSample != 8 && format.wBitsPerSample != 16)
		format.wBitsPerSample = 16;

	//	BytesPerSample
	unsigned short BPS = format.wBitsPerSample / 8;

	datac.resize(BPS * format.nChannels * buf.size());

	if (BPS == 1)		//	モノラル8bit音源
	{
		for (int t = 0; t<buf.size(); t++) datac[t] = (buf[t] >> 8) + 0x80;
	}
	else //if (BPS == 2)		//	モノラル16bit音源
	{
		short *ptr = (short*)&datac[0];
		for (int t = 0; t<buf.size(); t++) ptr[t] = buf[t];
	}
}