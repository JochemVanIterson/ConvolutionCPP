
#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <queue>
#include <thread>
#include "../jack/jack_module.h"

class Convolution {
protected:
  Convolution();
  Convolution(bool verbose);
  virtual ~Convolution();
  bool active = false;
  std::queue<double> inQueue;
  std::queue<double> outQueue;
  bool inQueueLock = false;
  bool outQueueLock = false;
  bool verbose = false;

public:
  void pushInput(std::vector<jack_default_audio_sample_t*> *inputBuffers, jack_nframes_t nframes);
  void pullOutput(std::vector<jack_default_audio_sample_t*> *outputBuffers, jack_nframes_t nframes);

  virtual void setImpulseResponse(std::string file) = 0;
  void start();
  void stop();
  void setInQueueLock(bool inQueueLock);
  void setOutQueueLock(bool outQueueLock);

private:
  virtual void convolute() = 0;
  virtual bool checkStartConditions() = 0;
  std::thread calcThread;
};

#endif
