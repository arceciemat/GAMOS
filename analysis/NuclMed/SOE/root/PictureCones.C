int Main()
{
	m_c1 = new TCanvas("m_c1", "m_c1", 700, 700);
	m_c1->Divide(2, 2);

	ifstream ifile("picturecones.dat");
	if (!ifile.is_open())
	{
		cout << "file not open" << endl;
		exit(1);
	}

	// circle
	TH2D* h1 = new TH2D("h1", "circle", 100, -1.5, 1.5, 100, -1.5, 1.5);
	TH2D* h1a = new TH2D("h1a", "circle", 100, -1.5, 1.5, 100, -1.5, 1.5);
	TH2D* h1b = new TH2D("h1b", "circle", 100, -1.5, 1.5, 100, -1.5, 1.5);
	TH2D* h1c = new TH2D("h1c", "circle", 100, -1.5, 1.5, 100, -1.5, 1.5);
	TH2D* h1d = new TH2D("h1d", "circle", 100, -1.5, 1.5, 100, -1.5, 1.5);
	// Rotate around Y
	TH2D* h2 = new TH2D("h2", "rotated around Y", 100, -2.5, 2.5, 100, -2.5, 2.5);
	TH2D* h2a = new TH2D("h2a", "rotated around Y", 100, -2.5, 2.5, 100, -2.5, 2.5);
	TH2D* h2b = new TH2D("h2b", "rotated around Y", 100, -2.5, 2.5, 100, -2.5, 2.5);
	TH2D* h2c = new TH2D("h2c", "rotated around Y", 100, -2.5, 2.5, 100, -2.5, 2.5);
	TH2D* h2d = new TH2D("h2d", "rotated around Y", 100, -2.5, 2.5, 100, -2.5, 2.5);
	// Rotate around Z
	TH2D* h3 = new TH2D("h3", "rotated around Z", 100, -2.5, 2.5, 100, -2.5, 2.5);
	TH2D* h3a = new TH2D("h3a", "rotated around Z", 100, -2.5, 2.5, 100, -2.5, 2.5);
	TH2D* h3b = new TH2D("h3b", "rotated around Z", 100, -2.5, 2.5, 100, -2.5, 2.5);
	TH2D* h3c = new TH2D("h3c", "rotated around Z", 100, -2.5, 2.5, 100, -2.5, 2.5);
	TH2D* h3d = new TH2D("h3d", "rotated around Z", 100, -2.5, 2.5, 100, -2.5, 2.5);

	int ih, iangle;
	double x, y;
	while (!ifile.eof())
	{
		ifile >> ih >> iangle >> x >> y;
		if (!ifile.eof())
		{
			if (ih == 0) 
			{
				h1->Fill(x, y, 1);
				if (iangle == 90) h1a->Fill(x, y, 1);
				else if (iangle == 180) h1b->Fill(x, y, 1);
				else if (iangle == 270) h1c->Fill(x, y, 1);
				else if (iangle == 0) h1d->Fill(x, y, 1);
			}
			else if (ih == 1) 
			{
				h2->Fill(x, y, 1);
				if (iangle == 90) h2a->Fill(x, y, 1);
				else if (iangle == 180) h2b->Fill(x, y, 1);
				else if (iangle == 270) h2c->Fill(x, y, 1);
				else if (iangle == 0) h2d->Fill(x, y, 1);
			}
			else if (ih == 2) 
			{
				h3->Fill(x, y, 1);
				if (iangle == 90) h3a->Fill(x, y, 1);
				else if (iangle == 180) h3b->Fill(x, y, 1);
				else if (iangle == 270) h3c->Fill(x, y, 1);
				else if (iangle == 0) h3d->Fill(x, y, 1);
			}
		
		}
	}

	m_c1->cd(1);
	h1->Draw("P");	

	h1a->SetMarkerColor(2);
	h1a->SetMarkerStyle(20);
	h1a->Draw("P SAME");	
	h1b->SetMarkerColor(3);
	h1b->SetMarkerStyle(20);
	h1b->Draw("P SAME");	
	h1c->SetMarkerColor(4);
	h1c->SetMarkerStyle(20);
	h1c->Draw("P SAME");	
	h1d->SetMarkerColor(5);
	h1d->SetMarkerStyle(20);
	h1d->Draw("P SAME");	

	m_c1->cd(2);
	h2->Draw("P");	

	h2a->SetMarkerColor(2);
	h2a->SetMarkerStyle(20);
	h2a->Draw("P SAME");	
	h2b->SetMarkerColor(3);
	h2b->SetMarkerStyle(20);
	h2b->Draw("P SAME");	
	h2c->SetMarkerColor(4);
	h2c->SetMarkerStyle(20);
	h2c->Draw("P SAME");	
	h2d->SetMarkerColor(5);
	h2d->SetMarkerStyle(20);
	h2d->Draw("P SAME");	

	m_c1->cd(3);
	h3->Draw("P");	

	h3a->SetMarkerColor(2);
	h3a->SetMarkerStyle(20);
	h3a->Draw("P SAME");	
	h3b->SetMarkerColor(3);
	h3b->SetMarkerStyle(20);
	h3b->Draw("P SAME");	
	h3c->SetMarkerColor(4);
	h3c->SetMarkerStyle(20);
	h3c->Draw("P SAME");	
	h3d->SetMarkerColor(5);
	h3d->SetMarkerStyle(20);
	h3d->Draw("P SAME");	

	m_c1->Print("picturecones.pdf");
	m_c1->Print("picturecones.jpg");
}

