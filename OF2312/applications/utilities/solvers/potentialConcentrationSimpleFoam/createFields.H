// * * * * * * * * * * * * * Properties fields * * * * * * * * * * * * * * * //

Info<< "Reading speciesProperties\n" << endl;

IOdictionary speciesProperties
(
    IOobject
    (
        "speciesProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);

const wordList speciesNameList(speciesProperties.toc());

PtrList<species> speciesList(speciesNameList.size());

forAll(speciesNameList, i)
{
    const word& speciesName = speciesNameList[i];
    const dictionary& speciesDict = speciesProperties.subDict(speciesName);

    speciesList.set
    (
        i,
        new species
        (
            speciesDict,
            mesh
        )
    );

}

// * * * * * * * * * * * * * Reading fields * * * * * * * * * * * * * * * * //


Info<< "Reading field p\n" << endl;
volScalarField p
(
    IOobject
    (
        "p",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

//From here

Info<< "Reading field fi\n" << endl;

volScalarField fi
(
    IOobject
    (
	    "fi",
	    runTime.timeName(),
	    mesh,
	    IOobject::MUST_READ,
	    IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field Deff\n" << endl;

volScalarField Deff
(
	IOobject
	(
	    "Deff",
	    runTime.timeName(),
	    mesh,
	    IOobject::NO_READ,//MUST_READ,/
	    IOobject::AUTO_WRITE//NO_WRITE
	),
	mesh,
	dimensionedScalar("Deff",dimensionSet(0,2,-1,0,0,0,0),0)
); 

volScalarField Flux
(
	IOobject
	(
	    "Flux",
	    runTime.timeName(),
	    mesh,
	    IOobject::NO_READ,
	    IOobject::NO_WRITE//AUTO_WRITE//
	),
	mesh,
	dimensionedScalar("0", dimensionSet(0,-2,0,0,0,1,0), 0.0)
);

// * * * * * * * * * * * * * Reading Properties * * * * * * * * * * * * * * * * //

Info<< "Reading transportProperties\n" << endl;

IOdictionary transportProperties
(
	IOobject
	(
	    "transportProperties",
	    runTime.constant(),
	    mesh,
	    IOobject::MUST_READ_IF_MODIFIED,
	    IOobject::NO_WRITE
	)
);

#include "createPhi.H"
#include "readTransportProperties.H"

/*dimensionedScalar Sct
(
	transportProperties.lookup("Sct")
);*/

/*dimensionedScalar kf
(
    transportProperties.lookup("kf")
);*/


volScalarField keff
(
	IOobject
	(
	    "keff",
	    runTime.timeName(),
	    mesh,
	    IOobject::MUST_READ,
	    IOobject::AUTO_WRITE
	),
	mesh,
	dimensionedScalar("keff", kf)
);

Info<< "Reading controlProperties\n" << endl;

IOdictionary controlProperties
(
    IOobject
    (
        "controlProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);

IOdictionary controlDict
(
    IOobject
    (
        "controlDict",
        runTime.system(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::AUTO_WRITE
    )
);


IOdictionary Results
(
    IOobject
    (
        "Results",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::AUTO_WRITE
    )
);

//Until here

label pRefCell = 0;
scalar pRefValue = 0.0;
setRefCell(p, simple.dict(), pRefCell, pRefValue);
mesh.setFluxRequired(p.name());


//singlePhaseTransportModel laminarTransport(U, phi);

autoPtr<incompressible::turbulenceModel> turbulence
(
    incompressible::turbulenceModel::New(U, phi, laminarTransport)
);

#include "createMRF.H"
#include "createFvOptions.H"

