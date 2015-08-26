#ifndef Structs_hh
#define Structs_hh

#include "TGraph.h"
#include "TMultiGraph.h"
#include "TObjArray.h"

namespace display {
  enum channeltype {kSingleChannel,kSumChannel=1000,kAllChannel=1001,kSelectedChannel=1002};

  struct LSVROI {
    int    id;
    int    max_multiplicity;
    double start_ns;
    double end_ns;
    double charge;
  };
  struct LSVCluster {
    int    id;
    int    max_multiplicity;
    double start_ns;
    double end_ns;
    double charge;
    double height;
  };
  struct TPCPulse {
    int    id;
    double start_us;
    double end_us;
    double peak_us;
    double base;
    double height;
  };
  struct TPCSPE {
    int    id;
    int    channel;
    double start_us;
    double end_us;
    double base;
    double height;
  };
}
#endif
