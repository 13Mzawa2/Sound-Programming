#include "TestForm.h"


//----------------------------------------
//	XAudio2�̏���������
//----------------------------------------
System::Void TestForm::TestForm_Load(System::Object^  sender, System::EventArgs^  e)
{
	//XAudio2�̏�����
	device = gcnew XAudio2();
	xMVoice = gcnew MasteringVoice(device);

	//	�g�`�t�H�[�}�b�g�̐���
	format = gcnew WaveFormat();
	format->FormatTag = static_cast<WaveFormatTag>(WAVE_FORMAT_PCM);		//	�t�H�[�}�b�g�`��:PCM
	format->Channels = 1;													//	�`���l����:���m����
	format->BitsPerSample = 16;												//	16 bit/sample
	format->SamplesPerSecond = 44100;										//	�T���v�����O���[�g
	format->BlockAlignment = format->BitsPerSample / 8 * format->Channels;				//	1�u���b�N�������byte��
	format->AverageBytesPerSecond = format->SamplesPerSecond * format->BlockAlignment;	//	1�b������̃o�b�t�@��

	//	���g�����̃p���[��ۑ�����z���p��
	power = gcnew array<float>(9);
}
//----------------------------------------
//	�������������
//----------------------------------------
System::Void TestForm::TestForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
{
	delete xBuf;
	delete xSrcVoice;
	delete mStream;
	delete xMVoice;
	delete device;
}
//----------------------------------------
//	Sin�g��������ɍĐ�
//----------------------------------------
System::Void TestForm::button1_Click(System::Object^  sender, System::EventArgs^  e)
{
	//	Sin�g����
	//createWaveData(waveData);
	//drawWaveGraphics(waveData);

	//	�o�b�t�@�̍쐬
	xBuf = gcnew AudioBuffer();
	mStream = gcnew IO::MemoryStream(byteArray);
	xBuf->AudioData = mStream;
	xBuf->AudioBytes = (int)mStream->Length;
	xBuf->Flags = BufferFlags::EndOfStream;

	//	Source Voice �̍쐬
	xSrcVoice = gcnew SourceVoice(device, format);
	xSrcVoice->SubmitSourceBuffer(xBuf);

	//	�Đ�
	xSrcVoice->Start();
	MessageBox::Show("�I�����܂��D");

	//	�I��
	xSrcVoice->Stop();
}

//----------------------------------------
//	�g�`�𐶐�
//----------------------------------------
System::Void TestForm::createWaveData(array<short>^ wavedata)
{
	waveData = gcnew array<short>((int)(format->AverageBytesPerSecond * time));	//	t�b���̃o�b�t�@

	//	�܂������ō��
	array<double> ^waveDatad = gcnew array<double>(waveData->Length);
	double overshoot = 1.0, undershoot = -1.0;
	for (int i = 0; i < waveDatad->Length; i++)
	{
		waveDatad[i] = 0.0;
		for (int j = 0; j < 9; j++)
		{
			waveDatad[i] += power[j] * Math::Sin(i * PI * freq[j] / (double)format->SamplesPerSecond);
		}
		overshoot = Math::Max(waveDatad[i], overshoot);
		undershoot = Math::Max(waveDatad[i], undershoot);
	}
	//	�g�`��short�͈̔͊O�ɔ�яo������}������
	if (overshoot - undershoot > 2.0)
	{
		for (int i = 0; i < waveData->Length; i++)
		{
			waveData[i] = (short)(SHRT_MAX * a * waveDatad[i] / 2.0 * (overshoot - undershoot));
		}
	}
	else
	{
		for (int i = 0; i < waveData->Length; i++)
		{
			waveData[i] = (short)(SHRT_MAX * a * waveDatad[i]);
		}
	}
	
	//	short -> byte(PCM�t�H�[�}�b�g�Ń�������ɔg�`�f�[�^��W�J)
	byteArray = gcnew array<unsigned char>(waveData->Length * format->BitsPerSample / 8);	//	16bit�����Ȃ̂�2�{
	Buffer::BlockCopy(waveData, 0, byteArray, 0, byteArray->Length);
}

//----------------------------------------
//	�g�`�R���g���[��
//----------------------------------------
System::Void TestForm::vScrollBarMax_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e)
{
	a = (100.0 - vScrollBarMax->Value) / 100.0;
	//	Sin�g����
	createWaveData(waveData);
	drawWaveGraphics(waveData);
}
System::Void TestForm::vScrollBar10Hz_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e)
{
	power[0] = (100.0 - vScrollBar10Hz->Value) / 100.0;
	//	Sin�g����
	createWaveData(waveData);
	drawWaveGraphics(waveData);
}
System::Void TestForm::vScrollBar16Hz_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e)
{
	power[0] = (100.0 - vScrollBar16Hz->Value) / 100.0;
	//	Sin�g����
	createWaveData(waveData);
	drawWaveGraphics(waveData);
}
System::Void TestForm::vScrollBar25Hz_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e)
{
	power[0] = (100.0 - vScrollBar25Hz->Value) / 100.0;
	//	Sin�g����
	createWaveData(waveData);
	drawWaveGraphics(waveData);
}
System::Void TestForm::vScrollBar40Hz_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e)
{
	power[0] = (100.0 - vScrollBar40Hz->Value) / 100.0;
	//	Sin�g����
	createWaveData(waveData);
	drawWaveGraphics(waveData);
}
System::Void TestForm::vScrollBar63Hz_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e)
{
	power[0] = (100.0 - vScrollBar63Hz->Value) / 100.0;
	//	Sin�g����
	createWaveData(waveData);
	drawWaveGraphics(waveData);
}
System::Void TestForm::vScrollBar100Hz_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e)
{
	power[0] = (100.0 - vScrollBar100Hz->Value) / 100.0;
	//	Sin�g����
	createWaveData(waveData);
	drawWaveGraphics(waveData);
}
System::Void TestForm::vScrollBar158Hz_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e)
{
	power[0] = (100.0 - vScrollBar158Hz->Value) / 100.0;
	//	Sin�g����
	createWaveData(waveData);
	drawWaveGraphics(waveData);
}
System::Void TestForm::vScrollBar251Hz_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e)
{
	power[0] = (100.0 - vScrollBar251Hz->Value) / 100.0;
	//	Sin�g����
	createWaveData(waveData);
	drawWaveGraphics(waveData);
}
System::Void TestForm::vScrollBar398Hz_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e)
{
	power[0] = (100.0 - vScrollBar398Hz->Value) / 100.0;
	//	Sin�g����
	createWaveData(waveData);
	drawWaveGraphics(waveData);
}
System::Void TestForm::textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e)
{
	time = (float)(Convert::ToDouble(textBox1->Text));
	//	Sin�g����
	createWaveData(waveData);
	drawWaveGraphics(waveData);
}
//----------------------------------------
//	�g�`���O���t�B�b�N�\��
//----------------------------------------
System::Void TestForm::drawWaveGraphics(array<short>^ wavedata)
{
	int xmax = (int)((double)wavedata->Length / time / freq[0] * 3) + 1;
	for (int i = 0; i < xmax; i++)
		waveGraphics->Series["wavedata"]->Points->AddXY((double)i/time, (double)wavedata[i]);
}