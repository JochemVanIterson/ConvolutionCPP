
#include "audio_buffer.h"
#include "../audio_file/audio_file.h"
#include "../audio_file/wavaiff/AudioFileWav.h"
#include "../audio_file/mp3/AudioFileMp3.h"

#include <iostream>
#include <fstream>

AudioBuffer::AudioBuffer() {}

AudioBuffer::~AudioBuffer() {
  delete data;
}

void AudioBuffer::openFile(std::string file) {
  AudioFile* audioImport = nullptr;
  this->file = file;
  if(!checkFileExists()) {
    std::cout << "Error: File doesn't exist" << std::endl;
    return;
  }

  AudioFileFormat format = AudioFile::getAudioFormat(file);
  if(format == AudioFileFormat::Mp3)audioImport = new AudioFileMp3();
  else if(format == AudioFileFormat::Wave)audioImport = new AudioFileWav();
  else if(format == AudioFileFormat::Aiff)audioImport = new AudioFileWav();
  else {
    std::cout << "Impulse file wasn't an audio file.\nExit program" << '\n';
    exit(EXIT_FAILURE);
  }

  audioImport->load(file);
  numChannels = audioImport->getNumChannels();
  bufSize = audioImport->getNumSamplesPerChannel();
  absBufSize = bufSize * numChannels;
  data = new double[absBufSize];
  for(unsigned int i = 0; i < bufSize; i++) {
    for(unsigned int channel = 0; channel < numChannels; channel++) {
      data[i * numChannels + channel] = audioImport->samples[channel][i];
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
