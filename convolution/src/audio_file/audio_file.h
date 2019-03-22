#ifndef _AS_AUDIOFILE_H
#define _AS_AUDIOFILE_H

#include <iostream>
#include <vector>
#include <assert.h>
#include <string>


//=============================================================
/** The different types of audio file, plus some other types to
 * indicate a failure to load a file, or that one hasn't been
 * loaded yet
 */
enum class AudioFileFormat{
  Error,
  NotLoaded,
  Wave,
  Aiff,
  Mp3
};

//=============================================================
class AudioFile{
public:
  //=============================================================
  typedef std::vector<std::vector<double> > AudioBuffer;

  //=============================================================
  /** Constructor */
  AudioFile();
  virtual ~AudioFile() = 0;

  //=============================================================
  /** Loads an audio file from a given file path.
   * @Returns true if the file was successfully loaded
   */
  virtual bool load(std::string filePath) = 0;

  /** Saves an audio file to a given file path.
   * @Returns true if the file was successfully saved
   */
  bool save (std::string filePath, AudioFileFormat format = AudioFileFormat::Wave);

  //=============================================================
  /** @Returns the sample rate */
  uint32_t getSampleRate() const;

  /** @Returns the number of audio channels in the buffer */
  int getNumChannels() const;

  /** @Returns true if the audio file is mono */
  bool isMono() const;

  /** @Returns true if the audio file is stereo */
  bool isStereo() const;

  /** @Returns the bit depth of each sample */
  int getBitDepth() const;

  /** @Returns the number of samples per channel */
  int getNumSamplesPerChannel() const;

  /** @Returns the length in seconds of the audio file based on the number of samples and sample rate */
  double getLengthInSeconds() const;

  /** Prints a summary of the audio file to the console */
  void printSummary() const;

  //=============================================================

  /** Set the audio buffer for this AudioFile by copying samples from another buffer.
   * @Returns true if the buffer was copied successfully.
   */
  bool setAudioBuffer (AudioBuffer& newBuffer);

  /** Sets the audio buffer to a given number of channels and number of samples per channel. This will try to preserve
   * the existing audio, adding zeros to any new channels or new samples in a given channel.
   */
  void setAudioBufferSize (int numChannels, int numSamples);

  /** Sets the number of samples per channel in the audio buffer. This will try to preserve
   * the existing audio, adding zeros to new samples in a given channel if the number of samples is increased.
   */
  void setNumSamplesPerChannel (int numSamples);

  /** Sets the number of channels. New channels will have the correct number of samples and be initialised to zero */
  void setNumChannels (int numChannels);

  /** Sets the bit depth for the audio file. If you use the save() function, this bit depth rate will be used */
  void setBitDepth (int numBitsPerSample);

  /** Sets the sample rate for the audio file. If you use the save() function, this sample rate will be used */
  void setSampleRate (uint32_t newSampleRate);

  //=============================================================
  /** A vector of vectors holding the audio samples for the AudioFile. You can
   * access the samples by channel and then by sample index, i.e:
   *
   *      samples[channel][sampleIndex]
   */
  AudioBuffer samples;
  static AudioFileFormat getAudioFormat(std::string filepath);

protected:
  //=============================================================
  AudioFileFormat audioFileFormat;
  uint32_t sampleRate;
  int bitDepth;
  bool verbose = false;

  //=============================================================
  enum class Endianness{
      LittleEndian,
      BigEndian
  };

  //=============================================================
  void clearAudioBuffer();
private:

};

#endif /* AudioFile_h */
