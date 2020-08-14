#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <ctype.h>
#include <math.h>

bool contains(std::vector <std::string>& dict, std::string& search) {
    for (int i(0); i < dict.size(); i++) {
        if (dict[i] == search && !isupper(dict[i][0]) ) {
            return true;
        }
    }
    return false;
}

int main()
{
    std::cout << "Bienvenue dans ScrabbleHelper par Aldric L. (2020) !" << std::endl;
    std::cout << "Patientez le temps du chargement du dictionnaire..." << std::endl;
    std::ifstream fichier("fr.txt");
    //On optimise enormement en créant une map au lieu d'un vector : on ne parcourt donc que le vector qui nous intéresse
    //On ne peut pas aller plus precisement qu'une lettre puisque certaines prepositions ne font qu'une seule lettre.
    std::map <char, std::vector <std::string>> dictionnaire;

    if (fichier){
        std::string ligne; 
        char l = 'a';
        std::vector <std::string> char_dict;
        while (getline(fichier, ligne)) {
            if (!isupper(ligne[0]) && ligne[0] == l) {
                char_dict.push_back(ligne);
            }
            else if (!isupper(ligne[0])) {
                dictionnaire.insert(std::make_pair(l, char_dict));
                l = ligne[0];
                char_dict.clear();
                char_dict.push_back(ligne);
            }
        }
    }else {
        std::cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << std::endl;
        return 0;
    }

    while (true) {
        bool debug(false);
        std::cout << "Pret ! Entrez vos lettres sans les separer :";
        std::string lettres;
        std::cin >> lettres;
        //Mode debug : -d=lettres
        if (lettres.size() > 2 && lettres[0] == '-' && lettres[1] == 'd' && lettres[2] == '=') {
            debug = true;
            lettres = lettres.erase(0, 3);
            std::cout << "Debug mode - Lettres : " << lettres << std::endl;
        }
        bool no(false);
        for (int i(0); i < lettres.length(); i++) {
            if (!islower(lettres[i])) {
                no = true;
            }
        }

        while (no) {
            std::cout << "Erreur ! Lettres invalides. " << std::endl;
            std::cout << "Entrez vos lettres sans les separer :";
            std::cin >> lettres;
            no = false;
            for (int i(0); i < lettres.length(); i++) {
                if (!islower(lettres[i])) {
                    no = true;
                }
            }
        }
        
        int nb_max_tests = lettres.length() * lettres.length();
        int size = lettres.length();

        if (size > 4) {
            std::cout << "Comme il y a beaucoup de lettres, cette operation peut prendre du temps. (" + std::to_string((int)pow(size, size)) + " mots a tester)." << std::endl;
        }

        for (int i(0); i <= ((size + 1) * std::pow(10, size - 1)); i++) {
            std::string r_s = std::to_string(i);
            bool can(true);
            for (int y(0); y < r_s.length(); y++) {
                if ((int)r_s[y] - 48 > size)
                    can = false;
            }
            if (can) {
                std::string finals;
                bool invalide(false);
                int last(0);
                for (int y(0); y < r_s.length(); y++) {
                    int z = (int)r_s[y] - 48;
                    if (z != 0 && z != last) {
                        finals.push_back(lettres[z - 1]);
                        last = z;
                    }
                    else
                        invalide = true;
                }
                if (!invalide) {
                    //On verifie qu'il n'y a pas deux fois la meme lettre
                    bool repete = false;
                    for (int j(0); j < finals.length(); j++) {
                        for (int g(0); g < finals.length(); g++) {
                            if (finals[g] == finals[j] && j != g) {
                                repete = true;
                            }
                        }
                    }
                    if (debug)
                        std::cout << "Test de : " << finals << std::endl;
                    std::string test;

                    if (!repete && contains(dictionnaire[finals[0]], finals)) {
                        std::cout << "Possibilite : " << finals << std::endl;
                    }

                }
            }
        }

        std::cout << "Processus termine avec succes ! Voulez-vous entrer d'autres lettres ? (o/n)";
        std::string rep;
        std::cin >> rep;
        if (rep != "o" && rep != "O" && rep != "y" && rep != "Y" && rep != "oui" && rep != "Oui") {
            break;
        }
    }

}
