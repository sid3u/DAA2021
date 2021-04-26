#include "pch.h"

#include "Tool.h"

//Rajouter aussi l'en-tête des fonctions dans Tool.h

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
	//On récupère le nom de la balise
	std::getline(line_stream, balise, ' ');

	return balise;
}

std::string Tool::get_raw_balise(std::string line) {

	std::istringstream line_stream(line);
	std::string trash;
	std::string balise;

	//Permet de supprimer les blancs avant la balise
	std::getline(line_stream, trash, '<');
	//On récupère le nom de la balise
	std::getline(line_stream, balise, '>');

	return balise;
}


//Retourne le contenu de la balise (entre les chevrons)
std::string Tool::get_balise_name(std::string line) {

	std::istringstream line_stream(line);
	std::string trash;
	std::string balise_name;

	//Permet de supprimer les blancs avant la balise
	std::getline(line_stream, trash, '<');
	//On r�cup�re la premi�re partie de la balise mais �a ne nous int�resse pas
	std::getline(line_stream, trash, '"');
	//On stocke le texte qui se trouve dans la balise (le contenu)
	std::getline(line_stream, balise_name, '"');

	return balise_name;
}

//Retourne le contenu de la balise (entre les chevrons)
std::string Tool::get_balise_content(std::string line) {

	std::istringstream line_stream(line);
	std::string trash;
	std::string balise_content;

	//Permet de supprimer les blancs avant la balise
	std::getline(line_stream, trash, '<');
	//On récupère la première partie de la balise mais ça ne nous intéresse pas
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

	//On boucle pour arriver jusqu'à l'id parce tout ce qu'il y a avant ne nous intéresse pas
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
	Node *nodeString;
	std::string line;

	std::string balise;
	std::string balise_content;

	std::string lib_name = "";
	std::string fct_name = "";

	std::string lib = "";
	std::string lib_string = "";
	std::string lib_string2 = "";
	std::string function = "";
	std::string function_string = "";
	std::string description = "";
	std::string chaine_de_caracteres = "";

	std::ifstream file("techniquestest.xml");

	//On parcourt le fichier
	if (file.is_open()) {

		//La toute première technique :
		/*
		node = new NodeSearch(lib_name);
		node = new NodeSearch(fct_name, node);
		
		*/

		//On récupère chaque ligne du fichier XML
		while (getline(file, line)) {

			//FONCTIONNEL
			//Si c'est une balise technique (le début de la technique)
			if (get_balise(line) == "lib" || get_raw_balise(line) == "lib") {

				
				nb_nodes = 0;
				//std::cout << "On va créer une nouvelle technique" << std::endl;

				//On récup l'id de cette technique
				id++;
				lib_name = get_balise_name(line);
				lib_string='"'+get_balise_name(line)+'"';
				lib_string2='<'+get_balise_name(line)+'>';
				
				if (lib_name != ""){
					node = new NodeOperationOR(new NodeSearch(lib_string), new NodeSearch(lib_string2));
					technicals.push_back(new Technical(id, lib_name, node));
				}
				
			}
			//FONCTIONNEL
			//On arrive à la fin de la technique. On peut donc la construire
			else if (get_balise(line) == "/lib" || get_raw_balise(line) == "/lib") {
		
				lib_name = "";
				description = "";
				//system("PAUSE");
			}

			else {

				
				if (get_raw_balise(line) == "function" && get_balise_content(line) != "") {
					//Pour le moment on utilise juste une variable mais il va falloir stocker dans un vecteur puisqu'on peut avoir plusieurs fonctions
					function ="		call	ds:" + get_balise_content(line);
					function_string='"'+get_balise_content(line)+'"';
					fct_name = get_balise_content(line);
					//std::cout << "function : " << function << std::endl;
					
					if(lib_name != ""){		//S'il existe une librairie
						//std::cout << "node = new NodeOperationOR(new NodeSearch("<<function<<"), node);"<< std::endl;
						description = lib_name + " -> " + fct_name;
						
						
						//node = new NodeSearch(function);		//On crée un OU logique avec le noeud précédent et le nouveau
						//nodeString = new NodeSearch(function_string);

						node = new NodeOperationOR(new NodeSearch(function),new NodeSearch(function_string));	

						technicals.push_back(new Technical(id, description, node));
						//technicals.push_back(new Technical(id, description, nodeString));
					}
					
					else if (lib_name == ""){ 	
						description ="No Library " + fct_name;	//Si aucun librairie n'est spécifiée
						 	//S'il n'existe pas encore de noeuds
						node = new NodeSearch(function);	//Alors on en crée un nouveau
							//std::cout << "node = new NodeSearch("<<function<<");"<< std::endl;
						technicals.push_back(new Technical(id, description, node));
					}
					

					nb_nodes++;	//On incrémente ici car dans tous les cas on crée un nouveau noeud
					

				}

				else if (get_raw_balise(line) == "string") {
					chaine_de_caracteres = get_balise_content(line);
					
					description = lib_name + " : " + chaine_de_caracteres;
					 	//S'il n'existe pas encore de noeuds
					node = new NodeSearch(chaine_de_caracteres);	//Alors on en crée un nouveau
						//std::cout << "node = new NodeSearch("<<chaine_de_caracteres<<");"<< std::endl;
					technicals.push_back(new Technical(id, description, node));
					
					
					nb_nodes++;
					//std::cout << "chaine_de_caracteres : " << chaine_de_caracteres << std::endl;
				}
			}
		}

		file.close();
	}
	//std::cout << "Termine !" << std::endl;
	return technicals;
}