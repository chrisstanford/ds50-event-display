// display
#include "./EventDisplay.hh"

namespace display {
  EventDisplay::EventDisplay(std::string filepath1, std::string filepath2) :
    TEveManager(1024,768,kTRUE,"FIV"),							    
    tpc_enabled(false),
    lsv_enabled(false),
    wt_enabled(false),
    tpc_geo_enabled(false),
    lsv_geo_enabled(false),
    wt_geo_enabled(false)
  {
    if (tpc_settings_tree) std::cout<<tpc_settings_tree<<" tpctreeexists1!8\n";
    if (tpc_display_tree) std::cout<<"tpctreeexists2!8\n";
    if (od_settings_tree) std::cout<<od_settings_tree<<" odtreeexists1!8\n";
    if (od_display_tree) std::cout<<od_display_tree<<" odtreeexists2!8\n";
    std::cout<<"Please ignore libGL errors."<<std::endl;
    LoadFile(filepath1,filepath2);
  }

  EventDisplay::EventDisplay(std::string directory) :
    TEveManager(1024,768,kTRUE,"FIV"),
    tpc_enabled(false),
    lsv_enabled(false),
    wt_enabled(false),
    tpc_geo_enabled(false),
    lsv_geo_enabled(false),
    wt_geo_enabled(false)
  {
    LoadDirectory(directory);
  }

  ///////////////////////////////////////
  /////////// Initialization ////////////
  ///////////////////////////////////////

  //________________________________________________________________________________________
  void EventDisplay::LoadFile(std::string tpc_filepath, std::string od_filepath) {
    // Get trees from files
    TTree* t;
    // Get TPC tree
    if (tpc_filepath!="") {
      tpc_settings_tree = new TChain("display/tpc_settings_tree");
      tpc_display_tree  = new TChain("display/tpc_display_tree");
      tpc_settings_tree->Add(tpc_filepath.c_str());
      tpc_display_tree->Add(tpc_filepath.c_str());
      // f1 = TFile::Open(tpc_filepath.c_str());
      // f1->GetObject("display/tpc_settings_tree",t);
      // if (t) tpc_settings_tree = t;
      // f1->GetObject("display/tpc_display_tree",t);
      // if (t) tpc_display_tree = t; 
    }
    // Get OD tree
    if (od_filepath!="") {
      od_settings_tree = new TChain("display/od_settings_tree");
      od_display_tree  = new TChain("display/od_display_tree");
      od_settings_tree->Add(od_filepath.c_str());
      od_display_tree->Add(od_filepath.c_str());
      // f2 = TFile::Open(od_filepath.c_str());
      // f2->GetObject("display/od_settings_tree",t);
      // if (t) od_settings_tree = t;
      // f2->GetObject("display/od_display_tree",t);
      // if (t) od_display_tree = t; 
    }
    EventDisplay::SetBranchAddresses();
  }

  //________________________________________________________________________________________
  void EventDisplay::LoadDirectory(std::string directory) {
    std::cout<<"Loading directory "<<directory<<std::endl;
    // Make chain out of all files in directory. Currently only works for tpc
    TChain* settings_chain = new TChain("display/tpc_settings_tree");
    TChain* display_chain = new TChain("display/tpc_display_tree");
    char* dir = gSystem->ExpandPathName(directory.c_str());
    void* dirp = gSystem->OpenDirectory(dir);
    const char* filename;
    TString str, sub;
    std::ostringstream os;
    while((filename = (char*)gSystem->GetDirEntry(dirp))) {
      str = filename;
      if(!str.BeginsWith("tpc"))
	continue;
      if(!str.EndsWith(".root"))
	continue;
      TString filepath = directory+"/";
      filepath+=str;
      std::cout<<"Adding "<<filepath<<" to chain.\n";
      settings_chain->Add(filepath);
      display_chain->Add(filepath);
    }
    if (!settings_chain || !display_chain) {
      std::cout<<"\nError: No valid TPC events found in "<<directory<<".\n\n";
      return;
    }
    // The TChain::GetEntries command generates a tree in the TChain data structure
    //    settings_chain->GetEntries();
    //    display_chain->GetEntries();
    //    tpc_settings_tree = settings_chain->GetTree();
    //    tpc_display_tree = display_chain->GetTree();
    tpc_settings_tree = settings_chain;
    tpc_display_tree = display_chain;
    std::cout<<"Found a total of "<<tpc_display_tree->GetEntries()<<" events."<<std::endl; 
    if (!tpc_settings_tree || !tpc_display_tree) {
      std::cout<<"No display trees were found"<<std::endl;
      return;
    }
    //    std::cout<<tpc_settings_tree->GetEntries()<<"\n";
    //    std::cout<<tpc_display_tree->GetEntries()<<"\n";
    EventDisplay::SetBranchAddresses();
  }  

  //________________________________________________________________________________________
  void EventDisplay::SetBranchAddresses() {
    // Load TPC Settings
    if (tpc_settings_tree) {
      tpc_settings_tree->SetBranchAddress("tpc_enabled",    &tpc_enabled);
      tpc_settings_tree->SetBranchAddress("tpc_geo_enabled",&tpc_geo_enabled);
      tpc_settings_tree->GetEntry(0);
    }
    // Load TPC data
    if (tpc_display_tree) {
      tpc_display_tree->SetBranchAddress("tpc_run_id",     &tpc_run_id);
      tpc_display_tree->SetBranchAddress("tpc_event_id",   &tpc_event_id);
      tpc_display_tree->SetBranchAddress("tpc_sum",        &tpc_sum);
      tpc_display_tree->SetBranchAddress("tpc_chan",       &tpc_chan);
      tpc_display_tree->SetBranchAddress("tpc_pulse_tree", &tpc_pulse_tree);
      tpc_display_tree->SetBranchAddress("tpc_spe_tree",   &tpc_spe_tree);
    }
    // Load OD settings
    if (od_settings_tree) {
      od_settings_tree->SetBranchAddress("lsv_enabled",    &lsv_enabled);
      od_settings_tree->SetBranchAddress("wt_enabled",     &wt_enabled);
      od_settings_tree->SetBranchAddress("lsv_geo_enabled",&lsv_geo_enabled);
      od_settings_tree->SetBranchAddress("wt_geo_enabled", &wt_geo_enabled);
      od_settings_tree->GetEntry(0);
    }
    // Load OD data
    if (od_display_tree) {
      od_display_tree->SetBranchAddress("od_run_id",       &od_run_id);
      od_display_tree->SetBranchAddress("od_event_id",     &od_event_id);
      od_display_tree->SetBranchAddress("lsv_ampl_sum",    &lsv_ampl_sum);
      od_display_tree->SetBranchAddress("lsv_ampl_chan",   &lsv_ampl_chan);
      od_display_tree->SetBranchAddress("lsv_disc_sum",    &lsv_disc_sum);
      od_display_tree->SetBranchAddress("lsv_disc_chan",   &lsv_disc_chan);
      od_display_tree->SetBranchAddress("wt_ampl_sum",     &wt_ampl_sum);
      od_display_tree->SetBranchAddress("wt_ampl_chan",    &wt_ampl_chan);
      od_display_tree->SetBranchAddress("wt_disc_sum",     &wt_disc_sum);
      od_display_tree->SetBranchAddress("wt_disc_chan",    &wt_disc_chan);
      od_display_tree->SetBranchAddress("lsv_cluster_tree",&lsv_cluster_tree);
      od_display_tree->SetBranchAddress("lsv_roi_tree",    &lsv_roi_tree);
    }
    std::cout<<"TPC enabled: "         <<tpc_enabled    <<std::endl;
    std::cout<<"LSV enabled: "         <<lsv_enabled    <<std::endl;
    std::cout<<"WT  enabled: "          <<wt_enabled    <<std::endl;
    std::cout<<"TPC geometry enabled: "<<tpc_geo_enabled<<std::endl;
    std::cout<<"LSV geometry enabled: "<<lsv_geo_enabled<<std::endl;
    std::cout<<"WT  geometry enabled: " <<wt_geo_enabled<<std::endl;
  }

  //________________________________________________________________________________________
  int EventDisplay::LoadEvent(int id) {
    std::cout<<"Loading event."<<std::endl;
    // Initialize main window title
    std::ostringstream os;
    // Load TPC event
    if (tpc_enabled) {
      // Check if event exists
      if (id >= tpc_display_tree->GetEntries()) {
	std::cout<<"No next TPC event."<<std::endl;
	return 0;
      }
      if (id < 0) {
	std::cout<<"No previous TPC event."<<std::endl;
	return 0;
      }
      // Clear structures from previous event
      for (int i=0;i<tpc_pulse_vec.size();++i) delete tpc_pulse_vec.at(i);
      for (int i=0;i<tpc_spe_vec.size();++i) delete tpc_spe_vec.at(i);
      tpc_pulse_vec.clear();      
      tpc_spe_vec.clear();      
      // Get event from tree
      tpc_display_tree->GetEntry(id);
      current_event_id=id;
      // Fill title
      os<<"tpc r"<<std::setw(6)<<std::setfill('0')<<tpc_run_id
	<<"e"<<std::setw(6)<<std::setfill('0')<<tpc_event_id<<" ";
      // Set Channel Colors
      display::SetChannelColors("tpc",tpc_chan);
      // Load TPC Pulses
      {
	const int N_pulses = tpc_pulse_tree->GetEntries();
	std::cout<<"Found "<<N_pulses<<" TPC pulses.\n";
	int channel;
	double start_us;
	double end_us;
	double peak_us;
	double base;
	double height;
	tpc_pulse_tree->SetBranchAddress("start_us",&start_us);
	tpc_pulse_tree->SetBranchAddress("end_us",&end_us);
	tpc_pulse_tree->SetBranchAddress("peak_us",&peak_us);
	tpc_pulse_tree->SetBranchAddress("base",&base);
	tpc_pulse_tree->SetBranchAddress("height",&height);
	for(int i=0; i<N_pulses; i++) {
	  tpc_pulse_tree->GetEntry(i);
	  display::TPCPulse* p = new display::TPCPulse(); 
	  p->start_us = start_us;
	  p->end_us   = end_us;
	  p->peak_us  = peak_us;
	  p->base     = base;
	  p->height   = height;
	  tpc_pulse_vec.push_back(p);
	}  
      }
      // Load SPEs
      {
	const int N_spes = tpc_spe_tree->GetEntries();
	std::cout<<"Found "<<N_spes<<" TPC SPEs.\n";
	int channel;
	double start_us;
	double end_us;
	double base;
	double height;
	tpc_spe_tree->SetBranchAddress("channel",&channel);
	tpc_spe_tree->SetBranchAddress("start_us",&start_us);
	tpc_spe_tree->SetBranchAddress("end_us",&end_us);
	tpc_spe_tree->SetBranchAddress("base",&base);
	tpc_spe_tree->SetBranchAddress("height",&height);
	for(int i=0; i<N_spes; i++) {
	  tpc_spe_tree->GetEntry(i);
	  display::TPCSPE* s = new display::TPCSPE(); 
	  s->channel  = channel;
	  s->start_us = start_us;
	  s->end_us   = end_us;
	  s->base     = base;
	  s->height   = height;
	  tpc_spe_vec.push_back(s);
	}  
      }
    }
    // Load OD event
    if (lsv_enabled||wt_enabled) {
      // Check if event exists
      if (id >= od_display_tree->GetEntries()) {
	std::cout<<"No next OD event."<<std::endl;
	return 0;
      }
      if (id < 0) {
	std::cout<<"No previous OD event."<<std::endl;
	return 0;
      }
      // Clear structures from previous event
      for (int i=0;i<lsv_cluster_vec.size();++i) delete lsv_cluster_vec.at(i);
      for (int i=0;i<lsv_roi_vec.size();++i) delete lsv_roi_vec.at(i);
      lsv_cluster_vec.clear();      
      lsv_roi_vec.clear();      
      // Get event from tree
      od_display_tree->GetEntry(id);
      current_event_id=id;
      // Fill title
      os<<"od r"<<std::setw(6)<<std::setfill('0')<<od_run_id
	<<"e"<<std::setw(6)<<std::setfill('0')<<od_event_id;
      // Set Channel Colors
      display::SetChannelColors("lsv",lsv_ampl_chan);
      display::SetChannelColors("lsv",lsv_disc_chan);
      display::SetChannelColors("wt",wt_ampl_chan);
      display::SetChannelColors("wt",wt_disc_chan);      
      // Load Clusters
      {
	const int N_clusters = lsv_cluster_tree->GetEntries();
	std::cout<<"Found "<<N_clusters<<" LSV clusters.\n";
	double start_ns;
	double end_ns;
	double charge;
	double height;
	int max_multiplicity;
	lsv_cluster_tree->SetBranchAddress("start_ns",&start_ns);
	lsv_cluster_tree->SetBranchAddress("end_ns",&end_ns);
	lsv_cluster_tree->SetBranchAddress("charge",&charge);
	lsv_cluster_tree->SetBranchAddress("height",&height);
	lsv_cluster_tree->SetBranchAddress("max_multiplicity",&max_multiplicity);      
	for(int i=0; i<N_clusters; i++) {
	  lsv_cluster_tree->GetEntry(i);
	  display::LSVCluster* c = new display::LSVCluster(); 
	  c->start_ns = start_ns;
	  c->end_ns   = end_ns;
	  c->charge   = charge;
	  c->height   = height;
	  c->max_multiplicity = max_multiplicity;
	  lsv_cluster_vec.push_back(c);
	}  
      }
      // Load ROIs
      {
	const int N_rois = lsv_roi_tree->GetEntries();
	std::cout<<"Found "<<N_rois<<" LSV ROIs.\n";
	double start_ns;
	double end_ns;
	double charge;
	int max_multiplicity;
	lsv_roi_tree->SetBranchAddress("start_ns",&start_ns);
	lsv_roi_tree->SetBranchAddress("end_ns",&end_ns);
	lsv_roi_tree->SetBranchAddress("charge",&charge);
	lsv_roi_tree->SetBranchAddress("max_multiplicity",&max_multiplicity);      
	for(int i=0; i<N_rois; i++) {
	  lsv_roi_tree->GetEntry(i);
	  display::LSVROI* r = new display::LSVROI(); 
	  r->start_ns = start_ns;
	  r->end_ns   = end_ns;
	  r->charge   = charge;
	  r->max_multiplicity = max_multiplicity;
	  lsv_roi_vec.push_back(r);
	}  
      }
    }
    // Set title
    std::string title = os.str();
    GetBrowser()->GetMainFrame()->SetWindowName(title.c_str());
    return 1;
  }

  //________________________________________________________________________________________
  void EventDisplay::Create() {
    // Load first event in display tree
    LoadEvent(0);
    // Create Tabs
    CreateWaveformTab();
    CreateOptionsTab();
    CreateCanvas();
    CreateGeometry();
    GetBrowser()->GetTabLeft()->SetTab(2);    
    EventDisplay::DrawDefaultWaveform();
  }

  ///////////////////////////////////////
  /////////// Waveform Tab //////////////
  ///////////////////////////////////////
  //________________________________________________________________________________________
  void EventDisplay::CreateWaveformTab() {
    TEveWindowSlot* slot = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabLeft());
    tab_waveforms = slot->MakeTab();
    tab_waveforms->SetElementName("Waveforms");
    tab_waveforms->SetShowTitleBar(kFALSE);
    if (tpc_enabled) CreateWaveformTPCTab();
    if (lsv_enabled) CreateWaveformODTab("lsv");
    if (wt_enabled)  CreateWaveformODTab("wt");
  }

  //________________________________________________________________________________________
  void EventDisplay::CreateWaveformTPCTab() {
    std::string detector = "tpc";
    TMultiGraph* wf_sum  = tpc_sum;
    TMultiGraph* wf_chan = tpc_chan;
    TEveWindowSlot* slot = tab_waveforms->NewSlot();
    TEveWindowFrame* tab_frame = slot->MakeFrame();
    tab_frame->SetShowTitleBar(kFALSE);
    TGCompositeFrame* comp_frame = tab_frame->GetGUICompositeFrame();
    // Waveform box
    WaveformFrame* wf_frame = new WaveformFrame("Waveforms", wf_sum, wf_chan, comp_frame);
    wf_frame->button_draw->Connect("Clicked()","display::EventDisplay",this,Form("DrawWaveform(=\"%s\")",detector.c_str()));
    comp_frame->AddFrame(wf_frame, new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY));
    // TPC pulse frame
    TPCPulseFrame* pulse_frame = new TPCPulseFrame(comp_frame);
    pulse_frame->button_print_info->Connect("Clicked()","display::EventDisplay",this,"PrintTPCPulses()");
    pulse_frame->button_zoom_axis->Connect("Clicked()","display::EventDisplay",this,"ZoomAxisByTPCPulse()");
    comp_frame->AddFrame(pulse_frame, new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    // TPC SPE frame
    TPCSPEFrame* spe_frame = new TPCSPEFrame(comp_frame);
    comp_frame->AddFrame(spe_frame, new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    // Event selection box
    EventSelectionFrame* event_selection_frame = new EventSelectionFrame(comp_frame);
    event_selection_frame->button_next->Connect("Clicked()","display::EventDisplay",this,"NextEvent()");
    event_selection_frame->button_prev->Connect("Clicked()","display::EventDisplay",this,"PrevEvent()");
    comp_frame->AddFrame(event_selection_frame,new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    // Set globals
    tpc_wf_frame = wf_frame;
    tpc_pulse_frame = pulse_frame;
    tpc_spe_frame = spe_frame;
    tab_frame->SetElementName("TPC");
    // Update Layout
    comp_frame->MapSubwindows();
    comp_frame->Layout();
    comp_frame->MapWindow();
  }

  //________________________________________________________________________________________
  void EventDisplay::CreateWaveformODTab(std::string detector) {
    TMultiGraph* ampl_sum;
    TMultiGraph* disc_sum;
    TMultiGraph* ampl_chan;
    TMultiGraph* disc_chan;
    if (detector=="lsv") {
      ampl_sum = lsv_ampl_sum;
      disc_sum = lsv_disc_sum;
      ampl_chan = lsv_ampl_chan;
      disc_chan = lsv_disc_chan;
    }
    if (detector=="wt") {
      ampl_sum = wt_ampl_sum;
      disc_sum = wt_disc_sum;
      ampl_chan = wt_ampl_chan;
      disc_chan = wt_disc_chan;
    }
    TEveWindowSlot* slot = tab_waveforms->NewSlot();
    TEveWindowFrame* tab_frame = slot->MakeFrame();
    tab_frame->SetShowTitleBar(kFALSE);
    TGCompositeFrame* comp_frame = tab_frame->GetGUICompositeFrame();
    // Amplitude waveform box
    WaveformFrame* ampl_frame = new WaveformFrame("Amplitude", ampl_sum, ampl_chan, comp_frame);
    ampl_frame->button_draw->Connect("Clicked()","display::EventDisplay",this,Form("DrawWaveform(=\"%s_ampl\")",detector.c_str()));
    comp_frame->AddFrame(ampl_frame, new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY));
    // Discriminator waveform box
    WaveformFrame* disc_frame = new WaveformFrame("Discriminator", disc_sum, disc_chan, comp_frame);
    disc_frame->button_draw->Connect("Clicked()","display::EventDisplay",this,Form("DrawWaveform(=\"%s_disc\")",detector.c_str()));
    comp_frame->AddFrame(disc_frame, new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY));
    // Right now both LSV and OD use the LSV clusters
    // LSV cluster frame
    LSVClusterFrame* cluster_frame = new LSVClusterFrame(comp_frame);
    cluster_frame->button_print_info->Connect("Clicked()","display::EventDisplay",this,"PrintLSVClusters()");
    cluster_frame->button_zoom_axis->Connect("Clicked()","display::EventDisplay",this,Form("ZoomAxisByLSVCluster(=\"%s\")",detector.c_str()));
    comp_frame->AddFrame(cluster_frame, new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    if (detector=="wt") cluster_frame->check_box->SetState(kButtonUp);
    // LSV roi frame
    LSVROIFrame* roi_frame = new LSVROIFrame(comp_frame);
    roi_frame->button_print_info->Connect("Clicked()","display::EventDisplay",this,"PrintLSVROIs()");
    roi_frame->button_zoom_axis->Connect("Clicked()","display::EventDisplay",this,Form("ZoomAxisByLSVROI(=\"%s\")",detector.c_str()));
    comp_frame->AddFrame(roi_frame, new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    // Event selection box
    EventSelectionFrame* event_selection_frame = new EventSelectionFrame(comp_frame);
    event_selection_frame->button_next->Connect("Clicked()","display::EventDisplay",this,"NextEvent()");
    event_selection_frame->button_prev->Connect("Clicked()","display::EventDisplay",this,"PrevEvent()");
    comp_frame->AddFrame(event_selection_frame,new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    // Set globals
    if (detector=="lsv") {
      lsv_ampl_frame = ampl_frame;
      lsv_disc_frame = disc_frame;
      lsv_cluster_frame = cluster_frame;
      lsv_roi_frame = roi_frame;
      tab_frame->SetElementName("LSV");
    }
    if (detector=="wt") {
      wt_ampl_frame = ampl_frame;
      wt_disc_frame = disc_frame;
      wt_cluster_frame = cluster_frame;
      wt_roi_frame = roi_frame;
      tab_frame->SetElementName("WT");
    }
    // Update Layout
    comp_frame->MapSubwindows();
    comp_frame->Layout();
    comp_frame->MapWindow();
  }

  //________________________________________________________________________________________
  EventDisplay::WaveformFrame::WaveformFrame(std::string title, TMultiGraph* mg_sum, TMultiGraph* mg_chan, const TGCompositeFrame* p) : TGGroupFrame(p, title.c_str(), kVerticalFrame) {
    //    int sumchan_id = 1000;
    //    int allchan_id = 1001;
    listbox_waveforms = new TGListBox(this);
    listbox_waveforms->AddEntry("Sum Channels",display::channeltype::kSumChannel);
    listbox_waveforms->AddEntry("All Channels",display::channeltype::kAllChannel);
    TList* list_chan = mg_chan->GetListOfGraphs();
    std::cout<<"Found "<<list_chan->GetSize()<<" channel graphs.\n";
    for(int i=0;i<list_chan->GetSize();i++) {
      if (!list_chan->At(i)) continue;	  
      TGraph* tg = (TGraph*)(list_chan->At(i));
      std::string title = tg->GetTitle();
      std::string::size_type pos1 = title.find("Ch");
      std::string::size_type pos2 = title.find(';');
      if (pos1 != std::string::npos && pos2 != std::string::npos) title = title.substr(pos1, pos2-pos1);
      listbox_waveforms->AddEntry(title.c_str(),i);
    }
    listbox_waveforms->Select(display::channeltype::kSumChannel);
    AddFrame(listbox_waveforms, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandY | kLHintsExpandX,0,0,10,0));
    button_draw = new TGTextButton(this,"Draw");
    AddFrame(button_draw, new TGLayoutHints(kLHintsBottom | kLHintsExpandX));
  }
  //________________________________________________________________________________________
  EventDisplay::EventSelectionFrame::EventSelectionFrame(const TGCompositeFrame* p) : TGGroupFrame(p, "Event Selection", kHorizontalFrame) {
    button_prev = new TGTextButton(this,"Prev Event");
    button_next = new TGTextButton(this,"Next Event");
    AddFrame(button_prev, new TGLayoutHints(kLHintsLeft | kLHintsExpandX,0,0,5,0));
    AddFrame(button_next, new TGLayoutHints(kLHintsLeft | kLHintsExpandX,0,0,5,0));
  }
  //________________________________________________________________________________________
  EventDisplay::TPCSPEFrame::TPCSPEFrame(const TGCompositeFrame* p) : TGGroupFrame(p, "SPE", kVerticalFrame) {
    // Draw SPE check box
    check_box = new TGCheckButton(this,"Draw SPE with Channel Waveforms");
    check_box->SetState(kButtonDown);
    AddFrame(check_box, new TGLayoutHints(kLHintsTop | kLHintsLeft,2,2,2,2));
  }

//__________________________________________________________________________________________
  EventDisplay::TPCPulseFrame::TPCPulseFrame(const TGCompositeFrame* p) : TGGroupFrame(p, "Pulses", kVerticalFrame) {
    // Draw Clusters check box
    check_box = new TGCheckButton(this,"Draw TPC Pulses with Waveforms");
    check_box->SetState(kButtonDown);
    AddFrame(check_box, new TGLayoutHints(kLHintsTop | kLHintsLeft,2,2,2,2));
    // Zoom frame
    TGHorizontalFrame* frame_zoom = new TGHorizontalFrame(this);
    button_zoom_axis = new TGTextButton(frame_zoom,"Zoom Axis to Pulse:");
    entry_region_number = new TGNumberEntry(frame_zoom, 0, 3, -1, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 999);
    frame_zoom->AddFrame(button_zoom_axis,new TGLayoutHints(kLHintsLeft,2,2,2,2));
    frame_zoom->AddFrame(entry_region_number,new TGLayoutHints(kLHintsLeft,2,2,2,2));
    AddFrame(frame_zoom, new TGLayoutHints(kLHintsTop | kLHintsExpandX,0,2,2,0));
    // Print Cluster info
    button_print_info = new TGTextButton(this,"Print All Pulse Info");
    AddFrame(button_print_info, new TGLayoutHints(kLHintsTop,2,2,2,2));
  }    

//________________________________________________________________________________________
  EventDisplay::LSVClusterFrame::LSVClusterFrame(const TGCompositeFrame* p) : TGGroupFrame(p, "Clusters", kVerticalFrame) {
    // Draw Clusters check box
    check_box = new TGCheckButton(this,"Draw LSV Clusters with Waveforms");
    check_box->SetState(kButtonDown);
    AddFrame(check_box, new TGLayoutHints(kLHintsTop | kLHintsLeft,2,2,2,2));
    // Zoom frame
    TGHorizontalFrame* frame_zoom = new TGHorizontalFrame(this);
    button_zoom_axis = new TGTextButton(frame_zoom,"Zoom Axis to Cluster:");
    entry_region_number = new TGNumberEntry(frame_zoom, 0, 3, -1, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 999);
    frame_zoom->AddFrame(button_zoom_axis,new TGLayoutHints(kLHintsLeft,2,2,2,2));
    frame_zoom->AddFrame(entry_region_number,new TGLayoutHints(kLHintsLeft,2,2,2,2));
    AddFrame(frame_zoom, new TGLayoutHints(kLHintsTop | kLHintsExpandX,0,2,2,0));
    // Print Cluster info
    button_print_info = new TGTextButton(this,"Print All Cluster Info");
    AddFrame(button_print_info, new TGLayoutHints(kLHintsTop,2,2,2,2));
  }

  //________________________________________________________________________________________
  EventDisplay::LSVROIFrame::LSVROIFrame(const TGCompositeFrame* p) : TGGroupFrame(p, "ROIs", kVerticalFrame) {
    // Draw ROIs check box
    check_box = new TGCheckButton(this,"Draw LSV ROIs with Waveforms");
    check_box->SetState(kButtonDown);
    AddFrame(check_box, new TGLayoutHints(kLHintsTop | kLHintsLeft,2,2,2,2));
    // Zoom frame
    TGHorizontalFrame* frame_zoom = new TGHorizontalFrame(this);
    button_zoom_axis = new TGTextButton(frame_zoom,"Zoom Axis to ROI:");
    entry_region_number = new TGNumberEntry(frame_zoom, 0, 3, -1, TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 999);
    frame_zoom->AddFrame(button_zoom_axis,new TGLayoutHints(kLHintsLeft,2,2,2,2));
    frame_zoom->AddFrame(entry_region_number,new TGLayoutHints(kLHintsLeft,2,2,2,2));
    AddFrame(frame_zoom, new TGLayoutHints(kLHintsTop | kLHintsExpandX,0,2,2,0));
    // Print ROI info
    button_print_info = new TGTextButton(this,"Print All ROI Info");
    AddFrame(button_print_info, new TGLayoutHints(kLHintsTop,2,2,2,2));
  }

  //________________________________________________________________________________________
  EventDisplay::GIFFrame::GIFFrame(const TGCompositeFrame* p) : TGGroupFrame(p, "Create GIF", kVerticalFrame) {
    // Make warning label
    label_warning = new TGLabel(this,"Warning: This feature is graphic \nintensive and very slow if \nnot done locally");
    AddFrame(label_warning, new TGLayoutHints(kLHintsTop | kLHintsLeft,2,2,2,2));
    // Step size frame
    TGHorizontalFrame* frame_step_size = new TGHorizontalFrame(this);
    label_step_size = new TGLabel(frame_step_size,"Step Size:");
    entry_step_size = new TGNumberEntry(frame_step_size, 0, 6, -1, TGNumberFormat::kNESReal, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 0.001, 9999);
    frame_step_size->AddFrame(label_step_size,new TGLayoutHints(kLHintsLeft,2,2,2,2));
    frame_step_size->AddFrame(entry_step_size,new TGLayoutHints(kLHintsLeft,2,2,2,2));
    AddFrame(frame_step_size, new TGLayoutHints(kLHintsTop | kLHintsExpandX,0,2,2,0));
    // window size frame
    TGHorizontalFrame* frame_window_size = new TGHorizontalFrame(this);
    label_window_size = new TGLabel(frame_window_size,"Window Size:");
    entry_window_size = new TGNumberEntry(frame_window_size, 0, 6, -1, TGNumberFormat::kNESReal, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 0.001, 9999);
    frame_window_size->AddFrame(label_window_size,new TGLayoutHints(kLHintsLeft,2,2,2,2));
    frame_window_size->AddFrame(entry_window_size,new TGLayoutHints(kLHintsLeft,2,2,2,2));
    AddFrame(frame_window_size, new TGLayoutHints(kLHintsTop | kLHintsExpandX,0,2,2,0));
    // FPS frame
    TGHorizontalFrame* frame_fps = new TGHorizontalFrame(this);
    label_fps = new TGLabel(frame_fps,"Frames per s:");
    entry_fps = new TGNumberEntry(frame_fps, 0, 6, -1, TGNumberFormat::kNESReal, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 0.001, 9999);
    frame_fps->AddFrame(label_fps,new TGLayoutHints(kLHintsLeft,2,2,2,2));
    frame_fps->AddFrame(entry_fps,new TGLayoutHints(kLHintsLeft,2,2,2,2));
    AddFrame(frame_fps, new TGLayoutHints(kLHintsTop | kLHintsExpandX,0,2,2,0));
    // Use log scaling check box
    check_box_log = new TGCheckButton(this,"Use log scaling");
    check_box_log->SetState(kButtonUp);
    AddFrame(check_box_log, new TGLayoutHints(kLHintsTop | kLHintsLeft,2,2,2,2));    
    // Filename frame
    TGHorizontalFrame* frame_filename = new TGHorizontalFrame(this);
    label_filename = new TGLabel(frame_filename,"Filename override");
    entry_filename = new TGTextEntry(frame_filename, "default");
    frame_filename->AddFrame(label_filename,new TGLayoutHints(kLHintsLeft,2,2,2,2));
    frame_filename->AddFrame(entry_filename,new TGLayoutHints(kLHintsLeft | kLHintsExpandX,2,2,2,2));
    AddFrame(frame_filename, new TGLayoutHints(kLHintsTop | kLHintsExpandX,0,2,2,0));
    // Record button
    button_record = new TGTextButton(this,"Record GIF using Canvas Limits");
    AddFrame(button_record, new TGLayoutHints(kLHintsTop,2,2,2,2));
  }

  //________________________________________________________________________________________
  void EventDisplay::DrawWaveform(const char* charinput) {
    const std::string input = charinput;
    int selected;
    TMultiGraph* mg_sum;
    TMultiGraph* mg_chan;
    if (input=="tpc") {
      mg_sum =  tpc_sum;
      mg_chan = tpc_chan;
      selected = tpc_wf_frame->listbox_waveforms->GetSelected();
    }
    if (input=="lsv_ampl") {
      mg_sum =  lsv_ampl_sum;
      mg_chan = lsv_ampl_chan;
      selected = lsv_ampl_frame->listbox_waveforms->GetSelected();
    }
    if (input=="lsv_disc") {
      mg_sum =  lsv_disc_sum;
      mg_chan = lsv_disc_chan;
      selected = lsv_disc_frame->listbox_waveforms->GetSelected();
    }
    if (input=="wt_ampl") {
      mg_sum =  wt_ampl_sum;
      mg_chan = wt_ampl_chan;
      selected = wt_ampl_frame->listbox_waveforms->GetSelected();
    }
    if (input=="wt_disc") {
      mg_sum =  wt_disc_sum;
      mg_chan = wt_disc_chan;
      selected = wt_disc_frame->listbox_waveforms->GetSelected();
    }
    // Get previous axis limits
    bool is_drawn = EventDisplay::IsWaveformDrawn();
    std::string prev_det = EventDisplay::GetDetectorInActivePad();
    double start_t = EventDisplay::GetAxisValue("min");
    double end_t = EventDisplay::GetAxisValue("max"); 
    // Draw waveform
    TCanvas* c = gPad->GetCanvas();
    c->Clear();
    if (selected == display::channeltype::kSumChannel) {
      TGraph* gr = (TGraph*)(mg_sum->GetListOfGraphs()->First());
      if (!gr) return;
      if (input=="tpc") {
	EventDisplay::SetIntegralGraph(gr);
	mg_sum->Add(wf_integral);
      }
      mg_sum->Draw("al");
      //      if (gr) gr->Draw("al"); 
    } else if (selected == display::channeltype::kAllChannel) {
      mg_chan->Draw("al");
    } else {
      TGraph* gr = (TGraph*)(mg_chan->GetListOfGraphs()->At(selected));
      gr->Draw("al");
    }
    std::string new_det = EventDisplay::GetDetectorInActivePad();
    // Zoom new graph to previously set limits, if any
    if (is_drawn && prev_det==new_det) EventDisplay::ZoomAxis(start_t,end_t);
    c->Update();  
    // Draw boxes
    if ((input=="tpc")&&tpc_pulse_frame->check_box->IsOn())
      DrawTPCPulses(selected);
    if ((input=="tpc")&&
	(selected!=display::channeltype::kSumChannel)&&
	(selected!=display::channeltype::kAllChannel)&&
	(tpc_spe_frame->check_box->IsOn()))
      DrawTPCSPEs(GetChannelIDFromMultigraphID(selected,tpc_chan));
    if ((input=="lsv_ampl"||input=="lsv_disc")&&lsv_cluster_frame->check_box->IsOn()) 
      DrawLSVClusters();
    if ((input=="wt_ampl"||input=="wt_disc")&&wt_cluster_frame->check_box->IsOn())
      DrawLSVClusters();
    if ((input=="lsv_ampl"||input=="lsv_disc")&&lsv_roi_frame->check_box->IsOn()) 
      DrawLSVROIs();
    if ((input=="wt_ampl"||input=="wt_disc")&&wt_roi_frame->check_box->IsOn())
      DrawLSVROIs();                  
    // Update
    c->Update();  
    gEve->GetBrowser()->GetTabRight()->SetTab(1);
  }

  //________________________________________________________________________________________
  void EventDisplay::DrawDefaultWaveform() {
    // Currently set to draw sum waveform. 
    // Call this function after loading a new event.
    if (tpc_enabled) {
      tpc_wf_frame->listbox_waveforms->Select(display::channeltype::kSumChannel);
      EventDisplay::DrawWaveform("tpc");
    } else if (lsv_enabled) {
      lsv_ampl_frame->listbox_waveforms->Select(display::channeltype::kSumChannel);
      EventDisplay::DrawWaveform("lsv_ampl");
    } else if (wt_enabled) {
      wt_ampl_frame->listbox_waveforms->Select(display::channeltype::kSumChannel);
      EventDisplay::DrawWaveform("wt_ampl");
    }
  }

  //________________________________________________________________________________________
  bool EventDisplay::IsWaveformDrawn() {
    TList* list_graphs = gPad->GetCanvas()->GetListOfPrimitives();
    for (int i=0;i<list_graphs->GetSize();i++) {
      std::string obj_name = list_graphs->At(i)->GetName();
      //	std::cout<<obj_name<<std::endl;
      if (obj_name.find("tpc") != std::string::npos) return true;
      if (obj_name.find("lsv") != std::string::npos) return true;
      if (obj_name.find("wt")  != std::string::npos) return true;
    }
    //      std::cout<<"No waveform found to zoom.\n";
    return false;
  }
  //________________________________________________________________________________________
  bool EventDisplay::IsWaveformDrawn(std::string detector) {
    TList* list_graphs = gPad->GetCanvas()->GetListOfPrimitives();
    for (int i=0;i<list_graphs->GetSize();i++) {
      std::string obj_name = list_graphs->At(i)->GetName();
      if (obj_name.find(detector.c_str())!= std::string::npos) return true;
    }
    return false;
  }
  //________________________________________________________________________________________
  void EventDisplay::ZoomAxisByTPCPulse() {
    int selected = tpc_pulse_frame->entry_region_number->GetIntNumber();
    // Check if active pad contains a tpc graph
    if (!(display::EventDisplay::GetDetectorInActivePad()=="tpc")) {
      std::cout<<"Active pad does not contain a TPC graph."<<std::endl;
      return;
    }      
    // Check if pulse is within bounds
    const int N = tpc_pulse_vec.size();
    if (selected<0||selected>=N) {
      std::cout<<"Pulse "<<selected<<" is out of bounds."<<std::endl;
      return;
    }
    // Get selected pulse bounds
    double start_us = tpc_pulse_vec.at(selected)->start_us;
    double end_us = tpc_pulse_vec.at(selected)->end_us;      
    EventDisplay::ZoomAxis(start_us,end_us);
  }

  //________________________________________________________________________________________
  void EventDisplay::ZoomAxisByLSVCluster(const char* det) {
    std::string detector = det;
    int selected;
    if (detector=="lsv") selected = lsv_cluster_frame->entry_region_number->GetIntNumber();
    if (detector=="wt")  selected = wt_cluster_frame->entry_region_number->GetIntNumber();
    // Check if active pad contains an od graph
    if (!(EventDisplay::GetDetectorInActivePad()=="od")) {
      std::cout<<"Active pad does not contain an OD graph."<<std::endl;
      return;
    }
    // Check if cluster is within bounds
    const int N = lsv_cluster_vec.size();
    if (selected<0||selected>=N) {
      std::cout<<"Cluster "<<selected<<" is out of bounds."<<std::endl;
      return;
    }
    // Get selected cluster bounds
    double start_ns = lsv_cluster_vec.at(selected)->start_ns;
    double end_ns = lsv_cluster_vec.at(selected)->end_ns;      
    EventDisplay::ZoomAxis(start_ns,end_ns);
  }

  //________________________________________________________________________________________
  void EventDisplay::ZoomAxisByLSVROI(const char* det) {
    std::string detector = det;
    int selected;
    if (detector=="lsv") selected = lsv_roi_frame->entry_region_number->GetIntNumber();
    if (detector=="wt")  selected = wt_roi_frame->entry_region_number->GetIntNumber();      
    // Check if roi is within bounds
    const int N = lsv_roi_vec.size();
    if (selected<0||selected>=N) {
      std::cout<<"ROI "<<selected<<" is out of bounds."<<std::endl;
      return;
    }
    // Get selected roi bounds
    double start_ns = lsv_roi_vec.at(selected)->start_ns;
    double end_ns = lsv_roi_vec.at(selected)->end_ns;      
    EventDisplay::ZoomAxis(start_ns,end_ns);
  }

  //________________________________________________________________________________________     
  void EventDisplay::ZoomAxis(double start_t,double end_t) {
    // Find the graph in the active canvas
    TMultiGraph* mg;
    TGraph* gr;
    TList* list_graphs = gPad->GetCanvas()->GetListOfPrimitives();
    for (int i=0;i<list_graphs->GetSize();i++) {
      std::string obj_name = list_graphs->At(i)->GetName();
      //	std::cout<<obj_name<<std::endl;
      if (obj_name.find("mg_")!= std::string::npos) {
	mg = (TMultiGraph*)gPad->GetPrimitive(obj_name.c_str());
	int start_bin = mg->GetHistogram()->GetXaxis()->FindBin(start_t);
	int end_bin = mg->GetHistogram()->GetXaxis()->FindBin(end_t);
	mg->GetXaxis()->SetRange(start_bin,end_bin);
	break;
      }
      if (obj_name.find("gr_")!= std::string::npos) {
	gr = (TGraph*)gPad->GetPrimitive(obj_name.c_str());
	int start_bin = gr->GetXaxis()->FindBin(start_t);
	int end_bin = gr->GetXaxis()->FindBin(end_t);
	gr->GetXaxis()->SetRange(start_bin,end_bin);
	break;
      }
    }
    gPad->Modified();
    gPad->Update();
  }
  
  //________________________________________________________________________________________
  int EventDisplay::GetChannelIDFromMultigraphID(int mg_id, TMultiGraph* mg) {
    TList* list_graphs = mg->GetListOfGraphs();
    const int N_channels = list_graphs->GetSize();
    if (mg_id>N_channels) return -1;
    TGraph* gr = (TGraph*)list_graphs->At(mg_id);
    std::string gr_name = gr->GetName();
    int last_index = gr_name.find_last_not_of("0123456789");
    std::string str_ch_id = gr_name.substr(last_index + 1);  
    std::string::size_type sz;
    int ch_id = std::stoi(str_ch_id,&sz);
    return ch_id;
}

  //________________________________________________________________________________________
  int EventDisplay::GetMultigraphIDFromChannelID(int ch_id, TMultiGraph* mg) {
    TList* list_graphs = mg->GetListOfGraphs();
    const int N_channels = list_graphs->GetSize();
    for (int i=0;i<N_channels;i++) {
      TGraph* gr = (TGraph*)list_graphs->At(i);
      std::string gr_name = gr->GetName();
      int last_index = gr_name.find_last_not_of("0123456789");
      std::string str_ch = gr_name.substr(last_index + 1);  
      std::string::size_type sz;
      int ch = std::stoi(str_ch,&sz);
      if (ch==ch_id) return ch;
    }
    return -1;
}
  
  //________________________________________________________________________________________
  std::string EventDisplay::TPCorOD(std::string detector) {
    if (detector=="tpc") return "tpc";
    if (detector=="lsv"||detector=="wt") return "od";
  }
  
  //________________________________________________________________________________________
  std::string EventDisplay::GetDetectorInActivePad() {
    std::string det_string = "";
    TList* canvas_graphs = gPad->GetCanvas()->GetListOfPrimitives();
    for (int i=0;i<canvas_graphs->GetSize();i++) {
      std::string obj_name = canvas_graphs->At(i)->GetName();
      if (obj_name.find("tpc") != std::string::npos) det_string += "tpc";
      if (obj_name.find("lsv") != std::string::npos) det_string += "lsv";
      if (obj_name.find("wt")  != std::string::npos) det_string += "wt";
    }
    if (det_string.find("tpc") != std::string::npos) return        "tpc";
    else if (det_string.find("lsv") != std::string::npos) return   "od";
    else if (det_string.find("wt")  != std::string::npos) return   "od";
    else return "";
  }

  ///////////////////////////////////////
  /////////// Options Tab ///////////////
  ///////////////////////////////////////

  //________________________________________________________________________________________
  void EventDisplay::CreateOptionsTab() {
    TEveWindowSlot* slot = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabLeft());
    tab_options = slot->MakeTab();
    tab_options->SetElementName("3D Options");
    tab_options->SetShowTitleBar(kFALSE);
    if (tpc_enabled) CreateOptionsTPCTab();
    if (lsv_enabled) CreateOptionsODTab("lsv");
    if (wt_enabled)  CreateOptionsODTab("wt");
  }

  //________________________________________________________________________________________
  void EventDisplay::CreateOptionsTPCTab() {
    TEveWindowSlot* slot = tab_options->NewSlot();
    TEveWindowFrame* tab_frame = slot->MakeFrame();
    tab_frame->SetShowTitleBar(kFALSE);
    TGCompositeFrame* comp_frame = tab_frame->GetGUICompositeFrame();
    // Save Canvas button
    TGTextButton* button_save_canvas = new TGTextButton(comp_frame,"Save Current Canvas to png");
    button_save_canvas->Connect("Clicked()","display::EventDisplay",this,"SaveCanvas()");
    comp_frame->AddFrame(button_save_canvas,new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    // Color button
    TGTextButton* button_color_by_axis = new TGTextButton(comp_frame,"Color PMTs by Current Canvas Limits");
    button_color_by_axis->Connect("Clicked()","display::EventDisplay",this,"ColorByAxis(=\"tpc\")");
    comp_frame->AddFrame(button_color_by_axis,new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    // Save picture button
    TGTextButton* button_save_picture = new TGTextButton(comp_frame,"Save Picture of 3D Display");
    button_save_picture->Connect("Clicked()","display::EventDisplay",this,"SavePicture(=\"\")");
    comp_frame->AddFrame(button_save_picture,new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    // Create movie frame
    tpc_gif_frame = new GIFFrame(comp_frame);
    tpc_gif_frame->button_record->Connect("Clicked()","display::EventDisplay",this,"CreateMovieByAxis(=\"tpc\")");
    comp_frame->AddFrame(tpc_gif_frame,new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    // Set globals
    tab_frame->SetElementName("TPC");
    comp_frame->MapSubwindows();
    comp_frame->Layout();
    comp_frame->MapWindow();
  }

  //________________________________________________________________________________________
  void EventDisplay::CreateOptionsODTab(std::string detector) {
    TEveWindowSlot* slot = tab_options->NewSlot();
    TEveWindowFrame* tab_frame = slot->MakeFrame();
    tab_frame->SetShowTitleBar(kFALSE);
    TGCompositeFrame* comp_frame = tab_frame->GetGUICompositeFrame();
    // Save Canvas button
    TGTextButton* button_save_canvas = new TGTextButton(comp_frame,"Save Current Canvas to png");
    button_save_canvas->Connect("Clicked()","display::EventDisplay",this,"SaveCanvas()");
    comp_frame->AddFrame(button_save_canvas,new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    // Color button
    TGTextButton* button_color_by_axis = new TGTextButton(comp_frame,"Color PMTs by Current Axis Limits");
    button_color_by_axis->Connect("Clicked()","display::EventDisplay",this,Form("ColorByAxis(=\"%s\")",detector.c_str()));
    comp_frame->AddFrame(button_color_by_axis,new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    // Save picture button
    TGTextButton* button_save_picture = new TGTextButton(comp_frame,"Save Picture of 3D Display");
    button_save_picture->Connect("Clicked()","display::EventDisplay",this,"SavePicture(=\"\")");
    comp_frame->AddFrame(button_save_picture,new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    // Create movie frame
    GIFFrame* gif_frame = new GIFFrame(comp_frame);
    gif_frame->button_record->Connect("Clicked()","display::EventDisplay",this,Form("CreateMovieByAxis(=\"%s\")",detector.c_str()));
    comp_frame->AddFrame(gif_frame,new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    // Set globals
    if (detector=="lsv") {
      tab_frame->SetElementName("LSV");
      lsv_gif_frame = gif_frame;
    }
    if (detector=="wt") {
      tab_frame->SetElementName("WT");
      wt_gif_frame = gif_frame;
    }
    comp_frame->MapSubwindows();
    comp_frame->Layout();
    comp_frame->MapWindow();
  }

  ///////////////////////////////////////
  //////////// Canvas Tab ///////////////
  ///////////////////////////////////////

  //________________________________________________________________________________________
  void EventDisplay::CreateCanvas() {
    GetBrowser()->StartEmbedding(TRootBrowser::kRight);
    canvas = new TCanvas();
    GetBrowser()->StopEmbedding("Canvas");
  }

  ///////////////////////////////////////
  /////////// Geometry Tab //////////////
  ///////////////////////////////////////

  //________________________________________________________________________________________
  void EventDisplay::CreateGeometry() {
    // Set camera orientation         
    GetDefaultGLViewer()->SetCurrentCamera(TGLViewer::kCameraPerspXOY);
    //#define WHITE_BACKGROUND
#ifdef WHITE_BACKGROUND
    GetDefaultGLViewer()->SetClearColor(kWhite);
#endif
    // Create the geometries
    if (tpc_geo_enabled) CreateDetector("tpc");
    if (lsv_geo_enabled) CreateDetector("lsv");
    if (wt_geo_enabled)  CreateDetector("wt");
  }
  //________________________________________________________________________________________
  void EventDisplay::CreateDetector(std::string detector) {
    int min_ch_id=0;
    int max_ch_id=0;
    if(detector=="tpc") {
      std::cout<<"Making TPC geometry.\n";
      min_ch_id=0;
      max_ch_id=37;
    }
    if(detector=="lsv") {
      min_ch_id=0;
      max_ch_id=119;
    }
    if(detector=="wt") {
      min_ch_id=128;
      max_ch_id=231;
    }
    TEveElementList* detectorlist = new TEveElementList(detector.c_str(),detector.c_str());
    AddElement(detectorlist);
    for (int ch=min_ch_id;ch<=max_ch_id;ch++) {
      display::PMT_positions* p = 
	new display::PMT_positions(detector,ch);  
      if (p->GetPMTx()>10000) continue; // Channels with no corresponding pmt were given large coordinates
      TEveVector vec(p->GetPMTx(),p->GetPMTy(),p->GetPMTz());
      PMT* pmt = new PMT(detector,ch);
      //	TEveGeoShape* pmtshape = pmt->GetShape();
      pmt->RefMainTrans().SetPos(vec);
      pmt->RefMainTrans().SetRotByAngles(p->GetPMTRotx(), p->GetPMTRoty(), p->GetPMTRotz());
      detectorlist->AddElement(pmt);
      delete p;
    }
    // Set globals
    if(detector=="tpc") {
      tpc_geometry = detectorlist;
    }
    if(detector=="lsv") {
      lsv_geometry = detectorlist;
    }
    if(detector=="wt") {
      wt_geometry = detectorlist;
    }
    Redraw3D(kTRUE);
  }

  //________________________________________________________________________________________
  double EventDisplay::GetMaxOfMultiGraph(TMultiGraph* mg, double start_t, double end_t) {    
    TList* list_graphs = mg->GetListOfGraphs();
    const int N_channels = list_graphs->GetSize();
    double max = 0;
    double maxtime = 0;
    double maxchan = 0;
    for (int i=0;i<N_channels;i++) {
      TGraph* gr = (TGraph*)list_graphs->At(i);
      const int nbins = gr->GetN();
      double *x = new double[nbins];
      double *y = new double[nbins];
      x = gr->GetX();
      y = gr->GetY();
      std::vector<double> subset;
      for (int j=0;j<nbins;j++) {
	if (x[j]<start_t) continue;
	if (x[j]>end_t)   break;
	subset.push_back(y[j]);
      }
      //      delete x; //this causes seg fault
      //      delete y; //this causes seg fault
      double lower = fabs(*std::min_element(subset.begin(), subset.end()));
      double upper = fabs(*std::max_element(subset.begin(), subset.end()));
      double tempmax = (lower<upper)?upper:lower;
      if (tempmax>max) {
	max = tempmax;
      }
    }
    std::cout<<"Max of muligraph in region "<<max<<"\n";
    return max;
  }
    
  //________________________________________________________________________________________
  void EventDisplay::CreateMovieByAxis(const char* det) {
    std::string detector = det;
    if (TPCorOD(detector)!=EventDisplay::GetDetectorInActivePad()) {
      std::cout<<"Requested detector is not drawn in current canvas."<<std::endl;
      return;
    }    
    if (detector=="tpc"&&(!tpc_enabled||!tpc_geo_enabled)) return;
    if (detector=="lsv"&&(!lsv_enabled||!lsv_geo_enabled)) return;
    if (detector=="wt" &&(!wt_enabled ||!wt_geo_enabled))  return;
    GIFFrame* gif_frame;
    TMultiGraph* mg_chan;
    int run_id, event_id;
    if (detector=="tpc") {
      gif_frame = tpc_gif_frame; 
      mg_chan   = tpc_chan;
      run_id    = tpc_run_id;
      event_id  = tpc_event_id;
    }
    if (detector=="lsv") {
      gif_frame = lsv_gif_frame; 
      mg_chan   = lsv_ampl_chan;
      run_id    = od_run_id;
      event_id  = od_event_id;
    }
    if (detector=="wt") {
      gif_frame = wt_gif_frame; 
      mg_chan   = wt_ampl_chan;
      run_id    = od_run_id;
      event_id  = od_event_id;
    }
    double step_size = gif_frame->entry_step_size->GetNumber(); //us
    double window_size = gif_frame->entry_window_size->GetNumber(); //us
    double window_size_bins = window_size/EventDisplay::GetBinWidth(detector);
    double fps = gif_frame->entry_fps->GetNumber();
    int frame_length = (1./fps)*100; // convert from fps to 10s of ms
    double start_us = EventDisplay::GetAxisValue("min");
    double end_us =   EventDisplay::GetAxisValue("max");
    std::ostringstream filename_no_ext;
    filename_no_ext<<detector<<"_r"<<run_id<<"_e"<<event_id<<"_start"<<start_us<<"_end"<<end_us<<"_step"<<step_size<<"_window"<<window_size<<"_fps"<<fps;    
    std::string str_filename = gif_frame->entry_filename->GetText();
    if (str_filename=="default") 
      str_filename = filename_no_ext.str();	   
    std::ostringstream filename_with_ext;
    filename_with_ext<<str_filename<<".gif+"<<frame_length;
    double value_max = EventDisplay::GetMaxOfMultiGraph(mg_chan,start_us,end_us);
    double integral_max = window_size_bins*value_max;
    integral_max = EventDisplay::AdjustIntegral(detector,integral_max);
    //    std::cout<<"Max integral Adjusted "<<integral_max<<"\n";
    std::cout<<"Limits: "<<start_us<<" to "<<end_us<<"\n";
    for (int i = 0;start_us<end_us;start_us+=step_size) {
      std::cout<<"Coloring from "<<start_us<<" to "<<start_us+window_size<<std::endl;
      EventDisplay::ColorByStartEnd(detector,start_us,start_us+window_size,integral_max/*/5*/,false);
      EventDisplay::SavePicture(filename_with_ext.str().c_str());
    }
    std::cout<<"Complete! GIF saved as \n"<<filename_no_ext.str()<<".gif"<<std::endl;
    return;
  }
  
  //________________________________________________________________________________________
  void EventDisplay::SavePicture(const char* fname) {
    std::string filename = fname;
    if (filename == "") filename = "ds50_3D.png";
    TGLViewer* glv = GetDefaultGLViewer(); 
    if (!glv) return;
    //    std::cout<<"Saving "<<filename<<std::endl;
    glv->SavePictureUsingBB(filename.c_str());
  }

  void EventDisplay::SaveCanvas() {
    std::string filename;
    int min = EventDisplay::GetAxisValue("min");
    int max = EventDisplay::GetAxisValue("max");
    if (EventDisplay::GetDetectorInActivePad()=="tpc")
      filename=Form("canvas_tpc_r%d_e%d_from_%d_to_%d.png",tpc_run_id,tpc_event_id,min,max);
    else if (EventDisplay::GetDetectorInActivePad()=="od")
      filename=Form("canvas_od_r%d_e%d_from_%d_to_%d.png",od_run_id,od_event_id,min,max);
    else {
      std::cout<<"No waveform found in active pad."<<std::endl;
      return;
    }
    TCanvas* c = gPad->GetCanvas();
    if (!c) return;
    c->SaveAs(filename.c_str());
  }

  //________________________________________________________________________________________
  double EventDisplay::GetAxisValue(std::string option) {
    int    start_bin = 0;
    int    end_bin   = 0;
    double start_ns  = 0.;
    double end_ns    = 0.;
    TList* canvas_graphs = gPad->GetCanvas()->GetListOfPrimitives();
    for (int i=0;i<canvas_graphs->GetSize();i++) {
      std::string obj_name = canvas_graphs->At(i)->GetName();
      if (obj_name.find("mg_") != std::string::npos) {
	TMultiGraph* mg = (TMultiGraph*)gPad->GetPrimitive(obj_name.c_str());
	start_bin = mg->GetHistogram()->GetXaxis()->GetFirst();
	end_bin   = mg->GetHistogram()->GetXaxis()->GetLast();
	start_ns  = mg->GetHistogram()->GetXaxis()->GetBinCenter(start_bin);
	end_ns    = mg->GetHistogram()->GetXaxis()->GetBinCenter(end_bin);
	break;
      }
      if (obj_name.find("gr_") != std::string::npos) {
	TGraph* gr = (TGraph*)gPad->GetPrimitive(obj_name.c_str());
	start_bin = gr->GetHistogram()->GetXaxis()->GetFirst();
	end_bin   = gr->GetHistogram()->GetXaxis()->GetLast();
	start_ns  = gr->GetHistogram()->GetXaxis()->GetBinCenter(start_bin);
	end_ns    = gr->GetHistogram()->GetXaxis()->GetBinCenter(end_bin);
	break;
      }
      if (i>=canvas_graphs->GetSize()) {
	std::cout<<"No axis found."<<std::endl;
	return 0.;	// No graphs found
      }
    }
    if (option=="min") return start_ns;
    if (option=="max") return end_ns;
    //    if (option=="minbin") return start_bin;
    //    if (option=="maxbin") return end_bin;
    return 0.;
  }

  //________________________________________________________________________________________
  void EventDisplay::ColorByAxis(const char* det) {
    std::string detector = det;
    if (TPCorOD(detector)!=EventDisplay::GetDetectorInActivePad()) {
      std::cout<<"Requested detector is not drawn in current canvas."<<std::endl;
      return;
    }    
    double start_t = EventDisplay::GetAxisValue("min");
    double end_t   = EventDisplay::GetAxisValue("max");
    EventDisplay::ColorByStartEnd(detector, start_t, end_t);
  }

  //________________________________________________________________________________________
  void EventDisplay::ColorByStartEnd(std::string detector, double start_t, double end_t, double max_integral_override, bool draw_palette) {
    if (detector=="tpc"&&!tpc_geo_enabled) return;
    if (detector=="lsv"&&!lsv_geo_enabled) return;
    if (detector=="wt" &&!wt_geo_enabled)  return;
    TMultiGraph* mg_chan;
    if (detector=="tpc") mg_chan = tpc_chan;
    if (detector=="lsv") mg_chan = lsv_ampl_chan;
    if (detector=="wt")  mg_chan = wt_ampl_chan;      
    if (!mg_chan) {std::cout<<"Error coloring "<<detector<<std::endl; return;}
    // Get list of channels
    TList* list_graphs = mg_chan->GetListOfGraphs();
    const int N_channels = list_graphs->GetSize();
    // Get channel integrals and max integral
    double max_integral  = 0; // needed to set palette scale
    std::vector<int>    chan_id; // chan ids from mg
    std::vector<double> chan_integral; // used to fill pmts 
    // loop over channels to find max integral
    for (int i=0;i<N_channels;i++) {
      TGraph* gr = (TGraph*)list_graphs->At(i);
      // Get integral
      double integral = EventDisplay::GetGraphIntegral(gr,start_t,end_t);
      integral = EventDisplay::AdjustIntegral(detector,integral);
      chan_integral.push_back(integral);
      if (integral>max_integral) max_integral=integral;
      // Get channel id
      int id = EventDisplay::GetChannelIDFromMultigraphID(i,mg_chan);
      //std::cout<<"channel "<<i<<" integral "<<integral<<"\n";
      chan_id.push_back(id);
    }
    std::cout<<"Max integral in region: "<<max_integral<<std::endl;
    // Check if override was set
    if (max_integral_override>0) 
      max_integral = max_integral_override; 
    // Set palette
    TEveRGBAPalette* pal = EventDisplay::MakePalette();      
    // Get detector list
    TEveElementList* detector_list;
    if (detector=="tpc") detector_list = tpc_geometry; 
    if (detector=="lsv") detector_list = lsv_geometry; 
    if (detector=="wt")  detector_list = wt_geometry; 
    // Color pmts by integral
    for (int i=0;i<N_channels;i++) {
      int id  = chan_id.at(i);
      double integral = chan_integral.at(i);
      TEveGeoShape* pmt = (TEveGeoShape*)detector_list->FindChild(Form("%s_channel_%d",detector.c_str(),id));
      if (!pmt) continue;
      //      std::cout<<"Channel "<<id<<" integral: "<<integral<<std::endl;
      const UChar_t* col = pal->ColorFromValue(integral/max_integral*100);
      //      std::cout<<"Coloring pmt "<<id<<"\n";
      pmt->SetMainColorRGB(col[0],col[1],col[2]);
    }
    TGLViewer* glv = gEve->GetDefaultGLViewer();
    if (!glv) return;
    // Get palette overlay
    double po_x = 0.7;
    double po_y; // detector dependent
    double width = 0.28;
    double height = 0.05;
    if (detector=="tpc") {delete tpc_palette; po_y = 0.25;} 
    if (detector=="lsv") {delete lsv_palette; po_y = 0.15;} 
    if (detector=="wt")  {delete wt_palette;  po_y = 0.05;} 
    TEveRGBAPaletteOverlay* po = new TEveRGBAPaletteOverlay(pal, po_x, po_y, width, height);
    if (draw_palette) glv->AddOverlayElement(po);
    gEve->Redraw3D(kTRUE);
    gEve->GetBrowser()->GetTabRight()->SetTab(0);
    gSystem->ProcessEvents();
    if (detector=="tpc") tpc_palette = po;
    if (detector=="lsv") lsv_palette = po;
    if (detector=="wt")  wt_palette  = po;
  }

  //________________________________________________________________________________________
  double EventDisplay::GetBinWidth(std::string detector) {
    TGraph* gr = EventDisplay::GetSumGraph(detector);
    int nbins = gr->GetN();
    double* x = new double[nbins];
    x = gr->GetX();
    //    std::cout<<"limits "<<x[nbins-1]<<" "<<x[0]<<"\n";
    double window_size = x[nbins-1]-x[0];
    //    delete x;
    return window_size/nbins; // returns time/bin
  }

  //________________________________________________________________________________________
  double EventDisplay::GetGraphIntegral(TGraph* g, double start_t, double end_t) {
    TH1D* h = new TH1D("h","h",g->GetN(),g->GetX()[0],g->GetX()[g->GetN()-1]);
    for(int i=0; i<g->GetN();i++) h->Fill(g->GetX()[i],g->GetY()[i]);
    int start_bin = h->GetXaxis()->FindBin(start_t);
    int end_bin = h->GetXaxis()->FindBin(end_t);
    double integral=h->Integral(start_bin,end_bin);
    delete h;
    return integral;
  }

  //________________________________________________________________________________________
  double EventDisplay::AdjustIntegral(std::string detector, double integral) {
    integral=fabs(integral);
    if (integral<0.0001) integral = 0.0001; // prevent log(0)
    if (detector=="tpc") {
      if (tpc_gif_frame->check_box_log->IsOn()) {
	integral = log10(integral);
	if (integral<0) integral = 0.;
	return integral;
      }
      else
	return integral;
    }
    if (detector=="lsv") {
      if (lsv_gif_frame->check_box_log->IsOn()) {
	integral = log10(integral);
	if (integral<0) integral = 0.;
	return integral;
      }
      else
	return integral;
    }
    if (detector=="wt") {
      if (wt_gif_frame->check_box_log->IsOn()) {
	integral = log10(integral);
	if (integral<0) integral = 0.;
	return integral;
      }
      else
	return integral;
    }
  }

  //________________________________________________________________________________________
  TGraph* EventDisplay::GetSumGraph(std::string detector) {
    TMultiGraph* mg = GetSumMultiGraph(detector);
    if (!mg) return NULL;
    TList* list_graphs = mg->GetListOfGraphs();
    for (int i=0;i<list_graphs->GetSize();i++) {
      TGraph* gr = (TGraph*)list_graphs->At(i);
      std::string obj_name = gr->GetName();
      if ((obj_name.find("gr_")!= std::string::npos)&&
	  (obj_name.find("sum")!= std::string::npos)) 
	return gr;
    }
    return NULL;
  }

  //________________________________________________________________________________________
  TMultiGraph* EventDisplay::GetSumMultiGraph(std::string detector) {
    if (detector=="tpc") return tpc_sum;
    if (detector=="lsv") return lsv_ampl_sum;
    if (detector=="wt")  return wt_ampl_sum;
    return NULL;
  }

  //________________________________________________________________________________________
  TMultiGraph* EventDisplay::GetChannelMultiGraph(std::string detector) {
    if (detector=="tpc") return tpc_chan;
    if (detector=="lsv") return lsv_ampl_chan;
    if (detector=="wt")  return wt_ampl_chan;
    return NULL;
  }

  //________________________________________________________________________________________
  TGraph* EventDisplay::GetChannelGraph(std::string detector, int ch) {
    TMultiGraph* mg = GetChannelMultiGraph(detector);
    if (!mg) return NULL;
    int mg_id = GetMultigraphIDFromChannelID(ch,mg);
    return (TGraph*)mg->GetListOfGraphs()->At(mg_id);
  }

  //________________________________________________________________________________________  
  void EventDisplay::SetIntegralGraph(TGraph* gr) {
    const int N = gr->GetN();
    const double* x = gr->GetX();
    const double* y = gr->GetY();
    const double max = std::max(fabs(TMath::MaxElement(N,y)),fabs(TMath::MinElement(N,y)));
    double iy[N];
    double integral(0.);
    // Fill integral values
    for (int i=0;i<N;i++) {
      integral+=y[i];
      iy[i]=integral;
    }
    // Scale the integral
    for (int i=0;i<N;i++) {
      iy[i]=iy[i]*fabs(max/integral);
    }
    if (wf_integral) delete wf_integral;
    wf_integral = new TGraph(N,x,iy);
    wf_integral->SetLineColor(kBlue);
    return;
  }

  //________________________________________________________________________________________
  TEveRGBAPalette* EventDisplay::MakePalette() {
#ifdef WHITE_BACKGROUND
    const int NRGBs = 4;
    const int NCont = 100;
    double stops[NRGBs] = { 0.00, 0.40, 0.60, 1.00 };
    double red[NRGBs]   = { 0.15, 1.00, 1.00, 1.00 };
    double green[NRGBs] = { 0.15, 1.00, 1.00, 1.00 };
    double blue[NRGBs]  = { 0.15, 1.00, 1.00, 1.00 };
#else
    const int NRGBs = 6;
    const int NCont = 100;
    double stops[NRGBs] = { 0.00, 0.05, 0.34, 0.61, 0.84, 1.00 };
    double red[NRGBs]   = { 0.00, 0.00, 0.00, 0.87, 1.00, 0.51 };
    double green[NRGBs] = { 0.00, 0.00, 0.81, 1.00, 0.20, 0.00 };
    double blue[NRGBs]  = { 0.30, 0.51, 1.00, 0.12, 0.00, 0.00 };
#endif
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    TEveRGBAPalette *pal = new TEveRGBAPalette(0,100);
    return pal;
  }

  //________________________________________________________________________________________
  void EventDisplay::DrawTPCPulses(int ch) {
    for(int i=0;i<tpc_pulse_vec.size();i++) {
      // Draw pulse box
      double min;
      if (ch == display::channeltype::kSumChannel)
	min = tpc_pulse_vec.at(i)->base + tpc_pulse_vec.at(i)->height;
      else min = gPad->GetUymin();
      TBox* box = new TBox(tpc_pulse_vec.at(i)->start_us, min, tpc_pulse_vec.at(i)->end_us, tpc_pulse_vec.at(i)->base);
      box->SetLineColor(kGreen);
      box->SetFillStyle(0);
      box->Draw();
      // Draw peak line for sum channel
      if (ch == display::channeltype::kSumChannel) {
	TLine* line = new TLine(tpc_pulse_vec.at(i)->peak_us,tpc_pulse_vec.at(i)->base,tpc_pulse_vec.at(i)->peak_us,tpc_pulse_vec.at(i)->base + tpc_pulse_vec.at(i)->height);
	line->SetLineColor(kMagenta);
	line->Draw();
      }
    }
  }

  //________________________________________________________________________________________
  void EventDisplay::DrawTPCSPEs(int ch) {
    for(int i=0;i<tpc_spe_vec.size();i++) {
      if (tpc_spe_vec.at(i)->channel != ch) continue;  // Only draw SPEs for the plotted channel
      TBox* box = new TBox(tpc_spe_vec.at(i)->start_us, tpc_spe_vec.at(i)->base + tpc_spe_vec.at(i)->height, tpc_spe_vec.at(i)->end_us, tpc_spe_vec.at(i)->base);
      box->SetLineColor(kRed);
      box->SetFillStyle(0);
      box->Draw();
    }
  }

  //________________________________________________________________________________________
  void EventDisplay::PrintTPCPulses() {
    for(int i=0; i<tpc_pulse_vec.size(); i++) {
      std::cout<<"Pulse: " <<i
	       <<"\tStart: " <<tpc_pulse_vec.at(i)->start_us
	       <<"\tEnd: "   <<tpc_pulse_vec.at(i)->end_us
	       <<"\tPeak: "  <<tpc_pulse_vec.at(i)->peak_us
	       <<"\tBase: "  <<tpc_pulse_vec.at(i)->base
	       <<"\tHeight: "<<tpc_pulse_vec.at(i)->height
	       <<std::endl;
    }
  }          
                    
  //________________________________________________________________________________________
  void EventDisplay::DrawLSVClusters() {
    for(int i=0;i<lsv_cluster_vec.size();i++) {
      TBox* box = new TBox(lsv_cluster_vec.at(i)->start_ns, 0, lsv_cluster_vec.at(i)->end_ns, gPad->GetUymax());
      box->SetLineColor(kGreen);
      box->SetFillStyle(0);
      box->Draw();
    }
  }

  //________________________________________________________________________________________
  void EventDisplay::PrintLSVClusters() {
    for(int i=0; i<lsv_cluster_vec.size(); i++) {
      std::cout<<"Cluster: " <<i
	       <<"\tStart: " <<lsv_cluster_vec.at(i)->start_ns
	       <<"\tEnd: "   <<lsv_cluster_vec.at(i)->end_ns
	       <<"\tCharge: "<<lsv_cluster_vec.at(i)->charge
	       <<"\tHeight: "<<lsv_cluster_vec.at(i)->height
	       <<"\tMax Multiplicity: "<<lsv_cluster_vec.at(i)->max_multiplicity
	       <<std::endl;
    }
  }          

  //________________________________________________________________________________________
  void EventDisplay::DrawLSVROIs() {
    for(int i=0;i<lsv_roi_vec.size();i++) {
      TBox* box = new TBox(lsv_roi_vec.at(i)->start_ns, 0, lsv_roi_vec.at(i)->end_ns, gPad->GetUymax());
      box->SetLineColor(kRed);
      box->SetFillStyle(0);
      box->Draw();
    }
  }

  //________________________________________________________________________________________
  void EventDisplay::PrintLSVROIs() {
    double start_ns;
    double end_ns;
    double charge;
    int max_multiplicity;
    lsv_roi_tree->SetBranchAddress("start_ns",&start_ns);
    lsv_roi_tree->SetBranchAddress("end_ns",&end_ns);
    lsv_roi_tree->SetBranchAddress("charge",&charge);
    lsv_roi_tree->SetBranchAddress("max_multiplicity",&max_multiplicity);
    for(int i=0; i<lsv_roi_tree->GetEntries(); i++) {
      lsv_roi_tree->GetEntry(i);
      std::cout<<"ROI: "     <<i
	       <<"\tStart: " <<start_ns
	       <<"\tEnd: "   <<end_ns
	       <<"\tCharge: "<<charge
	       <<"\tMax Multiplicity: "<<max_multiplicity
	       <<std::endl;
    }
  }

  //________________________________________________________________________________________
  void EventDisplay::NextEvent() {
    if (LoadEvent(current_event_id+1))
      EventDisplay::DrawDefaultWaveform();
 }
  void EventDisplay::PrevEvent() {
    if (LoadEvent(current_event_id-1))
      EventDisplay::DrawDefaultWaveform();
  }
  
}// end of display namespace

void PrintUsage() {
  std::cout<<"Correct Usage:"<<std::endl;  
  std::cout<<"./EventDisplay tpc_display_output.root"<<std::endl;
  std::cout<<"./EventDisplay od_display_output.root"<<std::endl;
  std::cout<<"./EventDisplay tpc_display_output.root od_display_output.root"<<std::endl;
  std::cout<<"./EventDisplay -d /directory/containing/tpc/display/output/files"<<std::endl;
}

int main(int argc, char* argv[]) {
  // Check arguments
  if (argc!=2&&argc!=3) {
    std::cout<<"Invalid usage.\n";
    PrintUsage();
    return 0;
  }
  std::string arg1;
  std::string arg2;
  if (argc>1) arg1 = argv[1];
  if (argc>2) arg2 = argv[2];
  
  // Set up environment
  gEnv->SetValue("Gui.IconFont",  "-*-helvetica-medium-r-*-*-14-*-*-*-*-*-iso8859-1");
  gEnv->SetValue("Gui.StatusFont","-*-helvetica-medium-r-*-*-14-*-*-*-*-*-iso8859-1");
  TApplication app("app",0,0);
  TApplication::NeedGraphicsLibs();
  gApplication->InitializeGraphics();
  TEveUtil::SetupEnvironment();
  TEveUtil::SetupGUI();
  // Set up display
  display::EventDisplay* sed;
  std::cout<<"Initializing display."<<std::endl;

  // There are currently two modes the event display can be run in.
  // 1. Single display file mode: 
  //           ./EventDisplay tpc_display_output od_display_output
  // 2. Directory mode (combines all tpc display files in a given directory)
  //           ./EventDisplay -d /path/to/tpc/display/files/directory
  //
  // We check the value of the first argument to determine the mode
  if (arg1 == "-d") {
    // Open the display with the directory path
    sed = new display::EventDisplay(arg2);
  } else {
    // Verify input files. Make sure root files contain 
    // TTrees that can be read by the event display
    TFile* f;
    TTree* t;
    std::string tpc_filepath="";
    std::string  od_filepath="";
    // Verify first file
    if (argc>1) {
      if (arg1.find("tpc") != std::string::npos) tpc_filepath = arg1;
      else if (arg1.find("od") != std::string::npos) od_filepath = arg1;
      else {
	std::cout<<"\nError. Input file "<<arg1<<" not recognized." 
	  "Must contain \"tpc\" or \"od\" in the file name."<<std::endl; 
	return 0;
      }
      // The commented way of doing it caused a crash when just an od file was given
      // f = new TFile(arg1.c_str(),"UPDATE");
      // if (f->FindObjectAny("tpc_display_tree")) tpc_filepath = arg1;
      // if (f->FindObjectAny("od_display_tree"))  od_filepath  = arg1;
      // f->Close();
    }
    // Verify second file
    if (argc>2) {
      if (arg2.find("tpc") != std::string::npos) tpc_filepath = arg2;
      else if (arg2.find("od") != std::string::npos) od_filepath = arg2;
      else {
	std::cout<<"\nError. Input file "<<arg2<<" not recognized." 
	  "Must contain \"tpc\" or \"od\" in the file name."<<std::endl; 
	return 0;
      }
    }
    if (tpc_filepath==""&&od_filepath=="") {
      std::cout<<"\nError: No events found. Aborting."<<std::endl;
      return 0;
    }
    // Open the display
    sed = new display::EventDisplay(tpc_filepath,od_filepath);
  }
  std::cout<<"Loaded display."<<std::endl;
  if (!sed) {
    std::cout<<"Error during initialization."<<std::endl;
    return 0;
  }
  sed->Create();
  std::cout<<"Created display."<<std::endl;
  app.Run();
  return 0;
}
