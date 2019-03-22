OUTDIR  = build/
OUT     = $(OUTDIR)convolution
CXX     = g++
CFLAGS  := -Wall -std=c++17
LDFLAGS = -ljack -lmpg123
ODIR    = $(OUTDIR)obj
SDIR    = src
INC     = -Iinc

all: $(OUT)

_OBJS = \
	audio_buffer/audio_buffer.o \
	audio_file/audio_file.o \
	audio_file/wavaiff/AudioFileWav.o \
	audio_file/mp3/AudioFileMp3.o \
	convolution/convolution.o \
	convolution/direct_convolution/direct_convolution.o \
	file_playback/file_playback.o \
	jack/jack_module.o \
	main.o \

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

$(ODIR)/%.o: $(SDIR)/%.cpp
	@if [ ! -d $(@D) ]; then mkdir -p $(@D); echo "entering folder $(@D)"; fi
	@echo "building $<"; $(CXX) $(CFLAGS) -c $(INC) -o $@ $< $(CFLAGS)

$(ODIR)/%.a: $(SDIR)/%.a
	@if [ ! -d $(@D) ]; then mkdir -p $(@D); fi
	@echo "copying library $<"; cp $< $@

$(OUT): $(OBJS)
	@echo "Linking files";
	@$(CXX) $(LDFLAGS) -o $(OUT) $^;
	@echo "\nBuild complete\nRun $(OUT)"

.PHONY: clean

$(BUILD_DIR)%/:
	 mkdir -p $@

clean:
	rm -f -R $(OUTDIR)
