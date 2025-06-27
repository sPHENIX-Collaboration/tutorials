#!/usr/bin/env python
# coding: utf-8

# In[31]:


from hepdata_lib import Submission, Table, Variable, Uncertainty, RootFileReader

# Load ROOT file using RootFileReader
reader = RootFileReader("spectrum_output.root")

# Initialize submission
submission = Submission()

# ------------------ Table 1: Spectrum -----------------------
#This objects hold page on HEPDATA - idealy 1 Table per figure
table1 = Table("Exponential Spectrum")
table1.description = "Dummy cross-section with statistical uncertainties and asymmetric systematic uncertainties."
table1.location = "Data from Figure 1, located on page 3."
# Add some keywords so it is searchable on HEPDATA
table1.keywords["observables"] = ["D2SIG/DPT/DETARAP"]
table1.keywords["cmenergies"] = ["200"]
table1.keywords["reactions"] = ["P P --> D0 X", "P P --> DBAR0 X"]
table1.keywords["phrases"] = ["Cross Section", "Single Differential Cross Section","Proton-Proton Scattering","Transverse Momentum Dependence"]
# Add image, can be pdf
table1.add_image("spectrum.png")

# Get data from TH1D and TGraphAsymmErrors
hist = reader.read_hist_1d("hist_stat")
graph = reader.read_graph("graph_asym")

# This is the structure of the loaded objects (dictionary of lists)
#print(graph.keys())
#dict_keys(['x', 'y', 'dx', 'dy'])

# Define x-axis (independent)
histogram_x = Variable("$p_{T}$", is_independent=True, is_binned=False, units="GeV")
histogram_x.values = hist["x"]
# Define y-axis values (since we have 1D plot, this is dependent - ir depends on the independent variable x)
histogram_y = Variable("Cross-section", is_independent=False, is_binned=False, units="mb")
histogram_y.values = hist["y"]
#Define urcertainties, you have have any number you want, you are naming them: norm,lumi, etc.
#stat unc
stat_unc = Uncertainty("stat", is_symmetric=True)
stat_unc.values = hist["dy"]
#sys unc
sys_unc = Uncertainty("sys", is_symmetric=False)
sys_unc.values = graph["dy"]

#accidentelly saved symetrical errors in Assymerrors? No problem!
#sys_unc = Uncertainty("sys", is_symmetric=True)
#sys_unc.values =[high for (_, high) in graph["dy"]]

# Add uncertainty to data
histogram_y.add_uncertainty(stat_unc)
histogram_y.add_uncertainty(sys_unc)

# Add variables to the table
table1.add_variable(histogram_x)
table1.add_variable(histogram_y)

#lets add another histogram in the same table, without any uncertainties
hist_halved = reader.read_hist_1d("hist_halved")
histogram_y_halved = Variable("Cross-section halved", is_independent=False, is_binned=False, units="mb")
histogram_y_halved.values = hist_halved["y"]
table1.add_variable(histogram_y_halved)

# ------------------ Table 2: Correlation Matrix ------------------
#lib also supports 2D hsitograms!
table2 = Table("Bin-to-bin Correlation Matrix")
table2.description = "Random symmetric bin-to-bin correlation matrix used for uncertainty modeling."
table2.add_image("corr_matrix.png")
corr_matrix = reader.read_hist_2d("corr_matrix")

# Create variable objects, since we have 2D histogram, x and y are independent variables
x = Variable("First Axis", is_independent=True, is_binned=True)
x.values = corr_matrix["x_edges"]
y = Variable("Second Axis", is_independent=True, is_binned=True)
y.values = corr_matrix["y_edges"]
correlation = Variable("Correlation coefficient", is_independent=False, is_binned=False)
correlation.values = corr_matrix["z"]
#Uncertainties can be added similarly to the 1D case

for var in [x,y,correlation]:
    table2.add_variable(var)

# Add tables to submission
submission.add_table(table1)
submission.add_table(table2)

# Output submission to directory
submission.create_files("submission_hepdata",remove_old=True)



# In[ ]:




