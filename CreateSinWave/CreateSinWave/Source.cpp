
//---------------------------------------------------------------------------
#include <iostream>
#include <math.h>
#include "Wave.h"

using namespace std;
#define MATH_PI		atan(1.0)*4
//---------------------------------------------------------------------------

int main(void)
{
	Wave		wav1;
	WaveFormat	fmt1;

	fmt1.samplingRate = 44100;
	fmt1.resolution = WAVE_RESOLUTION_16BIT;
	fmt1.numChannels = WAVE_CH_MONAURAL;
	
	vector<double>	wavedata(fmt1.samplingRate * 1);
	vector<short> wave16(wavedata.size());
	double a = 0.2;			//	êUïù
	double f0 = 880.0;		//	é¸îgêî
	for (int t = 0; t < wavedata.size(); t++)
	{
		wavedata[t] = a * sin(2.0 * MATH_PI * f0 * t / fmt1.samplingRate);
		wave16[t] = (short)(wavedata[t] * SHRT_MAX);
	}
	wav1.setChannel(wave16, fmt1);
	wav1.saveWave("test.wav");

	return 0;
}