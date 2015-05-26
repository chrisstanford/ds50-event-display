#ifndef EventDisplay_hh
#define EventDisplay_hh

#include <iostream>
#include <sstream>
#include <cstdio>
#include <iomanip>
#include <vector> 

// root includes
#include "TCanvas.h"
#include "TRootCanvas.h"
#include "TPad.h"
#include "TObject.h"
#include "TAxis.h"
#include "TGaxis.h"
#include "TBox.h"
#include "TLine.h"
#include "TList.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TColor.h"
#include "TString.h"
#include "TMath.h"
#include "TH1.h"
#include "TFile.h"
#include "TEnv.h"
#include "TFolder.h"
#include "TRootBrowser.h"
#include "TMacro.h"
#include "TROOT.h"
#include "TBrowser.h"
#include "TChain.h"
#include "TGString.h"
#include "TApplication.h"

// eve includes
#include "TEveManager.h"
#include "TEveElement.h"
#include "TEveVector.h"
#include "TEveGeoShape.h"
#include "TEveTrans.h"
#include "TEveBrowser.h"
#include "TEveWindow.h"
#include "TEveRGBAPalette.h"
#include "TEveRGBAPaletteOverlay.h"
// GL includes
#include "TGLViewer.h"

// geo incudes
#include "TGFileBrowser.h"
#include "TGButton.h"
#include "TGTab.h"
#include "TGListBox.h"
#include "TGFrame.h"
#include "TGNumberEntry.h"

// display includes
#include "./Products/DS50_geometry.hh"
#include "./Products/ChannelColors.hh"
#include "./Products/PMT.hh"
#include "./Products/Structs.hh"

namespace display {

  class EventDisplay : public TEveManager {
  public:
    explicit EventDisplay(std::string, std::string);
    void Create();

    // Functions called by buttons
    void NextEvent();
    void PrevEvent();
    void DrawWaveform(const char* input);      
    void ZoomAxisByTPCPulse();
    void ZoomAxisByLSVCluster(const char* det);
    void ZoomAxisByLSVROI(const char* det);
    void PrintTPCPulses();
    void PrintLSVClusters();
    void PrintLSVROIs();
    void ColorByAxis(const char* det);      

  private:
    // GUI Structures
    class WaveformFrame : public TGGroupFrame {
    public:
      WaveformFrame(std::string title, TMultiGraph* mg_sum, TMultiGraph* mg_chan, const TGCompositeFrame* p);
      TGListBox*          listbox_waveforms;
      TGTextButton*       button_draw;
    };
    class TPCPulseFrame : public TGGroupFrame {
    public:
      TPCPulseFrame(const TGCompositeFrame* p);
      TGCheckButton*      check_box;
      TGTextButton*       button_zoom_axis;
      TGNumberEntry*      entry_region_number;
      TGTextButton*       button_print_info;
    };
    class TPCSPEFrame : public TGGroupFrame {
    public:
      TPCSPEFrame(const TGCompositeFrame* p);
      TGCheckButton*      check_box;
      //TGTextButton*       button_zoom_axis;
      //TGNumberEntry*      entry_region_number;
      //TGTextButton*       button_print_info;
    };
    class LSVClusterFrame : public TGGroupFrame {
    public:
      LSVClusterFrame(const TGCompositeFrame* p);
      TGCheckButton*      check_box;
      TGTextButton*       button_zoom_axis;
      TGNumberEntry*      entry_region_number;
      TGTextButton*       button_print_info;
    };
    class LSVROIFrame : public TGGroupFrame {
    public:
      LSVROIFrame(const TGCompositeFrame* p);
      TGCheckButton*      check_box;
      TGTextButton*       button_zoom_axis;
      TGNumberEntry*      entry_region_number;
      TGTextButton*       button_print_info;
    };
    class EventSelectionFrame : public TGGroupFrame {
    public:
      EventSelectionFrame(const TGCompositeFrame* p);
      TGTextButton*       button_next;
      TGTextButton*       button_prev;
    };
      
    // Objects
    //                  gui tabs
    TCanvas*            canvas;

    //                  tabs
    TEveWindowTab*      tab_waveforms;
    TEveWindowTab*      tab_options;

    //                  input files
    TFile*              f1;
    TFile*              f2;
    //                  main tree
    TTree*              od_display_tree;
    TTree*              od_settings_tree;
    TTree*              tpc_display_tree;
    TTree*              tpc_settings_tree;
     
    //                  main variables
    int                 od_run_id;
    int                 od_event_id;
    int                 tpc_run_id;
    int                 tpc_event_id;
    int                 current_event_id;

    //                  what to enable
    bool                tpc_enabled;
    bool                lsv_enabled;
    bool                wt_enabled;
    bool                tpc_geo_enabled;
    bool                lsv_geo_enabled;
    bool                wt_geo_enabled;

    //                  waveform data
    TMultiGraph*        tpc_sum;
    TMultiGraph*        tpc_chan;
    TMultiGraph*        lsv_ampl_sum;
    TMultiGraph*        lsv_ampl_chan;
    TMultiGraph*        lsv_disc_sum;
    TMultiGraph*        lsv_disc_chan;
    TMultiGraph*        wt_ampl_sum;
    TMultiGraph*        wt_ampl_chan;
    TMultiGraph*        wt_disc_sum;
    TMultiGraph*        wt_disc_chan;
            
    //                  lsv cluster data
    TTree*              lsv_cluster_tree;
    std::vector<display::LSVCluster*>
                        lsv_cluster_vec;
      
    //                  lsv roi data
    TTree*              lsv_roi_tree;
    std::vector<display::LSVROI*>   
                        lsv_roi_vec;

    //                  tpc pulse data
    TTree*              tpc_pulse_tree;
    std::vector<display::TPCPulse*>   
                        tpc_pulse_vec;

    //                  tpc spe data
    TTree*              tpc_spe_tree;
    std::vector<display::TPCSPE*>   
                        tpc_spe_vec;

    //                  gui structures
    WaveformFrame*      tpc_wf_frame;

    WaveformFrame*      lsv_ampl_frame;
    WaveformFrame*      lsv_disc_frame;
    WaveformFrame*      wt_ampl_frame;
    WaveformFrame*      wt_disc_frame;

    TPCPulseFrame*      tpc_pulse_frame;
    TPCSPEFrame*        tpc_spe_frame;
    LSVClusterFrame*    lsv_cluster_frame;
    LSVROIFrame*        lsv_roi_frame;
    LSVClusterFrame*    wt_cluster_frame;
    LSVROIFrame*        wt_roi_frame;

    TEveElementList*    tpc_geometry;
    TEveElementList*    lsv_geometry;
    TEveElementList*    wt_geometry;

    TEveRGBAPaletteOverlay* tpc_palette;
    TEveRGBAPaletteOverlay* lsv_palette;
    TEveRGBAPaletteOverlay* wt_palette;
      
    void LoadFile(std::string, std::string); 
    void LoadEvent(int); 
    void DrawTPCPulses(int ch);     
    void DrawTPCSPEs(int ch);
    void DrawLSVClusters();     
    void DrawLSVROIs();
    void ZoomAxis(double start_ns,double end_ns);
    void CreateWaveformTab();
    void CreateWaveformTPCTab();
    void CreateWaveformODTab(std::string detector);
    void CreateOptionsTab();
    void CreateOptionsTPCTab();
    void CreateOptionsODTab(std::string detector);
    void CreateCanvas();
    void CreateGeometry();
    void CreateDetector(std::string detector);
    bool IsWaveformDrawn();
    bool IsWaveformDrawn(std::string detector);
    int  GetChannelIDFromMultigraphID(int mg_id, TMultiGraph* mg); // Channel id from graph at index mg_id in multigraph
    double GetGraphIntegral(TGraph* g, double start_ns, double end_ns);
    double GetAxisValue(std::string option);
    std::string TPCorOD(std::string detector); // returns "tpc" or "od"
    std::string GetDetectorInActivePad(); // returns "tpc" or "od"
    TEveRGBAPalette* MakePalette(int int_max_integral);
    TEveGeoShape* MakePMT(std::string detector, int ch_id, double size);
  };
  
}// end of display namespace

#endif
