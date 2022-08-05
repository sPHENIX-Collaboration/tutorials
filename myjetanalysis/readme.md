New files:

~/vv/coresoftware/offline/packages/jetbackground/FastJetAlgoSubArea.h
~/vv/coresoftware/offline/packages/jetbackground/FastJetAlgoSubArea.cc
~/vv/coresoftware/simulation/g4simulation/g4jets/JetArea.h
~/vv/coresoftware/simulation/g4simulation/g4jets/JetArea.cc
~/vv/coresoftware/simulation/g4simulation/g4jets/JetReco.h

# 2022.08.05
* Missing information for the CentralityInfo node -- so just cut it out for now
* Needed:
    * modify the jet base class to have a field for Area
    * add a fastjetalgo to save the jet area, took




# Some important files:
~/vv/coresoftware/offline/packages/jetbackground/FastJetAlgoSub.cc
~/vv/coresoftware/offline/packages/jetbackground/FastJetAlgoSub.cc
~/vv/coresoftware/offline/packages/jetbackground/FastJetAlgoSub.h
~/vv/coresoftware/simulation/g4simulation/g4jets/FastJetAlgo.h
~/vv/coresoftware/simulation/g4simulation/g4jets/FastJetAlgo.cc
~/vv/coresoftware/simulation/g4simulation/g4jets/JetAlgo.h
~/vv/coresoftware/simulation/g4simulation/g4jets/Jetv1.h


# Things to do:

Output:
Trees with:
    1: rho_event
    2: Jets_truth:
        - vector<float> pt
        - vector<float> eta
        - vector<float> phi
        - vector<float> area
    3: Jets_meas:
        - vector<flaot> pt
        - vector<flaot> eta
        - vector<flaot> phi
        - vector<flaot> area
