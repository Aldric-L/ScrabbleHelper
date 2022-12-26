#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <ctype.h>
#include <math.h>
#include <thread>

int main()
{
    std::cout << "Bienvenue dans ScrabbleHelper2 par Aldric L. (2022) !" << std::endl;
    std::cout << "Patientez le temps du chargement du dictionnaire..." << std::endl;
    std::ifstream fichier("fr.txt");
    //On optimise enormement en créant une map au lieu d'un vector : on ne parcourt donc que le vector qui nous intéresse
    std::map <int, std::vector <std::string>> dictionnaire;

    if (fichier) {
        std::string ligne;
        int n = 0;
        //std::map<int, std::vector <std::string>> char_dict;
        while (getline(fichier, ligne)) {
            if (!isupper(ligne[0]) && ligne.length() == n) {
                dictionnaire[n].push_back(ligne);
            }
            else if (!isupper(ligne[0]) && ligne.length() != n) {
                if (dictionnaire.count(ligne.length()) == 0) {
                    std::vector<std::string> temp;
                    temp.push_back(ligne);
                    dictionnaire.insert(std::make_pair(ligne.length(), temp));
                    n = ligne.length();
                }
                else {
                    n = ligne.length();
                    dictionnaire[n].push_back(ligne);
                }
            }
        }
    }
    else {
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

        for (int z(0); z <= fmin(dictionnaire.size(), lettres.length()); z++) {
            for (int i(0); i < dictionnaire[z].size(); i++) {
                std::vector<bool> test;
                std::vector<int>  letter_used;
                for (int j(0); j < dictionnaire[z][i].length(); j++) {
                    bool result(false);
                    for (int k(0); k < lettres.length(); k++) {
                        if (lettres[k] == dictionnaire[z][i][j]
                            && std::find(begin(letter_used), end(letter_used), k) == end(letter_used)) {
                            result = true;
                            letter_used.push_back(k);
                            break;
                        }

                    }
                    test.push_back(result);
                }
                letter_used.clear();
                if (std::find(test.begin(), test.end(), false) == test.end())
                    std::cout << "Possibilite : " << dictionnaire[z][i] << std::endl;
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

        std::cout << "Processus termine avec succes ! Voulez-vous entrer d'autres lettres ? (o/n)";
        std::string rep;
        std::cin >> rep;
        if (rep != "o" && rep != "O" && rep != "y" && rep != "Y" && rep != "oui" && rep != "Oui") {
            break;
        }
    }
}