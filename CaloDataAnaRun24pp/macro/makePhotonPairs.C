#include <sPhenixStyle.C>

void makePhotonPairs(const std::string &fin = "commissioning.root")
{
  SetsPhenixStyle();
  TFile *f = new TFile(fin.c_str());

  TTree *T = (TTree *) f->Get("T");

  std::vector<float> *clusterE{0};
  std::vector<float> *clusterPhi{0};
  std::vector<float> *clusterEta{0};
  std::vector<float> *clusterPt{0};
  std::vector<float> *clusterChi2{0};
  std::vector<float> *clusterNtow{0};
  std::vector<float> *clusterTowMaxE{0};
  std::vector<float> *clusterECore{0};

  T->SetBranchAddress("clusterE", &clusterE);
  T->SetBranchAddress("clusterPhi", &clusterPhi);
  T->SetBranchAddress("clusterEta", &clusterEta);
  T->SetBranchAddress("clusterPt", &clusterPt);
  T->SetBranchAddress("clusterChi2", &clusterChi2);
  T->SetBranchAddress("clusterNtow", &clusterNtow);
  T->SetBranchAddress("clusterTowMaxE", &clusterTowMaxE);
  T->SetBranchAddress("clusterECore", &clusterECore);

  TH1 *mass = new TH1F("invMass", "invMass", 100, 0, 1);
  mass->SetTitle(";M_{#gamma#gamma};");

  float maxChi2 = 4;
  float minClusE = 1;
  float maxAsym = 0.7;

  for (int i = 0; i < T->GetEntries(); i++)
  {
    T->GetEntry(i);

    for (int clus1 = 0; clus1 < clusterE->size(); clus1++)
    {
      if (clusterChi2->at(clus1) > maxChi2) continue;

      if (clusterE->at(clus1) < minClusE) continue;

      TLorentzVector photon1;
      photon1.SetPtEtaPhiE(clusterPt->at(clus1), clusterEta->at(clus1), clusterPhi->at(clus1), clusterE->at(clus1));

      for (int clus2 = 0; clus2 < clusterE->size(); clus2++)
      {
        if (clus2 <= clus1) continue;

        if (clusterE->at(clus2) < minClusE) continue;

        if (clusterChi2->at(clus2) > maxChi2) continue;

        float asym = sqrt(pow(clusterE->at(clus1) - clusterE->at(clus2), 2)) / (clusterE->at(clus1) + clusterE->at(clus2));
        if (asym > maxAsym) continue;

        TLorentzVector photon2;
        photon2.SetPtEtaPhiE(clusterPt->at(clus2), clusterEta->at(clus2), clusterPhi->at(clus2), clusterE->at(clus2));

        TLorentzVector meson = photon1 + photon2;

        mass->Fill(meson.M());
      }
    }
  }
  TCanvas *cMass = new TCanvas("cMass", "cMass");
  mass->Draw("ep");
}
