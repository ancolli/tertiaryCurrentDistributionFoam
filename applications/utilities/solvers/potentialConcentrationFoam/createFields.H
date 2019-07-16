Info<< "Reading field C_ox\n" << endl;

volScalarField C_ox
(
    IOobject
    (
        "C_ox",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field C_red\n" << endl;

volScalarField C_red
(
    IOobject
    (
        "C_red",
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

Info<< "Reading field nut\n" << endl;

    volScalarField nut
    (
        IOobject
        (
            "nut",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

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

volScalarField Flux
    (
        IOobject
        (
            "Flux",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE//AUTO_WRITE
        ),
        mesh,
	dimensionedScalar("0", dimensionSet(0,-2,0,0,0,1,0), 0.0)
    );


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

Info<< "Reading diffusivity Dmol\n" << endl;

dimensionedScalar Dmol_ox
(
    transportProperties.lookup("Dmol_ox")
);

dimensionedScalar Dmol_red
(
    transportProperties.lookup("Dmol_red")
);

Info<< "Reading turbulent Schmidt number\n" << endl;

dimensionedScalar Sct
(
    transportProperties.lookup("Sct")
);


Info<< "Reading electric conductivity kf\n" << endl;

dimensionedScalar kf
(
    transportProperties.lookup("kf")
);

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

    volScalarField D_ox
    (
        IOobject
        (
            "D_ox",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,//NO_READ,
            IOobject::AUTO_WRITE//NO_WRITE
        ),
	//mesh
        nut/Sct + Dmol_ox,
        nut.boundaryField().types()
    );

    volScalarField D_red
    (
        IOobject
        (
            "D_red",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,//NO_READ,
            IOobject::AUTO_WRITE//NO_WRITE
        ),
	//mesh
        nut/Sct + Dmol_red,
        nut.boundaryField().types()
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

#include "createPhi.H"

#include "createFvOptions.H"
