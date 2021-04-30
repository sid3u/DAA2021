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
	vector<Technical*> technicalsString = Tool::getTecnicalsString();

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

				for (Technical *technicalS : technicalsString) {

					if (!technicalS->getNode()->isActivate()) {

						technicalS->getNode()->searchString(line);
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

	int maxNameSizeString = Tool::getMaxSizeNames(technicalsString), cpt_detectionString = 0;

	PatchCommand *patch_command = NULL;


	cout << "\n";
			cout << "---------------[Technical detail as LIBRARY]---------------\n";
			for (Technical *technical : technicals) {

				if (technical->getNode()->isActivate() && !(technical->getName().find(" ->")!= string::npos) ) {
				
					
						cout << technical->getName();
						


					for (int i = 0; i < maxNameSize - (int)technical->getName().size(); i++) {

						cout << " ";
					}

					cout << "\t";
			
			
					cout << "[CALL LIBRARY]\n";
			
			
					//cpt_detection++;

					if (technical->hasAPatch()) {

						patch_command = technical->getPatchCommand();
					}
				}
			}


	printFunction:
	cout << "\n[*] Do you want to see the suspicous function list [Y/N] ? ";

		cin >> answer;

		switch (answer) {

			case 'Y':
			case 'y':

			cout << "\n";
			cout << "---------------[Technical detail as LIBRARY or FUNCTION]---------------\n";
			for (Technical *technical : technicals) {

			if (technical->getNode()->isActivate()) {
			
				cout << technical->getName();
			
				if(technical->getNode()->isCall()){
				res = "[CALL FUNCTION]\n";
				}
				else if(!(technical->getName().find(" ->")!= string::npos)){
					res = "[CALL LIBRARY]\n";
				}
				else{ 
					res = "[STRING]\n";
				}
			

				for (int i = 0; i < maxNameSize - (int)technical->getName().size(); i++) {

					cout << " ";
				}

				cout << "\t";
			
			
				cout << res;
			
			
				cpt_detection++;

				if (technical->hasAPatch()) {

					patch_command = technical->getPatchCommand();
				}
			}

		}
		case 'N':
			case 'n':
				break;

			default:
			goto printFunction;
			break;
	}

	cout << "\n\n[*] Technical detected: " << cpt_detection << "/" << technicals.size();

	printString:
	cout << "\n[*] Do you want de see the suspicous string list [Y/N] ? ";

		cin >> answer;

		switch (answer) {

			case 'Y':
			case 'y':
				cout << "\n---------------[Technical detail as STRING]---------------\n";
				for (Technical *technicalS : technicalsString) {

					if (technicalS->getNode()->isActivate()) {
						
						cout << technicalS->getName();
						

					for (int i = 0; i < maxNameSize - (int)technicalS->getName().size(); i++) {

						cout << " ";
					}

					cout << "\t";
						
						
						cout << "[STRING]\n";
						
						
						cpt_detectionString++;

						if (technicalS->hasAPatch()) {

							patch_command = technicalS->getPatchCommand();
						}
					}
				}

			case 'N':
			case 'n':
				break;

			default:
			goto printString;
			break;

		}
	cout << "\n\n[*] Technical detected: " << cpt_detectionString << "/" << technicalsString.size();


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
	for (Technical *technicalS : technicalsString) {

		delete technicalS;
	}

	cout << "\n\n";

	return 0;
}