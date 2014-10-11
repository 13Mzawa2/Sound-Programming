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
Synthesizer::Synthesizer(const Synthesizer& synth)
{

}
//	コピーコンストラクタ
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

//	波形データの取得
void Synthesizer::loadWaveData(Wave &wav, unsigned short ch)
{
	samplingRate = wav.getFormat().samplingRate;
	wav.getChannel(waveData, ch);
}

//---------------------------------------------------------------------------
//	正弦波
void Synthesizer::createSinWave(double a, double f, double t, double th = 0.0)
{
	waveData.resize(samplingRate * t);
	for (int i = 0; i < waveData.size(); i++)
	{
		waveData[i] = a * sin(2.0 * MATH_PI * f * i / (double)samplingRate + th);
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
//	三角波
void Synthesizer::createTriangleWave(double a, double f, double t)
{
	waveData.resize(samplingRate * t);
	int l = (int)(samplingRate / f) / 2;			//	半波長
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
//	ノコギリ波
void Synthesizer::createSawtoothWave(double a, double f, double t)
{
	waveData.resize(samplingRate * t);
	int l = (int)(samplingRate / f);			//	波長
	int j = 0;
	for (int i = 0; i < waveData.size(); i++)
	{
		waveData[i] = a * (1.0 - 2.0 * j / (double)(l - 1));
		j++;
		if (j >= l)	j = 0;
	}
	
}

//---------------------------------------------------------------------------
//	波形のクリッピング デフォルトは1を超えたら頭打ち
void Synthesizer::clip(double a = 1.0)
{
	for (int i = 0; i < waveData.size(); i++)
	{
		if (waveData[i] < -a) waveData[i] = -a;
		else if (waveData[i] > a) waveData[i] = a;
	}
}
//---------------------------------------------------------------------------
//	加算合成
const Synthesizer Synthesizer::operator+(const Synthesizer& synth)
{
	//	サイズが異なる場合は後の方に合わせる
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
//	減算合成
const Synthesizer Synthesizer::operator-(const Synthesizer& synth)
{
	//	サイズが異なる場合は後の方に合わせる
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
//	定数倍
const Synthesizer Synthesizer::operator*(const double a)
{
	for (int i = 0; i < waveData.size(); i++)
		waveData[i] *= a;
	return *this;
}
//	除算
const Synthesizer Synthesizer::operator/(const double a)
{
	for (int i = 0; i < waveData.size(); i++)
		waveData[i] /= a;
	return *this;
}
//---------------------------------------------------------------------------
//	フーリエ変換
//	デフォルトでは理論値(サンプリング周期の1/2)まで読み取る
void Synthesizer::dft(double startTime, int N = 0, bool useWindow = true)
{
	if (!N) N = samplingRate / 2;
	//	指定した周波数の分だけ確保
	ampF.resize(N);
	thetaF.resize(N);
	//	DFT本体
	vector<double> X_real(N), X_imag(N);
	int j = (int)(startTime * samplingRate);		//	フーリエ変換開始時間
	for (int n = 0; n < N; n++)
	{
		X_real[n] = waveData[j + N];
		X_imag[n] = 0.0;
	}
	//	窓関数の設定(ハミング窓)
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
			X_real[k] += W_real * X_real[n] - W_imag * X_imag[n];		//	実部
			X_imag[k] += W_real * X_imag[n] + W_imag * X_real[n];		//	虚部
		}
	}
	//	周波数特性の計算
	for (int n = 0; n < N; n++)
	{
		ampF[n] = sqrt(X_real[n] * X_real[n] + X_imag[n] * X_imag[n]);
		thetaF[n] = atan(X_imag[n] / X_real[n]);
	}
}

void Synthesizer::fft(double startTime, int N = 0, bool useWindow = true)
{
	//	窓の幅を2^nにする
	if (!N) N = samplingRate / 2;
	//	指定した周波数の分だけ確保
	ampF.resize(N);
	thetaF.resize(N);
	//	FFTの収納部のリセット
	vector<double> X_real(N), X_imag(N);
	int j = (int)(startTime * samplingRate);		//	フーリエ変換開始時間
	for (int n = 0; n < N; n++)
	{
		X_real[n] = waveData[j + N];
		X_imag[n] = 0.0;
	}
	//	窓関数の設定(ハミング窓)
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
	//	バタフライ演算
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
			//	インデックステーブルの初期化
			index[(int)pow(2, stage - 1) + i] = index[i] + (int)pow(2, numStage - stage);
		}
	}
	//	インデックスの並び替え
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