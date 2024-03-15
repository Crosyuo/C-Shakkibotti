#pragma once
#include <limits>
#include <map>
#include <vector>
#include "shakki.h"
#include "siirto.h"


// Minimax-funktion palauttama arvo. Sis‰lt‰‰ sek‰
// minimax-arvon ett‰ sit‰ vastaavan parhaan siirron.
class MinimaxArvo
{
public:
	MinimaxArvo(float arvo, Siirto siirto) :
		_arvo(arvo), _siirto(siirto)
	{}

	float	_arvo;
	Siirto	_siirto;
};

class Asema
{
public:

	// Tyhjent‰‰ laudan.
	void tyhjenna();

	// Tekee annetun siirron laudalla. Voidaan olettaa, ett‰
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
		// j‰tt‰‰ kuninkaan uhatuksi.
		std::vector<Siirto> raakasiirrot;
		anna_kaikki_raakasiirrot(pelaaja, raakasiirrot);
		anna_linnoitukset(pelaaja, raakasiirrot);

		// Testataan jokainen raakasiirto.
		for (Siirto& rs : raakasiirrot)
		{
			// Luodaan kopio nykyisest‰ asemasta.
			Asema testiasema = *this;

			// Pelataan raakasiirto testiasemassa.
			testiasema.tee_siirto(rs);

			// Etsit‰‰n kuningas.
			int rivi, sarake;
			testiasema.etsi_nappula(kuningas, rivi, sarake);

			// J‰ikˆ kuningas shakkiin?
			if (!testiasema.onko_ruutu_uhattu(rivi, sarake, vihu))
			{
				// Siirto on laillinen!
				siirrot.push_back(rs);
			}
		}
	}


	// Pisteytt‰‰ pelin lopputuloksen seuraavasti:
	//
	// Valkea tehnyt matin			1000000
	// Tasapeli (patti)				0
	// Musta tehnyt matin		   -1000000
	//
	// Funktiota kutsutaan, kun asemassa ei ole en‰‰ yht‰‰n laillista
	// siirtoa (anna_siirrot on palattanyt tyhj‰n siirtovektorin).
	//
	float pisteyta_lopputulos(int syvyys) const
	{
		if (_siirtovuoro == VALKEA)
		{
			// Etsit‰‰n valkean kuningas
			int rivi, linja;
			etsi_nappula(wK, rivi, linja);

			// Onko valkean kuningas uhattu?
			if (onko_ruutu_uhattu(rivi, linja, MUSTA))
				return -1000000 + syvyys; // musta on tehnyt matin
			else
				return 0 + syvyys; // patti
		}
		else
		{
			// Etsit‰‰n mustan kuningas
			int rivi, linja;
			etsi_nappula(bK, rivi, linja);

			// Onko mustan kuningas uhattu?
			if (onko_ruutu_uhattu(rivi, linja, VALKEA))
				return 1000000 - syvyys; // valkea on tehnyt matin
			else
				return 0 - syvyys; // patti
		}
	}


	// Pisteytt‰‰ shakkiaseman heuristisesti.
	float evaluoi() const
	{
		return 1.0 * materiaali() + 0.05 * mobiliteetti();
	}




	//
	// Palauttaa aseman minimax-arvon. Syvyys m‰‰ritt‰‰,
	// kuinka monta asekelta syvemm‰lle pelipuuta k‰yd‰‰n l‰pi.
	//
	// Testaaminen esim. p‰‰ohjelmasta:
	//
	// Asema asema;
	// MinimaxArvo arvo = asema.minimax(4);
	// 
	// Nyt tietokoneen siirto saadaan pelattua n‰in:
	// asema.tee_siirto(arvo._siirto);
	//
	MinimaxArvo minimax(int syvyys)
	{
		// Generoidaan aseman siirrot.
		std::vector<Siirto> siirrot;
		anna_siirrot(siirrot);

		if (siirrot.size() == 0)
		{
			// Rekursion kantatapaus 1:
			// peli on p‰‰ttynyt (ei yht‰‰n laillista siirtoa).
			return MinimaxArvo(pisteyta_lopputulos(syvyys), Siirto());
		}

		if (syvyys == 0)
		{
			// Rekursion kantatapaus 2:
			// ollaan katkaisusyvyydess‰.
			return MinimaxArvo(evaluoi(), Siirto());
		}

		// Siirtoja on j‰ljell‰ ja ei olla katkaisusyvyydess‰,
		// joten kokeillaan yksitellen mahdollisia siirtoja,
		// ja kutsutaan minimax:a kullekin seuraaja-asemalle.
		// Otetaan paras minimax-arvo talteen (alustetaan
		// paras_arvo mahdollisimman huonoksi siirtovuoroisen
		// pelaajan kannalta).
		float paras_arvo = _siirtovuoro == VALKEA ?
			std::numeric_limits<float>::lowest() : std::numeric_limits<float>::max();
		Siirto paras_siirto;
		for (Siirto& s : siirrot)
		{
			Asema uusi = *this;
			uusi.tee_siirto(s);

			// Rekursioasekel: kutsutaan minimax:ia seuraaja-asemalle.
			MinimaxArvo arvo = uusi.minimax(syvyys - 1);

			// Jos saatiin paras arvo, otetaan se talteen.
			if (_siirtovuoro == VALKEA && arvo._arvo > paras_arvo)
			{
				paras_arvo = arvo._arvo;
				paras_siirto = s;
			}
			else if (_siirtovuoro == MUSTA && arvo._arvo < paras_arvo)
			{
				paras_arvo = arvo._arvo;
				paras_siirto = s;
			}
		}

		// Palautetaan paras arvo.
		return MinimaxArvo(paras_arvo, paras_siirto);
	}



	// Laskee materiaalitasapainon (valkean nappuloiden arvo - mustan nappuloiden arvo).
	// Nappuloiden arvot:
	//
	// sotilas		1
	// ratsu		3
	// l‰hetti		3
	// torni		5
	// daami		9
	//
	float materiaali() const
	{
		// Liitet‰‰n nappulatyyppeihin niiden arvot.
		static std::map<int, float> nappuloiden_arvot = {
			{wP,  1.0f}, {wN,  3.0f}, {wB,  3.0f}, {wR,  5.0f}, {wQ,  9.0f},
			{bP, -1.0f}, {bN, -3.0f}, {bB, -3.0f}, {bR, -5.0f}, {bQ, -9.0f},
			{NA,  0.0f}
		};

		float arvo = 0;
		for (int rivi = 0; rivi < 8; ++rivi)
			for (int linja = 0; linja < 8; ++linja)
			{
				int nappula = _lauta[rivi][linja];
				arvo += nappuloiden_arvot[nappula];
			}
		return arvo;
	}


	// Palauttaa valkean ja mustan (raaka)siirtojen lukum‰‰rien erotuksen.
	float mobiliteetti() const
	{
		std::vector<Siirto> valkean_siirrot;
		std::vector<Siirto> mustan_siirrot;

		// Funktion totetus on raskas, koska generoidaan raakasiirtoja.
		// TODO: voisiko optimoida optimoida jotenkin?
		anna_kaikki_raakasiirrot(VALKEA, valkean_siirrot);
		anna_kaikki_raakasiirrot(MUSTA, mustan_siirrot);

		return (float)valkean_siirrot.size() - (float)mustan_siirrot.size();
	}



	// Antaa aseman kaikki raakasiirrot
	void anna_kaikki_raakasiirrot(int pelaaja, std::vector<Siirto>& siirrot) const;


	// Linnoitukset ja ohestalyˆnnit
	void anna_linnoitukset(int pelaaja, std::vector<Siirto>& siirrot) const
	{
		if (pelaaja == VALKEA)
		{
			// kuningas ja torni eiv‰t liikkuneet, f1 tyhj‰, g1 tyhj‰,
			// kuningas ei shakissa (e1 ei uhattu), f1 ei uhattu (kuningas ei
			// saa s‰‰ntˆjen mukaan hyp‰t‰ uhatun ruudun yli)
			if (_valkea_lyhyt_linna_sallittu && _lauta[7][5] == NA && _lauta[7][6] == NA &&
				!onko_ruutu_uhattu(7, 4, MUSTA) && !onko_ruutu_uhattu(7, 5, MUSTA) &&
				!onko_ruutu_uhattu(7, 6, MUSTA))
			{
				// Lis‰t‰‰n lyhyt linnoitus
				siirrot.push_back(Siirto(7, 4, 7, 6));
			}
			if (_valkea_pitka_linna_sallittu && _lauta[7][1] == NA &&
				_lauta[7][2] == NA && _lauta[7][3] == NA &&
				!onko_ruutu_uhattu(7, 4, MUSTA) && !onko_ruutu_uhattu(7, 3, MUSTA) &&
				!onko_ruutu_uhattu(7, 2, MUSTA))
			{
				// Lis‰t‰‰n pitk‰ linnoitus
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
				// Lis‰t‰‰n lyhyt linnoitus
				siirrot.push_back(Siirto(0, 4, 0, 6));
			}
			if (_musta_pitka_linna_sallittu && _lauta[0][1] == NA &&
				_lauta[0][2] == NA && _lauta[0][3] == NA &&
				!onko_ruutu_uhattu(0, 4, VALKEA) && !onko_ruutu_uhattu(0, 3, VALKEA) &&
				!onko_ruutu_uhattu(0, 2, VALKEA))
			{
				// Lis‰t‰‰n pitk‰ linnoitus
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

	// Antaa tornin raakasiirrot (shakeista ei v‰litet‰).
	void anna_tornin_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;

	void anna_daamin_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;

	void anna_kuninkaan_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;


	/// Anna nappulan mahdolliset raakasiirrot tiettyyn suntaa,
	///
	///		\param rivi		nappulan l‰htˆrivi
	///		\param linja	nappulan l‰htˆlinja
	///		\ param rivi_muutos rivien koodinaattimuutos (esim. -1 on "ylˆs")
	///		\ param linja_muutos linjojen koodinaattimuutos (esim. 1 on "vasen")
	///		\param pelaaja	nappulan v‰ri (VALKEA, MUSTA)
	///		\param max_askeleet maksimiaskeleet ko. suuntaan?
	///		\param
	///
	void anna_raakasiirrot_suunassa(int rivi, int linja, int rivi_muutos, int linja_muutos, int pelaaja,
		int max_askeleet, bool voi_lyoda, bool pakko_lyoda,
		std::vector<Siirto>& siirrot) const;

	// Laudan nappulat. Indeksointi [rivi][linja], esim.
	//
	// [0][0] : vasen yl‰nurkka ("a8")
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

	// Kirjanpito siit‰, onko kuningas tai torni liikkunut.
	// Asetetaan tarpeen mukaan falseksi.
	bool _valkea_lyhyt_linna_sallittu = true;
	bool _valkea_pitka_linna_sallittu = true;
	bool _musta_lyhyt_linna_sallittu = true;
	bool _musta_pitka_linna_sallittu = true;

	// Ohestalyˆnti
	int _kaksoisaskel_linjalla = -1;
};

