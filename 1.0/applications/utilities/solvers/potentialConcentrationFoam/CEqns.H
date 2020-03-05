{
            // concentration fields

	fvScalarMatrix CEqn_ox
            (
                fvm::ddt(C_ox)
              + fvm::div(phi, C_ox)
              - fvm::laplacian(D_ox, C_ox)
             ==
                fvOptions(C_ox)
            );

            CEqn_ox.relax();
            fvOptions.constrain(CEqn_ox);
            CEqn_ox.solve();
            fvOptions.correct(C_ox);

Info<< "Min/max C_ox:" << min(C_ox).value() << ' '
    << max(C_ox).value() << endl;

	fvScalarMatrix CEqn_red
            (
                fvm::ddt(C_red)
              + fvm::div(phi, C_red)
              - fvm::laplacian(D_red, C_red)
             ==
                fvOptions(C_red)
            );

            CEqn_red.relax();
            fvOptions.constrain(CEqn_red);
            CEqn_red.solve();
            fvOptions.correct(C_red);

Info<< "Min/max C_red:" << min(C_red).value() << ' '
    << max(C_red).value() << endl;

}




// ************************************************************************* //
