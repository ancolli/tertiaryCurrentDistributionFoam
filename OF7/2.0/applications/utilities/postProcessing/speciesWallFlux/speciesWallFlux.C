/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2017 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "speciesWallFlux.H"

#include "surfaceInterpolate.H"
#include "fvcSnGrad.H"
#include "wallPolyPatch.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(speciesWallFlux, 0);
    addToRunTimeSelectionTable(functionObject, speciesWallFlux, dictionary);
}
}

void Foam::functionObjects::speciesWallFlux::writeFileHeader(const label i)
{
    // Add headers to output data
    writeHeader(file(), "Species Wall flux");
    writeCommented(file(), "Time");
    writeTabbed(file(), "patch");
    writeTabbed(file(), "min");
    writeTabbed(file(), "max");
    writeTabbed(file(), "integral");
    file() << endl;
}

void Foam::functionObjects::speciesWallFlux::calcFlux //void
(
    //const volScalarField& D_red_,
    const volScalarField& C_, //const
    volScalarField& speciesWallFlux
)

{
    
    surfaceScalarField flux
    (
      -1*F_*D_*fvc::snGrad(C_) // -n*F*D*dC/dx
    );

    volScalarField::Boundary& speciesWallFluxBf =
        speciesWallFlux.boundaryFieldRef();

    const surfaceScalarField::Boundary& fluxBf =
        flux.boundaryField();

    forAll(speciesWallFluxBf, patchi)
    {
        speciesWallFluxBf[patchi] = fluxBf[patchi];
    }
}
// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


Foam::functionObjects::speciesWallFlux::speciesWallFlux
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    fvMeshFunctionObject(name, runTime, dict),

    logFiles(obr_, name),
    writeLocalObjects(obr_, log),
    patchSet_(),
    fvOptions_(mesh_),


    C_
    (
       IOobject
       (
           type(),//"C_" + CName_,
           mesh_.time().timeName(),
           mesh_,
           IOobject::NO_READ,//MUST_READ,
           IOobject::NO_WRITE
       ),
       mesh_,
       dimensionedScalar("0", C_.dimensions(), 0.0)
    )
       
{
    volScalarField* speciesWallFluxPtr
    (
	new volScalarField
        (
            IOobject
            (
                type(),
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar("0", C_.dimensions()/dimTime*dimLength, 0.0)
        )
    );


    
    mesh_.objectRegistry::store(speciesWallFluxPtr);
    
    read(dict);
    resetName(typeName);
    resetLocalObjectName(typeName);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::functionObjects::speciesWallFlux::~speciesWallFlux()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::speciesWallFlux::read(const dictionary& dict)
{


    fvMeshFunctionObject::read(dict);
    writeLocalObjects::read(dict);
     
    const polyBoundaryMesh& pbm = mesh_.boundaryMesh();

    patchSet_ =
        mesh_.boundaryMesh().patchSet
        (
            wordReList(dict.lookupOrDefault("patches", wordReList()))
        );

    Info<< type() << " " << name() << ":" << nl;

    if (patchSet_.empty())
    {
        forAll(pbm, patchi)
        {
            if (isA<wallPolyPatch>(pbm[patchi]))
            {
                patchSet_.insert(patchi);
            }
        }

        Info<< "    processing all wall patches" << nl << endl;
    }
    else
    {
        Info<< "    processing wall patches: " << nl;
        labelHashSet filteredPatchSet;
        forAllConstIter(labelHashSet, patchSet_, iter)
        {
            label patchi = iter.key();
            if (isA<wallPolyPatch>(pbm[patchi]))
            {
                filteredPatchSet.insert(patchi);
                Info<< "        " << pbm[patchi].name() << endl;
            }
            else
            {
                WarningInFunction
                    << "Requested wall flux on non-wall boundary "
                    << "type patch: " << pbm[patchi].name() << endl;
            }
        }

        Info<< endl;

        patchSet_ = filteredPatchSet;
    }


    if (dict.found("fvOptions"))
    {
        fvOptions_.reset(dict.subDict("fvOptions"));
    }

    
    dict.lookup("CName") >> CName_; //

    
    IOdictionary speciesProperties
    (
    	IOobject
    	(
        	"speciesProperties",
        	mesh_.time().constant(),//runTime.constant(),
        	mesh_,
        	IOobject::MUST_READ,//MUST_READ_IF_MODIFIED,
        	IOobject::NO_WRITE
    	)
    );


    dictionary speciesName
    (
    	speciesProperties.subDict(CName_)
    );

    speciesName.lookup("D") >> D_;

    IOdictionary transportProperties
    (
    	IOobject
    	(
        	"transportProperties",
		mesh_.time().constant(),//mesh_.time().timeName(),
        	mesh_,
        	IOobject::MUST_READ,
        	IOobject::NO_WRITE
   	)
   );

   transportProperties.lookup("F") >> F_;

   return true;
}


bool Foam::functionObjects::speciesWallFlux::execute()
{
    volScalarField& speciesWallFlux = lookupObjectRef<volScalarField>(type());

    volScalarField C_
    (
       IOobject
       (
           "C_" + CName_,
           mesh_.time().timeName(),
           mesh_,
           IOobject::MUST_READ,
           IOobject::NO_WRITE
       ),
       mesh_ 
    );

    calcFlux(C_, speciesWallFlux);

    return true;
}


bool Foam::functionObjects::speciesWallFlux::end()
{

    return true;
}


bool Foam::functionObjects::speciesWallFlux::write()
{
    Log << type() << " " << name() << " write:" << nl;

    writeLocalObjects::write();

    logFiles::write();

    const volScalarField& speciesWallFlux =
        obr_.lookupObject<volScalarField>(type());

    const fvPatchList& patches = mesh_.boundary();

    const surfaceScalarField::Boundary& magSf =
        mesh_.magSf().boundaryField();

    forAllConstIter(labelHashSet, patchSet_, iter)
    {
        label patchi = iter.key();
        const fvPatch& pp = patches[patchi];

        const scalarField& hfp = speciesWallFlux.boundaryField()[patchi];

        const scalar minHfp = gMin(hfp);
        const scalar maxHfp = gMax(hfp);
        const scalar integralHfp = gSum(magSf[patchi]*hfp)/gSum(magSf[patchi]);
	const scalar current = gSum((magSf[patchi]*hfp)); //absolute value

        if (Pstream::master())
        {
            file()
                << mesh_.time().value()
                << tab << pp.name()
                << tab << minHfp
                << tab << maxHfp
                << tab << integralHfp
		<< tab << current
                << endl;
        }

        Log << "    min/max/average/current(" << pp.name() << ") = "
            << minHfp << ", " << maxHfp << ", " << integralHfp << ", " << current << endl;
    }

    Log << endl;

    return true;
}


// ************************************************************************* //
