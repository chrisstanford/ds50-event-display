all: AverageWaveform EventDisplay

AverageWaveform: AverageWaveform.cc
	g++ -std=c++0x -o AverageWaveform AverageWaveform.cc `root-config --cflags --glibs --evelibs` -Wall

EventDisplay: EventDisplayDict.cc EventDisplay.cc
	g++ -std=c++0x -o EventDisplay EventDisplay.cc EventDisplayDict.cc `root-config --cflags --glibs --evelibs` -Wall

EventDisplayDict.cc: EventDisplay.hh EventDisplayLinkDef.hh #Products/*
	rootcint -f EventDisplayDict.cc -c EventDisplay.hh EventDisplayLinkDef.hh