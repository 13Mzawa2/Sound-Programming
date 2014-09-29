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

Wave::Wave()
{

}

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
		cerr << "\t" << string(filename) << endl;
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
				if (format.formatID != 1)
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

bool Wave::saveWave(char *filename)
{
	ofstream ofs(filename, ios::binary);

	if (!ofs)
	{
		cerr << "file open error." << endl;
		cerr << "\t" << string(filename) << endl;
		return false;
	}

	try
	{
		WaveChunk chunk;

		//��O�ݒ�?
		ofs.exceptions(ios::badbit);

		//	RIFF���̋L�q
		strncpy(chunk.ID, "RIFF", 4);
		chunk.size = sizeof(chunk)+4 + sizeof(WaveFormat)+data.size();
		ofs.write((char*)&chunk, sizeof(WaveChunk));
		ofs.write("WAVE", 4);
		//	fmt�`�����N�̋L�q
		strncpy(chunk.ID, "fmt ", 4);
		chunk.size = sizeof(WaveFormat);
		ofs.write((char*)&chunk, sizeof(WaveChunk));
		ofs.write((char*)&format, 16);
		//	data�`�����N�̋L�q
		strncpy(chunk.ID, "data", 4);
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
void Wave::setChannel(std::vector<short> &ch0, WaveFormat _fmt)
{
	format = _fmt;

	if (format.resolution != WAVE_RESOLUTION_8BIT && format.resolution != WAVE_RESOLUTION_16BIT)
		format.resolution = WAVE_RESOLUTION_16BIT;

	//BytesPerSample
	unsigned short BPS = format.resolution / 8;

	format.formatID = 1;
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
//---------------------------------------------------------------------------
void WAVE::set_channel(std::vector<short> &ch0, std::vector<short> &ch1, WAVE_FORMAT _fmt)
{
	fmt = _fmt;

	if (fmt.bits_per_sample != 8 && fmt.bits_per_sample != 16) fmt.bits_per_sample = 16;

	//BytesPerSample
	unsigned short BPS = fmt.bits_per_sample / 8;

	fmt.format_id = 1;
	fmt.num_of_channels = 2;
	fmt.block_size = fmt.num_of_channels*BPS;
	fmt.bytes_per_sec = fmt.samples_per_sec*fmt.block_size;

	sampling_size = BPS*fmt.num_of_channels;
	data_size = min(ch0.size(), ch1.size());

	data.resize(sampling_size*ch0.size());

	if (BPS == 1){
		for (int t = 0; t<data_size; t++){
			data[2 * t] = (ch0[t] >> 8) + 0x80;
			data[2 * t + 1] = (ch1[t] >> 8) + 0x80;
		}
	}
	else if (BPS == 2){
		short *ptr = (short*)&data[0];
		for (int t = 0; t<data_size; t++){
			ptr[2 * t] = ch0[t];
			ptr[2 * t + 1] = ch1[t];
		}
	}
}
//---------------------------------------------------------------------------