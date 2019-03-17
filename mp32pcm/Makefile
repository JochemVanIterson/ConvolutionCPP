OUTDIR  = build/
OUT     = $(OUTDIR)mp32pcm
CXX     = g++
CFLAGS  := -Wall -std=c++17 -DVERBOSE
LDFLAGS = -ljack -lmpg123
ODIR    = $(OUTDIR)obj
SDIR    = src
INC     = -Iinc

all: $(OUT)

_OBJS = \
	main.o \
	jack/jack_module.o \
	FilePlayback/FilePlayback.o \
	AudioFile/AudioFile.o\
	AudioFile/wavaiff/AudioFileWav.o\
	AudioFile/mp3/AudioFileMp3.o\

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
