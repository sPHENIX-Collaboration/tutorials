* This short tutorial give a example macro on how to generate HEPData submission using hepdata_lib (https://github.com/HEPData/hepdata_lib/)
* It can be installed on SDCC or local using: python -m pip install hepdata_lib
* The most powerful feature is it allows to read ROOT objects directly - in this case you also need to install ROOT for your python, installing both root and hepdata_lib can be done as
  * conda config --set channel_priority strict
  * conda create -c conda-forge --name my_env_name root hepdata-lib
  * conda activate my_env_name

* Files:
  * GenerateDummyTree.ipynb: Create s TTree with TH1D with statistical errors, TGraphAsymmError holding the systematic uncertainties, and TH2D correlation matrix
  * spectrum_output.root: generated ROOT file
  * CreateSubmission.ipynb: Example code to create Submission for histogram with stat and syst errors while reading TH1D and TGraphAsymmError, and second histogram for the TH2D
  * CreateSubmission.py: just pure python of the above code
