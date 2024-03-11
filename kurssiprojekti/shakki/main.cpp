#include "asema.h"
#include "siirto.h"

int main()
{
	std::vector<Siirto> lll;

	// Alkuasema.
	Asema asema;
	asema.tulosta();

	// e2e4
	Siirto siirto(6, 4, 4, 4);
	asema.tee_siirto(siirto);
	asema.tulosta();

	Siirto siirto2(6, 5, 4, 5);
	asema.tee_siirto(siirto2);
	asema.tulosta();

	

	asema.anna_lahetin_raakasiirrot(7, 5, 1, lll);
	//asema.tyhjenna();
	//asema.tulosta();

	return 0;
}
