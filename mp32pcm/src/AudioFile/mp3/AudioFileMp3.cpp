#include "AudioFileMp3.h"

AudioFileMp3::AudioFileMp3(){
  mpg123_init();

  mh = mpg123_new(NULL, &err);
  if(mh == NULL){
		std::cerr << "Unable to create mpg123 handle:\n" << mpg123_plain_strerror(err) << '\n';
    return;
  }

  buffer_size = mpg123_outblock(mh);
  buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

  if(verbose)std::cout << "mpg123 init done" << '\n';
}
AudioFileMp3::~AudioFileMp3(){}

bool AudioFileMp3::load(std::string filePath){
  if(verbose)std::cout << "FilePath: " << filePath << '\n';

  mpg123_open(mh, filePath.c_str());
  mpg123_getformat(mh, &rate, &channels, &encoding);
  samples.resize(channels);
  sampleRate = rate;

  unsigned int counter = 0;
  for (int totalBtyes = 0; mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK; ) {
    short* tst = reinterpret_cast<short*>(buffer);
    for (auto i = 0; i < buffer_size / 2; i++) {
      // std::cout<< counter + i<<"\t"<< tst[i] << "\n";
      samples[i%channels].push_back(tst[i]*0.00005 );
    }
    counter += buffer_size/2;
    totalBtyes += done;
  }

  free(buffer);
  mpg123_close(mh);
  mpg123_delete(mh);
  mpg123_exit();

  return true;
}
