#include <iostream>
#include <string>
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
#include "TObjArray.h"
#include "TMath.h"
#include "TObjString.h"

using namespace std;

#include <fstream>
std::ifstream::pos_type filesize(const char* filename)
{
  std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
  return in.tellg(); 
}

int main(int argc, char* argv[]) {
  // Directory containing output files to be averaged should be the first argument
  if (argc<8) {
    cout<<"Usage: ./AverageWaveform <display_output_directory> <pulse_id> <pre_start_time> <post_start_time> <number_of_histogram_bins> <max_number_of_waveforms> <pre-pulse cut threshold>"<<endl;
    return 0;
  }
  char* dir = argv[1];
  int pulse_id = std::atoi(argv[2]);
  double pre_start_time = std::atof(argv[3]);
  double post_start_time = std::atof(argv[4]);
  int nbins = std::atof(argv[5]);
  int max_files = std::atof(argv[6]);
  double pre_pulse_threshold = std::atof(argv[7]);
  TString stringpath = dir;
  TObjArray* tobjarr_path = stringpath.Tokenize("/");
  const TString stringdir = ((TObjString*)tobjarr_path->Last())->GetString();
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
    TString filepath = stringpath+"/"+filename;
    //    TFile* f = TFile::Open(filepath);
    if (filesize(filepath.Data())<1000) {
      cout<<filepath.Data()<<" is too small to contain any waveforms. Skipping..."<<endl;
      continue;
    }
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
  double* eX = new double[N];
  double* eY = new double[N];  
  for (int i=0; i<N; i++) {
    X[i] = i*interval; 
    Y[i] = 0.;
    eX[i]= 0.;
    eY[i]= 0.;
  }
  TString outname = "average_"+stringdir+"_pulse"+to_string(pulse_id)+".root";
  TFile* fout = new TFile(outname,"RECREATE");


  //  TGraph gr;
  // int n;
  // double* x;
  // double* y;
  int N_waveforms = 0;
  for (int i=0; i<tpc_display_tree->GetEntries(); i++) {
    cout<<i<<"/"<<min((int)tpc_display_tree->GetEntries(),max_files)<<endl;
    tpc_display_tree->GetEntry(i);
    if (!tpc_sum) continue;
    if (!tpc_sum->GetListOfGraphs()) continue;
    if (!tpc_pulse_tree) continue;

    // Get Run and event id from file
    TString fname = tpc_display_tree->GetFile()->GetName();
    int slash = fname.Last('/');
    fname.Remove(0,slash+1);
    int i1 = fname.Index("_r");
    int i2 = fname.Index("_e");
    int i3 = fname.Index(".r");
    TString tsr (fname(i1+2,i2-i1-2));
    TString tse (fname(i2+2,i3-i2-2));
    cout<<fname.Data()<<endl;
    string sr = tsr.Data();
    string se = tse.Data();
    int ir = stoi(sr);
    int ie = stoi(se);
    if (ir!=tpc_run_id||ie!=tpc_event_id) {
      cout<<"Error: Event info (r"<<tpc_run_id<<" e"<<tpc_event_id<<")"<<" does not match file info (r"<<ir<<" e"<<ie<<"). Skipping..."<<endl;
      continue;
    }
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
      if (TMath::Abs(y[j])>0.5*TMath::Abs(height)) { // Align by half maximum
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

    // Detect prepulse       
    double prepulse_integral=0.;
    for (int i=0; i<n; i++) {
      if (x[i]<average_start-5) continue;
      else if (x[i]>average_start-0.02) break;
      else prepulse_integral+=fabs(y[i]);
    }
    cout<<"Prepulse integral (-5us to -20ns) "<<prepulse_integral<<endl;
    if (prepulse_integral>pre_pulse_threshold) {
      cout<<"Prepulse detected in "<<fname<<". Skipping..."<<endl;
      skip=true; // Skip events with a pre-pulse
    }

    if (skip) {
      continue;
    }
    // Add waveform
    int k=0; // iterator over averaged graph which goes from 0 to pre_start_time+post_start_time
    for (int j=0; k<N && j<n; j++) { // j<n prevents overflow of the individual graph
      if (x[j]<average_start-pre_start_time) continue;
      Y[k] += fabs(y[j]); // Invert graph from negative to positive
      k++;
    }
    N_waveforms++;
    // Look for prepulse
    // Normalize integral to 1                                                                                                                              
    // double s=0.;
    // for (int i=0; i<n; i++) {
    //   s+=y[i];
    // }
    // for (int i=0; i<n; i++) {
    //   y[i]/=s;
    // }

  }

  // Errors
  for (int i=0; i<N; i++) {
    eY[i]=sqrt(Y[i]);
  }
  
  // Normalize by number of waveforms
  for (int i=0; i<N; i++) {
    Y[i]/=N_waveforms;
    eY[i]/=N_waveforms;
  }

  // Make TGraph
  TGraphErrors* tgr_N = new TGraphErrors(N, X, Y, eX, eY);
  tgr_N->SetName("Graph_NormN");
  tgr_N->Write();

  // Make Histogram
  TH1D* h_N = new TH1D("Histogram_NormN","Average Waveform",nbins,-pre_start_time,post_start_time);
  for (int i=0; i<N; i++) h_N->Fill(X[i],Y[i]);
  h_N->Write();

  // Make Canvas
  TCanvas* c_N = new TCanvas("Canvas_NormN","Canvas",1000,800);
  tgr_N->Draw("al");
  c_N->SetLogy();
  tgr_N->GetYaxis()->SetRangeUser(1e-9,1.5*TMath::MaxElement(N,Y));
  c_N->Write();

  // Normalize integral to 1
  double sum=0.;
  for (int i=1; i<N; i++) {
    sum+=Y[i]*(X[i]-X[i-1]);
  }
  for (int i=0; i<N; i++) {
    Y[i]/=sum;
    eY[i]/=sum;
  }
 
  // Make TGraph
  TGraphErrors* tgr = new TGraphErrors(N, X, Y, eX, eY);
  tgr->Write();

  // Make Integral Graph
  double iY[N];
  iY[0] = 0;
  for  (int i=1; i<N; i++) iY[i]=iY[i-1]+Y[i]*(X[i]-X[i-1]);
  TGraph* tgr_I = new TGraph(N, X, iY);
  tgr_I->SetName("Graph_Integral");
  tgr_I->SetLineColor(kBlue);
  tgr_I->Write();

  // Make Histogram
  TH1D* h = new TH1D("Histogram","Average Waveform",nbins,-pre_start_time,post_start_time);
  for (int i=0; i<N; i++) h->Fill(X[i],Y[i]);
  h->Write();

  // Make Canvas
  TCanvas* c = new TCanvas("Canvas","Canvas",1000,800);
  tgr->Draw("al");
  c->SetLogy();
  tgr->GetYaxis()->SetRangeUser(1e-9,1.5*TMath::MaxElement(N,Y));
  c->Write();
  
  cout<<"Saving to "<<outname<<endl;  
  fout->Close();

}
