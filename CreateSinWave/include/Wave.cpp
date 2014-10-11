#include "Wave.h"

#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

//	�`�����N�̃��[�h�p�\����
struct WaveChunk
{
	char	ID[4];		//	�`�����N���	"RIFF", "fmt ", "data" �̂ǂꂩ
	int		size;		//	�`�����N�T�C�Y
};

Wave::Wave()
{
	samplingSize = 0;
	dataSize = 0;
}

Wave::Wave(char *filename)
{
	loadWave(filename);
}

Wave::~Wave()
{

}

//---------------------------------------------------------------------------
//	wav�t�@�C���̃��[�h
bool Wave::loadWave(char *filename)
{
	WaveChunk	chunk;
	char		type[4];

	ifstream	ifs(filename, ios::binary);

	data.clear();			//	�f�[�^�t�@�C���̏�����
	dataSize = 0;

	//	�t�@�C�����J���Ȃ������ꍇ�̏���
	if (!ifs)
	{
		cerr << "File open error." << endl;
		cerr << "\t" << filename << endl;
		return false;
	}
	//	�`�����N�i�t�H�[�}�b�gID, �f�[�^�T�C�Y�j�̃��[�h
	//	RIFF�t�H�[�}�b�g�ł��邱�Ƃ̊m�F strncmp() ... �������r
	ifs.read((char*)&chunk, 8);
	if (ifs.bad() || strncmp(chunk.ID, "RIFF", 4)) return false;
	//	RIFF�̎�ނ�WAVE�ł��邱�Ƃ̊m�F
	ifs.read((char*)type, 4);
	if (ifs.bad() || strncmp(type, "WAVE", 4)) return false;

	unsigned char flag = 0;

	try
	{
		//	��O�ݒ�
		ifs.exceptions(ios::badbit);

		while (ifs.read((char*)&chunk, sizeof(WaveChunk)))
		{
			//	fmt�`�����NID�̃��[�h
			if (strncmp(chunk.ID, "fmt ", 4))
			{
				//	���j�APCA�̂�
				ifs.read((char*)&format, min(16, chunk.size));
				//	���j�APCA�łȂ�������G�X�P�[�v
				if (format.formatID != WAVE_FORMAT_PCM)
				{
					cerr << "unsupported wave file format." << endl;
					goto WAVE_FILE_ERROR;
				}
				flag++;
			}
			//	data�`�����N�̃��[�h
			else if (strncmp(chunk.ID, "data", 4))
			{
				data.resize(chunk.size);
				ifs.read((char*)&data[0], data.size());
				flag++;
				break;
			}
			else
			{
				ifs.seekg(chunk.size, ios::cur);
			}
		}
	}
	catch (ios_base::failure& e)
	{
		ifs.seekg(chunk.size, ios::cur);	//	���݈ʒu����`�����N�T�C�Y�������ړ�
		goto WAVE_FILE_ERROR;
	}

	//	fmt�`�����N��data�`�����N�������ǂݍ��߂����Ƃ��m�F
	if (flag != 2)
	{
		cerr << "file format error." << endl;
		goto WAVE_FILE_ERROR;
	}

	samplingSize = format.resolution / 8 * format.numChannels;
	dataSize = data.size() / samplingSize;

	return true;

WAVE_FILE_ERROR:
	data.clear();
	samplingSize = 0;
	dataSize = 0;
	return false;
}

//	wav�t�@�C���̕ۑ�
bool Wave::saveWave(char *filename)
{
	ofstream ofs(filename, ios::binary);

	if (!ofs)
	{
		cerr << "file open error." << endl;
		cout << "\t" << filename << endl;
		return false;
	}

	try
	{
		WaveChunk chunk;

		//��O�ݒ�?
		ofs.exceptions(ios::badbit);

		//	RIFF���̋L�q
		memcpy(chunk.ID, "RIFF", 4 * sizeof(char));
		chunk.size = sizeof(chunk)+4 + sizeof(WaveFormat)+data.size();
		ofs.write((char*)&chunk, sizeof(WaveChunk));
		ofs.write("WAVE", 4);
		//	fmt�`�����N�̋L�q
		memcpy(chunk.ID, "fmt ", 4 * sizeof(char));
		chunk.size = sizeof(WaveFormat);
		ofs.write((char*)&chunk, sizeof(WaveChunk));
		ofs.write((char*)&format, 16);
		//	data�`�����N�̋L�q
		memcpy(chunk.ID, "data", 4 * sizeof(char));
		chunk.size = data.size();
		ofs.write((char*)&chunk, sizeof(WaveChunk));
		ofs.write((char*)&data[0], data.size());
	}
	catch (ios_base::failure& e)
	{
		cerr << "file write error." << endl;
		return false;
	}

	return true;
}

//	�g�`�f�[�^�̎擾�i1�`�����l���̂݁j
void Wave::getChannel(vector<short> &channel, unsigned short ch)
{
	channel.resize(dataSize);		//	�x�N�g�����f�[�^�i�[�\��Ԃɂ���
	//	���m�����M���̏ꍇ
	if (format.numChannels == WAVE_CH_MONAURAL)
	{
		if (format.resolution == WAVE_RESOLUTION_8BIT)
		{
			//8bit�Ȃ�f�[�^�� unsigned char (0�`255 ������ 128)�Ȃ̂ŕ␳
			for (int t = 0; t < dataSize; t++)
				channel[t] = (short(data[t]) - 0x80) << 8;
		}
		else if (format.resolution == WAVE_RESOLUTION_16BIT)
		{
			//16bit�Ȃ�f�[�^�� signed short (-32768�`+32767 ������ 0)
			short *ptr = (short*)&data[0];
			for (int t = 0; t < dataSize; t++) channel[t] = ptr[t];
		}
	}
	else if (format.numChannels == WAVE_CH_STEREO)
	{
		if ( ch > 1 ) ch = 1;

		if (format.resolution == WAVE_RESOLUTION_8BIT)
		{
			//8bit�Ȃ�f�[�^�� unsigned char (0�`255 ������ 128)�Ȃ̂ŕ␳
			for (int t = 0; t < dataSize; t++)
				channel[t] = (short(data[2 * t + ch]) - 0x80) << 8;
		}
		else if (format.resolution == WAVE_RESOLUTION_16BIT)
		{
			//16bit�Ȃ�f�[�^�� signed short (-32768�`+32767 ������ 0)
			short *ptr = (short*)&data[0];
			for (int t = 0; t<dataSize; t++) channel[t] = ptr[2 * t + ch];
		}
	}
}
//---------------------------------------------------------------------------
//	�g�`�f�[�^�̃Z�b�g
//	�f�[�^�̏������݂͂��̊֐��ōs��
//	������̓��m�����p
void Wave::setChannel(vector<short> &ch0, WaveFormat _fmt)
{
	format = _fmt;

	//	�𑜓x�s���̂Ƃ���16bit�Ƃ��ď���
	if (format.resolution != WAVE_RESOLUTION_8BIT && format.resolution != WAVE_RESOLUTION_16BIT)
		format.resolution = WAVE_RESOLUTION_16BIT;

	//	BytesPerSample
	unsigned short BPS = format.resolution / 8;

	format.formatID = WAVE_FORMAT_PCM;
	format.numChannels = WAVE_CH_MONAURAL;
	format.blockSize = format.numChannels * BPS;
	format.bytesPerSec = format.samplingRate * format.blockSize;

	samplingSize = BPS * format.numChannels;
	dataSize = ch0.size();

	data.resize(samplingSize * ch0.size());

	if (BPS == 1){
		for (int t = 0; t<ch0.size(); t++) data[t] = (ch0[t] >> 8) + 0x80;
	}
	else if (BPS == 2){
		short *ptr = (short*)&data[0];
		for (int t = 0; t<ch0.size(); t++) ptr[t] = ch0[t];
	}
}
//	�g�`�f�[�^�̃Z�b�g
//	������̓X�e���I�p
void Wave::setChannel(vector<short> &ch0, vector<short> &ch1, WaveFormat _fmt)
{
	format = _fmt;

	if (format.resolution != WAVE_RESOLUTION_8BIT && format.resolution != WAVE_RESOLUTION_16BIT)
		format.resolution = WAVE_RESOLUTION_16BIT;

	//BytesPerSample
	unsigned short BPS = format.resolution / 8;

	//	�t�H�[�}�b�g�̃Z�b�g
	format.formatID = WAVE_FORMAT_PCM;
	format.numChannels = WAVE_CH_STEREO;
	format.blockSize = format.numChannels * BPS;
	format.bytesPerSec = format.samplingRate * format.blockSize;

	samplingSize = BPS * format.numChannels;
	dataSize = min(ch0.size(), ch1.size());

	data.resize(samplingSize * ch0.size());

	if (BPS == 1){
		for (int t = 0; t < dataSize; t++){
			data[2 * t] = (ch0[t] >> 8) + 0x80;
			data[2 * t + 1] = (ch1[t] >> 8) + 0x80;
		}
	}
	else if (BPS == 2){
		short *ptr = (short*)&data[0];
		for (int t = 0; t < dataSize; t++){
			ptr[2 * t] = ch0[t];
			ptr[2 * t + 1] = ch1[t];
		}
	}
}

//	double�^�ň������߂̃��b�p�[
void Wave::getChannel(vector<double> &channel, unsigned short ch)
{
	vector<short> buf;
	getChannel(buf, ch);
	channel.resize(buf.size());
	for (int t = 0; t < buf.size(); t++)
		channel[t] = (double)buf[t] / SHRT_MAX;
}
void Wave::setChannel(vector<double> &ch0, WaveFormat _fmt)
{
	vector<short> buf(ch0.size());
	for (int t = 0; t < ch0.size(); t++)
		buf[t] = (short)(ch0[t] * SHRT_MAX);
	setChannel(buf, _fmt);
}
void Wave::setChannel(vector<double> &ch0, vector<double> &ch1, WaveFormat _fmt)
{
	vector<short> buf0(ch0.size()), buf1(ch1.size());
	for (int t = 0; t < ch0.size(); t++)
	{
		buf0[t] = (short)(ch0[t] * SHRT_MAX);
		buf1[t] = (short)(ch1[t] * SHRT_MAX);
	}
	setChannel(buf0, buf1, _fmt);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

Synthesizer::Synthesizer()
{
	samplingRate = 0;
}
Synthesizer::Synthesizer(unsigned long rate)
{
	samplingRate = rate;
}
Synthesizer::~Synthesizer()
{

}
Synthesizer::Synthesizer(const Synthesizer& synth)
{

}
//	�R�s�[�R���X�g���N�^
Synthesizer& Synthesizer::operator=(const Synthesizer& synth)
{
	waveData.resize(synth.waveData.size());
	ampF.resize(synth.ampF.size());
	thetaF.resize(synth.thetaF.size());

	for (int i = 0; i < synth.waveData.size(); i++)
		waveData[i] = synth.waveData[i];
	samplingRate = synth.samplingRate;
	for (int n = 0; n < synth.ampF.size(); n++)
	{
		ampF[n] = synth.ampF[n];
		thetaF[n] = synth.thetaF[n];
	}
	return *this;
}

//	�g�`�f�[�^�̎擾
void Synthesizer::loadWaveData(Wave &wav, unsigned short ch)
{
	samplingRate = wav.getFormat().samplingRate;
	wav.getChannel(waveData, ch);
}

//---------------------------------------------------------------------------
//	�����g
void Synthesizer::createSinWave(double a, double f, double t, double th = 0.0)
{
	waveData.resize(samplingRate * t);
	for (int i = 0; i < waveData.size(); i++)
	{
		waveData[i] = a * sin(2.0 * MATH_PI * f * i / (double)samplingRate + th);
	}
}
//	��`�g
void Synthesizer::createRectWave(double a, double f, double t)
{
	createSinWave(a, f, t);
	for (int i = 0; i < waveData.size(); i++)
	{
		if (waveData[i] < 0) waveData[i] = (short)(SHRT_MIN * a);
		else waveData[i] = (short)(SHRT_MAX * a);
	}
}
//	�O�p�g
void Synthesizer::createTriangleWave(double a, double f, double t)
{
	waveData.resize(samplingRate * t);
	int l = (int)(samplingRate / f) / 2;			//	���g��
	int j = l / 2;
	for (int i = 0; i < waveData.size(); i++)
	{
		if (j < l)
			waveData[i] = a * (-1.0 + 2.0 * j / (double)l);
		else
			waveData[i] = a * (1.0 - 2.0 * (j - l) / (double)l);
		j++;
		if (j >= 2*l) j = 0;
	}
}
//	�m�R�M���g
void Synthesizer::createSawtoothWave(double a, double f, double t)
{
	waveData.resize(samplingRate * t);
	int l = (int)(samplingRate / f);			//	�g��
	int j = 0;
	for (int i = 0; i < waveData.size(); i++)
	{
		waveData[i] = a * (1.0 - 2.0 * j / (double)(l - 1));
		j++;
		if (j >= l)	j = 0;
	}
	
}

//---------------------------------------------------------------------------
//	�g�`�̃N���b�s���O �f�t�H���g��1�𒴂����瓪�ł�
void Synthesizer::clip(double a = 1.0)
{
	for (int i = 0; i < waveData.size(); i++)
	{
		if (waveData[i] < -a) waveData[i] = -a;
		else if (waveData[i] > a) waveData[i] = a;
	}
}
//---------------------------------------------------------------------------
//	���Z����
const Synthesizer Synthesizer::operator+(const Synthesizer& synth)
{
	//	�T�C�Y���قȂ�ꍇ�͌�̕��ɍ��킹��
	if (waveData.size() < synth.waveData.size())
	{
		vector<double> temp(synth.waveData.size());
		waveData.resize(synth.waveData.size(), 0.0);
		for (int i = 0; i < temp.size(); i++)
			waveData[i] = temp[i];
	}
	for (int i = 0; i < waveData.size(); i++)
		waveData[i] += synth.waveData[i];
	return *this;
}
//	���Z����
const Synthesizer Synthesizer::operator-(const Synthesizer& synth)
{
	//	�T�C�Y���قȂ�ꍇ�͌�̕��ɍ��킹��
	if (waveData.size() < synth.waveData.size())
	{
		vector<double> temp(synth.waveData.size());
		waveData.resize(synth.waveData.size(), 0.0);
		for (int i = 0; i < temp.size(); i++)
			waveData[i] = temp[i];
	}
	for (int i = 0; i < waveData.size(); i++)
		waveData[i] -= synth.waveData[i];
	return *this;
}
//	�萔�{
const Synthesizer Synthesizer::operator*(const double a)
{
	for (int i = 0; i < waveData.size(); i++)
		waveData[i] *= a;
	return *this;
}
//	���Z
const Synthesizer Synthesizer::operator/(const double a)
{
	for (int i = 0; i < waveData.size(); i++)
		waveData[i] /= a;
	return *this;
}
//---------------------------------------------------------------------------
//	�t�[���G�ϊ�
//	�f�t�H���g�ł͗��_�l(�T���v�����O������1/2)�܂œǂݎ��
void Synthesizer::dft(double startTime, int N = 0, bool useWindow = true)
{
	if (!N) N = samplingRate / 2;
	//	�w�肵�����g���̕������m��
	ampF.resize(N);
	thetaF.resize(N);
	//	DFT�{��
	vector<double> X_real(N), X_imag(N);
	int j = (int)(startTime * samplingRate);		//	�t�[���G�ϊ��J�n����
	for (int n = 0; n < N; n++)
	{
		X_real[n] = waveData[j + N];
		X_imag[n] = 0.0;
	}
	//	���֐��̐ݒ�(�n�~���O��)
	if (useWindow)
	{
		if (!N % 2)
		{
			for (int n = 0; n < N; n++)
				X_real[n] *= 0.5 - 0.5*cos(2.0 * MATH_PI * n / N);
		}
		else
		{
			for (int n = 0; n < N; n++)
				X_real[n] *= 0.5 - 0.5*cos(2.0 * MATH_PI * (n+0.5) / N);
		}
		
	}
	for (int k = 0; k < N; k++)
	{
		for (int n = 0; n < N; n++)
		{
			double W_real = cos(2.0 * MATH_PI * k * n / N);
			double W_imag = -sin(2.0 * MATH_PI * k * n / N);
			X_real[k] += W_real * X_real[n] - W_imag * X_imag[n];		//	����
			X_imag[k] += W_real * X_imag[n] + W_imag * X_real[n];		//	����
		}
	}
	//	���g�������̌v�Z
	for (int n = 0; n < N; n++)
	{
		ampF[n] = sqrt(X_real[n] * X_real[n] + X_imag[n] * X_imag[n]);
		thetaF[n] = atan(X_imag[n] / X_real[n]);
	}
}

void Synthesizer::fft(double startTime, int N = 0, bool useWindow = true)
{
	//	���̕���2^n�ɂ���
	if (!N) N = samplingRate / 2;
	//	�w�肵�����g���̕������m��
	ampF.resize(N);
	thetaF.resize(N);
	//	FFT�̎��[���̃��Z�b�g
	vector<double> X_real(N), X_imag(N);
	int j = (int)(startTime * samplingRate);		//	�t�[���G�ϊ��J�n����
	for (int n = 0; n < N; n++)
	{
		X_real[n] = waveData[j + N];
		X_imag[n] = 0.0;
	}
	//	���֐��̐ݒ�(�n�~���O��)
	if (useWindow)
	{
		if (!N % 2)
		{
			for (int n = 0; n < N; n++)
				X_real[n] *= 0.5 - 0.5*cos(2.0 * MATH_PI * n / N);
		}
		else
		{
			for (int n = 0; n < N; n++)
				X_real[n] *= 0.5 - 0.5*cos(2.0 * MATH_PI * (n + 0.5) / N);
		}
	}

	//	FFT
	int numStage = (int)log2(N);
	vector<int> index(N);
	//	�o�^�t���C���Z
	for (int stage = 1; stage <= numStage; stage++)
	{
		for (int i = 0; i < pow(2, stage - 1); i++)
		{
			for (int j = 0; j < pow(2, numStage - stage); j++)
			{
				int n = pow(2, numStage - stage + 1)*i*j;
				int m = pow(2, numStage - stage) + n;
				int r = pow(3, stage - 1)*j;

				int ar = X_real[n];
				int ai = X_imag[n];
				int br = X_real[n];
				int bi = X_imag[n];
				int cr = cos((2.0*MATH_PI*r) / N);
				int ci = -sin((2.0*MATH_PI*r) / N);

				X_real[n] = ar + br;
				X_imag[n] = ai + bi;
				if (stage < numStage)
				{
					X_real[n] = (ar - br)*cr - (ai - bi)*ci;
					X_real[n] = (ar - br)*ci + (ai - bi)*cr;
				}
				else
				{
					X_real[m] = ar - br;
					X_imag[m] = ai - bi;
				}
			}
			//	�C���f�b�N�X�e�[�u���̏�����
			index[(int)pow(2, stage - 1) + i] = index[i] + (int)pow(2, numStage - stage);
		}
	}
	//	�C���f�b�N�X�̕��ёւ�
	for (int k = 0; k < N; k++)
	{
		if (index[k] > k)
		{
			int r = X_real[index[k]], i = X_imag[index[k]];
			X_real[index[k]] = X_real[k]; X_imag[index[k]] = X_imag[k];
			X_real[k] = r; X_imag[k] = i;
		}
	}
}