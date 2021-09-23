{
  TH1F *h = new TH1F("h", "h", 50, -200, 200);
  h->SetMinimum(0);
  h->SetMaximum(0.5e-6);
  h->SetTitle("Bragg peak in PMMA phantom. 226.62 MeV");
  h->SetStats(00000000);
  h->GetXaxis()->SetTitle("Depth, mm");
  h->GetYaxis()->SetTitle("Dose, Gy");
  h->GetYaxis()->SetTitleOffset(1.);
  h->Draw();

  TFile f1("doseHistos.a.root");
  TH1F *h1 = (TH1F*)f1.Get("BraggPeak_2cm");
  h1->SetLineColor(kBlack);
  h1->SetLineWidth(1);
  h1->SetFillColor(1);
  h1->SetFillStyle(3013);
  h1->Draw("histsame");
  h1->Scale(10000);

  TFile f2("doseHistos.d.root");
  TH1F *h2 = (TH1F*)f2.Get("BraggPeak_2cm");
  h2->SetLineColor(kRed);
  h2->SetLineWidth(1);
  h2->Draw("histsame");
  h2->Scale(10000);

  TFile f3("doseHistos.e.root");
  TH1F *h3 = (TH1F*)f3.Get("BraggPeak_2cm");
  h3->SetLineColor(kGreen+2);
  h3->SetLineWidth(1);
  h3->Draw("histsame");
  h3->Scale(10000000);

  leg = new TLegend(0.15,0.65,0.6,0.85);
  leg->AddEntry(h1,"Nuclear+EM","f");
  leg->AddEntry(h2,"no nuclear reactions","l");
  leg->AddEntry(h3,"no EM reactions x1000","l");
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->Draw("same");

}
