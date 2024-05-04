singlePhaseTransportModel laminarTransport(U, phi);

dimensionedScalar Sct("Sct", dimless, laminarTransport);

dimensionedScalar kf("kf", dimensionSet (-1,-3,3,0,0,2,0), laminarTransport);


