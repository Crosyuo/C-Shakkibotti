#pragma once
#include <string>


// Kuvaa aseman muutosta.
class Siirto
{
public:
	Siirto(int a_r, int a_l, int l_r, int l_l) :
		_a_r(a_r), _a_l(a_l), _l_r(l_r), _l_l(l_l)
	{}

	// LÄKSY
	// Tee konstruktori, jonka avulla siirto-olio alustetaan
	// annetusta merkkijonosta, esim. "e2e4" tai "g8f6".
	Siirto(const std::string& s);

private:

	int _a_r; // siirron alkuruudun rivi
	int _a_l; // siirron alkuruudu linja (eli sarake)

	int _l_r; // siirron loppuruudun rivi
	int _l_l; // siirron loppuruudun linja



	friend class Asema;
};
