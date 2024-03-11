#include "asema.h"

// Tyhjent‰‰ laudan.
void Asema::tyhjenna()
{
	for (int rivi = 0; rivi < 8; ++rivi)
		for (int linja = 0; linja < 8; ++linja)
			_lauta[rivi][linja] = NA;
}


// Tekee annetun siirron laudalla. Voidaan olettaa, ett‰
// siirto on laillinen.
void Asema::tee_siirto(const Siirto& s)
{
	// Otetaan alkuruudussa oleva nappula talteen.
	int nappula = _lauta[s._a_r][s._a_l];

	// Tyhjennet‰‰n alkuruutu.
	_lauta[s._a_r][s._a_l] = NA;

	// Sijoitetaan loppuruutuun alkuper‰inen nappula.
	_lauta[s._l_r][s._l_l] = nappula;
}