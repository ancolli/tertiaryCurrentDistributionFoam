/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2020 OpenFOAM Foundation
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

#include "species.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::species::species
(
    const dictionary& speciesDict,
    const fvMesh& mesh
)
:
    volScalarField
    (
        IOobject
        (
            "C_" + speciesDict.dictName(),
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    ),
    name_("C_" + speciesDict.dictName()),
    speciesDict_(speciesDict)
    
   
{
    Info<< "Reading field " << name_ <<"\n" << endl;
    this->read(speciesDict);
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::species::read(const dictionary& speciesDict)
{
    	speciesDict_ = speciesDict;
    
    	speciesDict_.lookup("diffusionModel") >> diffusionModel_;

    	if (diffusionModel_ == "constant")
    	{
        	speciesDict_.lookup("D") >> D_;
        	return true;
    	}
        else
        {
	        FatalErrorIn
        	(
            		": In changeDictionaryDict, diffusionModel should be constant"
            		"\n"
        	)   
            	<< exit(FatalError);	
                return false;
        }

}


// ************************************************************************* //
