{
//potential field
    fi.storePrevIter();
    solve
    (
       - fvm::laplacian(keff, fi)
    );

    fi.relax();

Info<< "Min/max fi:" << min(fi).value() << ' '
    << max(fi).value() << endl;

//check if wall_anode exist
string patchNameNeeded = "wall_anode";
scalar patchOK = 0;

    forAll(mesh.boundaryMesh(), p_ith)
    {     
      string patchName = mesh.boundaryMesh()[p_ith].name();	
      if (patchNameNeeded == patchName)  // checks if "wall_anode" is in the current patchName
      {
        patchOK = 1;
      }
    }  

if (patchOK==1)  // checks if "wall_anode" is in the current patchName
      {
// if wall_anode exists then: Calculates flux on the anode

        label patchi = mesh.boundaryMesh().findPatchID("wall_anode");   
        Flux.boundaryFieldRef()[patchi] = keff.boundaryFieldRef()[patchi]*fi.boundaryFieldRef()[patchi].snGrad();
			    
// Calculates average Flux
	//scalar area = gSum(mesh.magSf().boundaryField()[patchi]);
        scalar sumField = 0.0;

        //if (area > 0)
        {
        	sumField = gSum
                (
                	mesh.magSf().boundaryField()[patchi]
                      * Flux.boundaryField()[patchi]
                );// /area;
        }

        Info<< "Total current = " << sumField << " A "<< endl;

// reading and writing probeLocation (reference electrode)

        label cellNo = mesh.findCell(vector(controlProperties.lookup("refLocation")));
	scalar Vref = fi[cellNo];

// calculate cell potential
        scalar mult = 0.0;
        scalar Vcell_old = 0.0;
        scalar var = 0.0;
        scalar Vcell = readScalar(Results.lookup("Vcell")); 

        scalar control = readScalar(controlProperties.lookup("control"));  
        scalar I = readScalar(controlProperties.lookup("I"));
        scalar maxPot = readScalar(controlProperties.lookup("maxPot"));
        scalar alpha = readScalar(controlProperties.lookup("alpha"));
        scalar tol_I = readScalar(controlProperties.lookup("tol_I"));

        scalar ba = readScalar(transportProperties.lookup("ba"));

// potentiostatic mode
        scalar ref = readScalar(controlProperties.lookup("ref"));
        scalar maxvar = readScalar(controlProperties.lookup("maxvar"));
        scalar tol_ref = readScalar(controlProperties.lookup("tol_ref"));

        var=(ref < 0.0) ? -ref-Vref : ref-(Vcell-Vref); // <0 cathodic overpotential, >0 anodic overpotential//var = ref-Vref;
        var=(var>0) ? min(var,maxvar) : max(var,-maxvar);
        var=(mag(var/ref) > tol_ref) ? var : 0; // limito ref a x%

// galvanostatic
    
        mult = (mag(sumField-I)/I > tol_I) ? (sumField-I)/(I/ba) : 0.0; // limito corriente a x%

// overrelaxation 
        Vcell_old = Vcell;
	Vcell = (control==1) ? Vcell+var : ((control==2) ? maxPot : Vcell-mult);//Vcell = Vcell-mult;//
        
        Vcell = Vcell_old+alpha*(Vcell-Vcell_old);

	Vcell = max(min(Vcell, maxPot),mag(ref)); // limited potentials

// writing Vcell, I and Vref         
        Results.set("Vcell", Vcell);
        Results.set("I", sumField); 
	Results.set("Vref", (ref < 0.0) ? -Vref : (Vcell-Vref));//  
   }
   else
   {
	FatalErrorIn
        (
            " Foam::patchProbes::findElements(const fvMesh&)"
        )   << " No patch name: wall_anode found "
            << " the patch name: wall_anode should exist "
            << endl
            << exit(FatalError);     
   }
}


// ************************************************************************* //
