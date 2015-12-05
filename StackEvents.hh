#ifndef StackEvents_hh
#define StackEvents_hh

#include <iostream>
#include <sstream>
#include <cstdio>
#include <iomanip>
#include <vector> 

// root includes
#include "TFile.h"
#include "TChain.h"
#include "TGraph.h"
#include "TMultiGraph.h"

class StackEvents {
public:
  explicit StackEvents(std::string inDirFileList, std::string outFileName);

  void StackWaveforms();

private:

  std::string         fOutName;      

  //                  main tree
  TChain*             tpc_display_tree_in;
    TChain*             tpc_settings_tree_in;

    int                 tpc_run_id_in;
    int                 tpc_event_id_in;
    TMultiGraph*        tpc_sum_in;
    TMultiGraph*        tpc_chan_in;
    TTree*              tpc_pulse_tree_in;
    TTree*              tpc_spe_tree_in;

    TBranch*            b_tpc_run_id_in;
    TBranch*            b_tpc_event_id_in;
    TBranch*            b_tpc_sum_in;
    TBranch*            b_tpc_chan_in;
    TBranch*            b_tpc_pulse_tree_in;
    TBranch*            b_tpc_spe_tree_in;

    int                 tpc_run_id_out;
    int                 tpc_event_id_out;
    TMultiGraph*        tpc_sum_out;
    TMultiGraph*        tpc_chan_out;
    TTree*              tpc_pulse_tree_out;
    TTree*              tpc_spe_tree_out;

    TBranch*            b_tpc_run_id_out;
    TBranch*            b_tpc_event_id_out;
    TBranch*            b_tpc_sum_out;
    TBranch*            b_tpc_chan_out;
    TBranch*            b_tpc_pulse_tree_out;
    TBranch*            b_tpc_spe_tree_out;

  void Finalize();
  void LoadDirectoryIntoTChain(std::vector<std::string> vInFileNames); 
  void SetBranchAddresses_In();

  void InitVars();

  //double GetMaxOfMultiGraph(TMultiGraph* mg, double start_t, double end_t); //This might be used in the future...
  
  bool                tpc_enabled;
  bool                tpc_geo_enabled;

  std::vector<std::vector<double>> vStackedWaveforms; //one entry in this vector is one channel, which is a vector of double. In it the stacked waveforms of all events are collected before written into tGraphs and tMultigraphs.
  std::vector<double> StackedSum;
  int n_channels;
  int n_samples;

  };


#endif
