#include <iostream>
#include "asema.h"
#include "siirto.h"

int main()
{
	std::string P1;
	std::string P2;

	std::cout << "Is white human or computer: H or C" << std::endl;
	std::cin >> P1;

	std::cout << "Is black human or computer: H or C" << std::endl;
	std::cin >> P2;

	std::string players[2] = {P1, P2};


	// Alkuasema.
	Asema asema;
	std::string pelaajaVari;

	std::vector<Siirto> siirrot;

	std::string xyz;

	asema.anna_siirrot(siirrot);

	while (siirrot.size() > 0)
	{
		asema.tulosta();

		for (Siirto& s : siirrot)
		{
			s.tulosta();
			std::cout << std::endl;
		}

		std::cout << std::endl << "Laillisia siirtoja: " << siirrot.size() << std::endl;

		if (players[asema._siirtovuoro] == "C") {
			std::cout << std::endl << "MINIMAXING" << std::endl;
			MinimaxArvo arvo = asema.minimax(3);
			asema.tee_siirto(arvo._siirto);
		}
		else
		{
			while (true) {
				std::cout << "Input move: ";
				std::cin >> xyz;

				bool isValidMove = false;
				for (const Siirto& s : siirrot)
				{
					if (s._a_l == (xyz[0] - 'a') && s._a_r == ('8' - xyz[1]) &&
						s._l_l == (xyz[2] - 'a') && s._l_r == ('8' - xyz[3])) {
						isValidMove = true;
						break;
					}
				}

				if (!isValidMove) {
					std::cout << "Invalid move. Please enter a legal move." << std::endl;
				}
				else {
					break;
				}
			}
			Siirto siirtoAAA(xyz);
			asema.tee_siirto(siirtoAAA);
		}

		siirrot.clear();
		asema.anna_siirrot(siirrot);
	}

	asema.tulosta();

	float lopputulos = asema.pisteyta_lopputulos(0);

	if (lopputulos >= 1000000) {
		std::cout << "Musta Kuningas on matissa, valkea voitti pelin." << std::endl;
	}
	else if (lopputulos == 0) {
		std::cout << "Peli päättyi pattiin." << std::endl;
	}
	else if (lopputulos <= -1000000) {
		std::cout << "Valkea Kuningas on matissa, musta voitti pelin!" << std::endl;
	}

	return 0;
}
