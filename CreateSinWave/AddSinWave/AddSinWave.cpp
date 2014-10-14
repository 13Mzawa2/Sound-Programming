#include <iostream>
#include <math.h>
#include "Wave.h"

int main(void)
{
	Wave		wav1;
	WaveFormat	fmt1;
	Synthesizer synth1, synth2, synth3;

	fmt1.samplingRate = 44100;
	fmt1.resolution = WAVE_RESOLUTION_16BIT;
	fmt1.numChannels = WAVE_CH_MONAURAL;

	synth1.samplingRate = fmt1.samplingRate;
	synth2.samplingRate = fmt1.samplingRate;
	synth1.createSawtoothWave(0.2, 250.0, 1.0);
	synth2.createSawtoothWave(0.2, 500.0, 1.0);
	synth3 = (synth1 + synth2);
	synth3 = synth3 / 2.0;
	wav1.setChannel(synth3.waveData, fmt1);
	wav1.saveWave("test.wav");
	
	return 0;
}