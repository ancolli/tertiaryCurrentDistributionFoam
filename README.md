# tertiaryCurrentDistributionFoam
# v1.0.0

Cite as: ...
https://doi.org/....

# tertiaryCurrentDistributionFoam
It is described how to simulate tertiary current distribution (without the effect of migration of ions) in electrochemical reactors with the help of OpenFOAM, the developed solver potentialConcentrationFoam and the BC as codedMixed. It shows how to pre-process, run and post-process a basic case involving in a 2D domain. 
The proposed strategy allows to have as an input i-th electrochemical reactions per electrode.

# Disclaimer
This offering is not approved or endorsed by OpeFOAM Foundation, producer and distributor of the OpenFOAM software via www.openfoam.org.

# Usage
In applications (A) you will find the scripts to compile the solver and post-processing utilities in order to solve tertiary current distribution.
In tutorial (B) you will find an example of a 2D parallel-plate cell. 

# #  A) Applications
**1.**  Solver  
_A)_ Paste applications/utilities/solvers/potentialConcentrationFoam inside OpenFOAM user directory (Applications/Utilities/Solvers).  
_B)_ Open a terminal inside potentialConcentrationFoam.  
_C)_ Run wmake.  
**2.**  Current density due to the Ox specie  
_A)_ Paste applications/utilities/postProcessing/wallFlux inside OpenFOAM user directory (Applications/Utilities/postProcessing).  
_B)_ Open a terminal inside wallFlux.  
_C)_ Run wmake.  
**3.**  Total current density (Ox specie + secondary reaction)  
_A)_ Paste applications/utilities/postProcessing/potentialWallFlux inside OpenFOAM user directory (Applications/Utilities/postProcessing).  
_B)_ Open a terminal inside potentialWallFlux.  
_C)_ Run wmake.  


# #  B) Tutorial
**1-** Paste tutorial inside OpenFOAM user directory (Run/Tutorials).  
**2-** Enter to tutorial and open a Terminal.  
**3-** Modify transport properties (conductivity, kinetic parameters, concentrations, equilibrium potentials, Sct, diffusion coefficients) inside constant/transportProperties.
**4-** Modify control properties (kind of electric control, under-over relaxation parameter, maxPot, tolerances) inside constant/controlProperties 
**5-** Run ./Allrun.  
