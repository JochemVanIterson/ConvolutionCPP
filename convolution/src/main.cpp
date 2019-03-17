// USAGE:
// ./build/convolution --help

#include <getopt.h>
#include <iostream>
#include <thread>
#include <string>
#include <vector>

#include "jack/jack_module.h"
#include "convolution/direct_convolution/direct_convolution.h"
#include "file_playback/file_playback.h"

std::vector<std::string> modes {"direct_mic", "direct_file", "impulse_mic", "impulse_file"};
std::string mode = "";
std::string filename = "";
std::string impulseFile = "";
int jackOutputs = 2;
bool loadFile = false;
bool loadImpulse = false;
bool verbose = false; // use if(verbose) for verbose logging
bool setupDone = false;
bool in_array(const std::string &value, const std::vector<std::string> &array){
    return std::find(array.begin(), array.end(), value) != array.end();
}
void getArguments(int *argc, char **argv[]){
  int c;

  while (1) {
    int option_index = 0;
    static struct option long_options[] = {
      {"file",     required_argument, 0,  'f' },
      {"impulse",  required_argument, 0,  'i' },
      {"mode",     required_argument, 0,  'm' },
      {"outputs",  required_argument, 0,  'o' },
      {"verbose",  no_argument,       0,  1 },
      {"help",     no_argument,       0,  'h' },
      {0,          0,                 0,  0 }
    };

    c = getopt_long((*argc), (*argv), "hf:i:m:o:01", long_options, &option_index);
    if (c == -1)
      break;

    switch (c) {
      case 0:
        printf("option %s", long_options[option_index].name);
        if (optarg)
          printf(" with arg %s", optarg);
        printf("\n");
        break;

      case 'f':
        filename = optarg;
        break;
      case 'i':
        impulseFile = optarg;
        break;
      case 'o':
        jackOutputs = std::atoi (optarg);
        break;
      case 'm':
        mode = optarg;
        if(!in_array(mode, modes)){
          std::cout << "Unknown mode" << '\n';
          exit(EXIT_FAILURE);
        } else if(mode=="direct_mic"){
          std::cout << "Make sure you're using headphones\nPress enter to continue..." << std::endl;
          std::cin.ignore();
        } else if(mode=="impulse_mic"){
          std::cout << "Make sure you're using headphones\nPress enter to continue..." << std::endl;
          std::cin.ignore();
          loadImpulse = true;
        } else if(mode=="direct_file"){
          loadFile = true;
        } else if(mode=="impulse_file"){
          loadImpulse = true;
          loadFile = true;
        }
        break;
      case 'h':
        std::cout << "Usage: convolution\n\t";
        std::cout << "[-f | --file <filepath>] ";
        std::cout << "[-h | --help] ";
        std::cout << "[-i | --impulse <filepath>]\n\t";
        std::cout << "[-m | --mode <'direct_mic' | 'direct_file' | 'impulse_mic' | 'impulse_file'>]\n\t";
        std::cout << "[--verbose]\n\n";
        std::cout << "Modes:\n";
        std::cout << "direct_mic:\n";
        std::cout << "   Writes the microphone input directly to the output\n";
        std::cout << "   Make sure you're using headphones\n";
        std::cout << "impulse_mic:\n";
        std::cout << "   Convolves the microphone input\n";
        std::cout << "   Make sure you're using headphones\n";
        std::cout << "direct_file:\n";
        std::cout << "   Writes the file input directly to the output\n";
        std::cout << "impulse_file:\n";
        std::cout << "   Convolves the file input\n";

        exit(EXIT_SUCCESS);
        break;
      case 1:
        verbose = true;
        break;

      case '?':
        break;

      default:
        printf("?? getopt returned character code 0%o ??\n", c);
    }
  }

  if (optind < (*argc)) {
    printf("non-option ARGV-elements: ");
    while (optind < (*argc))
      printf("%s ", (*argv)[optind++]);
      printf("\n");
  }
}

int main(int argc, char** argv) {
  // ------------------------------- Arguments ------------------------------ //
  getArguments(&argc, &argv);
  if(verbose)std::cout << "Mode: " << mode << '\n';

  if(mode==""){
    std::cout << "No mode was provided\nUse --help for more information" << '\n';
    exit(EXIT_FAILURE);
  }

  FilePlayback filePlayback;
  DirectConvolution convolution(verbose);  //Create DirectConvolution instance.

  if(loadImpulse){
    if(impulseFile==""){
      std::cout << "No Impulse filename provided\nUse --impulse <filename>" << '\n';
      exit(EXIT_FAILURE);
    } else if(verbose)std::cout << "Impulse File: " << impulseFile << '\n';
    convolution.setImpulseResponse(impulseFile);
    if(verbose)std::cout << "Impulse loaded" << std::endl;
  }
  if(loadFile){
    if(filename==""){
      std::cout << "No Filename provided\nUse --file <filename>" << '\n';
      exit(EXIT_FAILURE);
    } else if(verbose)std::cout << "Playback file: " << filename << '\n';
    filePlayback.openFile(filename);
    if(verbose)std::cout << "File loaded" << std::endl;
  }

  setupDone = true;

  JackModule jack(jackOutputs,2); //Create jackmodule.
  jack.init(argv[0]);
  jack.onProcess = [&convolution, &filePlayback](
    std::vector<jack_default_audio_sample_t*> *inputBuffers,
    std::vector<jack_default_audio_sample_t*> *outputBuffers,
    jack_nframes_t nframes
  ){
    // --------------------------- Direct Mic out --------------------------- //
    if(mode == "direct_mic"){
      for(int channels = 0; channels<outputBuffers->size();channels++){
        for(int i = 0; i<nframes;i++){
          (*outputBuffers)[channels][i] = (*inputBuffers)[0][i];
        }
      }
      return 0;
    }
    // ---------------------------- File Playback --------------------------- //
    if(mode == "direct_file"){
      filePlayback.fillBuffer(outputBuffers, nframes, true);
      return 0;
    }
    // -------------------------- Convolution File -------------------------- //
    if(mode == "impulse_file"){
      filePlayback.fillBuffer(inputBuffers, nframes, true); // File input, comment to use mic input
      convolution.pushInput(inputBuffers, nframes);
      convolution.pullOutput(outputBuffers, nframes);
      return 0;
    }
    // --------------------------- Convolution Mic -------------------------- //
    if(mode == "impulse_mic"){
      convolution.pushInput(inputBuffers, nframes);
      convolution.pullOutput(outputBuffers, nframes);
      return 0;
    }
    return 1;
  };
  jack.autoConnect();
  if(loadImpulse) convolution.start();

  //Keep jack running.
  bool running = true;
  while(running) {
    std::system("stty raw");
    char input = std::getchar();
    std::system("stty cooked");

    if(input == 'q') {
      if(verbose)std::cout << "STOPPING PROCESS..." << '\n';
      running = false;
      filePlayback.stop();
      convolution.stop();
      jack.end();
    } else if(input == ' ') {
      if(loadFile && setupDone){
        if(filePlayback.getIsPlaying()) filePlayback.pauze();
        else filePlayback.play();
      }
    }
  }
  if(verbose)std::cout << "PROCESS STOPPED" << '\n';
  return 0;
}
