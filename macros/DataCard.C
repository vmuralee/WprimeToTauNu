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
      cout.precision(4);
      cout<<"Bin no. "<<ib<<"  BinContent "<<h->GetBinContent(ib)<<" Bin error "<<h->GetBinError(ib)<<endl;
    }
  }

}


void DataCard(TString varname = "mT",TString mass = "1", Int_t nbins=12,Float_t xlow=0,Float_t xhigh=2500){

  vector <TString> rootfiles = {   //List of files 
    "background_",
    "signal_"
  };
  TH1F* hgen[rootfiles.size()];
  TH1F* hrec[rootfiles.size()];
    
  int nfile =1;
  for(auto filename: rootfiles){
    TString path = "/Users/vinaykrishnan/Dropbox/WprimeToTauNu/data/DataCardFiles/classification/";
    TFile* file = new TFile(path+filename+mass+"TeV.root");
    TTree* tree = (TTree*)file->Get("datacard");
    
    TString histname_rec = filename+"_rec";

    hrec[nfile-1]  = new TH1F(histname_rec,"",nbins,xlow,xhigh);
    
    // TCut cut = "weight*(pTratio > 0.7 && pTratio < 1.3 && dtheta > 2.4)";
    TCut cut = "weight*(dtheta > 0)";
    TCanvas *dummy = new TCanvas();

    tree->Draw(varname+">>"+histname_rec,cut);
    dummy->Delete();
    ++nfile;
  }

  TH1F* hsig = new TH1F("hsig","",nbins,xlow,xhigh);
  
  TH1F* hbak = new TH1F("hbak","",nbins,xlow,xhigh);
  TH1F* hbakErr = new TH1F("hbakErr","",nbins,xlow,xhigh);

  hsig = (TH1F*)hrec[1]->Clone();
  hbak = (TH1F*)hrec[0]->Clone();

  hbakErr = (TH1F*)hrec[0]->Clone();

  hsig->SetLineColor(kRed);
  hsig->SetLineWidth(3);
  hbakErr->SetFillStyle(3144);
  hbakErr->SetFillColor(13);
  hbak->SetFillColor(46);

  TCanvas *c1 = new TCanvas("c1","",700,700);
  gStyle->SetOptStat(0);
  gPad->SetLogy();
  TH1F* href = new TH1F("href","",nbins,xlow,xhigh);
  href->GetYaxis()->SetRangeUser(0.1,1000000);
  href->GetXaxis()->SetTitle(varname);
  
  href->Draw("histo");  
  hbak->Draw("histsame");
  hbakErr->Draw("e2same");
  hsig->Draw("histosame");
  
  auto legend = new TLegend(0.636103,0.813333,0.899713,0.899259);
  
  legend->AddEntry(hsig,"#bf{M}_{W'} = "+mass+" TeV","l");
  legend->AddEntry(hbak,"W#rightarrow l#nu","f");
  legend->Draw();

  c1->SaveAs("/Users/vinaykrishnan/Dropbox/WprimeToTauNu/plots/DataCardPlots/"+varname+"_"+mass+"TeV.pdf");
  
  
  vector<TH1F*> histos = {hsig,hbak};
  HistBinContent(histos);
  
}

void makeDataCard(){
  
  // DataCard("mT","1000",11,0,2500);
  // DataCard("mT","2000",7,0,3500); 
  // DataCard("mT","3000",11,0,3000); 
  // DataCard("mT","4000",11,0,3000); 
  // DataCard("mT","5000",11,0,3000); 
  cout<<"=================================="<<endl;
  cout<<"=================================="<<endl;
  cout<<"=================================="<<endl;
  
  DataCard("score","1",3,0,1.1);
  /* DataCard("score","2",2,.1,1.1); */
  /* DataCard("score","3",2,0,1.1); */
  /* DataCard("scofe","4",2,0,1.1); */
  // DataCard("score","5",2,0,1.1); 
 
}
