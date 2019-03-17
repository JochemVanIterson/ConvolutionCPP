#include "FilePlayback.h"
#include "../AudioFile/wavaiff/AudioFileWav.h"
#include "../AudioFile/mp3/AudioFileMp3.h"

#include <iostream>
#include <fstream>

// --------------------------------- PUBLIC --------------------------------- //
// ----------------------- Constructor and Destructor ----------------------- //
FilePlayback::FilePlayback(){}
FilePlayback::~FilePlayback(){}

// ------------------------------ Play actions ------------------------------ //
void FilePlayback::play(){
  isPlaying = true;
}
void FilePlayback::pauze(){
  isPlaying = false;
}
void FilePlayback::stop(){
  isPlaying = false;
  playHead = 0;
}

// ----------------------------- AudioFile pipe ----------------------------- //
void FilePlayback::openFile(std::string file){
  this->file = file;
  if(!checkFileExists()){
    std::cout << "Error: File doesn't exist" <<  std::endl;
    return;
  }

  AudioFileFormat format = AudioFile::getAudioFormat(file);
  if(format == AudioFileFormat::Mp3)audioFile = new AudioFileMp3();
  else if(format == AudioFileFormat::Wave)audioFile = new AudioFileWav();
  else if(format == AudioFileFormat::Aiff)audioFile = new AudioFileWav();

  audioFile->load(file);
  filesize = audioFile->getNumSamplesPerChannel();
  numChannels = audioFile->getNumChannels();
}
void FilePlayback::printFileInfo(){
  if(checkFileExists())
  audioFile->printSummary();
}
int FilePlayback::getSampleRate(){
  return audioFile->getSampleRate();
}
AudioFile::AudioBuffer FilePlayback::getBuffer(){
  return audioFile->samples;
}
void FilePlayback::fillBuffer(std::vector<jack_default_audio_sample_t*> *outputBuffers, jack_nframes_t nframes, bool overwrite){
  for(unsigned int i = 0; i < nframes; i++) {
    for(int channel = 0; channel<numChannels && channel<outputBuffers->size(); channel++){
      if(isPlaying && playHead<filesize){
        if(overwrite)(*outputBuffers)[channel][i] = 0;
        (*outputBuffers)[channel][i] += audioFile->samples[channel][playHead];
      } else if(!isPlaying){
        if(overwrite)(*outputBuffers)[channel][i] = 0;
      } else if(playHead>=filesize){
        if(overwrite)(*outputBuffers)[channel][i] = 0;
        if(!loopPlayback) isPlaying = false;
        playHead = 0;
      }
    }
    if(isPlaying)playHead++;
  }
}

// -------------------------- Getters and setters --------------------------- //
void FilePlayback::setIsPlaying(bool isPlaying){
  this->isPlaying = isPlaying;
}
bool FilePlayback::getIsPlaying(){
  return isPlaying;
}
void FilePlayback::setLoop(bool loopPlayback){
  this->loopPlayback = loopPlayback;
}
bool FilePlayback::getLoop(){
  return loopPlayback;
}


// -------------------------------- PRIVATE --------------------------------- //
// ------------------------------- Functions -------------------------------- //
bool FilePlayback::checkFileExists(){
  std::ifstream infile(file);
  return infile.good();
}
