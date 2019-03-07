/*
#
# 2017 Marc Groenewegen
# altered by Ciska Vriezenga to serve as a simple example
#
*/

#ifndef _JACK_MODULE_MC_H_
#define _JACK_MODULE_MC_H_

#include <string>
#include <jack/jack.h>

#include <functional>
#include <vector>

class JackModuleMC
{
public:
  JackModuleMC();
  JackModuleMC(int outputCount, int inputCount);
  ~JackModuleMC();
  int init();
  int init(std::string clientName);
  unsigned long getSamplerate();
  int getOutputCount();
  int getInputCount();
  void autoConnect();
  void end();
  //the onProcess function that needs to be assigned to a JackModule object
  std::function <int(
    std::vector<jack_default_audio_sample_t*>,
    std::vector<jack_default_audio_sample_t*>,
    jack_nframes_t)> onProcess;

private:
  int outputCount, inputCount;
  std::vector<jack_port_t*> outputPorts, inputPorts;
  static int _wrap_jack_process_cb(jack_nframes_t nframes,void *arg);
  jack_client_t *client;
  const char **ports;
};

#endif
