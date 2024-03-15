#pragma once
#include <string>
#include "shakki.h"


// Kuvaa aseman muutosta.
class Siirto
{
public:
	Siirto(int a_r, int a_l, int l_r, int l_l, int korotettava_nappula = NA) :
		_a_r(a_r), _a_l(a_l), _l_r(l_r), _l_l(l_l), _korotettava_nappula(korotettava_nappula)
	{}

	Siirto(const std::string& s);

	void tulosta() const;



	int _a_r; // siirron alkuruudun rivi
	int _a_l; // siirron alkuruudu linja (eli sarake)

	int _l_r; // siirron loppuruudun rivi
	int _l_l; // siirron loppuruudun linja

	int _korotettava_nappula = NA; // esim. wQ, wR, bN jne.


	friend class Asema;
};
