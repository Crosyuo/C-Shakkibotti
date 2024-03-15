#include <iostream>
#include <string>
#include <vector>
#include "asema.h"
using namespace std;

void Asema::anna_kaikki_raakasiirrot(int pelaaja, std::vector<Siirto>& siirrot) const
{
	for (int rivi = 0; rivi < 8; ++rivi)
		for (int linja = 0; linja < 8; ++linja)
		{
			int nappula = _lauta[rivi][linja];

			// Tyhjä ruutu?
			if (nappula == NA)
				continue;

			// Vastustajan nappula?
			if (nappulan_vari(_lauta[rivi][linja]) != pelaaja)
				continue;

			// Haetaan nappulan raakasiirrot.
			switch (nappula)
			{
			case wR: case bR:
				anna_tornin_raakasiirrot(rivi, linja, pelaaja, siirrot);
				break;
			case wQ: case bQ:
				anna_daamin_raakasiirrot(rivi, linja, pelaaja, siirrot);
				break;
			case wN: case bN:
				anna_ratsun_raakasiirrot(rivi, linja, pelaaja, siirrot);
				break;
			case wB: case bB:
				anna_lahetin_raakasiirrot(rivi, linja, pelaaja, siirrot);
				break;
			case wK: case bK:
				anna_kuninkaan_raakasiirrot(rivi, linja, pelaaja, siirrot);
				break;
			case wP: case bP:
				anna_sotilaan_raakasiirrot(rivi, linja, pelaaja, siirrot);
				break;
			}
		}
}

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
	bool on_sotilas = (nappula == wP || nappula == bP);

	// Tyhjennetään alkuruutu.
	_lauta[s._a_r][s._a_l] = NA;

	// Kun ohestalyönti tapahtuu
	if (on_sotilas && s._a_l != s._l_l && _lauta[s._l_r][s._l_l] == NA) {
		_lauta[s._a_r][s._l_l] = NA;
	}

	if (s._korotettava_nappula == NA)
	{
		_lauta[s._l_r][s._l_l] = nappula;
	}
	else
	{
		_lauta[s._l_r][s._l_l] = s._korotettava_nappula;
	}

	// Tutkitaan, oliko siirto linnoitus. Jos oli, niin pitää
	// siirtää myös tornia. Huom! linnoitussiirron alku- ja loppukoordinaatit
	// ovat kuninkaan alku- ja loppukoordinaatit (esim. "e1g1").
	if (nappula == wK && s._a_r == 7 && s._a_l == 4 && s._l_r == 7 && s._l_l == 6)
	{
		// siirto oli valkean lyhyt linnoitus, joten
		// siirretään myös tornia
		_lauta[7][7] = NA;
		_lauta[7][5] = wR;
	}
	else if (nappula == wK && s._a_r == 7 && s._a_l == 4 && s._l_r == 7 && s._l_l == 2)
	{
		// siirto oli valkean pitkä linnoitus, joten
		// siirretään myös tornia
		_lauta[7][0] = NA;
		_lauta[7][3] = wR;
	}
	else if (nappula == bK && s._a_r == 0 && s._a_l == 4 && s._l_r == 0 && s._l_l == 6)
	{
		// siirto oli mustan lyhyt linnoitus, joten
		// siirretään myös tornia
		_lauta[0][7] = NA;
		_lauta[0][5] = bR;
	}
	else if (nappula == bK && s._a_r == 0 && s._a_l == 4 && s._l_r == 0 && s._l_l == 2)
	{
		// siirto oli mustan pitkä linnoitus, joten
		// siirretään myös tornia
		_lauta[0][0] = NA;
		_lauta[0][3] = bR;
	}

	// Päivitetään aseman linnoitus-flagit:
	// Jos nappula on kuningas, niin linnoitus ei enää sallittu
	if (nappula == wK)
	{
		_valkea_lyhyt_linna_sallittu = false;
		_valkea_pitka_linna_sallittu = false;
	}
	else if (nappula == bK)
	{
		_musta_lyhyt_linna_sallittu = false;
		_musta_pitka_linna_sallittu = false;
	}
	if ((s._a_r == 7 && s._a_l == 7) || (s._l_r == 7 && s._l_l == 7))
		// Siirron alku- tai loppuruutu oli h1.
		_valkea_lyhyt_linna_sallittu = false;
	else if ((s._a_r == 7 && s._a_l == 0) || (s._l_r == 7 && s._l_l == 0))
		// Siirron alku- tai loppuruutu oli h1.
		_valkea_pitka_linna_sallittu = false;
	else if ((s._a_r == 0 && s._a_l == 7) || (s._l_r == 0 && s._l_l == 7))
		// Siirron alku- tai loppuruutu oli h1.
		_musta_lyhyt_linna_sallittu = false;
	else if ((s._a_r == 0 && s._a_l == 0) || (s._l_r == 0 && s._l_l == 0))
		// Siirron alku- tai loppuruutu oli h1.
		_musta_pitka_linna_sallittu = false;

	// Tallennataan sotilaan kaksoisaskel tieto
	if (on_sotilas && abs(s._a_r - s._l_r) == 2)
	{
		_kaksoisaskel_linjalla = s._l_l;
	}
	else
	{
		_kaksoisaskel_linjalla = -1;
	}

	// vaihdetaan siirtovuoro
	_siirtovuoro = vastustaja(_siirtovuoro);
}

void Asema::anna_sotilaan_raakasiirrot(int rivi, int linja, int pelaaja,
	vector<Siirto>& siirrot) const
{
	vector<Siirto> sotilaan_liikkeet;

	if (pelaaja == 0) {
		anna_raakasiirrot_suunassa(rivi, linja, -1, 0, pelaaja, 1, false, false, sotilaan_liikkeet);

		anna_raakasiirrot_suunassa(rivi, linja, -1, -1, pelaaja, 1, true, true, sotilaan_liikkeet);
		anna_raakasiirrot_suunassa(rivi, linja, -1, 1, pelaaja, 1, true, true, sotilaan_liikkeet);
		if (rivi == 6) {
			anna_raakasiirrot_suunassa(rivi, linja, -2, 0, pelaaja, 1, false, false, sotilaan_liikkeet);
		}
		if (_kaksoisaskel_linjalla != -1 && _kaksoisaskel_linjalla == linja - 1 &&
			rivi == 3 && _lauta[rivi + 1][_kaksoisaskel_linjalla] == NA
			||
			_kaksoisaskel_linjalla != -1 && _kaksoisaskel_linjalla == linja + 1 &&
			rivi == 3 && _lauta[rivi + 1][_kaksoisaskel_linjalla] == NA) {
			siirrot.push_back(Siirto(rivi, linja, 2, _kaksoisaskel_linjalla));
		}
	}
	else {
		anna_raakasiirrot_suunassa(rivi, linja, 1, 0, pelaaja, 1, false, false, sotilaan_liikkeet);

		anna_raakasiirrot_suunassa(rivi, linja, 1, -1, pelaaja, 1, true, true, sotilaan_liikkeet);
		anna_raakasiirrot_suunassa(rivi, linja, 1, 1, pelaaja, 1, true, true, sotilaan_liikkeet);
		if (rivi == 1) {
			anna_raakasiirrot_suunassa(rivi, linja, 2, 0, pelaaja, 1, false, false, sotilaan_liikkeet);
		}
		if (_kaksoisaskel_linjalla != -1 && _kaksoisaskel_linjalla == linja - 1 &&
			rivi == 4 && _lauta[rivi - 1][_kaksoisaskel_linjalla] == NA
			||
			_kaksoisaskel_linjalla != -1 && _kaksoisaskel_linjalla == linja + 1 &&
			rivi == 4 && _lauta[rivi - 1][_kaksoisaskel_linjalla] == NA) {
			siirrot.push_back(Siirto(rivi, linja, 5, _kaksoisaskel_linjalla));
		}
	}

	for (Siirto& x : sotilaan_liikkeet)
	{
		if (x._l_r!=0 && x._l_r!=7)
		{
			siirrot.push_back(x);
		}
		else if(x._l_r == 0 || x._l_r == 7){
			if (x._l_r == 0) {
				siirrot.push_back(Siirto(x._a_r, x._a_l, x._l_r, x._l_l, wQ));
				siirrot.push_back(Siirto(x._a_r, x._a_l, x._l_r, x._l_l, wR));
				siirrot.push_back(Siirto(x._a_r, x._a_l, x._l_r, x._l_l, wB));
				siirrot.push_back(Siirto(x._a_r, x._a_l, x._l_r, x._l_l, wN));
			}
			else if (x._l_r == 7) {
				siirrot.push_back(Siirto(x._a_r, x._a_l, x._l_r, x._l_l, bQ));
				siirrot.push_back(Siirto(x._a_r, x._a_l, x._l_r, x._l_l, bR));
				siirrot.push_back(Siirto(x._a_r, x._a_l, x._l_r, x._l_l, bB));
				siirrot.push_back(Siirto(x._a_r, x._a_l, x._l_r, x._l_l, bN));
			}
		}
	}
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
	anna_raakasiirrot_suunassa(rivi, linja, -2, -1, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, -2, 1, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 2, 1, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 2, -1, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, -1, -2, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 1, -2, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, -1, 2, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 1, 2, pelaaja, 1, true, false, siirrot);
}

void Asema::anna_daamin_raakasiirrot(int rivi, int linja, int pelaaja,
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

void Asema::anna_kuninkaan_raakasiirrot(int rivi, int linja, int pelaaja,
	vector<Siirto>& siirrot) const
{
	anna_raakasiirrot_suunassa(rivi, linja, -1, -1, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, -1, 1, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 1, 1, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 1, -1, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, -1, 0, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 1, 0, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 0, -1, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunassa(rivi, linja, 0, 1, pelaaja, 1, true, false, siirrot);
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