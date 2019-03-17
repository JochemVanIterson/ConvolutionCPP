
#ifndef DIRECT_CONVOLUTION_H
#define DIRECT_CONVOLUTION_H

#include <string>

#include "../../audio_buffer/audio_buffer.h"
#include "../../circular_buffer/circular_buffer.h"
#include "../convolution.h"

class DirectConvolution : public Convolution {
public:
  DirectConvolution();
  DirectConvolution(bool verbose);
  ~DirectConvolution();

  void setImpulseResponse(std::string file) override;

private:
  void convolute() override;
  void calcSamples(int *begin, int *end, const int numChannels);
  bool checkStartConditions() override;
  CircularBuffer<double>* inputBuf;
  AudioBuffer IR_Buf;
  double normAmount;
};

#endif
