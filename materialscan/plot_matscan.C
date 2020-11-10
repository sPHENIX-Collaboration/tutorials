void plot_matscan(const std::string &datfile, const int plot_theta = 0)
{
  FILE *f = fopen(datfile.c_str(),"r");
  if (!f)
    {
      cout << "file " << datfile << " cannot be opened" << endl;
      return;
    }
  float theta;
  float phi;
  float path;
  float x0;
  float lamda0;
  vector<float> thetavec;
  vector<float> phivec;
  vector<float> x0vec;
  vector<float> lamda0vec;
  float thetamin = 10000;
  float thetamax = -10000;
  float phimin = 10000;
  float phimax = -10000;
  float x0min = 100000;
  float x0max = -100000;
  float lamda0min = 100000;
  float lamda0max = -100000;
  while(fscanf(f,"%f %f %f %f %f",&theta,&phi,&path,&x0,&lamda0) != EOF)
    {
      cout << "theta: " << theta
	   << ", phi: " << phi
	   << ", path: " << path
	   << ", x0: " << x0
	   << ", lamda0: " << lamda0
	   << endl;
      if (plot_theta == 2)
      {
      theta = (90+theta)/180.*M_PI;
      theta = log(TMath::Tan(theta/2.));
      }
      if (theta > thetamax)
	{
	  thetamax = theta;
	}
      if (theta < thetamin)
	{
	  thetamin = theta;
	}
      if (phi > phimax)
	{
	  phimax = phi;
	}
      if (phi < phimin)
	{
	  phimin = phi;
	}
     if (x0 > x0max)
	{
	  x0max = x0;
	}
      if (x0 < x0min)
	{
	  x0min = x0;
	}
     if (lamda0 > lamda0max)
	{
	  lamda0max = lamda0;
	}
      if (lamda0 < lamda0min)
	{
	  lamda0min = lamda0;
	}
      thetavec.push_back(theta);
      phivec.push_back(phi);
      x0vec.push_back(x0);
      lamda0vec.push_back(lamda0);
    }
  float *thetaarr = new float[thetavec.size()];
  float *phiarr = new float[phivec.size()];
  float *x0arr = new float[x0vec.size()];
  float *lamda0arr = new float[lamda0vec.size()];
  for (int i=0; i<phivec.size(); i++)
    {
      thetaarr[i] = thetavec[i];
      phiarr[i] = phivec[i];
      x0arr[i] = x0vec[i];
      lamda0arr[i] = lamda0vec[i];
    }
  TCanvas *c = new TCanvas("c1","material scan");
  c->Divide(1,2);
  c->cd(1);
  char htitlex[100];
  char htitlelamda[100];
  double xmin;
  double xmax;
  TGraph *gr = nullptr;
  TGraph *grl = nullptr;

  if (plot_theta)
  {
    if (plot_theta == 1)
    {
  sprintf(htitlex,"x0 vs theta");
  sprintf(htitlelamda,"lamda0 vs theta");
  xmin = thetamin - 1;
  xmax = thetamax + 1;
    }
    else
    {
  sprintf(htitlex,"x0 vs eta");
  sprintf(htitlelamda,"lamda0 vs eta");
  xmin = thetamin - 0.1;
  xmax = thetamax + 0.1;
    }

  gr = new TGraph(thetavec.size(),thetaarr,x0arr);
  grl = new TGraph(thetavec.size(),thetaarr,lamda0arr);
  }
  else
  {
  sprintf(htitlex,"x0 vs phi");
  sprintf(htitlelamda,"lamda0 vs phi");
  xmin = phimin - 1;
  xmax = phimax + 1;
  gr = new TGraph(phivec.size(),phiarr,x0arr);
  grl = new TGraph(phivec.size(),phiarr,lamda0arr);
  }
  TH2 *h2 = new TH2F("hx2",htitlex,2,xmin,xmax,2,0,x0max+0.1*x0max);
  h2->SetStats(kFALSE);
  h2->Draw();
  gr->SetMarkerStyle(21);
  gr->SetMarkerSize(0.1);
  gr->Draw("LP");
  c->cd(2);  
  TH2 *hl2 = new TH2F("hlamda2",htitlelamda,2,xmin,xmax,2,0,lamda0max+lamda0max/10.);
  hl2->SetStats(kFALSE);
  hl2->Draw();
  grl->SetMarkerStyle(21);
  grl->SetMarkerSize(0.1);
  grl->Draw("LP");
  fclose(f);
  return;
}

void plot_matscan_tdr(const char *datfile, const int plot_theta = 0)
{
  FILE *f = fopen(datfile,"r");
  if (!f)
    {
      cout << "file " << datfile << " cannot be opened" << endl;
      return;
    }
  float theta;
  float phi;
  float path;
  float x0;
  float lamda0;
  vector<float> thetavec;
  vector<float> phivec;
  vector<float> x0vec;
  vector<float> lamda0vec;
  float thetamin = -1.10;
  float thetamax = 1.1;
  float phimin = 10000;
  float phimax = -10000;
  float x0min = 100000;
  float x0max = -100000;
  float lamda0min = 100000;
  float lamda0max = -100000;
  while(fscanf(f,"%f %f %f %f %f",&theta,&phi,&path,&x0,&lamda0) != EOF)
    {
      theta = (theta+90)/180.*M_PI;
      theta =  log(TMath::Tan(theta/2.));
      cout << "theta: " << theta
	   << ", phi: " << phi
	   << ", path: " << path
	   << ", x0: " << x0
	   << ", lamda0: " << lamda0
	   << endl;
      if (phi > phimax)
	{
	  phimax = phi;
	}
      if (phi < phimin)
	{
	  phimin = phi;
	}
     if (x0 > x0max)
	{
	  x0max = x0;
	}
      if (x0 < x0min)
	{
	  x0min = x0;
	}
     if (lamda0 > lamda0max)
	{
	  lamda0max = lamda0;
	}
      if (lamda0 < lamda0min)
	{
	  lamda0min = lamda0;
	}
      thetavec.push_back(theta);
      phivec.push_back(phi);
      x0vec.push_back(x0);
      lamda0vec.push_back(lamda0);
    }
  float *thetaarr = new float[thetavec.size()];
  float *phiarr = new float[phivec.size()];
  float *x0arr = new float[x0vec.size()];
  float *lamda0arr = new float[lamda0vec.size()];
  for (int i=0; i<phivec.size(); i++)
    {
      thetaarr[i] = thetavec[i];
      phiarr[i] = phivec[i];
      x0arr[i] = x0vec[i];
      lamda0arr[i] = lamda0vec[i];
    }
  TCanvas *c = new TCanvas("c1","material scan");
  char htitlex[100];
  char htitlelamda[100];
  char titlexaxis[100];
  double xmin;
  double xmax;
  TGraph *gr = nullptr;
  TGraph *grl = nullptr;

  if (plot_theta)
  {
  sprintf(htitlex,"x0 vs theta");
  sprintf(htitlelamda,"");
  sprintf(titlexaxis,"#eta");
  xmin = thetamin;
  xmax = thetamax;
  gr = new TGraph(thetavec.size(),thetaarr,x0arr);
  grl = new TGraph(thetavec.size(),thetaarr,lamda0arr);
  }
  else
  {
  sprintf(htitlex,"x0 vs phi");
  sprintf(htitlelamda,"");
  sprintf(titlexaxis,"#phi");
  xmin = phimin - 1;
  xmax = phimax + 1;
  gr = new TGraph(phivec.size(),phiarr,x0arr);
  grl = new TGraph(phivec.size(),phiarr,lamda0arr);
  }
  TH2 *hl2 = new TH2F("hlamda2",htitlelamda,2,xmin,xmax,2,0,lamda0max+lamda0max/10.);
  hl2->SetStats(kFALSE);
  hl2->GetXaxis()->SetTitle(titlexaxis);
  hl2->GetXaxis()->SetTitleSize(0.05);
  hl2->GetXaxis()->SetLabelSize(0.04);
  hl2->GetXaxis()->SetTitleOffset(0.8);
  hl2->GetYaxis()->SetTitle("#lambda_{0}");
  hl2->GetYaxis()->SetTitleSize(0.05);
  hl2->GetYaxis()->SetTitleOffset(0.6);
  hl2->GetYaxis()->SetLabelSize(0.04);
  hl2->Draw();
  grl->SetMarkerStyle(21);
  grl->SetMarkerSize(0.1);
  grl->SetLineWidth(2);
  grl->Draw("LP");
  fclose(f);
  return;
}
