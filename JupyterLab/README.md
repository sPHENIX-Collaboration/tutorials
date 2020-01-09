This is an example [sPHENIX](https://github.com/sPHENIX-Collaboration/) [ROOT Jupyter Notebook](https://root.cern.ch/notebooks/HowTos/HowTo_ROOT-Notebooks.html) to run the sPHENIX anaysis on the [BNL SDCC Jupyter Lab](https://jupyter.sdcc.bnl.gov/). 

<p  align="center">
<a href = "https://jupyter.sdcc.bnl.gov/jupyterhub/htc"><img  src="https://jupyter.sdcc.bnl.gov/images/jupyterhub-logo-card-htc.png" alt="SDCC HTC Jupyter Hub"
	title="SDCC HTC Jupyter Hub" height="200" />
</a></p>

Starting from https://jupyter.sdcc.bnl.gov/jupyterhub/htc ( = `lauch` button on the **HTC** Jupyter hub) for this tutorial, the username / password is the same for your BNL RCF / SDCC account. At the 1st time login, SDCC will ask you to setup a two factor anthentication, such as using Google Authenticator ([Android](https://play.google.com/store/apps/details?id=com.google.android.apps.authenticator2&hl=en_US)/[iPhone](https://apps.apple.com/us/app/google-authenticator/id388497605)). 

Jupyter Notebook provide an analysis interface that can integrate documentation, code and output plot in a single file. It supports all sPHENIX environment and most sPHENIX macros. It is suitable for final analysis step, while large scale analysis and reconstruction should go through the compiled `c++` analysis module.  

Two examples are provided in this tutorial, one for each supported sPHENIX Jupyter kernels:

* `sPHENIX ROOT C++` kernel: An analysis in ROOT `C++` macros: [example_tpc_testbeam2019_root.ipynb](./example_tpc_testbeam2019_root.ipynb)
* `Python (sPHENIX)` kernel: `pyROOT` macro perform similar task if you would like to use `python` for analysis:  [example_tpc_testbeam2019_python.ipynb](./example_tpc_testbeam2019_python.ipynb)

When you make a new notebook, please select one of these two kernels, which carry the most recent sPHENIX software environment on SDCC/RACF. Notebooks can be previewed on GitHub, and edited/run on [BNL SDCC Jupyter Lab](https://jupyter.sdcc.bnl.gov/). 

The JupyterLab interface can also be launched on your computer/local computing cluster via [sPHENIX Singularity Container](https://github.com/sPHENIX-Collaboration/Singularity). Instruction to come, and please contact Jin Huang if you would like to try early releases. 
