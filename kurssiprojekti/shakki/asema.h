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


	// SIIRTOGENERAATTORI
	// Tuottaa aseman kaikki LAILLISET siirrot.
	void anna_siirrot(std::vector<Siirto>& siirrot) const
	{
		int kuningas = _siirtovuoro == VALKEA ? wK : bK;
		int pelaaja = _siirtovuoro;
		int vihu = vastustaja(pelaaja);

		// Luodaan pelaajan raakasiirrot. Osa siirroista saattaa
		// jättää kuninkaan uhatuksi.
		std::vector<Siirto> raakasiirrot;
		anna_kaikki_raakasiirrot(pelaaja, raakasiirrot);
		anna_linnoitukset(pelaaja, raakasiirrot);

		// Testataan jokainen raakasiirto.
		for (Siirto& rs : raakasiirrot)
		{
			// Luodaan kopio nykyisestä asemasta.
			Asema testiasema = *this;

			// Pelataan raakasiirto testiasemassa.
			testiasema.tee_siirto(rs);

			// Etsitään kuningas.
			int rivi, sarake;
			testiasema.etsi_nappula(kuningas, rivi, sarake);

			// Jäikö kuningas shakkiin?
			if (!testiasema.onko_ruutu_uhattu(rivi, sarake, vihu))
			{
				// Siirto on laillinen!
				siirrot.push_back(rs);
			}
		}
	}


	// Antaa aseman kaikki raakasiirrot
	void anna_kaikki_raakasiirrot(int pelaaja, std::vector<Siirto>& siirrot) const;


	// Linnoitukset ja ohestalyönnit
	void anna_linnoitukset(int pelaaja, std::vector<Siirto>& siirrot) const
	{
		if (pelaaja == VALKEA)
		{
			// kuningas ja torni eivät liikkuneet, f1 tyhjä, g1 tyhjä,
			// kuningas ei shakissa (e1 ei uhattu), f1 ei uhattu (kuningas ei
			// saa sääntöjen mukaan hypätä uhatun ruudun yli)
			if (_valkea_lyhyt_linna_sallittu && _lauta[7][5] == NA && _lauta[7][6] == NA &&
				!onko_ruutu_uhattu(7, 4, MUSTA) && !onko_ruutu_uhattu(7, 5, MUSTA) &&
				!onko_ruutu_uhattu(7, 6, MUSTA))
			{
				// Lisätään lyhyt linnoitus
				siirrot.push_back(Siirto(7, 4, 7, 6));
			}
			if (_valkea_pitka_linna_sallittu && _lauta[7][1] == NA &&
				_lauta[7][2] == NA && _lauta[7][3] == NA &&
				!onko_ruutu_uhattu(7, 4, MUSTA) && !onko_ruutu_uhattu(7, 3, MUSTA) &&
				!onko_ruutu_uhattu(7, 2, MUSTA))
			{
				// Lisätään pitkä linnoitus
				siirrot.push_back(Siirto(7, 4, 7, 2));
			}
		}
		else
		{
			// mustan linnoitukset
			if (_musta_lyhyt_linna_sallittu && _lauta[0][5] == NA && _lauta[0][6] == NA &&
				!onko_ruutu_uhattu(0, 4, VALKEA) && !onko_ruutu_uhattu(0, 5, VALKEA) &&
				!onko_ruutu_uhattu(0, 6, VALKEA))
			{
				// Lisätään lyhyt linnoitus
				siirrot.push_back(Siirto(0, 4, 0, 6));
			}
			if (_musta_pitka_linna_sallittu && _lauta[0][1] == NA &&
				_lauta[0][2] == NA && _lauta[0][3] == NA &&
				!onko_ruutu_uhattu(0, 4, VALKEA) && !onko_ruutu_uhattu(0, 3, VALKEA) &&
				!onko_ruutu_uhattu(0, 2, VALKEA))
			{
				// Lisätään pitkä linnoitus
				siirrot.push_back(Siirto(0, 4, 0, 2));
			}
		}
	}


	// etsii annetun nappulan (esim. wK) rivin ja linjan
	void etsi_nappula(int nappula, int& rivi, int& linja) const
	{
		for (int i = 0; i < 8; ++i)
			for (int j = 0; j < 8; ++j)
				if (_lauta[i][j] == nappula)
				{
					rivi = i;
					linja = j;
					return;
				}
	}


	bool onko_ruutu_uhattu(int rivi, int linja, int uhkaava_pelaaja) const
	{
		std::vector<Siirto> vihollis_siirrot;
		anna_kaikki_raakasiirrot(uhkaava_pelaaja, vihollis_siirrot);

		for (const Siirto& x : vihollis_siirrot) {
			int vihollisRivi = x._l_r;
			int VihollisLinja = x._l_l;

			if (vihollisRivi == rivi && VihollisLinja == linja) {
				return true;
			}
		}
		return false;
	}

	void anna_sotilaan_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;

	void anna_lahetin_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;

	void anna_ratsun_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;

	// Antaa tornin raakasiirrot (shakeista ei välitetä).
	void anna_tornin_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;

	void anna_daamin_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;

	void anna_kuninkaan_raakasiirrot(int rivi, int linja, int pelaaja,
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

	// Kirjanpito siitä, onko kuningas tai torni liikkunut.
	// Asetetaan tarpeen mukaan falseksi.
	bool _valkea_lyhyt_linna_sallittu = true;
	bool _valkea_pitka_linna_sallittu = true;
	bool _musta_lyhyt_linna_sallittu = true;
	bool _musta_pitka_linna_sallittu = true;

	// Ohestalyönti
	int _kaksoisaskel_linjalla = -1;
};

