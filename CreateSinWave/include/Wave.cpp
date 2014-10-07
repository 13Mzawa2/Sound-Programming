#include "Wave.h"

#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

//	チャンクのロード用構造体
struct WaveChunk
{
	char	ID[4];		//	チャンク種類	"RIFF", "fmt ", "data" のどれか
	int		size;		//	チャンクサイズ
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
//	wavファイルのロード
bool Wave::loadWave(char *filename)
{
	WaveChunk	chunk;
	char		type[4];

	ifstream	ifs(filename, ios::binary);

	data.clear();			//	データファイルの初期化
	dataSize = 0;

	//	ファイルが開けなかった場合の処理
	if (!ifs)
	{
		cerr << "File open error." << endl;
		cerr << "\t" << filename << endl;
		return false;
	}
	//	チャンク（フォーマットID, データサイズ）のロード
	//	RIFFフォーマットであることの確認 strncmp() ... 文字列比較
	ifs.read((char*)&chunk, 8);
	if (ifs.bad() || strncmp(chunk.ID, "RIFF", 4)) return false;
	//	RIFFの種類がWAVEであることの確認
	ifs.read((char*)type, 4);
	if (ifs.bad() || strncmp(type, "WAVE", 4)) return false;

	unsigned char flag = 0;

	try
	{
		//	例外設定
		ifs.exceptions(ios::badbit);

		while (ifs.read((char*)&chunk, sizeof(WaveChunk)))
		{
			//	fmtチャンクIDのロード
			if (strncmp(chunk.ID, "fmt ", 4))
			{
				//	リニアPCAのみ
				ifs.read((char*)&format, min(16, chunk.size));
				//	リニアPCAでなかったらエスケープ
				if (format.formatID != WAVE_FORMAT_PCM)
				{
					cerr << "unsupported wave file format." << endl;
					goto WAVE_FILE_ERROR;
				}
				flag++;
			}
			//	dataチャンクのロード
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
		ifs.seekg(chunk.size, ios::cur);	//	現在位置からチャンクサイズ分だけ移動
		goto WAVE_FILE_ERROR;
	}

	//	fmtチャンクとdataチャンクが両方読み込めたことを確認
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

//	wavファイルの保存
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

		//例外設定?
		ofs.exceptions(ios::badbit);

		//	RIFF情報の記述
		memcpy(chunk.ID, "RIFF", 4 * sizeof(char));
		chunk.size = sizeof(chunk)+4 + sizeof(WaveFormat)+data.size();
		ofs.write((char*)&chunk, sizeof(WaveChunk));
		ofs.write("WAVE", 4);
		//	fmtチャンクの記述
		memcpy(chunk.ID, "fmt ", 4 * sizeof(char));
		chunk.size = sizeof(WaveFormat);
		ofs.write((char*)&chunk, sizeof(WaveChunk));
		ofs.write((char*)&format, 16);
		//	dataチャンクの記述
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

//	波形データの取得（1チャンネルのみ）
void Wave::getChannel(vector<short> &channel, unsigned short ch)
{
	channel.resize(dataSize);		//	ベクトルをデータ格納可能状態にする
	//	モノラル信号の場合
	if (format.numChannels == WAVE_CH_MONAURAL)
	{
		if (format.resolution == WAVE_RESOLUTION_8BIT)
		{
			//8bitならデータは unsigned char (0～255 無音は 128)なので補正
			for (int t = 0; t < dataSize; t++)
				channel[t] = (short(data[t]) - 0x80) << 8;
		}
		else if (format.resolution == WAVE_RESOLUTION_16BIT)
		{
			//16bitならデータは signed short (-32768～+32767 無音は 0)
			short *ptr = (short*)&data[0];
			for (int t = 0; t < dataSize; t++) channel[t] = ptr[t];
		}
	}
	else if (format.numChannels == WAVE_CH_STEREO)
	{
		if ( ch > 1 ) ch = 1;

		if (format.resolution == WAVE_RESOLUTION_8BIT)
		{
			//8bitならデータは unsigned char (0～255 無音は 128)なので補正
			for (int t = 0; t < dataSize; t++)
				channel[t] = (short(data[2 * t + ch]) - 0x80) << 8;
		}
		else if (format.resolution == WAVE_RESOLUTION_16BIT)
		{
			//16bitならデータは signed short (-32768～+32767 無音は 0)
			short *ptr = (short*)&data[0];
			for (int t = 0; t<dataSize; t++) channel[t] = ptr[2 * t + ch];
		}
	}
}
//---------------------------------------------------------------------------
//	波形データのセット
//	データの書き込みはこの関数で行う
//	こちらはモノラル用
void Wave::setChannel(vector<short> &ch0, WaveFormat _fmt)
{
	format = _fmt;

	//	解像度不明のときは16bitとして処理
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
//	波形データのセット
//	こちらはステレオ用
void Wave::setChannel(vector<short> &ch0, vector<short> &ch1, WaveFormat _fmt)
{
	format = _fmt;

	if (format.resolution != WAVE_RESOLUTION_8BIT && format.resolution != WAVE_RESOLUTION_16BIT)
		format.resolution = WAVE_RESOLUTION_16BIT;

	//BytesPerSample
	unsigned short BPS = format.resolution / 8;

	//	フォーマットのセット
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

//	double型で扱うためのラッパー
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

void Synthesizer::loadWaveData(Wave &wav, unsigned short ch)
{
	samplingRate = wav.getFormat().samplingRate;
	wav.getChannel(waveData, ch);
}

//	波形生成
void Synthesizer::createSinWave(double a, double f, double t)
{
	waveData.resize(samplingRate * t);
	for (int i = 0; i < waveData.size(); i++)
	{
		waveData[i] = a * sin(2.0 * MATH_PI * f * i / (double)samplingRate);
	}
}
//	矩形波
void Synthesizer::createRectWave(double a, double f, double t)
{
	createSinWave(a, f, t);
	for (int i = 0; i < waveData.size(); i++)
	{
		if (waveData[i] < 0) waveData[i] = (short)(SHRT_MIN * a);
		else waveData[i] = (short)(SHRT_MAX * a);
	}
}
void Synthesizer::createTriangleWave(double a, double f, double t)
{
	waveData.resize(samplingRate * t);
	for (int i = 0; i <= samplingRate / 10; i++)
	{
		for (int j = 0; j < (int)(samplingRate / f); j++)
		{

		}
	}
}
//---------------------------------------------------------------------------
