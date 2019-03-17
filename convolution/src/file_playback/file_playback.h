#include "../audio_file/audio_file.h"
#include "../jack/jack_module.h"

#include <string>

class FilePlayback {
public:
  // ---------------------- Constructor and Destructor ---------------------- //
  FilePlayback();
  FilePlayback(std::string file);
  ~FilePlayback();

  // ----------------------------- Play actions ----------------------------- //
  void play();
  void pauze();
  void stop();

  // ---------------------------- AudioFile pipe ---------------------------- //
  void openFile(std::string file);
  void printFileInfo();
  int getSampleRate();
  AudioFile<double>::AudioBuffer getBuffer(); // Actually AudioBuffer is just a vector of vectors
  void fillBuffer(std::vector<jack_default_audio_sample_t*> *outputBuffers, jack_nframes_t nframes, bool overwrite);

  // ------------------------- Getters and setters -------------------------- //
  void setIsPlaying(bool isPlaying);
  bool getIsPlaying();
  void setLoop(bool loopPlayback);
  bool getLoop();

private:
  // ------------------------------ AudioFile ------------------------------- //
  std::string file;
  AudioFile<double> audioFile;
  long filesize = 0;
  int numChannels = 0;

  // ---------------------------- Play variables ---------------------------- //
  bool isPlaying = false;
  bool loopPlayback = false;
  long playHead = 0;

  // ------------------------------ Functions ------------------------------- //
  bool checkFileExists();
};
