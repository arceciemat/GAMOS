
const double kPI = 3.14159265;

TCanvas* m_c1;

const int kAxis_X = 0;
const int kAxis_Y = 1;
const int kAxis_Z = 2;

int Main()
{
	m_c1 = new TCanvas("m_c1", "m_c1", 700, 700);
	m_c1->Divide(2, 2);

	// circle
	TH2D* h1 = new TH2D("h1", "circle", 100, -1.5, 1.5, 100, -1.5, 1.5);
	TH2D* h1a = new TH2D("h1a", "circle", 100, -1.5, 1.5, 100, -1.5, 1.5);
	TH2D* h1b = new TH2D("h1b", "circle", 100, -1.5, 1.5, 100, -1.5, 1.5);
	TH2D* h1c = new TH2D("h1c", "circle", 100, -1.5, 1.5, 100, -1.5, 1.5);
	TH2D* h1d = new TH2D("h1d", "circle", 100, -1.5, 1.5, 100, -1.5, 1.5);
		
	// Rotate around Y
	double** m_a;
	double angleMatY = 0.8;
	MakeMatrix( kAxis_Y, angleMatY, m_a);

	TH2D* h2 = new TH2D("h2", "rotated around Y", 100, -2.5, 2.5, 100, -2.5, 2.5);
	TH2D* h2a = new TH2D("h2a", "rotated around Y", 100, -2.5, 2.5, 100, -2.5, 2.5);
	TH2D* h2b = new TH2D("h2b", "rotated around Y", 100, -2.5, 2.5, 100, -2.5, 2.5);
	TH2D* h2c = new TH2D("h2c", "rotated around Y", 100, -2.5, 2.5, 100, -2.5, 2.5);
	TH2D* h2d = new TH2D("h2d", "rotated around Y", 100, -2.5, 2.5, 100, -2.5, 2.5);

cout << "row 0 m_a: " << m_a[0][0] << " " << m_a[1][0] << " " << m_a[2][0] << endl;

	// Rotate around Z
	double** m_a2;
	double angleMatZ = 4.2;
	MakeMatrix( kAxis_Z, angleMatZ, m_a2);

	TH2D* h3 = new TH2D("h3", "rotated around Z", 100, -2.5, 2.5, 100, -2.5, 2.5);
	TH2D* h3a = new TH2D("h3a", "rotated around Z", 100, -2.5, 2.5, 100, -2.5, 2.5);
	TH2D* h3b = new TH2D("h3b", "rotated around Z", 100, -2.5, 2.5, 100, -2.5, 2.5);
	TH2D* h3c = new TH2D("h3c", "rotated around Z", 100, -2.5, 2.5, 100, -2.5, 2.5);
	TH2D* h3d = new TH2D("h3d", "rotated around Z", 100, -2.5, 2.5, 100, -2.5, 2.5);

cout << "row 0 m_a2: " << m_a2[0][0] << " " << m_a2[1][0] << " " << m_a2[2][0] << endl;

	double sinTh= 1.0;
	double z = sinTh;
	for (int iangle = 0; iangle < 360; iangle++)
	{
		double phi = iangle * (2.* kPI) / 360;
		double x = cos(phi);
		double y = sin(phi);

		double weight = 1;

		h1->Fill(x, y, weight);

		double xp = m_a[0][0] * x + m_a[1][0] * y + m_a[2][0] * z;
		double yp = m_a[0][1] * x + m_a[1][1] * y + m_a[2][1] * z;
		double zp = m_a[0][2] * x + m_a[1][2] * y + m_a[2][2] * z;

		double len = std::sqrt(xp*xp + yp*yp + zp*zp);
		
		xp = xp/len;
		yp = yp/len;
		zp = zp/len;

		h2->Fill(xp, yp, weight);

		double xpp = m_a2[0][0] * xp + m_a2[1][0] * yp + m_a2[2][0] * zp;
		double ypp = m_a2[0][1] * xp + m_a2[1][1] * yp + m_a2[2][1] * zp;
		double zpp = m_a2[0][2] * xp + m_a2[1][2] * yp + m_a2[2][2] * zp;

		double len = std::sqrt(xpp*xpp + ypp*ypp + zpp*zpp);
		
		xpp = xpp/len;
		ypp = ypp/len;
		zpp = zpp/len;

		h3->Fill(xpp, ypp, weight);
		if (iangle == 90) 
		{
			h1a->Fill(x, y, 1000);
			h2a->Fill(xp, yp, 1000);
			h3a->Fill(xpp, ypp, 1000);
		}
		else if (iangle == 180) 
		{
			h1b->Fill(x, y, 1000);
			h2b->Fill(xp, yp, 1000);
			h3b->Fill(xpp, ypp, 1000);
		}
		else if (iangle == 270) 
		{
			h1c->Fill(x, y, 1000);
			h2c->Fill(xp, yp, 1000);
			h3c->Fill(xpp, ypp, 1000);
		}
		else if (iangle == 0) 
		{
			h1d->Fill(x, y, 1000);
			h2d->Fill(xp, yp, 1000);
			h3d->Fill(xpp, ypp, 1000);
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


	m_c1->Print("plot.pdf");
	m_c1->Print("plot.jpg");
}

void
MakeMatrix( int in_axis, const double& in_angle, double**& m_a)
{
	InitMatrixElements( m_a );

	double sinAngle = sin(in_angle);
	double cosAngle = cos(in_angle);

	if (in_axis == kAxis_X)
	{
		m_a[0][0] = 1.0;  m_a[1][0] = 0.0; 			m_a[2][0] = 0.0;
		m_a[0][1] = 0.0;  m_a[1][1] = cosAngle; 	m_a[2][1] = - sinAngle;
		m_a[0][2] = 0.0;  m_a[1][2] = sinAngle; 	m_a[2][2] = cosAngle;
	}
	else if (in_axis == kAxis_Y)
	{
		m_a[0][0] = cosAngle;  		m_a[1][0] = 0.0; 	m_a[2][0] = sinAngle;
		m_a[0][1] = 0.0;  			m_a[1][1] = 1; 		m_a[2][1] = 0.0;
		m_a[0][2] = - sinAngle;  	m_a[1][2] = 0.0; 	m_a[2][2] = cosAngle;
	}
	else if (in_axis == kAxis_Z)
	{
		m_a[0][0] = cosAngle;  	m_a[1][0] = - sinAngle; m_a[2][0] = 0.0;
		m_a[0][1] = sinAngle;  	m_a[1][1] = cosAngle; 	m_a[2][1] = 0.0;
		m_a[0][2] = 0.0;  		m_a[1][2] = 0.0; 		m_a[2][2] = 1.0;
	}
}

void 
InitMatrixElements( double**& m_a )
{
	m_a = new double* [3];
	for (int i=0; i < 3; i++)
		m_a[i] = new double [3];

	for (int i=0; i < 3; i++)
		for (int j=0; j < 3; j++)
			m_a[j][i] = 0.0;
}

