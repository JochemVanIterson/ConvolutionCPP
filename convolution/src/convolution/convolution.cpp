
#include <iostream>
#include <thread>
#include "convolution.h"

Convolution::Convolution() {}
Convolution::Convolution(bool verbose) {
  this->verbose = verbose;
}

Convolution::~Convolution() {}

void Convolution::pushInput(std::vector<jack_default_audio_sample_t*> *inputBuffers, jack_nframes_t nframes) {
  if(active && !inQueueLock){
    inQueueLock = true;
    for(unsigned int i = 0; i < nframes; i++) {
      double sample = 0;
      for(unsigned int channel = 0; channel < inputBuffers->size(); channel++) {
        sample+=(*inputBuffers)[channel][i];
      }
      inQueue.push(sample);
    }
    inQueueLock = false;
  }
}

void Convolution::pullOutput(std::vector<jack_default_audio_sample_t*> *outputBuffers, jack_nframes_t nframes) {
  if(!outQueueLock){
    outQueueLock = true;
    for(unsigned int i = 0; i < nframes; i++) {
      for(unsigned int channel = 0; channel < outputBuffers->size(); channel++) {
        if(outQueue.size()>0) {
          outputBuffers->at(channel)[i] = outQueue.front();
          outQueue.pop();
        } else outputBuffers->at(channel)[i] = 0.;
      }
    }
    outQueueLock = false;
  }
}

void Convolution::start() {
  if(!active && checkStartConditions()) {
    active = true;
    calcThread = std::thread{&Convolution::convolute, this};
  }
}

void Convolution::stop() {
  if(active){
    active = false;
    calcThread.join();
  } else {
    std::cout << "Thread not running" << std::endl;
  }
}

void Convolution::setInQueueLock(bool inQueueLock){
  this->inQueueLock = inQueueLock;
}
void Convolution::setOutQueueLock(bool outQueueLock){
  this->outQueueLock = outQueueLock;
}
