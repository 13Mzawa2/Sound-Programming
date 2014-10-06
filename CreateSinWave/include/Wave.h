/*==========================================================*/
/*				波形データ入出力・操作用クラス				*/
/*==========================================================*/

#ifndef __WAVE_H
#define __WAVE_H

#include <vector>

//	フォーマットID
#define WAVE_FORMAT_PCM		1

//	チャンネル数
#define WAVE_CH_MONAURAL	1
#define WAVE_CH_STEREO		2

//	レゾリューション
#define WAVE_RESOLUTION_8BIT	8
#define WAVE_RESOLUTION_16BIT	16

//	サンプリングレート
#define WAVE_SAMPLING_44K1		44100
#define WAVE_SAMPLING_8K		8000

#define MATH_PI		atan(1.0)*4

struct WaveFormat
{
	unsigned short	formatID;			//	PCMなら1
	unsigned short	numChannels;		//	チャンネル数 WAVE_MONAURAL or WAVE_STEREO
	unsigned long	samplingRate;		//	標本化周波数
	unsigned long	bytesPerSec;		//	= blockSize * samplingRate
	unsigned short	blockSize;			//	= resolution * numChannels / 8
	unsigned short	resolution;			//	量子化精度
};

//	wavファイル読込・書込クラス
//	http://d.hatena.ne.jp/colorcle/20100203/1265209117　参照
class Wave
{
	//	データへの直接書き込みは禁止
private:
	WaveFormat format;
	std::vector<unsigned char> data;	//	波形の内部データ本体 [-32768, 32767]
	unsigned long samplingSize;
	unsigned long dataSize;

public:
	Wave();
	Wave(char *filename);
	~Wave();
	Wave(const Wave&);					//	コピーコンストラクタ
	Wave& operator=(const Wave&);		//	代入演算子( = )の定義

	//	ファイル入出力
	bool loadWave(char *filename);
	bool saveWave(char *filename);

	//	データの書き出し 明示的にiと付けると整数値で返す
	void setChannel(std::vector<short> &ch0, WaveFormat fmt);
	void setChannel(std::vector<short> &ch0, std::vector<short> &ch1, WaveFormat fmt);
	void setChannel(std::vector<double> &ch0, WaveFormat fmt);
	void setChannel(std::vector<double> &ch0, std::vector<double> &ch1, WaveFormat fmt);

	//	データの取り出し
	void getChannel(std::vector<short> &buf, unsigned short ch);
	void getChannel(std::vector<double> &buf, unsigned short ch);
	WaveFormat		getFormat() { return format; }
	unsigned long	getSize()	{ return dataSize; }
	unsigned short	getChannels()	{ return format.numChannels; }
	unsigned long	getSamplingRate()	{ return format.samplingRate; }

};

//	波形データの計算用クラス
//	このクラス1個で1列の波形データを持つことができる
class Synthesizer
{
public:
	Synthesizer();
	Synthesizer(unsigned long rate);
	~Synthesizer(); 
	Synthesizer(const Synthesizer&);
	Synthesizer& operator=(const Synthesizer&);

	//	シンセサイザ内ではすべての波形データを[-1, 1]の実数に納める
	std::vector<double>		waveData;
	unsigned long			samplingRate;

	void loadWaveData(Wave &wav, unsigned short ch);		//	Waveクラスから波形データを読込
	void createSinWave(double a, double f, double t);		//	振幅a∈[0, 1），周波数f[Hz]，時間t[s]の正弦波の生成
	void createRectWave(double a, double f, double t);		//	矩形波生成
	void createTriangleWave(double a, double f, double t);	//	三角波生成	
};

#endif // !__WAVE_H
