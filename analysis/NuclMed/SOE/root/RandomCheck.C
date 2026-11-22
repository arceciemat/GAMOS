
TCanvas* m_c1;

void
Main()
{
	m_c1 = new TCanvas("m_c1", "m_c1", 600, 600);
	m_c1->Divide(1, 2);
	
	std::ifstream infile("random.dat");
	if (!infile.is_open())
	{
		cout << "random.dat is not open!" << endl;
		exit(1);
	}


	TH1F* h1 = new TH1F("h1", "check our random", 100, 0.0, 1.0);
	TH1F* h2 = new TH1F("h2", "check ROOT random", 100, 0.0, 1.0);

	TH1F* line1 = new TH1F("line1", "check our order", 100, -0.5, 99999.5 );
	TH1F* line2 = new TH1F("line2", "check ROOT order", 100, -0.5, 99999.5 );

	double x, x2;
	TRandom1 rand;
	int i = 0;
	while (!infile.eof())
	{
		infile >> x;
		if (!infile.eof())
		{
			h1->Fill(x);
			x2 = rand.Uniform( 1.0 );
			h2->Fill(x2);

			line1->Fill(i, x);
			line2->Fill(i, x2);

			i++;
		}
	}

	
	m_c1->cd(1);
	h1->SetMarkerSize(2);
	h1->SetMarkerStyle(7);
	h1->Draw("E");

	// ----

	m_c1->cd(2);
	h2->SetMarkerSize(2);
	h2->SetMarkerStyle(7);
	h2->Draw("E");

	m_c1->Print("randomcheck.pdf");
}
