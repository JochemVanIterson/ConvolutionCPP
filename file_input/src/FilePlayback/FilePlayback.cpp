#include "FilePlayback.h"
#include "../AudioFile/AudioFile.h"

#include <iostream>
#include <fstream>

// --------------------------------- PUBLIC --------------------------------- //
// ----------------------- Constructor and Destructor ----------------------- //
FilePlayback::FilePlayback(){}
FilePlayback::FilePlayback(const char* file){
  FilePlayback();
  openFile(file);
}
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
void FilePlayback::openFile(const char* file){
  this->file = file;
  if(!checkFileExists()){
    std::cout << "Error: File doesn't exist" <<  std::endl;
    return;
  }
  audioFile.load(file);
  filesize = audioFile.getNumSamplesPerChannel();
  numChannels = audioFile.getNumChannels();
}
void FilePlayback::printFileInfo(){
  if(checkFileExists())
  audioFile.printSummary();
}
int FilePlayback::getSampleRate(){
  return audioFile.getSampleRate();
}
AudioFile<double>::AudioBuffer FilePlayback::getBuffer(){
  return audioFile.samples;
}
void FilePlayback::fillBuffer(std::vector<jack_default_audio_sample_t*> outputBuffers, jack_nframes_t nframes, bool overwrite){
  for(unsigned int i = 0; i < nframes; i++) {
    for(int channel = 0; channel<numChannels; channel++){
      if(isPlaying && playHead<filesize){
        if(overwrite)outputBuffers[channel][i] = 0;
        outputBuffers[channel][i] = audioFile.samples[channel][playHead];
      } else if(!isPlaying){
        if(overwrite)outputBuffers[channel][i] = 0;
      } else if(playHead>=filesize){
        if(overwrite)outputBuffers[channel][i] = 0;
        if(!loopPlayback) isPlaying = false;
        playHead = 0;
      }
    }
    playHead++;
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
