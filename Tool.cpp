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

// On créer une nouvelle liste de techniques représentant uniquement les librairies et les fonctions
vector<Technical*> Tool::getTecnicals() {

	vector<Technical*> technicals;
	int id = 0;
	int nb_nodes = 0;
	Node *node;

	std::string line;

	std::string balise;
	std::string balise_content;

	std::string lib_name = "";
	std::string fct_name = "";

	std::string lib_string = "";
	std::string lib_string2 = "";

	std::string function = "";
	std::string function_string = "";

	std::string description = "";


	std::ifstream file("techniquestest.xml");

	//On parcourt le fichier
	if (file.is_open()) {

		//On récupère chaque ligne du fichier XML
		while (getline(file, line)) {

			//Si c'est une balise lib (le début de la librairie)
			if (get_balise(line) == "lib" || get_raw_balise(line) == "lib") {

				//On réinitialise le nombre de noeuds à 0
				nb_nodes = 0;
				//std::cout << "On va créer une nouvelle technique" << std::endl;

				//On récup l'id et le nom de cette librairie
				id = std::stoi(get_balise_id(line));
				lib_name = get_balise_name(line);

				// le fichier .asm fait appele aux librairies de 2 façons
				lib_string='"'+get_balise_name(line)+'"';
				lib_string2='<'+get_balise_name(line)+'>';

				//std::cout << id << std::endl;
				// Si le nom de la librairie est vide on ne crée pas de technique sur son nom
				if (lib_name != ""){
					node = new NodeOperationOR(new NodeSearch(lib_string), new NodeSearch(lib_string2));
					technicals.push_back(new Technical(id, lib_name, node));
				}
				
			}

			//On arrive à la fin de la librairie.
			else if (get_balise(line) == "/lib" || get_raw_balise(line) == "/lib") {
		
				lib_name = "";
				description = "";
				//system("PAUSE");
			}

			else {

				
				if (get_raw_balise(line) == "function" && get_balise_content(line) != "") {
					//On récup le nom de la fonction. 
					//Elle sera appelée dans le fichier .asm soit avec un call, soit sous forme de string
					function ="		call	ds:" + get_balise_content(line);
					function_string='"'+get_balise_content(line)+'"';
					
					fct_name = get_balise_content(line);
					//std::cout << "function : " << fct_name << std::endl;
					
					if(lib_name != ""){		//S'il existe une librairie
						
						description = lib_name + " -> " + fct_name;
						
						//On crée un OU logique avec les 2 types d'appels de la fonction
						node = new NodeOperationOR(new NodeSearch(function),new NodeSearch(function_string));	
						//std::cout << "node = new NodeOperationOR(new NodeSearch("<<function<<"),new NodeSearch("<<function_string<<"));"<< std::endl;
						technicals.push_back(new Technical(id, description, node));
					}
					
					else if (lib_name == ""){ 	//Si aucun librairie n'est spécifiée
						description ="No Library " + fct_name;	
						 
						 //On crée un nouveau noeud
						node = new NodeSearch(function);	
							//std::cout << "node = new NodeSearch("<<function<<");"<< std::endl;
						technicals.push_back(new Technical(id, description, node));
					}

					nb_nodes++;	//On incrémente ici car dans tous les cas on crée un nouveau noeud
					

				}

			}
		}

		file.close();
	}
	//std::cout << "Termine !" << std::endl;
	return technicals;
}

// On créer une nouvelle liste de techniques représentant uniquement les strings
vector<Technical*> Tool::getTecnicalsString() {

	vector<Technical*> technicals;
	int id = 0;
	int nb_nodes = 0;
	Node *node;
	
	std::string line;

	std::string balise;
	std::string balise_content;

	std::string lib_name = "";

	std::string description = "";

	std::string chaine_de_caracteres = "";

	std::ifstream file("techniquestest.xml");

	//On parcourt le fichier
	if (file.is_open()) {

		//On récupère chaque ligne du fichier XML
		while (getline(file, line)) {

			//Si c'est une balise lib (le début de la librairie)
			if (get_balise(line) == "lib" || get_raw_balise(line) == "lib") {

				//On réinitialise le nombre de noeuds à 0
				nb_nodes = 0;
				//std::cout << "On va créer une nouvelle technique" << std::endl;

				//On récup l'id et le nom de cette librairie
				id = std::stoi(get_balise_id(line));
				lib_name = get_balise_name(line);
				
			}
			//On arrive à la fin de la libraire.
			else if (get_balise(line) == "/lib" || get_raw_balise(line) == "/lib") {
		
				lib_name = "";
				description = "";
				//system("PAUSE");
			}

			else {

				if (get_raw_balise(line) == "string") {
					//On récup le contenu du string
					chaine_de_caracteres = get_balise_content(line);
					
					description = lib_name + " : " + chaine_de_caracteres;
					
					//On crée un nouveau noeud
					node = new NodeSearch(chaine_de_caracteres);

					//std::cout << "node = new NodeSearch("<<chaine_de_caracteres<<");"<< std::endl;
					technicals.push_back(new Technical(id++, description, node));
					
					
					nb_nodes++; 	//On incrémente ici car dans tous les cas on crée un nouveau noeud
					
				}
			}
		}

		file.close();
	}
	//std::cout << "Termine !" << std::endl;
	return technicals;
}