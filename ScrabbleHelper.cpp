#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <ctype.h>
#include <math.h>
#include <thread>

bool contains(std::vector <std::string>& dict, std::string& search) {
    for (int i(0); i < dict.size(); i++) {
        if ( dict[i] == search ) {
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
    std::map <char, std::map<int, std::vector <std::string>>> dictionnaire;

    if (fichier){
        std::string ligne; 
        char l = 'a';
        int n = 0;
        std::map<int, std::vector <std::string>> char_dict;
        while (getline(fichier, ligne)) {
            if (!isupper(ligne[0]) && ligne[0] == l && ligne.length() == n) {
                char_dict[n].push_back(ligne);
            }
            else if (!isupper(ligne[0]) && ligne[0] == l && ligne.length() != n) {
                if (char_dict.count(ligne.length()) == 0) {
                    std::vector<std::string> temp;
                    temp.push_back(ligne);
                    char_dict.insert(std::make_pair(ligne.length(), temp));
                    n = ligne.length();
                }
                else {
                    n = ligne.length();
                    char_dict[n].push_back(ligne);
                }
            }
            else if (!isupper(ligne[0]) && ligne[0] != l) {
                dictionnaire.insert(std::make_pair(l, char_dict));
                l = ligne[0];
                char_dict.clear();
                n = ligne.length();
                std::vector<std::string> temp;
                temp.push_back(ligne);
                char_dict.insert(std::make_pair(ligne.length(), temp));
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


        bool buffer_letter_bool = false;
        char buffer_letter_char = 'a';
        for (int i(0); i <= ((size + 1) * std::pow(10, size - 1)); i++) {

                std::string r_s = std::to_string(i);
                bool can(true);
                for (int y(0); y < r_s.length(); y++) {
                    if ((int)r_s[y] - 48 > size)
                        can = false;
                    for (int x(0); x < r_s.length(); x++) {
                        if (r_s[x] == r_s[y] && x != y) {
                            can = false;
                        }
                    }
                }
                if (can) {
                    std::string finals;
                    bool invalide(false);
                    int last(0);
                    int consonnes[4] = { 2, 2, 2, 2 };
                    for (int y(0); y < r_s.length(); y++) {
                        int z = (int)r_s[y] - 48;
                        if (z != 0 && z != last) {
                            finals.push_back(lettres[z - 1]);
                            last = z;
                        }
                        else {
                            invalide = true; break;
                        }
                        if (consonnes[0] == consonnes[1] && consonnes[1] == consonnes[2] && consonnes[1] == consonnes[3] && consonnes[2] != 2) {
                            invalide = true;
                            //std::cout << "Cons invalide,  refus de " << finals << " - " << consonnes[0] << consonnes[1] << consonnes[2] << consonnes[3] << std::endl;
                            //consonnes[0] = 2; consonnes[1] = 2; consonnes[2] = 2; consonnes[3] = 2;
                            break;
                        }
                        else if (y == 3) {
                            if (finals[0] == 'a' || finals[0] == 'e' || finals[0] == 'i' || finals[0] == 'o' || finals[0] == 'u' || finals[0] == 'y')
                                consonnes[0] = 0;
                            else
                                consonnes[0] = 1;
                            if (finals[1] == 'a' || finals[1] == 'e' || finals[1] == 'i' || finals[1] == 'o' || finals[1] == 'u' || finals[1] == 'y')
                                consonnes[1] = 0;
                            else
                                consonnes[1] = 1;
                            if (finals[2] == 'a' || finals[2] == 'e' || finals[2] == 'i' || finals[2] == 'o' || finals[2] == 'u' || finals[2] == 'y')
                                consonnes[2] = 0;
                            else
                                consonnes[2] = 1;
                        }
                        else {
                            consonnes[0] = consonnes[1];
                            consonnes[1] = consonnes[2];
                            consonnes[2] = consonnes[3];
                            if (lettres[z - 1] == 'a' || lettres[z - 1] == 'e' || lettres[z - 1] == 'i' || lettres[z - 1] == 'o' || lettres[z - 1] == 'u' || lettres[z - 1] == 'y') {
                                consonnes[3] = 0;
                            }
                            else {
                                consonnes[3] = 1;
                            }
                        }
                    }
                    if (!invalide) {
                        if (debug)
                            std::cout << "Test de : " << finals << " I=" << r_s << std::endl;

                        if (buffer_letter_char != finals[0]) {
                            if (dictionnaire[finals[0]].count(finals.length()) != 0) 
                                buffer_letter_bool = true;
                            else
                                buffer_letter_bool = false;
                        }
                        if (buffer_letter_bool && dictionnaire[finals[0]].count(finals.length()) != 0 && contains(dictionnaire[finals[0]][finals.length()], finals)) {
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
