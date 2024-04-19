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
		  fvOptions(speciesList[i])
	    );

	    fvOptions.constrain(MassTransport);
	    MassTransport.solve(speciesSolveDict);
	    fvOptions.correct(speciesList[i]);

	    Info<< "Min/max C_" << speciesNameList[i] << ":" << min(speciesList[i]).value() << ' '
            << max(speciesList[i]).value() << endl;
	}
}




// ************************************************************************* //
