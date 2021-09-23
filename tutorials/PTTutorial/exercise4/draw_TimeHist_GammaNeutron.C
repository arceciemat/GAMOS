// Exercise 8. Compare histograms for gammas and neutrons

{
  TH1F *h = new TH1F("h", "h", 50, 0, 30);
  h->SetMinimum(0);
  h->SetMaximum(0.75e-3);
  h->SetTitle("Time when particles enter to detector. Single det. @ BP. 4x4x4cm3");
  h->SetStats(00000000);
  h->GetXaxis()->SetTitle("Time of arrival, ns");
  h->GetYaxis()->SetTitle("");
  h->GetYaxis()->SetTitleOffset(1.3);
  h->Draw();

  TFile f1("FinalTime_enterPET.root");
  TH1F *h1 = (TH1F*)f1.Get("GmStepDataHistosUA_enterDetectorFilter_GmClassifierByParticle:gamma:FinalTime");
  h1->SetLineColor(kBlack);
  h1->Draw("histsame");

  TH1F *h2 = (TH1F*)f1.Get("GmStepDataHistosUA_enterDetectorFilter_GmClassifierByParticle:neutron:FinalTime");
  h2->SetLineColor(kRed);
  h2->Draw("histsame");

  leg = new TLegend(0.7,0.7,0.9,0.9);
  leg->AddEntry(h1,"gamma","l");
  leg->AddEntry(h2,"neutron","l");
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->Draw("same");

}
