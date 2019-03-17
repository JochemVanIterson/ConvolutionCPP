#include <iostream>
#include <string>
#include <getopt.h>

#include "FilePlayback/FilePlayback.h"
#include "jack/jack_module.h"

std::string filename = "";
bool verbose = false; // use if(verbose) for verbose logging
bool loop = false;

/* ------------------------------ getArguments ------------------------------ //
** Gets the command line arguments
** Inside 'long_options' you can put the arguments you want to react to. You can
** use whole words and single characters.
*/
void getArguments(int *argc, char **argv[]){
  int c;

  while (1) {
    int option_index = 0;
    static struct option long_options[] = {
      {"file",     required_argument, 0,  'f' },
      {"verbose",  no_argument,       0,  1 },
      {"loop",     no_argument,       0,  'l' },
      {"help",     no_argument,       0,  'h' },
      {0,          0,                 0,  0 }
    };

    c = getopt_long((*argc), (*argv), "hlf:01", long_options, &option_index);
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
      case 'l':
        loop = true;
        break;
      case 'h':
        std::cout << "Usage:" << std::endl;
        std::cout << "  -f \t--file  \t Filepath" << std::endl;
        std::cout << "  -h \t--help  \t Show this screen" << std::endl;
        std::cout << "  -l \t--loop  \t Loop playback" << std::endl;
        std::cout << "     \t--verbose \t Verbose logging" << std::endl;
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

int main(int argc, char *argv[]){
  getArguments(&argc, &argv);
  #ifdef VERBOSE
  verbose = true;
  #endif

  if(verbose)std::cout << "filename: " << filename << std::endl;

  FilePlayback filePlayback;
  filePlayback.openFile(filename);

  JackModule jack(16,1); // create a JackModuleMC instance
  jack.init(argv[0]); // init the jack, use program name as JACK client name
  double samplerate = jack.getSamplerate();

  if(samplerate != filePlayback.getSampleRate()){
    std::cout << "Samplerates don't match. Using jack Samplerate" << std::endl;
  }

  jack.onProcess = [&filePlayback](std::vector<jack_default_audio_sample_t*>* inputBuffers, std::vector<jack_default_audio_sample_t*>* outputBuffers, jack_nframes_t nframes) {
    filePlayback.fillBuffer(outputBuffers, nframes, true);
    return 0;
  };
  jack.autoConnect();

  filePlayback.setLoop(loop);
  filePlayback.play();

  //keep the program running and listen for user input
  std::cout << std::endl;
  std::cout << "Press 'q' when you want to quit the program." << std::endl;
  std::cout << "Press 's' to stop playback and reset the playhead to start" << std::endl;
  std::cout << "Press SPACE to play/pauze" << std::endl;
  bool running = true;
  while (running){
    // Output prompt
    // Set terminal to raw mode
    std::system("stty raw");

    // Wait for single character
    char input = std::getchar();
    if(input=='q'){
      running = false;
      jack.end();
    } else if(input==' '){
      if(filePlayback.getIsPlaying())filePlayback.pauze();
      else filePlayback.play();
    } else if(input=='s'){
      filePlayback.stop();
    }

    // Reset terminal to normal "cooked" mode
    std::system("stty cooked");
  }

  return EXIT_SUCCESS;
}
