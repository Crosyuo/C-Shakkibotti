#include <cassert>
#include <map>
#include "siirto.h"
#include <iostream>
using namespace std;

Siirto::Siirto(const std::string& s)
{
    // Alkuruudun koordinaatit
    _a_l = s[0] - 'a';
    _a_r = '8' - s[1];

    // Loppuruudun koordinaatit
    _l_l = s[2] - 'a';
    _l_r = '8' - s[3];

    if (_l_r == 0 && s.length() == 5) {
        switch (s[4]) {
        case 'Q': _korotettava_nappula = wQ; break;
        case 'R': _korotettava_nappula = wR; break;
        case 'N': _korotettava_nappula = wB; break;
        case 'B': _korotettava_nappula = wN; break;
        }
    }
    else if (_l_r == 7 && s.length() == 5) {
        switch (s[4]) {
        case 'Q': _korotettava_nappula = bQ; break;
        case 'R': _korotettava_nappula = bR; break;
        case 'N': _korotettava_nappula = bB; break;
        case 'B': _korotettava_nappula = bN; break;
        }
    }
}

void Siirto::tulosta() const
{
    // Taulukot, jotka sisältävät kirjainten vastaavuudet shakki ruutuihin.
    const char sarakkeet[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
    const char rivit[] = { '8', '7', '6', '5', '4', '3', '2', '1' };
    std::string temp;

    // Alkuruudun koordinaatit
    char alkuruutu[3];
    alkuruutu[0] = sarakkeet[_a_l];
    alkuruutu[1] = rivit[_a_r];
    alkuruutu[2] = '\0'; // Merkkijonon lopetusmerkki

    // Loppuruudun koordinaatit
    char loppuruutu[3];
    loppuruutu[0] = sarakkeet[_l_l];
    loppuruutu[1] = rivit[_l_r];
    loppuruutu[2] = '\0'; // Merkkijonon lopetusmerkki

    switch (_korotettava_nappula) {
    case 3: case 9:  temp = "Q"; break;
    case 0: case 6:  temp = "R"; break;
    case 2: case 8:  temp = "B"; break;
    case 1: case 7:  temp = "N"; break;
    }

    // Tulostetaan muodossa "e2e4"
    std::cout << "Sallitut siirrot: (" << alkuruutu << loppuruutu << temp << ")";
}