#include <iostream>

#include "TSystem.h"
#include "TFile.h"
#include "TChain.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"
#include "TString.h"
#include "TBranch.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TH1D.h"
using namespace std;

int main(int argc, char* argv[]) {
  // Directory containing output files to be averaged should be the first argument
  if (argc<4) {
    cout<<"Usage: ./AverageWaveform <display_output_directory> <pulse_id> <pre_start_time> <post_start_time> <number_of_histogram_bins> <max_number_of_waveforms>"<<endl;
    return 0;
  }
  char* dir = argv[1];
  int pulse_id = std::atoi(argv[2]);
  double pre_start_time = std::atof(argv[3]);
  double post_start_time = std::atof(argv[4]);
  int nbins = std::atof(argv[5]);
  int max_files = std::atof(argv[6]);
  TString stringdir = dir;
  void* opendir = gSystem->OpenDirectory(dir);
  const char* fname;
  TChain* tpc_display_tree  = new TChain("display/tpc_display_tree");
  int i_files = 0;
  while((fname = (char*)gSystem->GetDirEntry(opendir)) && i_files<max_files) {
    TString filename = fname;
    if(!filename.BeginsWith("tpc"))
      continue;
    if(!filename.EndsWith(".root"))
      continue;
    TString filepath = stringdir+"/"+filename;
    //    TFile* f = TFile::Open(filepath);
    cout<<"Adding "<<filepath.Data()<<endl;
    tpc_display_tree->Add(filepath);
    i_files++;
  }
  
  int tpc_run_id, tpc_event_id;
  TMultiGraph* tpc_sum = 0;
  //  TBranch* b_tpc_sum = 0;
  TMultiGraph* tpc_chan = 0;
  TTree* tpc_pulse_tree = 0;
  tpc_display_tree->SetBranchAddress("tpc_run_id",     &tpc_run_id);
  tpc_display_tree->SetBranchAddress("tpc_event_id",   &tpc_event_id);
  tpc_display_tree->SetBranchAddress("tpc_sum",        &tpc_sum);//, &b_tpc_sum);
  tpc_display_tree->SetBranchAddress("tpc_chan",       &tpc_chan);
  tpc_display_tree->SetBranchAddress("tpc_pulse_tree", &tpc_pulse_tree);

  double start_us;
  double end_us;
  double peak_us;
  double base;
  double height;
  
  double interval = 0.004; // 4 ns interval
  const int N = (pre_start_time+post_start_time)/interval;
  double* X = new double[N];
  double* Y = new double[N];  
  for (int i=0; i<N; i++) {
    X[i] = i*interval; 
    Y[i] = 0.;
  }

  TString outname = "average_"+stringdir+".root";
  TFile* fout = new TFile(outname,"RECREATE");


  //  TGraph gr;
  // int n;
  // double* x;
  // double* y;
  
  for (int i=0; i<tpc_display_tree->GetEntries(); i++) {
    cout<<i<<"/"<<min((int)tpc_display_tree->GetEntries(),max_files)<<endl;
    tpc_display_tree->GetEntry(i);
    if (!tpc_sum) continue;
    if (!tpc_sum->GetListOfGraphs()) continue;
    if (!tpc_pulse_tree) continue;
    tpc_pulse_tree->SetBranchAddress("start_us",&start_us);
    tpc_pulse_tree->SetBranchAddress("end_us",&end_us);
    tpc_pulse_tree->SetBranchAddress("peak_us",&peak_us);
    tpc_pulse_tree->SetBranchAddress("base",&base);
    tpc_pulse_tree->SetBranchAddress("height",&height);
    

    TGraph* gr = (TGraph*)(tpc_sum->GetListOfGraphs()->First());
    int n = gr->GetN();
    double* x = gr->GetX();
    double* y = gr->GetY();
    // TGraph* gr = (TGraph*)(tpc_sum->GetListOfGraphs()->First());
    // n = gr->GetN();
    // x = gr->GetX();
    // y = gr->GetY();


    // Get alignment
    if (!tpc_pulse_tree->GetEntry(pulse_id)) continue;
    double average_start = start_us;
    for (int j=0; j<n; j++) {
      if (x[j]<start_us) continue;
      if (fabs(y[j])>0.5*fabs(height)) { // Align by half maximum
	average_start = x[j];
	break;
      }
      if (x[j]>end_us) break;
    }
    
    // Skip events with a second pulse in the desired window
    bool skip = 0;
    for (int j=pulse_id+1; j<tpc_pulse_tree->GetEntries(); j++) {
      tpc_pulse_tree->GetEntry(j);
      if (start_us<average_start+post_start_time) skip = 1;
    }
    if (skip) {
      //      delete gr;
      // delete x;
      // delete y;
      continue;
    }
    //    cout<<tpc_run_id<<" "<<tpc_event_id<<" "<<start_us<<endl;    
    // Add waveform
    int k=0; // iterator over final fraph which goes from 0 to pre_start_time+post_start_time
    for (int j=0; k<N && j<n; j++) {
      if (x[j]<average_start-pre_start_time) continue;
      Y[k] += -y[j]*1e-8; // Invert graph from negative to positive
      //      if (k==100) cout<<Y[k]<<endl;
      k++;
    }
     //    delete gr;
    // delete x;
    // delete y;

      
  }

  // Normalize integral to 1
  double sum=0.;
  for (int i=0; i<N; i++) {
    sum+=Y[i];
  }
  for (int i=0; i<N; i++) {
    Y[i]/=sum;;
  }
  

  cout<<"Saving to "<<outname<<endl;

  // Make TGraph
  TGraph* tgr = new TGraph(N, X, Y);
  tgr->Write();

  // Make Histogram
  TH1D* h = new TH1D("Histogram","Average Waveform",nbins,-pre_start_time,post_start_time);
  for (int i=0; i<N; i++) h->Fill(X[i],Y[i]);
  h->Write();

  // Make Canvas
  TCanvas* c = new TCanvas("Canvas","Canvas",1000,800);
  tgr->Draw("al");
  c->SetLogy();
  tgr->GetYaxis()->SetRangeUser(1e-9,1e-1);
  c->Write();
  fout->Close();
  
  

}
