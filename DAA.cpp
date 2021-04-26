#include "pch.h"

#include <iostream>

#include "Tool.h"

using namespace std;

int main(int argc, char *argv[]) {
	string res="";
	cout << "DoubleAntiAnalysis for windows. A Anti anti analysis scanner.\n";
	cout << "Usage: DoubleAntiAnalysis.exe PATH/malware\n\n";

	if (argc != 2) {

		cout << "[!] Argument error\n";

		return 1;
	}

	string path = argv[1];
	vector<string> explode = Tool::explode(argv[1], '/');
	string name = explode[explode.size() - 1];
	char answer;

	Tool::deleteExtension(name);

	cout << "[+] Analyse of " << name << "\n";
	
	cout << "\n";
	cout << "[+] Generating the " << name << ".asm file...\n";

	if (!Tool::isAnAsmFile(path)) {

		Tool::buildAssembler(path);
	}

	cout << "\n";
	cout << "[+] Scan for " << name << ".asm ...\n";

	vector<Technical*> technicals = Tool::getTecnicals();

	string line;
	ifstream myfile(name + ".asm");

	if (myfile.is_open()) {

		while (getline(myfile, line)) {

			if (Tool::isInstruction(line)) {

				for (Technical *technical : technicals) {

					if (!technical->getNode()->isActivate()) {

						technical->getNode()->search(line);
					}
				}
			}
		}

		myfile.close();
	}
	else {

		throw invalid_argument("Unable to open file");
	}

	int maxNameSize = Tool::getMaxSizeNames(technicals), cpt_detection = 0;
	PatchCommand *patch_command = NULL;

	cout << "\n";
	cout << "---------------[Technical detail]---------------\n";
	for (Technical *technical : technicals) {

		

		if (technical->getNode()->isActivate()) {

			cout << technical->getName();
			//if(technical->getName().find(".dll\n") && !(technical->getName().find(".dll ->")!= string::npos)){
			//	res = "[CALL LIBRARY]\n";
			//}
			if(technical->getName().find(" ->")!= string::npos){
				res = "[CALL FUNCTION]\n";
			}
			else if(technical->getName().find(" :")!= string::npos){
				res = "[STRING]\n";
			}
			else{
				res = "[CALL LIBRARY]\n";
			}

		for (int i = 0; i < maxNameSize - (int)technical->getName().size(); i++) {

			cout << " ";
		}

		cout << "\t\t";
			
			
			cout << res;
			
			
			cpt_detection++;

			if (technical->hasAPatch()) {

				patch_command = technical->getPatchCommand();
			}
		}
	}

	cout << "\n\n[*] Technical detected: " << cpt_detection << "/" << technicals.size();

	if (!Tool::isAnAsmFile(path) && patch_command != NULL) {

	point:
		cout << "\n[*] There is a fix for a detected technique. Do you want to start the program with the hotfix [Y/N] ? ";

		cin >> answer;

		switch (answer) {

		case 'Y':
			cout << "\n\n[+] Run " << path << " :\n";
			patch_command->run(path);

		case 'N':
			break;

		default:
			goto point;
			break;
		}
	}

	for (Technical *technical : technicals) {

		delete technical;
	}

	cout << "\n\n";

	return 0;
}