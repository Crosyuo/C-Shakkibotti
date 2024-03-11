#include <iostream>
#include <string>
#include <vector>
#include "asema.h"
using namespace std;


void Asema::tyhjenna()
{
	for (int rivi = 0; rivi < 8; ++rivi)
		for (int linja = 0; linja < 8; ++linja)
			_lauta[rivi][linja] = NA;
}


void Asema::tee_siirto(const Siirto& s)
{
	// Otetaan alkuruudussa oleva nappula talteen.
	int nappula = _lauta[s._a_r][s._a_l];

	// Tyhjennetään alkuruutu.
	_lauta[s._a_r][s._a_l] = NA;

	// Sijoitetaan loppuruutuun alkuperäinen nappula.
	_lauta[s._l_r][s._l_l] = nappula;

	// vaihdetaan siirtovuoro
	_siirtovuoro = vastustaja(_siirtovuoro);
}

void Asema::anna_sotilaan_raakasiirrot(int rivi, int linja, int pelaaja,
	vector<Siirto>& siirrot) const
{
	
}

void Asema::anna_lahetin_raakasiirrot(int rivi, int linja, int pelaaja,
	vector<Siirto>& siirrot) const
{
	anna_raakasiirrot_suunassa(rivi, linja, -1, -1, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 1, 1, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, -1, 1, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 1, -1, pelaaja, 7, true, false, siirrot);
}

void Asema::anna_ratsun_raakasiirrot(int rivi, int linja, int pelaaja,
	vector<Siirto>& siirrot) const
{
	anna_raakasiirrot_suunassa(rivi, linja, -2, -1, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, -2, 1, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 2, 1, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 2, -1, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, -1, -2, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 1, -2, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, -1, 2, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 1, 2, pelaaja, 7, true, false, siirrot);
}

void Asema::anna_kuningattaren_raakasiirrot(int rivi, int linja, int pelaaja,
	vector<Siirto>& siirrot) const
{
	anna_raakasiirrot_suunassa(rivi, linja, -1, -1, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, -1, 1, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 1, 1, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 1, -1, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, -1, 0, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 1, 0, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 0, -1, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 0, 1, pelaaja, 7, true, false, siirrot);
}

void Asema::anna_tornin_raakasiirrot(int rivi, int linja, int pelaaja,
	vector<Siirto>& siirrot) const
{
	anna_raakasiirrot_suunassa(rivi, linja, -1, 0, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 1, 0, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 0, -1, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 0, 1, pelaaja, 7, true, false, siirrot);
}

void Asema::anna_raakasiirrot_suunassa(int rivi, int linja, int rivi_muutos, int linja_muutos, int pelaaja,
	int max_askeleet, bool voi_lyoda, bool pakko_lyoda,
	std::vector<Siirto>& siirrot) const
{
	int rivi_nyt = rivi;
	int linja_nyt = linja;
	int askeleet = 0;
	while (askeleet < max_askeleet)
	{
		rivi_nyt += rivi_muutos;
		linja_nyt += linja_muutos;

		// Ulkona laudalta?
		if (rivi_nyt < 0 || rivi_nyt > 7 || linja_nyt < 0 || linja_nyt > 7)
			break;

		// Tyhjä ruutu?
		if (_lauta[rivi_nyt][linja_nyt] == NA)
		{
			if (pakko_lyoda)
				break;
			siirrot.push_back(Siirto(rivi, linja, rivi_nyt, linja_nyt));
			askeleet++;
			continue;
		}

		// Törmätään omaan nappulaan?
		if (nappulan_vari(_lauta[rivi_nyt][linja_nyt]) == pelaaja)
			break;

		// Lyödään vastustajan nappula.
		if (voi_lyoda)
			siirrot.push_back(Siirto(rivi, linja, rivi_nyt, linja_nyt));
		//cout << siirrot;
		break;
	}
}


void Asema::tulosta() const
{
	const string nappulat[] = 
		{ "R", "N", "B", "Q", "K", "P", "r", "n", "b", "q", "k", "p", " " };

	for (int rivi = 0; rivi < 8; ++rivi)
	{
		string rivi_mj;
		for (int linja = 0; linja < 8; ++linja)
		{
			rivi_mj += (" | " + nappulat[_lauta[rivi][linja]]);
		}
		cout << "     +---+---+---+---+---+---+---+---+\n";
		cout << to_string(8 - rivi) + "   " + rivi_mj + " |\n";

	}
	cout << "     +---+---+---+---+---+---+---+---+\n";
	cout << "\n       A   B   C   D   E   F   G   H\n";
}