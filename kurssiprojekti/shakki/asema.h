#pragma once
#include <vector>
#include "shakki.h"
#include "siirto.h"


class Asema
{
public:

	// Tyhjentää laudan.
	void tyhjenna();

	// Tekee annetun siirron laudalla. Voidaan olettaa, että
	// siirto on laillinen.
	void tee_siirto(const Siirto& s);

	// Tulostaa lauta ascii-grafiikkana.
	void tulosta() const;

	void anna_sotilaan_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;

	void anna_lahetin_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;

	void anna_ratsun_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;

	// Antaa tornin raakasiirrot (shakeista ei välitetä).
	void anna_tornin_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;

	void anna_kuningattaren_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;


	/// Anna nappulan mahdolliset raakasiirrot tiettyyn suntaa,
	///
	///		\param rivi		nappulan lähtörivi
	///		\param linja	nappulan lähtölinja
	///		\ param rivi_muutos rivien koodinaattimuutos (esim. -1 on "ylös")
	///		\ param linja_muutos linjojen koodinaattimuutos (esim. 1 on "vasen")
	///		\param pelaaja	nappulan väri (VALKEA, MUSTA)
	///		\param max_askeleet maksimiaskeleet ko. suuntaan?
	///		\param
	///
	void anna_raakasiirrot_suunassa(int rivi, int linja, int rivi_muutos, int linja_muutos, int pelaaja,
		int max_askeleet, bool voi_lyoda, bool pakko_lyoda,
		std::vector<Siirto>& siirrot) const;

private:

	// Laudan nappulat. Indeksointi [rivi][linja], esim.
	//
	// [0][0] : vasen ylänurkka ("a8")
	// [7][0] : vasen alanurkka ("a1")
	// [7][7] : oikea alanurkka ("h1")
	//
	int _lauta[8][8] = {
		{ bR, bN, bB, bQ, bK, bB, bN, bR },
		{ bP, bP, bP, bP, bP, bP, bP, bP },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ wP, wP, wP, wP, wP, wP, wP, wP },
		{ wR, wN, wB, wQ, wK, wB, wN, wR }
	};

	int _siirtovuoro = VALKEA;
};

