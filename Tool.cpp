#include "pch.h"

#include "Tool.h"

//Rajouter aussi l'en-t�te des fonctions dans Tool.h

vector<string> Tool::explode(string const &s, char delim) {

	vector<std::string> result;
	istringstream iss(s);

	for (string token; getline(iss, token, delim); ) {

		result.push_back(std::move(token));
	}

	return result;
}

void Tool::buildAssembler(string path) {

	string command = "idag -B \"" + path + "\"";

	system(command.c_str());
}

bool Tool::isInstruction(string line) {

	return (int)line[0] == 9 && (int)line[1] == 9 && (int)line[2] != 9 && line[2] != ';';
}

bool Tool::isAnAsmFile(string file) {

	return file.find(".asm") != string::npos;
}

int Tool::getMaxSizeNames(vector<Technical*> technicals) {

	int size, max = 0;

	for (Technical *technical : technicals) {

		size = technical->getName().size();

		if (size > max) {

			max = size;
		}
	}

	return max;
}

void Tool::deleteExtension(string &file) {

	if (file[file.length() - 4] == '.') {

		file = file.substr(0, file.length() - 4);
	}
}

//Retourne la balise d'ouverture dans un fichier
std::string Tool::get_balise(std::string line) {

	std::istringstream line_stream(line);
	std::string trash;
	std::string balise;

	//Permet de supprimer les blancs avant la balise
	std::getline(line_stream, trash, '<');
	//On r�cup�re le nom de la balise
	std::getline(line_stream, balise, ' ');

	return balise;
}

std::string Tool::get_raw_balise(std::string line) {

	std::istringstream line_stream(line);
	std::string trash;
	std::string balise;

	//Permet de supprimer les blancs avant la balise
	std::getline(line_stream, trash, '<');
	//On r�cup�re le nom de la balise
	std::getline(line_stream, balise, '>');

	return balise;
}

//Retourne le contenu de la balise (entre les chevrons)
std::string Tool::get_balise_content(std::string line) {

	std::istringstream line_stream(line);
	std::string trash;
	std::string balise_content;

	//Permet de supprimer les blancs avant la balise
	std::getline(line_stream, trash, '<');
	//On r�cup�re la premi�re partie de la balise mais �a ne nous int�resse pas
	std::getline(line_stream, trash, '>');
	//On stocke le texte qui se trouve dans la balise (le contenu)
	std::getline(line_stream, balise_content, '<');

	return balise_content;
}

//Retourne le contenu de la balise (entre les chevrons)
std::string Tool::get_balise_id(std::string line) {

	std::istringstream line_stream(line);
	std::string trash;
	std::string balise_id;

	//Permet de supprimer les blancs avant la balise
	std::getline(line_stream, trash, '<');

	//On boucle pour arriver jusqu'� l'id parce tout ce qu'il y a avant ne nous int�resse pas
	std::getline(line_stream, trash, '\'');

	//On stocke le texte qui se trouve dans la balise (le contenu)
	std::getline(line_stream, balise_id, '\'');

	return balise_id;
}

vector<Technical*> Tool::getTecnicals() {

	vector<Technical*> technicals;
	int id = 0;
	int nb_nodes = 0;
	Node *node;

	std::string line;
	std::string line2;

	std::string balise;
	std::string balise_content;

	std::string lib = "";
	std::string function = "";
	std::string description = "";
	std::string chaine_de_caracteres = "";

	std::ifstream file("techniques.xml");
	std::ifstream file2("techniques3.xml");

	//On parcourt le fichier
	if (file.is_open() && file2.is_open()) {

		//La toute premi�re technique :
		/*
		node = new NodeSearch(lib_name);
		node = new NodeSearch(fct_name, node);
		
		*/

		//On r�cup�re chaque ligne du fichier XML
		while (getline(file, line) && getline(file2, line2)) {

			//FONCTIONNEL
			//Si c'est une balise technique (le d�but de la technique)
			if (get_balise(line) == "technique" || get_raw_balise(line) == "technique") {

				//std::cout << "On va cr�er une nouvelle technique" << std::endl;

				//On r�cup l'id de cette technique
				id = std::stoi(get_balise_id(line));

				//On r�initialise le nombre de noeuds � 0
				nb_nodes = 0;

			}
			//FONCTIONNEL
			//On arrive � la fin de la technique. On peut donc la construire
			else if (get_balise(line) == "/technique" || get_raw_balise(line) == "/technique") {

				//Si pas de description alors on prend par d�faut le nom de la lib
				if (description == ""){
					if(lib == ""){ description = function; }
					else { description = lib; }
				}

				technicals.push_back(new Technical(id, description, node));
				//std::cout << "Cr�ation de la technique ... Description : " << description << " avec nombre de noeuds = " << nb_nodes << std::endl;

				//On r�initialise la description. Car si celle-ci est vide c'est qu'il n'y a pas de description et donc on prend par d�faut le nom de la lib ???
				description = "";

				//system("PAUSE");
			}

			else {

				//std::cout << get_raw_balise(line) << std::endl;

				//Une seule lib pour chaque technique
				if (get_raw_balise(line) == "lib") {
					lib = get_balise_content(line);
					//std::cout << "lib : " << lib << std::endl;
					node = new NodeSearch(lib); //Ici on ne cr�e pas de node (premier noeud)
					//std::cout << "node = new NodeSearch("<<lib<<");" << std::endl;
					nb_nodes++;	//On incr�mente le nombre de noeuds

				}
				else if (get_raw_balise(line) == "function") {
					//Pour le moment on utilise juste une variable mais il va falloir stocker dans un vecteur puisqu'on peut avoir plusieurs fonctions
					function = get_balise_content(line);
					//std::cout << "function : " << function << std::endl;
					
					if(get_raw_balise(line2) == "string"){
						node = new NodeOperationOR(new NodeSearch(function), node);
						//std::cout << "node = new NodeOperationOR(new NodeSearch("<<function<<"), node);"<< std::endl;
					}
					if(lib != ""){		//S'il existe une librairie
						//std::cout << "node = new NodeOperationOR(new NodeSearch("<<function<<"), node);"<< std::endl;
						node = new NodeOperationOR(new NodeSearch(function), node);		//On cr�e un OU logique avec le noeud pr�c�dent et le nouveau
					}
					else { 		//Si aucun librairie n'est sp�cifi�e
						if(nb_nodes < 1){ 	//S'il n'existe pas encore de noeuds
							node = new NodeSearch(function);	//Alors on en cr�e un nouveau
							//std::cout << "node = new NodeSearch("<<function<<");"<< std::endl;
						}
						else {
							node = new NodeSearch(function, node);		//Sinon on le chaine en ET logique avec le pr�c�dent
							//std::cout << "node = new NodeSearch("<<function<<", node);"<< std::endl;
						}
					}
					

					nb_nodes++;	//On incr�mente ici car dans tous les cas on cr�e un nouveau noeud
					

				}
				//Une seule description pour chaque technique
				else if (get_raw_balise(line) == "description") {
					description = get_balise_content(line);
					//std::cout << "description : " << description << std::endl;
				}

				else if (get_raw_balise(line) == "string") {
					chaine_de_caracteres = get_balise_content(line);
					if(nb_nodes < 1){ 	//S'il n'existe pas encore de noeuds
						node = new NodeSearch(chaine_de_caracteres);	//Alors on en cr�e un nouveau
						//std::cout << "node = new NodeSearch("<<chaine_de_caracteres<<");"<< std::endl;
					}
					else {
						node = new NodeOperationOR(new NodeSearch(chaine_de_caracteres), node);		//On cr�e un OU logique avec le noeud pr�c�dent et le nouveau
						//std::cout << "node = new NodeOperationOR(new NodeSearch("<<chaine_de_caracteres<<"), node);"<< std::endl;
					} 

					nb_nodes++;
					//std::cout << "chaine_de_caracteres : " << chaine_de_caracteres << std::endl;
				}
			}
		}

		file.close();
		file2.close();
	}
	//std::cout << "Termine !" << std::endl;
	return technicals;
}