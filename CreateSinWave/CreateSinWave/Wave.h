#ifndef __WAVE_H
#define __WAVE_H

#include <vector>

//	�`�����l����
#define WAVE_CH_MONAURAL	1
#define WAVE_CH_STEREO		2

//	���]�����[�V����
#define WAVE_RESOLUTION_8BIT	8
#define WAVE_RESOLUTION_16BIT	16

//	�T���v�����O���[�g
#define WAVE_SAMPLING_44K1		44100
#define WAVE_SAMPLING_8K		8000

struct WaveFormat
{
	unsigned short	formatID;			//	PCM�Ȃ�1
	unsigned short	numChannels;		//	�`�����l���� WAVE_MONAURAL or WAVE_STEREO
	unsigned long	samplingRate;		//	�W�{�����g��
	unsigned long	bytesPerSec;		//	= blockSize * samplingRate
	unsigned short	blockSize;			//	= resolution * numChannels / 8
	unsigned short	resolution;			//	�ʎq�����x
};

class Wave
{
	//	�f�[�^�ւ̒��ڏ������݂͋֎~
private:
	WaveFormat format;
	std::vector<unsigned char> data;
	unsigned long samplingSize;
	unsigned long dataSize;

public:
	Wave();
	Wave(char *filename);
	~Wave();
	//	�N���X�̃R�s�[
	Wave(const Wave&);					//	�R�s�[�R���X�g���N�^
	Wave& operator=(const Wave&);		//	������Z�q( = )�̒�`

	bool loadWave(char *filename);
	bool saveWave(char *filename);
	void getChannel(std::vector<short> &buf, unsigned short ch);
	void setChannel(std::vector<short> &ch0, WaveFormat format);
	void setChannel(std::vector<short> &ch0, std::vector<short> &ch1 , WaveFormat fmt);

	WaveFormat		getFormat() { return format; }
	unsigned long	getSize()	{ return dataSize; }
	unsigned short	getChannels()	{ return format.numChannels; }
	unsigned long	getSamplingRate()	{ return format.samplingRate; }

};

#endif // !__WAVE_H
