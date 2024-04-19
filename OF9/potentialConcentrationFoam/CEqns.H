{
            // concentration fields
	dictionary speciesSolveDict = mesh.solutionDict().subDict("solvers").subDict("C_species");

	//turbulent diffusivity	
	volScalarField Dturb
	(
		"Dturb",
		nut/Sct
	);

        forAll(speciesNameList, i)
        {

            fvScalarMatrix MassTransport
            (
                  fvm::ddt(speciesList[i])
                + fvm::div(phi, speciesList[i], "div(phi,C_species)")
                - fvm::laplacian(speciesList[i].D()+Dturb, speciesList[i], "laplacian(D,C_species)") //  speciesList[i].D()+Dturb      
		==
		  fvModels.source(speciesList[i])
	    );
	    
	    MassTransport.relax();
            fvConstraints.constrain(MassTransport);
            MassTransport.solve(speciesSolveDict);
            fvConstraints.constrain(speciesList[i]);

	    Info<< "Min/max C_" << speciesNameList[i] << ":" << min(speciesList[i]).value() << ' '
            << max(speciesList[i]).value() << endl;
	}
}




// ************************************************************************* //
