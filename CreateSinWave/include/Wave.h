/*==========================================================*/
/*				�g�`�f�[�^���o�́E����p�N���X				*/
/*==========================================================*/

#ifndef __WAVE_H
#define __WAVE_H

#include <vector>

//	�t�H�[�}�b�gID
#define WAVE_FORMAT_PCM		1

//	�`�����l����
#define WAVE_CH_MONAURAL	1
#define WAVE_CH_STEREO		2

//	���]�����[�V����
#define WAVE_RESOLUTION_8BIT	8
#define WAVE_RESOLUTION_16BIT	16

//	�T���v�����O���[�g
#define WAVE_SAMPLING_44K1		44100
#define WAVE_SAMPLING_8K		8000

#define MATH_PI		atan(1.0)*4

struct WaveFormat
{
	unsigned short	formatID;			//	PCM�Ȃ�1
	unsigned short	numChannels;		//	�`�����l���� WAVE_MONAURAL or WAVE_STEREO
	unsigned long	samplingRate;		//	�W�{�����g��
	unsigned long	bytesPerSec;		//	= blockSize * samplingRate
	unsigned short	blockSize;			//	= resolution * numChannels / 8
	unsigned short	resolution;			//	�ʎq�����x
};

//	wav�t�@�C���Ǎ��E�����N���X
//	http://d.hatena.ne.jp/colorcle/20100203/1265209117�@�Q��
class Wave
{
	//	�f�[�^�ւ̒��ڏ������݂͋֎~
private:
	WaveFormat format;
	std::vector<unsigned char> data;	//	�g�`�̓����f�[�^�{�� [-32768, 32767]
	unsigned long samplingSize;
	unsigned long dataSize;

public:
	Wave();
	Wave(char *filename);
	~Wave();
	Wave(const Wave&);					//	�R�s�[�R���X�g���N�^
	Wave& operator=(const Wave&);		//	������Z�q( = )�̒�`

	//	�t�@�C�����o��
	bool loadWave(char *filename);
	bool saveWave(char *filename);

	//	�f�[�^�̏����o�� �����I��i�ƕt����Ɛ����l�ŕԂ�
	void setChannel(std::vector<short> &ch0, WaveFormat fmt);
	void setChannel(std::vector<short> &ch0, std::vector<short> &ch1, WaveFormat fmt);
	void setChannel(std::vector<double> &ch0, WaveFormat fmt);
	void setChannel(std::vector<double> &ch0, std::vector<double> &ch1, WaveFormat fmt);

	//	�f�[�^�̎��o��
	void getChannel(std::vector<short> &buf, unsigned short ch);
	void getChannel(std::vector<double> &buf, unsigned short ch);
	WaveFormat		getFormat() { return format; }
	unsigned long	getSize()	{ return dataSize; }
	unsigned short	getChannels()	{ return format.numChannels; }
	unsigned long	getSamplingRate()	{ return format.samplingRate; }

};

//	�g�`�f�[�^�̌v�Z�p�N���X
//	���̃N���X1��1��̔g�`�f�[�^�������Ƃ��ł���
class Synthesizer
{
public:
	Synthesizer();
	Synthesizer(unsigned long rate);
	~Synthesizer(); 
	Synthesizer(const Synthesizer&);
	Synthesizer& operator=(const Synthesizer&);

	//	�V���Z�T�C�U���ł͂��ׂĂ̔g�`�f�[�^��[-1, 1]�̎����ɔ[�߂�
	std::vector<double>		waveData;
	unsigned long			samplingRate;

	void loadWaveData(Wave &wav, unsigned short ch);		//	Wave�N���X����g�`�f�[�^��Ǎ�
	void createSinWave(double a, double f, double t);		//	�U��a��[0, 1�j�C���g��f[Hz]�C����t[s]�̐����g�̐���
	void createRectWave(double a, double f, double t);		//	��`�g����
	void createTriangleWave(double a, double f, double t);	//	�O�p�g����	
};

#endif // !__WAVE_H
