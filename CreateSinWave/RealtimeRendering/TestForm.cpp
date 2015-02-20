#include "TestForm.h"

using namespace RealtimeRendering;
using namespace std;

IXAudio2* xaudio;
IXAudio2MasteringVoice* mastering_voice;

//	XAudio2�̏���������
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
		//	�g�`�t�H�[�}�b�g�̐���
		WAVEFORMATEX format = { 0 };
		format.wFormatTag = WAVE_FORMAT_PCM;		//	�t�H�[�}�b�g�`��:PCM
		format.nChannels = 1;						//	�`���l����:���m����
		format.wBitsPerSample = 16;					//	16 bit/sample
		format.nSamplesPerSec = 44100;				//	�T���v�����O���[�g
		format.nBlockAlign = format.wBitsPerSample / 8 * format.nChannels;			//	1�u���b�N�������byte��
		format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;		//	1�b������̃o�b�t�@��

		//	�g�`����(double)
		vector<double> datad(format.wBitsPerSample * 1);		//	1�b���̃o�b�t�@�i�܂���double�^�Ő����j
		for (size_t i = 0; i < datad.size() / 2; i++)
		{
			datad[i] = (short)(32767 * sinf(i * PI * 440.0f / format.nSamplesPerSec));
		}
		//	double -> short�@�ւ̕ϊ�
		vector<char> datac;
		setWaveDatad(datad, datac, format);

		//	Source Voice �̍쐬
		IXAudio2SourceVoice* source_voice;
		HRESULT hr = xaudio->CreateSourceVoice(&source_voice, &format);
		if (FAILED(hr))
			throw "CreateSourceVoice";

		XAUDIO2_BUFFER buffer = { 0 };
		buffer.AudioBytes = datac.size();				//	�o�b�t�@�̃o�C�g��
		buffer.pAudioData = (BYTE*)&datac[0];			//	�o�b�t�@�̐擪�A�h���X
		buffer.Flags = XAUDIO2_END_OF_STREAM;			//	tell the source voice not to expect any data after this buffer
		source_voice->SubmitSourceBuffer(&buffer);		//	Source Voice �Ƀo�b�t�@�𑗐M

		//	�Đ�
		source_voice->Start();
		MessageBox::Show("�I�����܂��D");

		//	Source Voice �̔j��
		source_voice->Stop();
		source_voice->DestroyVoice();
	}
	catch (const char* e)
	{
		cout << e << endl;
	}

}
//	XAudio2�̏�����
void XAudio2Init(void)
{
	//	Initialize XAudio2
	HRESULT hr;
	if (FAILED(hr = CoInitializeEx(NULL, COINIT_MULTITHREADED)))		//	COM�̏�����
		throw "COInitializeEX";
	UINT32 flags = 0;

	if (FAILED(hr = XAudio2Create(&xaudio, flags)))
		throw "XAudio2Create";

	//	Create a mastering voice(����)
	if (FAILED(hr = xaudio->CreateMasteringVoice(&mastering_voice)))
		throw "CreateMasteringVoice";

}
//	XAudio2�̒��g����ɂ���iXAudio2�̔j���j
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
//	�g�`�f�[�^��double�^�ň������߂̃��b�p�[
void setWaveDatad(vector<double> &datad, vector<char> &datac, WAVEFORMATEX format)
{
	vector<short> buf(datad.size());
	for (int t = 0; t < datad.size(); t++)
		buf[t] = (short)(datad[t] * SHRT_MAX);

	//	�𑜓x�s���̂Ƃ���16bit�Ƃ��ď���
	if (format.wBitsPerSample != 8 && format.wBitsPerSample != 16)
		format.wBitsPerSample = 16;

	//	BytesPerSample
	unsigned short BPS = format.wBitsPerSample / 8;

	datac.resize(BPS * format.nChannels * buf.size());

	if (BPS == 1)		//	���m����8bit����
	{
		for (int t = 0; t<buf.size(); t++) datac[t] = (buf[t] >> 8) + 0x80;
	}
	else //if (BPS == 2)		//	���m����16bit����
	{
		short *ptr = (short*)&datac[0];
		for (int t = 0; t<buf.size(); t++) ptr[t] = buf[t];
	}
}