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
#include "TSystem.h"
#include "TObjArray.h"
#include "TObjString.h"

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
#include "TGLabel.h"
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
    explicit EventDisplay(std::string file1, std::string file2, std::string option);
    explicit EventDisplay(std::string directory, std::string option);
    void Create();

    // Functions called by buttons
    int LoadEventTPC(const char* dettab);
    int LoadEventOD (const char* dettab);
    void DrawWaveform(const char* input);      
    void ZoomAxisByTPCPulse();
    void ZoomAxisByLSVCluster(const char* det);
    void ZoomAxisByLSVROI(const char* det);
    void PrintTPCPulses();
    void PrintLSVClusters();
    void PrintLSVROIs();
    void ColorByAxis(const char* det);      
    void CreateMovieByAxis(const char* det);
    void SavePicture(const char* fname);
    void SaveCanvas();

  private:
    // GUI Structures
    class WaveformFrame : public TGGroupFrame {
    public:
      WaveformFrame(std::string title, TMultiGraph* mg_sum, TMultiGraph* mg_chan, const TGCompositeFrame* p);
      //      bool                GetSelectedEntryIDs(TList* selected);
      //      void                ToggleMultipleSelection();
      TGListBox*          listbox_waveforms;
      TGTextButton*       button_draw;
      //      TGCheckButton*      check_multi;
      TGLabel*            label_combine_channels;
      TGTextEntry*          entry_combine_channels;
      ClassDef(WaveformFrame,1);
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
    private:
      const EventDisplay* parent; // pointer to parent class
    public:
      EventSelectionFrame(const EventDisplay* parent, const TGCompositeFrame* p);
      TGListBox*          listbox_events_tpc;
      TGListBox*          listbox_events_od;
      TGTextButton*       button_load_tpc;
      TGTextButton*       button_load_od;
      // TGTextButton*       button_next;
      // TGTextButton*       button_prev;
    };
    class GIFFrame : public TGGroupFrame {
    public:
      GIFFrame(const TGCompositeFrame* p);
      TGCheckButton*      check_box;
      TGNumberEntry*      entry_step_size;
      TGNumberEntry*      entry_window_size;
      TGNumberEntry*      entry_fps;
      TGTextButton*       button_record;
      TGCheckButton*      check_box_log;
      TGCheckButton*      check_box_background;
      TGCheckButton*      check_box_seperate_files;
      TGTextEntry*        entry_filename;
    private:
      TGLabel*            label_warning;      
      TGLabel*            label_step_size;
      TGLabel*            label_window_size;
      TGLabel*            label_fps;
      TGLabel*            label_filename;
    };
      
    // Objects
    //                  gui tabs
    TCanvas*            canvas;

    bool                enabled_3d;

    //                  tabs
    TEveWindowTab*      tab_waveforms;
    TEveWindowTab*      tab_options;

    //                  input files
    TFile*              f1;
    TFile*              f2;

    //                  main tree
    TChain*             od_display_tree;
    TChain*             od_settings_tree;
    TChain*             tpc_display_tree;
    TChain*             tpc_settings_tree;
     
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
    TGraph*             wf_integral;
            
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

    EventSelectionFrame*tpc_event_selection_frame;
    EventSelectionFrame*lsv_event_selection_frame;
    EventSelectionFrame* wt_event_selection_frame;

    GIFFrame*           tpc_gif_frame;
    GIFFrame*           lsv_gif_frame;
    GIFFrame*           wt_gif_frame;

    TEveElementList*    tpc_geometry;
    TEveElementList*    lsv_geometry;
    TEveElementList*    wt_geometry;

    TEveRGBAPaletteOverlay* tpc_palette;
    TEveRGBAPaletteOverlay* lsv_palette;
    TEveRGBAPaletteOverlay* wt_palette;

    double axis_min;
    double axis_max;
      
    void LoadFile(std::string, std::string); 
    void LoadDirectory(std::string directory); 
    void SetBranchAddresses();
    //    int  LoadEvent(int); 
    void DrawDefaultWaveform(std::string detector);
    void DrawTPCPulses(int ch);     
    void DrawTPCSPEs(int ch);
    void DrawLSVClusters();     
    void DrawLSVROIs();
    void ZoomAxis(double start_t,double end_t);
    void SetAxisLimits(double start_t,double end_t);
    void CreateWaveformTab();
    void CreateWaveformTPCTab();
    void CreateWaveformODTab(std::string detector);
    void CreateOptionsTab();
    void CreateOptionsTPCTab();
    void CreateOptionsODTab(std::string detector);
    void CreateCanvas();
    void CreateGeometry();
    void CreateDetector(std::string detector);
    void ColorByStartEnd(std::string detector, double start_t, double end_t, double max_integral_override=-1., bool draw_palette=true, Color_t background=kBlack);
    bool IsWaveformDrawn();
    bool IsWaveformDrawn(std::string detector);
    int  GetChannelIDFromMultigraphID(int mg_id, TMultiGraph* mg); // Channel id from graph at index mg_id in multigraph
    int  GetMultigraphIDFromChannelID(int ch_id, TMultiGraph* mg); // Index of graph for channel ch_id in multigraph
    double GetMaxOfMultiGraph(TMultiGraph* mg, double start_t, double end_t);
    double GetGraphIntegral(TGraph* g, double start_t, double end_t, std::string option="abs");
    double GetAxisValue(std::string option);
    double GetBinWidth(std::string detector);
    double AdjustIntegral(std::string detector, double integral); // Adjust the numerical integral to proper units 
    std::string TPCorOD(std::string detector); // returns "tpc" or "od", takes in "tpc","lsv","wt"
    std::string GetDetectorInActivePad(); // returns "tpc" or "od"
    TGraph* GetChannelGraph(std::string detector, int ch);
    TGraph* GetSumGraph(std::string detector);
    TMultiGraph* GetSumMultiGraph(std::string detector);
    TMultiGraph* GetChannelMultiGraph(std::string detector);
    bool IsIntegralGraph(TGraph* gr); // Determin if the graph is an integral graph
    void SetIntegralGraph(TGraph* gr, double maxoverride=-1.); // Make integral for sum or channel
    void SetIntegralGraph(TGraph* gr, TMultiGraph* mg); // Make integral for allchannel using sum 
    bool MultiGraphContainsIntegral(TMultiGraph* mg); // check if multigraph contains an integral graph    
    TEveRGBAPalette* MakePalette(Color_t background);
    TEveGeoShape* MakePMT(std::string detector, int ch_id, double size);
  };
  
}// end of display namespace

#endif
