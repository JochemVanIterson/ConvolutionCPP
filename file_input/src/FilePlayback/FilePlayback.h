#include "../AudioFile/AudioFile.h"
#include "../jack/jack_module.h"

class FilePlayback {
public:
  // ---------------------- Constructor and Destructor ---------------------- //
  FilePlayback();
  FilePlayback(const char* file);
  ~FilePlayback();

  // ----------------------------- Play actions ----------------------------- //
  void play();
  void pauze();
  void stop();

  // ---------------------------- AudioFile pipe ---------------------------- //
  void openFile(const char* file);
  void printFileInfo();
  int getSampleRate();
  AudioFile<double>::AudioBuffer getBuffer(); // Actually AudioBuffer is just a vector of vectors
  void fillBuffer(jack_default_audio_sample_t *outBuf, jack_nframes_t nframes);

  // ------------------------- Getters and setters -------------------------- //
  void setIsPlaying(bool isPlaying);
  bool getIsPlaying();
  void setLoop(bool loopPlayback);
  bool getLoop();

private:
  // ------------------------------ AudioFile ------------------------------- //
  const char* file;
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
