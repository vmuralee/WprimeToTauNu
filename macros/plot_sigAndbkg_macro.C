#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include "TMath.h"
#include "TRandom.h"
#include "TFile.h"
#include <stdio.h>
#include <cstring>
#include "TLatex.h"

void HistBinContent(vector<TH1F*>hist){
  for(auto h : hist){
    cout<<" Histogram "<<endl;
    for(int ib = 1; ib < h->GetNbinsX(); ib++){
      cout<<"Bin no. "<<ib<<"  BinContent "<<h->GetBinContent(ib)<<" Bin error "<<h->GetBinError(ib)<<endl;
    }
  }

}


void plot_sigAndbkg_macro(Int_t nbins=12,Float_t xlow=0,Float_t xhigh=2500){

  vector <TString> rootfiles = {   //List of files 
    "SkimWprimeM-1000GeVToTauNu",
    "SkimWprimeM-2000GeVToTauNu",
    "SkimWprimeM-3000GeVToTauNu",
    "SkimWToLNuM-200",
    "SkimWToLNuM-500", 
    "SkimWToLNuM-1000",
    "SkimWToLNuM-3000",
    "SkimWJetsHT100-200ToLNu",
    "SkimWJetsHT200-400ToLNu",
    "SkimWJetsHT400-600ToLNu",
    "SkimWJetsHT600-infToLNu"
  };
  TH1F* hgen[rootfiles.size()];
  TH1F* hrec[rootfiles.size()];
    
  int nfile =1;
  vector <float> xsec = {2.457,0.107,0.011,6.7,0.2676,0.0171,0.0000415,1345,359,48.91,18};
  vector <int> TotEvents = {500000,500000,500000,500000,1000000,1000000,1000000,12542004,4949568,1943664,1041358};
  for(auto filename: rootfiles){
    TString path = "/home/vinay/private/HEPTools/WprimeToTauNu/data/ntuples/";
    TFile* file = new TFile(path+filename+".root");
    TTree* tree = (TTree*)file->Get("analysisTree");
    //Long_t TotEvents = tree->GetEntries();
    TString histname_gen = filename+"_gen";
    TString histname_rec = filename+"_rec";

    hgen[nfile-1]  = new TH1F(histname_gen,"",nbins,xlow,xhigh);
    hrec[nfile-1]  = new TH1F(histname_rec,"",nbins,xlow,xhigh);
    
    // hgen[nfile-1]->SetLineColor(nfile);
    // hrec[nfile-1]->SetLineColor(nfile);
    // hgen[nfile-1]->SetLineStyle(2);
    // hrec[nfile-1]->SetLineStyle(1);
    // hgen[nfile-1]->SetLineWidth(3);
    // hrec[nfile-1]->SetLineWidth(3);

    float xsec_weight = xsec[nfile-1]*1000*2.3/TotEvents[nfile-1];
    TCut gen_cut = "gen_pTratio > 0.7 && gen_pTratio < 1.3 && gen_dtheta > 2.4";
    TCut rec_cut = "pTratio > 0.7 && pTratio < 1.3 && dtheta > 2.4 && tau_vis_pt > 80 && met >120";
    TCanvas *dummy = new TCanvas();
    //tree->Draw("gen_mT>>"+histname_gen,gen_cut);
    tree->Draw("mT>>"+histname_rec,rec_cut);
    hrec[nfile-1]->Scale(xsec_weight);
    hgen[nfile-1]->Scale(xsec_weight);
    hrec[nfile-1]->Sumw2();
    dummy->Delete();
    ++nfile;
  }
  for(int i=4; i < rootfiles.size(); i++){
    hrec[3]->Add(hrec[i]);
  }

  TH1F* hsig1TeV = new TH1F("hsig1TeV","",nbins,xlow,xhigh);
  TH1F* hsig2TeV = new TH1F("hsig2TeV","",nbins,xlow,xhigh);
  TH1F* hsig3TeV = new TH1F("hsig3TeV","",nbins,xlow,xhigh);

  TH1F* hWJets = new TH1F("hWJets","",nbins,xlow,xhigh);
  TH1F* hWJetsErr = new TH1F("hWJetsErr","",nbins,xlow,xhigh);

  hsig1TeV = (TH1F*)hrec[0]->Clone();
  hsig2TeV = (TH1F*)hrec[1]->Clone();
  hsig3TeV = (TH1F*)hrec[2]->Clone();
  hWJets   = (TH1F*)hrec[3]->Clone();
  hWJetsErr   = (TH1F*)hrec[3]->Clone();

  hsig1TeV->SetLineColor(kBlack);
  hsig2TeV->SetLineColor(kGreen);
  hsig3TeV->SetLineColor(kRed);
  hsig1TeV->SetLineWidth(3);
  hsig2TeV->SetLineWidth(3);
  hsig3TeV->SetLineWidth(3);
  hWJetsErr->SetFillStyle(3144);
  hWJetsErr->SetFillColor(13);
  hWJets->SetFillColor(46);

  TCanvas *c1 = new TCanvas("c1","",700,700);
  gStyle->SetOptStat(0);
  gPad->SetLogy();
  TH1F* href = new TH1F("href","",nbins,xlow,xhigh);
  href->GetYaxis()->SetRangeUser(0.001,10000);

  href->Draw("histo");  
  hWJets->Draw("histsame");
  hWJetsErr->Draw("e2same");
  hsig1TeV->Draw("histosame");
  hsig2TeV->Draw("histosame");
  hsig3TeV->Draw("histosame");
  
  auto legend = new TLegend(0.1,0.7,0.48,0.9);
  
  legend->AddEntry(hsig1TeV,"#bf{M}_{W'} = 1 TeV","l");
  legend->AddEntry(hsig2TeV,"#bf{M}_{W'} = 2 TeV","l");
  legend->AddEntry(hsig3TeV,"#bf{M}_{W'} = 3 TeV","l");
  legend->AddEntry(hWJets,"W#rightarrow l#nu","f");
  legend->Draw();

  vector<TH1F*> histos = {hsig1TeV,hsig2TeV,hsig3TeV,hWJets};
  HistBinContent(histos);
  
}


