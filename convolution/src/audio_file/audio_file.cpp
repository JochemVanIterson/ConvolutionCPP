#include "audio_file.h"
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <iterator>

//=============================================================
AudioFile::AudioFile() {
  bitDepth = 16;
  sampleRate = 44100;
  samples.resize (1);
  samples[0].resize (0);
  audioFileFormat = AudioFileFormat::NotLoaded;
}
AudioFile::~AudioFile() {}

//=============================================================
uint32_t AudioFile::getSampleRate() const {
  return sampleRate;
}

//=============================================================
int AudioFile::getNumChannels() const {
  return (int)samples.size();
}

//=============================================================
bool AudioFile::isMono() const {
  return getNumChannels() == 1;
}

//=============================================================
bool AudioFile::isStereo() const {
  return getNumChannels() == 2;
}

//=============================================================
int AudioFile::getBitDepth() const {
  return bitDepth;
}

//=============================================================
int AudioFile::getNumSamplesPerChannel() const {
  if (samples.size() > 0)
    return (int) samples[0].size();
  else
    return 0;
}

//=============================================================
double AudioFile::getLengthInSeconds() const {
  return (double)getNumSamplesPerChannel() / (double)sampleRate;
}

//=============================================================
void AudioFile::printSummary() const {
  std::cout << "|======================================|" << std::endl;
  std::cout << "Num Channels: " << getNumChannels() << std::endl;
  std::cout << "Num Samples Per Channel: " << getNumSamplesPerChannel() << std::endl;
  std::cout << "Sample Rate: " << sampleRate << std::endl;
  std::cout << "Bit Depth: " << bitDepth << std::endl;
  std::cout << "Length in Seconds: " << getLengthInSeconds() << std::endl;
  std::cout << "|======================================|" << std::endl;
}

//=============================================================
bool AudioFile::setAudioBuffer (AudioBuffer& newBuffer) {
  int numChannels = (int)newBuffer.size();

  if (numChannels <= 0) {
    assert (false && "The buffer your are trying to use has no channels");
    return false;
  }

  int numSamples = (int)newBuffer[0].size();

  // set the number of channels
  samples.resize (newBuffer.size());

  for (int k = 0; k < getNumChannels(); k++) {
    assert (newBuffer[k].size() == numSamples);

    samples[k].resize (numSamples);

    for (int i = 0; i < numSamples; i++) {
      samples[k][i] = newBuffer[k][i];
    }
  }

  return true;
}

//=============================================================
void AudioFile::setAudioBufferSize (int numChannels, int numSamples) {
  samples.resize (numChannels);
  setNumSamplesPerChannel (numSamples);
}

//=============================================================
void AudioFile::setNumSamplesPerChannel (int numSamples) {
  int originalSize = getNumSamplesPerChannel();

  for (int i = 0; i < getNumChannels();i++) {
    samples[i].resize (numSamples);

    // set any new samples to zero
    if (numSamples > originalSize)
      std::fill (samples[i].begin() + originalSize, samples[i].end(), (double)0.);
  }
}

//=============================================================
void AudioFile::setNumChannels (int numChannels) {
  int originalNumChannels = getNumChannels();
  int originalNumSamplesPerChannel = getNumSamplesPerChannel();

  samples.resize (numChannels);

  // make sure any new channels are set to the right size
  // and filled with zeros
  if (numChannels > originalNumChannels) {
    for (int i = originalNumChannels; i < numChannels; i++) {
      samples[i].resize (originalNumSamplesPerChannel);
      std::fill (samples[i].begin(), samples[i].end(), (double)0.);
    }
  }
}

//=============================================================
void AudioFile::setBitDepth (int numBitsPerSample) {
  bitDepth = numBitsPerSample;
}

//=============================================================
void AudioFile::setSampleRate (uint32_t newSampleRate) {
  sampleRate = newSampleRate;
}

//=============================================================
bool AudioFile::load (std::string filePath){return false;}

//=============================================================
void AudioFile::clearAudioBuffer() {
  for (int i = 0; i < samples.size();i++) {
    samples[i].clear();
  }

  samples.clear();
}

AudioFileFormat AudioFile::getAudioFormat(std::string filePath){
  std::string extension = filePath.substr(filePath.find_last_of(".") + 1);
  if(extension=="wav")return AudioFileFormat::Wave;
  else if(extension=="aiff")return AudioFileFormat::Aiff;
  else if(extension=="mp3")return AudioFileFormat::Mp3;
  return AudioFileFormat::Error;
}
