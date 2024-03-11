#include "asema.h"
#include "siirto.h"

int main()
{
	// Alkuasema.
	Asema asema;

	// e2e4
	Siirto siirto(6, 4, 4, 4);
	asema.tee_siirto(siirto);


	return 0;
}
