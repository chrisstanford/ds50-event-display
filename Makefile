EventDisplay: EventDisplayDict.cc EventDisplay.cc
	g++ -std=c++11 -o EventDisplay EventDisplay.cc EventDisplayDict.cc `root-config --cflags --glibs --evelibs`

EventDisplayDict.cc: EventDisplay.hh EventDisplayLinkDef.hh Products/*
	rootcint -f EventDisplayDict.cc -c EventDisplay.hh EventDisplayLinkDef.hh

