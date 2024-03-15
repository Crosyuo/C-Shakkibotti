#include <iostream>
#include "asema.h"
#include "siirto.h"

int main()
{
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
		siirrot.clear();
		asema.anna_siirrot(siirrot);
	}

	asema.tulosta();

	if (asema._siirtovuoro == 0) {
		std::cout << std::endl << "Musta voitti!";
	}
	else {
		std::cout << std::endl << "Valkea voitti!";
	}

	return 0;
}
