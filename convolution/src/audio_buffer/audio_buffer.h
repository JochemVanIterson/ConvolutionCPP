
#ifndef AUDIO_BUFFER_H
#define AUDIO_BUFFER_H

#include <string>

class AudioBuffer {
public:
  AudioBuffer();
  ~AudioBuffer();

  void openFile(std::string file);
  double atIndex(int channel, int index);
  int getNumChannels();
  int size();
  bool empty();

private:
  double* data;
  std::string file;
  int numChannels = 0;
  int bufSize = 0;
  int absBufSize = 0;

  bool checkFileExists();
};

#endif
