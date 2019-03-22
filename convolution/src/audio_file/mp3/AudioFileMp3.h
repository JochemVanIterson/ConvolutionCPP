// SOURCE: https://www.mpg123.de

#ifndef  _ASAUDIOFILEMP3_H
#define _ASAUDIOFILEMP3_H

#define INBUFF 16384 * 2 * 2
#define WAVE_FORMAT_PCM 0x0001
#define WAVE_FORMAT_IEEE_FLOAT 0x0003

#include "../audio_file.h"

#include <mpg123.h>
#include <stdio.h>
#include <string>

class AudioFileMp3 : public AudioFile{
public:
  AudioFileMp3();
  AudioFileMp3(bool verbose);
  ~AudioFileMp3();

  bool load (std::string filePath);

private:
  bool verbose = true;

  mpg123_handle *mh;
  unsigned char *buffer;
  size_t buffer_size;
  size_t done;
  int err;

  int channels, encoding;
  long rate;
};

#endif /* end of include guard: _ASAUDIOFILEMP3_H */
