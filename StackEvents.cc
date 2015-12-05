#include "./StackEvents.hh"
#include <TH2.h>
#include <TCanvas.h>
#include <fstream>

using namespace std;

StackEvents::StackEvents(string inDirFileList, string outFileName){
  vector<string> vInFileNames;
  ifstream iF;
  iF.open(inDirFileList);
  cout<<"opening inDirFileList: "<<inDirFileList<<endl;

  fOutName=outFileName;

  string line="";
  while(iF>>line){
    
    if (line.find(".root")==string::npos){ 
      cout << "line does not have .root in it:" << line << endl;
      exit(-1);
    } else if(line.find("#")==0){ //allows to comment out events
      cout << "skip line: " << line << endl;
    }else {
      //cout << line << endl;
      vInFileNames.push_back(line);
    }
    

  }

  if(!vInFileNames.size()){
    cout << "no entries found in: " << inDirFileList << endl;
    exit(-1);
  }

  InitVars();
  if(vInFileNames.size()>1) LoadDirectoryIntoTChain(vInFileNames);


}

void StackEvents::Finalize(){

  //cout << "is writable: " << fOut->IsWritable() << endl;
  //build the TGraphs and TMultiGraph out of the vWaveforms vector of arrays
  TMultiGraph *mgCh=new TMultiGraph();
  TMultiGraph *mgSum=new TMultiGraph();

  TGraph *grX=(TGraph*)tpc_chan_in->GetListOfGraphs()->At(0);
  for(int i=0;i<n_channels;++i){
    TGraph *grIn=new TGraph(n_samples, grX->GetX(), &vStackedWaveforms.at(i)[0]);
    mgCh->Add(grIn);
  }
  TGraph *grSum=new TGraph(n_samples, grX->GetX(), &StackedSum[0]);
  mgSum->Add(grSum);


  cout << "outfile: " << fOutName << endl;
  TFile *fOut = TFile::Open(fOutName.c_str(), "RECREATE"); //open it here,
  TDirectory *fDisplayDir=fOut->mkdir("display");
  //TDirectory *tdDisplay=new TDirectory("display", "display", "", fOut);
  fDisplayDir->cd();
  //tpc_settings_tree_out = new TChain("display/tpc_settings_tree");
  TTree *tpc_display_tree_out = tpc_display_tree_in->CloneTree(0);

  tpc_display_tree_out->SetBranchAddress("tpc_run_id",     &tpc_run_id_out, &b_tpc_run_id_out);
  tpc_display_tree_out->SetBranchAddress("tpc_event_id",   &tpc_event_id_out, &b_tpc_event_id_out);
  tpc_display_tree_out->SetBranchAddress("tpc_sum",        &tpc_sum_out, &b_tpc_sum_out);
  tpc_display_tree_out->SetBranchAddress("tpc_chan",       &tpc_chan_out, &b_tpc_chan_out);
  tpc_display_tree_out->SetBranchAddress("tpc_pulse_tree", &tpc_pulse_tree_out, &b_tpc_pulse_tree_out);
  tpc_display_tree_out->SetBranchAddress("tpc_spe_tree",   &tpc_spe_tree_out, &b_tpc_spe_tree_out);
  
  tpc_run_id_out=-1;
  tpc_event_id_out=-1;
  tpc_chan_out=mgCh;
  tpc_sum_out=mgSum;
  tpc_pulse_tree_out=0;
  tpc_spe_tree_out=0;

  tpc_display_tree_out->Fill();
  tpc_display_tree_out->Write();

  tpc_display_tree_in->SetBranchStatus("tpc_chan",0);
  TTree *tpc_display_tree_out_perEvent = tpc_display_tree_in->CloneTree(); //this is a clone to preserve the run_id and event_id (and other information, except for the memory intensive waveform channel multigraph, the sum channel is kept.
  tpc_display_tree_out_perEvent->SetNameTitle("tpc_display_tree_perEvent", "per event display tree from the input files");
  tpc_display_tree_out_perEvent->Write();

  TTree *tpc_settings_tree_out=tpc_settings_tree_in->CloneTree();
  tpc_settings_tree_out->Write();

  TTree *tpc_pulse_tree_out=tpc_pulse_tree_in->CloneTree();
  tpc_pulse_tree_out->Write();
  /*
  TH2F *h2 =new TH2F("h2", "all channels next to each other", 1000, -10, 20, 38+1, 0, 38+1);
  h2->Sumw2();
  for(int i=0;i<38+1;++i){
    TGraph* gr=(TGraph*)(mgCh->GetListOfGraphs()->At(i));
    if(i==38) gr=(TGraph*)(mgSum->GetListOfGraphs()->At(0));
    if(!gr) return;
    int counter=0;
    for(int j=0;j<gr->GetN();++j){
      //if(counter<40) std::cout << Form("gr2D: %d, %d, %f, %f", counter, i, gr->GetX()[j], gr->GetY()[j]) << std::endl;
      if(gr->GetX()[j]>20) break;
      //gr2D->SetPoint(counter, i, gr->GetX()[j], gr->GetY()[j]);
      h2->Fill(gr->GetX()[j],i,-gr->GetY()[j]);
      counter++;
    }
  }
  */
  //h2->Write();
  fOut->Close();

}


  ///////////////////////////////////////
  /////////// Initialization ////////////
  ///////////////////////////////////////
  void StackEvents::InitVars(){

    tpc_run_id_in=0;
    tpc_event_id_in=0;
    tpc_sum_in=0;
    tpc_chan_in=0;
    tpc_pulse_tree_in=0;
    tpc_spe_tree_in=0;

    tpc_run_id_out=0;
    tpc_event_id_out=0;
    tpc_sum_out=0;
    tpc_chan_out=0;
    tpc_pulse_tree_out=0;
    tpc_spe_tree_out=0;

    tpc_enabled=true;
    tpc_geo_enabled=false;

    //queried from the waveforms itself, see below
    n_channels=0; 
    n_samples=0;
  }


  //________________________________________________________________________________________
  void StackEvents::LoadDirectoryIntoTChain(vector<string> vInFileNames) {
    // Make chain out of all files in directory
    TChain* settings_chain = new TChain("display/tpc_settings_tree");
    TChain* display_chain = new TChain("display/tpc_display_tree");
    

    for(int i=0;i<vInFileNames.size();++i){
      cout<<"Adding "<<vInFileNames.at(i)<<" to chain.\n";
      settings_chain->Add(vInFileNames.at(i).c_str());
      display_chain->Add(vInFileNames.at(i).c_str());
    }
    if (!settings_chain || !display_chain) {
      //if (!display_chain) {
      cout<<"\nError: No valid TPC events found in settings or display chain.\n\n";
      return;
    }
    tpc_settings_tree_in = settings_chain;
    tpc_display_tree_in = display_chain;

    cout<<"Found a total of "<<tpc_display_tree_in->GetEntries()<<" TPC events."<<endl; 
    if (!tpc_settings_tree_in || !tpc_display_tree_in) {
    //if (!tpc_display_tree_in) {
      cout<<"No input trees were found"<<endl;
      return;
    }

    SetBranchAddresses_In();
    
  }  

//________________________________________________________________________________________
void StackEvents::SetBranchAddresses_In() {

  // Load TPC Settings
  if (tpc_settings_tree_in) {
    tpc_settings_tree_in->SetBranchAddress("tpc_enabled",    &tpc_enabled);
    tpc_settings_tree_in->SetBranchAddress("tpc_geo_enabled",&tpc_geo_enabled);
    tpc_settings_tree_in->GetEntry(0);
  }
  //cout << "tpc_enabled: " << tpc_enabled << endl;
  //cout << "tpc_geo_enabled: " << tpc_geo_enabled << endl;

  // Load TPC data
  if (tpc_display_tree_in) {
    tpc_display_tree_in->SetBranchAddress("tpc_run_id",     &tpc_run_id_in, &b_tpc_run_id_in);
    tpc_display_tree_in->SetBranchAddress("tpc_event_id",   &tpc_event_id_in, &b_tpc_event_id_in);
    tpc_display_tree_in->SetBranchAddress("tpc_sum",        &tpc_sum_in, &b_tpc_sum_in);
    tpc_display_tree_in->SetBranchAddress("tpc_chan",       &tpc_chan_in, &b_tpc_chan_in);
    tpc_display_tree_in->SetBranchAddress("tpc_pulse_tree", &tpc_pulse_tree_in, &b_tpc_pulse_tree_in);
    tpc_display_tree_in->SetBranchAddress("tpc_spe_tree",   &tpc_spe_tree_in, &b_tpc_spe_tree_in);
    tpc_display_tree_in->GetEntry(0); //requires the tree variables to be initialized, otherwise there are seg faults!
    //tpc_display_tree_in->SetMakeClass(1);
  }
  //cout << "tpc_run_id_in: " << tpc_run_id_in << endl;
  //cout << "tpc_event_id_in: " << tpc_event_id_in << endl;
}

/*
//________________________________________________________________________________________
void StackEvents::SetBranchAddresses_Out() {
  // Load TPC data
  if (tpc_display_tree_out) {
    tpc_display_tree_out->SetBranchAddress("tpc_run_id",     &tpc_run_id_out, &b_tpc_run_id_out);
    tpc_display_tree_out->SetBranchAddress("tpc_event_id",   &tpc_event_id_out, &b_tpc_event_id_out);
    tpc_display_tree_out->SetBranchAddress("tpc_sum",        &tpc_sum_out, &b_tpc_sum_out);
    tpc_display_tree_out->SetBranchAddress("tpc_chan",       &tpc_chan_out, &b_tpc_chan_out);
    tpc_display_tree_out->SetBranchAddress("tpc_pulse_tree", &tpc_pulse_tree_out, &b_tpc_pulse_tree_out);
    tpc_display_tree_out->SetBranchAddress("tpc_spe_tree",   &tpc_spe_tree_out, &b_tpc_spe_tree_out);
    tpc_display_tree_out->GetEntry(0);
  } else {
    cout << "Error: " << tpc_display_tree_out << endl;
  }

}
*/

  //________________________________________________________________________________________
void StackEvents::StackWaveforms(){
  TGraph *grIn;
  
  tpc_display_tree_in->GetEntry(0);
  n_channels=tpc_chan_in->GetListOfGraphs()->GetSize();
  if(n_channels>0){ 
    grIn = (TGraph*)tpc_chan_in->GetListOfGraphs()->At(0);
    n_samples=grIn->GetN();
  } else {
    cout << "Error: n_channels must be bigger than 0!" << endl;
    exit(-1);
  }


  //prepare the vector of stackedWaveforms
  for(int i=0; i<n_channels; ++i){ 
    vector<double> oneCh(n_samples, 0);
    //cout << "oneCh: " << &oneCh << endl;
    vStackedWaveforms.push_back(oneCh);
  }
  vector<double> oneCh(n_samples, 0);
  StackedSum=(oneCh);
  


  //for each event/ input file:
  for(int ev=0;ev<tpc_display_tree_in->GetEntries();ev++){
    tpc_display_tree_in->GetEntry(ev);
    cout << "Processing event: " << ev << endl;
    //input added to output
    for(int i=0;i<n_channels;++i){
      //cout << "-- Channel: " << i << endl;
      grIn=(TGraph*)(tpc_chan_in->GetListOfGraphs()->At(i));
      if(!grIn) {
	cout << "Chan: something went wrong with grIn: " << grIn << endl;
	exit(-1);
      }
      
      double *yIn=grIn->GetY();
      for(int j=0;j<n_samples;++j){
	vStackedWaveforms.at(i)[j]+=yIn[j];
      } 
    }
    //cout << endl;

    //the sum channel:
    if(tpc_sum_in->GetListOfGraphs()->GetSize()!=1){
      cout << "Error: sum channel should have only one channel: " << tpc_sum_in->GetListOfGraphs()->GetSize() << endl;
      exit(-1);
    }
    for(int i=0;i<tpc_sum_in->GetListOfGraphs()->GetSize();++i){
      grIn=(TGraph*)(tpc_sum_in->GetListOfGraphs()->At(i));
      if(!grIn) {
	cout << "Sum: something went wrong with grIn: " << grIn << endl;
	exit(-1);
      }

      double *yIn=grIn->GetY();
      for(int j=0;j<n_samples;++j){
        StackedSum[j]+=yIn[j];
      } 
    }
  }
  Finalize();
}
  //________________________________________________________________________________________
/*  int StackEvents::LoadEventTPC(const char* dettab) {
    
    if (!tpc_enabled) return 0;
    // Load TPC event
    const string detectortab = dettab;

    cout<<"Loading TPC event."<<endl;
    // Check if event exists
    if (selected >= tpc_display_tree->GetEntries() || selected < 0) {
      cout<<"Invalid event."<<endl;
      return 0;
    }
    // Clear structures from previous event
    for (int i=0;i<tpc_pulse_vec.size();++i) delete tpc_pulse_vec.at(i);
    for (int i=0;i<tpc_spe_vec.size();++i) delete tpc_spe_vec.at(i);
    tpc_pulse_vec.clear();      
    tpc_spe_vec.clear();      
    // Get event from tree
    tpc_display_tree->GetEntry(selected);
    current_event_id=selected;
    // Set Channel Colors
    display::SetChannelColors("tpc",tpc_chan);
    // Load TPC Pulses
    {
      const int N_pulses = tpc_pulse_tree->GetEntries();
      cout<<"Found "<<N_pulses<<" TPC pulses.\n";
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
      cout<<"Found "<<N_spes<<" TPC SPEs.\n";
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
    if (canvas) StackEvents::DrawDefaultWaveform("tpc");
    cout<<"Event loaded."<<endl;
    return 1;
    
  }
*/
  //________________________________________________________________________________________
/*  void StackEvents::DrawWaveform(const char* charinput) {
    
    const string input = charinput;
    TMultiGraph* mg_sum;
    TMultiGraph* mg_chan;
    WaveformFrame* wf_frame;
    if (input=="tpc") {
      mg_sum =  tpc_sum;
      mg_chan = tpc_chan;
      wf_frame = tpc_wf_frame;
    }

    if (!mg_sum)  return;
    if (!mg_chan) return;
      // Make new multigraph
      TMultiGraph* mg = new TMultiGraph(newname,newtitle);
      for (int i=0;i<tobjarr_multi_chan->GetEntries();i++) {
	const TString tstr_chan = ((TObjString*)tobjarr_multi_chan->At(i))->GetString();
	const int ch_id = tstr_chan.Atoi();
	const int mg_id = StackEvents::GetMultigraphIDFromChannelID(ch_id, mg_chan);
	if (mg_id<0) {cout<<"No graph was found for channel "<<ch_id<<"."<<endl;continue;}
	TGraph* gr = (TGraph*)(mg_chan->GetListOfGraphs()->At(mg_id));
	if (!gr) {cout<<"No graph was found for channel "<<ch_id<<"."<<endl; continue;}
	newtitle += ch_id;
	newtitle += ",";
	mg->Add(gr);
      }
      if (!mg->GetListOfGraphs()) {cout<<"No graphs were found for specified channels."<<endl;c->Update();return;}
      // Cut off ending comma
      newtitle = newtitle(0,newtitle.Length()-1);
      // Add Axis titles
      newtitle += oldtitle(oldtitle.First(";"),oldtitle.Length()-oldtitle.First(";"));
      mg->SetTitle(newtitle);
      // Draw new mulitgraph
      mg->Draw("al");
    } else if (selected == display::channeltype::kSumChannel) {
      if (input=="tpc" && !StackEvents::MultiGraphContainsIntegral(mg_sum)) { // add integral 
	TGraph* gr = (TGraph*)(mg_sum->GetListOfGraphs()->First());
	if (!gr) return;
	StackEvents::SetIntegralGraph(gr);
	mg_sum->Add(wf_integral);
      }
      mg_sum->Draw("al");
    } else if (selected == display::channeltype::kAllChannel) {
      if (input=="tpc" && !StackEvents::MultiGraphContainsIntegral(mg_chan)) { // add integral
	TGraph* gr = (TGraph*)(mg_sum->GetListOfGraphs()->First());
	if (!gr) return;
	StackEvents::SetIntegralGraph(gr,mg_chan);
	mg_chan->Add(wf_integral);
      }
      //mg_chan->Draw("al");
      //

      TH2F *h2 =new TH2F("h2", "all channels next to each other", 1000, -10, 20, 38+1, 0, 38+1);
      h2->Sumw2();
      for(int i=0;i<38+1;++i){
	TGraph* gr=(TGraph*)(mg_chan->GetListOfGraphs()->At(i));
	if(i==38) gr=(TGraph*)(mg_sum->GetListOfGraphs()->At(0));
	if(!gr) return;
	for(int j=0;j<gr->GetN();++j){
	  //cout << Form("gr2D: %d, %d, %f, %f", counter, i, gr->GetX()[j], gr->GetY()[j]) << endl;
	  if(gr->GetX()[j]>20) break;
	  //gr2D->SetPoint(counter, i, gr->GetX()[j], gr->GetY()[j]);
	  h2->Fill(gr->GetX()[j],i,-gr->GetY()[j]);
	  //counter++;
	}
      }
      
      h2->Draw("colz");

    } else {
      TGraph* gr = (TGraph*)(mg_chan->GetListOfGraphs()->At(selected));
      gr->Draw("al");
    }
    string new_det = StackEvents::GetDetectorInActivePad();
    // Zoom new graph to previously set limits, if any
    if (is_drawn && prev_det==new_det) StackEvents::ZoomAxis(start_t,end_t);
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
*/
  //________________________________________________________________________________________
/*  double StackEvents::GetMaxOfMultiGraph(TMultiGraph* mg, double start_t=-9.e9, double end_t=9.e9) {    
    TList* list_graphs = mg->GetListOfGraphs();
    int N_channels = list_graphs->GetSize();
     double max = 0;
    double maxtime = 0;
    double maxchan = 0;
    
    for (int i=0;i<N_channels;i++) {
      TGraph* gr = (TGraph*)list_graphs->At(i);
      if (gr->GetName()=="Integral") continue;
      if (!gr) continue;
      const int nbins = gr->GetN();
      const double *x = gr->GetX();//new double[nbins];
      const double *y = gr->GetY();//new double[nbins];
      //      x = gr->GetX();
      //      y = gr->GetY();
      vector<double> subset;
      for (int j=0;j<nbins;j++) {
	if (x[j]<start_t) continue;
	if (x[j]>end_t)   break;
	subset.push_back(y[j]);
      }
      //      delete x; //this causes seg fault
      //      delete y; //this causes seg fault
      double lower = fabs(*min_element(subset.begin(), subset.end()));
      double upper = fabs(*max_element(subset.begin(), subset.end()));
      double tempmax = (lower<upper)?upper:lower;
      if (tempmax>max) {
	max = tempmax;
      }
    }
    
    //    cout<<"Max of muligraph in region "<<max<<"\n";
    return max;
    
  }
*/
/*    
  //________________________________________________________________________________________
  void StackEvents::PrintTPCPulses() {
    double p0integral(9.e9);
    std::vector<display::TPCPulse*> tpc_pulse_vec;
    for(int i=0; i<tpc_pulse_vec.size(); i++) {
      double integral = StackEvents::GetGraphIntegral(StackEvents::GetSumGraph("tpc"),tpc_pulse_vec.at(i)->start_us,tpc_pulse_vec.at(i)->end_us);
      double integral90ns = StackEvents::GetGraphIntegral(StackEvents::GetSumGraph("tpc"),tpc_pulse_vec.at(i)->start_us,tpc_pulse_vec.at(i)->start_us+0.09);
      double f90 = integral90ns/integral;
      if (i==0) p0integral = integral;
      double ratio = integral/p0integral;
      cout<<setprecision(1)<<fixed;
      cout<<"Pulse: " <<setw(2)<<i
	       <<" Start: " <<setw(6)<<tpc_pulse_vec.at(i)->start_us
	       <<" End: "   <<setw(6)<<tpc_pulse_vec.at(i)->end_us
	       <<" Peak: "  <<setw(6)<<tpc_pulse_vec.at(i)->peak_us
	       <<" Base: "  <<setw(2)<<tpc_pulse_vec.at(i)->base
	       <<" Height: "<<setw(7)<<tpc_pulse_vec.at(i)->height
	       <<" f90: "   <<setprecision(5)<<setw(8)<<f90<<setprecision(1)
	       <<" Integral: "<<setw(8)<<integral
	       <<" Integral/Pulse0Integral: "<<setw(5)<<ratio
	       <<endl;
    }
  }          
                    
 //  void StackEvents::NextEvent() {
 //    if (LoadEvent(current_event_id+1))
 //      StackEvents::DrawDefaultWaveform();
 // }
 //  void StackEvents::PrevEvent() {
 //    if (LoadEvent(current_event_id-1))
 //      StackEvents::DrawDefaultWaveform();
 //  }
  
}// end of display namespace
*/

string GetArgument(string argwanted, int argc, char* argv[]) {
  for (int i=0;i<argc;i++) {
    string arg = argv[i];
    if (argwanted=="inDir" && (arg.find(".txt")!=string::npos)) return arg;
    if (argwanted=="out"  && (arg.find(".root") !=string::npos)) return arg;
  }
  return "";
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int main(int argc, char* argv[]) {
  // Check arguments
  if (argc<2||argc>4) {
    cout<<"Invalid usage."<< endl;
    cout<<"Correct Usage:"<<endl;  
    cout<<"./EventDisplay inDir.txt outfile.root"<<endl;
    cout<<"./EventDisplay outfile.root inDir.txt"<<endl;
    cout<<"where inDir.txt contains a list of filepaths, which are fed into TFile::Open() as is." << endl;
    return 0;
  }
  string inDirFileName = GetArgument("inDir",argc,argv);
  string outfile  = GetArgument("out" ,argc,argv);

  StackEvents se(inDirFileName, outfile);
  se.StackWaveforms();

  return 0;
}
