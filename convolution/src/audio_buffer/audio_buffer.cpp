
#include "audio_buffer.h"
#include "../audio_file/audio_file.h"

#include <iostream>
#include <fstream>

AudioBuffer::AudioBuffer() {}

AudioBuffer::~AudioBuffer() {
  delete data;
}

void AudioBuffer::openFile(std::string file) {
  AudioFile<double> audioImport;
  this->file = file;
  if(!checkFileExists()) {
    std::cout << "Error: File doesn't exist" << std::endl;
    return;
  }

  audioImport.load(file);
  numChannels = audioImport.getNumChannels();
  bufSize = audioImport.getNumSamplesPerChannel();
  absBufSize = bufSize * numChannels;
  data = new double[absBufSize];
  for(unsigned int i = 0; i < bufSize; i++) {
    for(unsigned int channel = 0; channel < numChannels; channel++) {
      data[i * numChannels + channel] = audioImport.samples[channel][i];
    }
  }
}

bool AudioBuffer::checkFileExists() {
  std::ifstream infile(this->file);
  return infile.good();
}

double AudioBuffer::atIndex(int channel, int index) {
  if(channel < 0 || channel >= numChannels) {
    std::cerr << "Error: Buffer channel out of range." << '\n';
  }
  if(index < 0) {
    std::cerr << "Error: Buffer index can't be smaller than 0" << '\n';
  }
  if(index >= bufSize) {
    std::cerr << "Error: Buffer index out of range. index:" << index << "\tbufSize" << bufSize << '\n';
  }
  return data[index * numChannels + channel];
}

int AudioBuffer::getNumChannels() {
  return numChannels;
}

int AudioBuffer::size() {
  return bufSize;
}

bool AudioBuffer::empty() {
  return bufSize <= 0;
}
