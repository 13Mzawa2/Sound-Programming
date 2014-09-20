#ifndef __WAVE_H
#define __WAVE_H

#include <vector>


#define WAVE_MONAURAL	1
#define WAVE_STEREO		2

struct WaveFormat
{
	unsigned short	formatID;
	unsigned short	numChannels;
	unsigned long	samplingRate;
	unsigned long	bytesPerSec;
	unsigned short	blockSize;
	unsigned short	resolution;
};

class Wave
{
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
	Wave& operater=(const Wave&);		//	代入演算子( = )の定義

	bool loadWave(char *filename);
	bool saveWave(char *filename);
	void getChannel(std::vector<short> &buf, unsigned short ch);
	void setChannel(std::vector<short> &ch0, WaveFormat fmt);
	void setChannel(std::vector<short> &ch0, std::vector<short> &ch1 , WaveFormat fmt);

	WaveFormat		getFormat() { return fmt; }
	unsigned long	getSize()	{ return dataSize; }
	unsigned short	getChannels()	{ return fmt.numChannels; }
	unsigned long	getSamplingRate()	{ return fmt.samplingRate; }

};

#endif // !__WAVE_H
