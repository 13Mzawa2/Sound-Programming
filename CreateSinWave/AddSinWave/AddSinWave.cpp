#include <iostream>
#include <math.h>
#include "Wave.h"

int main(void)
{
	Wave		wav1;
	WaveFormat	fmt1;
	Synthesizer synth1;

	fmt1.samplingRate = 44100;
	fmt1.resolution = WAVE_RESOLUTION_16BIT;
	fmt1.numChannels = WAVE_CH_MONAURAL;

	synth1.samplingRate = fmt1.samplingRate;
	synth1.createRectWave(0.2, 880.0, 1.0);
	wav1.setChannel(synth1.waveData, fmt1);
	wav1.saveWave("test.wav");
	
	return 0;
}