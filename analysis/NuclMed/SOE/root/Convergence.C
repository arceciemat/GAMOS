
TCanvas* m_c1;

void Main()
{
	gStyle->SetOptStat(0); 

	m_c1 = new TCanvas("m_c1", "m_c1", 700, 700);
	m_c1->Divide(1, 3);

	std::ifstream dfile("convergence.dat");
	if (!dfile.is_open())
	{
		cout << "File convergence.dat NOT open" << endl;
		exit(1);
	}

	int maxiter = 200; 
	cout << "give maxiter" << endl;
	cin >> maxiter;
	TH1D* h0 = new TH1D("h0", "vector diff [all evts]", maxiter, 0., maxiter);
	TH1D* h1 = new TH1D("h1", "vector diff [moving evts]", maxiter, 0., maxiter);
	TH1D* h2 = new TH1D("h2", "num of diff events", maxiter, 0., maxiter);

	int iter;
	double avedist;
	int nchevts;
	bool first = true;
	double totevts = 1.0;
	while ( !dfile.eof())
	{
		dfile >> iter >> avedist >> nchevts;
		if ( !dfile.eof())
		{
			if (first)
			{
				totevts = nchevts;	// more or less
				cout << "totevts: " << totevts << endl;
			}
			first = false;
			double avedist0 = avedist*nchevts/totevts;
			if ( iter % 100 == 0)
			{
				h0->Fill(iter, avedist0);
				h1->Fill(iter, avedist);
				h2->Fill(iter, nchevts);
			}
		}
	}


	m_c1->cd(1);
	gPad->SetLogy();
	h0->SetMarkerSize(2);
	h0->SetMarkerStyle(7);
	h0->Draw("P");

	m_c1->cd(2);
	gPad->SetLogy();
	h1->SetMarkerSize(2);
	h1->SetMarkerStyle(7);
	h1->Draw("P");

	m_c1->cd(3);
	gPad->SetLogy();
	h2->SetMarkerSize(2);
	h2->SetMarkerStyle(7);
	h2->Draw("P");
	
	m_c1->Print("convergence.gif");
}
