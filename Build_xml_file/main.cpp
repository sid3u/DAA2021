#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

<<<<<<< HEAD
//Liste des balises dans lesquelles chercher le contenu pour le fichier strings.xml (folder est un cas particulier puisqu'il faut rï¿½cupï¿½rer aussi le nom).
//Pour chaque balise faire un autre vecteur qui contient une brï¿½ve description de chaque balise ?
=======
//Liste des balises dans lesquelles chercher le contenu pour le fichier strings.xml (folder est un cas particulier puisqu'il faut récupérer aussi le nom).
//Pour chaque balise faire un autre vecteur qui contient une brève description de chaque balise ?
>>>>>>> f64c0c08fb3437fd9319a659e1f222df0fe6885f
std::vector<std::string> balises = { "psws", "avs", "regexs", "privs", "oids", "agents", "exts", "sddls", "guids", "regs", "oss", "products", "sids", "offices", "prots" , "utils", "keys", "dosstubs", "strings" };
std::vector<std::string> balises_sans_s = { "psw", "av", "regex", "priv", "oid", "agent", "ext", "sddl", "guid", "reg", "os", "product", "sid", "office", "prot" , "util", "key", "dosstub", "string" };

//Retourne la balise d'ouverture dans un fichier
std::string get_balise(std::string line) {

	std::istringstream line_stream(line);
	std::string trash;
	std::string balise;

	//Permet de supprimer les blancs avant la balise
	std::getline(line_stream, trash, '<');
<<<<<<< HEAD
	//On rï¿½cupï¿½re le nom de la balise
=======
	//On récupère le nom de la balise
>>>>>>> f64c0c08fb3437fd9319a659e1f222df0fe6885f
	std::getline(line_stream, balise, ' ');

	return balise;
}

std::string get_raw_balise(std::string line) {

	std::istringstream line_stream(line);
	std::string trash;
	std::string balise;

	//Permet de supprimer les blancs avant la balise
	std::getline(line_stream, trash, '<');
<<<<<<< HEAD
	//On rï¿½cupï¿½re le nom de la balise
=======
	//On récupère le nom de la balise
>>>>>>> f64c0c08fb3437fd9319a659e1f222df0fe6885f
	std::getline(line_stream, balise, '>');

	return balise;
}

//Retourne le contenu de la balise (entre les chevrons)
std::string get_balise_name(std::string line) {

	std::istringstream line_stream(line);
	std::string trash;
	std::string balise_name;

	//Permet de supprimer les blancs avant la balise
	std::getline(line_stream, trash, '<');
<<<<<<< HEAD
	//On rï¿½cupï¿½re la premiï¿½re partie de la balise mais ï¿½a ne nous intï¿½resse pas
=======
	//On récupère la première partie de la balise mais ça ne nous intéresse pas
>>>>>>> f64c0c08fb3437fd9319a659e1f222df0fe6885f
	std::getline(line_stream, trash, '"');
	//On stocke le texte qui se trouve dans la balise (le contenu)
	std::getline(line_stream, balise_name, '"');

	return balise_name;
}

//Retourne le contenu de la balise (entre les chevrons)
std::string get_balise_content(std::string line) {

	std::istringstream line_stream(line);
	std::string trash;
	std::string balise_content;

	//Permet de supprimer les blancs avant la balise
	std::getline(line_stream, trash, '<');
<<<<<<< HEAD
	//On rï¿½cupï¿½re la premiï¿½re partie de la balise mais ï¿½a ne nous intï¿½resse pas
=======
	//On récupère la première partie de la balise mais ça ne nous intéresse pas
>>>>>>> f64c0c08fb3437fd9319a659e1f222df0fe6885f
	std::getline(line_stream, trash, '>');
	//On stocke le texte qui se trouve dans la balise (le contenu)
	std::getline(line_stream, balise_content, '<');

	return balise_content;
}

//Retourne le contenu de la balise (entre les chevrons)
std::string get_balise_id(std::string line) {

	std::istringstream line_stream(line);
	std::string trash;
	std::string balise_id;

	//Permet de supprimer les blancs avant la balise
	std::getline(line_stream, trash, '<');

<<<<<<< HEAD
	//On boucle pour arriver jusqu'ï¿½ l'id parce tout ce qu'il y a avant ne nous intï¿½resse pas
=======
	//On boucle pour arriver jusqu'à l'id parce tout ce qu'il y a avant ne nous intéresse pas
>>>>>>> f64c0c08fb3437fd9319a659e1f222df0fe6885f
	std::getline(line_stream, trash, '\'');

	//On stocke le texte qui se trouve dans la balise (le contenu)
	std::getline(line_stream, balise_id, '\'');

	return balise_id;
}


void create_file() {
	std::string line;
	std::string lib_name = "";
	std::string fct_name;
	std::ifstream file("xml_file.xml");		//Fichier XML qui comporte le contenu des fichiers functions.xml et strings.xml de PeStudio

	std::ofstream file_to_write("techniques.xml");		//Notre nouveau fichier XML

	int id = 0;

	//On parcourt le fichier
	if (file.is_open()) {

<<<<<<< HEAD
		//Dans le fichier on appelle ï¿½a technique mais ce n'en sont pas vraiment
=======
		//Dans le fichier on appelle ça technique mais ce n'en sont pas vraiment
>>>>>>> f64c0c08fb3437fd9319a659e1f222df0fe6885f

		if (file_to_write.is_open()) {
			//Fonctionne pour le fichier functions.xml et 
			while (getline(file, line)) {

				//Si c'est une balise lib
				if (get_balise(line) == "lib") {

<<<<<<< HEAD
					//Si c'est la premiï¿½re ligne alors on ne ferme pas la balise
=======
					//Si c'est la première ligne alors on ne ferme pas la balise
>>>>>>> f64c0c08fb3437fd9319a659e1f222df0fe6885f
					if (lib_name == "") {
						file_to_write << "<technique id='" << id << "'>\n";

					}
					else {
						file_to_write << "</technique>\n<technique id='" << id << "'>\n";
					}

					id++;

					//On sauvegarde cette balise
					lib_name = get_balise_name(line);

					file_to_write << "\t<lib>" << lib_name << "</lib>\n";

				}
<<<<<<< HEAD
				//Sinon si c'est une fonction alors on va rï¿½cupï¿½rer son contenu
=======
				//Sinon si c'est une fonction alors on va récupérer son contenu
>>>>>>> f64c0c08fb3437fd9319a659e1f222df0fe6885f
				else if (get_balise(line) == "fct") {
					fct_name = get_balise_content(line);
					file_to_write << "\t<function>" << fct_name << "</function>\n";

				}

				//On check si la balise est dans le vecteur balises
				else if (std::find(balises.begin(), balises.end(), get_raw_balise(line)) != balises.end()) {

<<<<<<< HEAD
					//On crï¿½e une nouvelle technique
=======
					//On crée une nouvelle technique
>>>>>>> f64c0c08fb3437fd9319a659e1f222df0fe6885f
					file_to_write << "</technique>\n<technique id='" << id << "'>\n\t<description>" << get_raw_balise(line) << "</description>\n";

					id++;
				}

				else if (std::find(balises_sans_s.begin(), balises_sans_s.end(), get_raw_balise(line)) != balises_sans_s.end() || std::find(balises_sans_s.begin(), balises_sans_s.end(), get_balise(line)) != balises_sans_s.end()) {

					fct_name = get_balise_content(line);

					file_to_write << "\t<string>" << fct_name << "</string>\n";
				}


			}
		}

<<<<<<< HEAD
		//On ferme la derniï¿½re balise
=======
		//On ferme la dernière balise
>>>>>>> f64c0c08fb3437fd9319a659e1f222df0fe6885f
		file_to_write << "</technique>";

		file_to_write.close();
		file.close();
	}
}

int main() {
	create_file();

	system("PAUSE");

	return 0;
}