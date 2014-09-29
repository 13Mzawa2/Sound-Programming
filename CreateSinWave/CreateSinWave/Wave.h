#ifndef __WAVE_H
#define __WAVE_H

#include <vector>

//	チャンネル数
#define WAVE_CH_MONAURAL	1
#define WAVE_CH_STEREO		2

//	レゾリューション
#define WAVE_RESOLUTION_8BIT	8
#define WAVE_RESOLUTION_16BIT	16

//	サンプリングレート
#define WAVE_SAMPLING_44K1		44100
#define WAVE_SAMPLING_8K		8000

struct WaveFormat
{
	unsigned short	formatID;			//	PCMなら1
	unsigned short	numChannels;		//	チャンネル数 WAVE_MONAURAL or WAVE_STEREO
	unsigned long	samplingRate;		//	標本化周波数
	unsigned long	bytesPerSec;		//	= blockSize * samplingRate
	unsigned short	blockSize;			//	= resolution * numChannels / 8
	unsigned short	resolution;			//	量子化精度
};

class Wave
{
	//	データへの直接書き込みは禁止
private:
	WaveFormat format;
	std::vector<unsigned char> data;
	unsigned long samplingSize;
	unsigned long dataSize;

public:
	Wave();
	Wave(char *filename);
	~Wave();
	//	クラスのコピー
	Wave(const Wave&);					//	コピーコンストラクタ
	Wave& operator=(const Wave&);		//	代入演算子( = )の定義

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
