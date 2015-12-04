all: EventDisplay StackEvents

EventDisplay: EventDisplayDict.cc EventDisplay.cc
	g++ -std=c++11 -o EventDisplay EventDisplay.cc EventDisplayDict.cc `root-config --cflags --glibs --evelibs`

EventDisplayDict.cc: EventDisplay.hh EventDisplayLinkDef.hh Products/*
	rootcint -f EventDisplayDict.cc -c EventDisplay.hh EventDisplayLinkDef.hh


StackEvents: StackEventsDict.cc StackEvents.cc
	g++ -g -std=c++11 -o StackEvents StackEvents.cc StackEventsDict.cc `root-config --cflags --glibs`

StackEventsDict.cc: StackEvents.hh StackEventsLinkDef.hh Products/*
	rootcint -f StackEventsDict.cc -c StackEvents.hh StackEventsLinkDef.hh

clean:
	rm -f EventDisplayDict.* StackEventsDict.* EventDisplay StackEvents