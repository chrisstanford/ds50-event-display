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
  virtual ~StackEvents();
  // Functions called by buttons
  int LoadEventTPC(const char* dettab);
  void DrawWaveform(const char* input);      
  //void PrintTPCPulses();
  void StackWaveforms();

private:

  //TFile*              fIn;
    TFile*              fOut;

    //                  main tree
    TChain*             tpc_display_tree_in;
    TChain*             tpc_settings_tree_in;

    TChain*             tpc_display_tree_out;
    TChain*             tpc_settings_tree_out;
     
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

  //std::vector<display::TPCPulse*> tpc_pulse_vec;
  //std::vector<display::TPCSPE*> tpc_spe_vec;
  

  void LoadDirectoryIntoTChain(std::vector<std::string> vInFileNames); 
  void SetBranchAddresses_In();
  void SetBranchAddresses_Out();
  //    int  LoadEvent(int); 
  void DrawTPCPulses(int ch);     
  void DrawTPCSPEs(int ch);
  void InitVars();

  double GetMaxOfMultiGraph(TMultiGraph* mg, double start_t, double end_t);
  
  bool                tpc_enabled;
  bool                tpc_geo_enabled;

  };


#endif
