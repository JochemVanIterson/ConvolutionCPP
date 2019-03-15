#include <iostream>
#include <math.h>
#include "direct_convolution.h"

DirectConvolution::DirectConvolution() {}

DirectConvolution::~DirectConvolution() {}

void DirectConvolution::setImpulseResponse(std::string file) {
  IR_Buf.openFile(file);
  int size = IR_Buf.size();
  if(size*IR_Buf.getNumChannels()>1500){
    std::cout << "Impulse file is larger than 1500 samples.\nOnly the first 1500 samples will be used" << '\n';
    size = 1500/IR_Buf.getNumChannels();
  }
  double totalAmound = 0;
  for(int walker = 0; walker<size; walker++){
    totalAmound+=IR_Buf.atIndex(0, walker);
  }
  std::cout << "totalAmound: " << totalAmound << '\n';
  normAmount = 1/totalAmound;
  std::cout << "normAmount: " << normAmount << '\n';
  inputBuf = new CircularBuffer<double>(size);
}

void DirectConvolution::convolute() {
  std::cout << "THREAD STARTED......." << '\n';
  int begin = 0;
  int end = 0;
  const int numChannels = IR_Buf.getNumChannels();
  std::cout << "numChannels: " << numChannels << '\n';
  while(active) {
    if(inQueue.size()>0 && !inQueueLock && !outQueueLock) {
      setOutQueueLock(true);
      setInQueueLock(true);
      inputBuf->push(inQueue.front());
      inQueue.pop();

      calcSamples(&begin, &end, numChannels);
      if(end < inputBuf->size()) {
        end++;
      }
      setOutQueueLock(false);
      setInQueueLock(false);
    }
  }
  std::cout << "while(active) false" << '\n';
  std::cout << "begin: " << begin << "\tend: " << end << '\n';
  while(begin < inputBuf->size()) {
    calcSamples(&begin, &end, numChannels);
    begin++;
  }
  std::cout << "THREAD STOPPED......." << '\n';
}

void DirectConvolution::calcSamples(int *begin, int *end, const int numChannels) {
  double sampleL = 0;
  double sampleR = 0;

  for(unsigned int i = (*begin); i < (*end); i++) {
    sampleL += inputBuf->atIndex(i) * IR_Buf.atIndex(0, (*end)-1 - i);
    if(numChannels > 1) {
      sampleR += inputBuf->atIndex(i) * IR_Buf.atIndex(1, (*end)-1 - i);
    }
  }
  if(numChannels==1){
    sampleR = sampleL;
  }
  outQueue.push(sampleL*normAmount);
  outQueue.push(sampleR*normAmount);
}

bool DirectConvolution::checkStartConditions() {
  if(IR_Buf.empty()) {
    std::cout << "Invalid start conditions: No impulse response loaded." << std::endl;
    return false;
  }
  return true;
}