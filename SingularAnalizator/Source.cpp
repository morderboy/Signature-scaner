#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <strsafe.h>

using namespace std;

int StrProcessing(TCHAR *dirpath, TCHAR *buff) {
	if (!dirpath) { cout << "Error 1" << endl; return 1; }
	if (!buff) { cout << "Error 2" << endl; return 2; }
	cout << "ENTER PATH TO DIRECTORY" << endl;
	if (cout.fail()) { cout << "Error 3" << endl; return 3; }
	string* intermed = new string;
	if(!intermed) { cout << "Error 4" << endl; return 4; }
	if(!getline(cin, *intermed, '\n')) { cout << "Error 5" << endl; return 5; }
	if(strcpy_s(dirpath, MAX_PATH, (*intermed).c_str())) { cout << "Error 6" << endl; return 6; }
	delete intermed;
	intermed = nullptr;
	if(StringCchCopyA(buff, MAX_PATH, dirpath) != S_OK) { cout << "Error 7" << endl; return 7; }
	if(StringCchCatA(buff, MAX_PATH, TEXT("\\*")) != S_OK) { cout << "Error 8" << endl; return 8; }
	if(StringCchCatA(dirpath, MAX_PATH, TEXT("\\")) != S_OK) { cout << "Error 9" << endl; return 9; }
	return 0;
}
	struct shiftdata {
		char signatt[MAX_PATH];
		string shift;
		shiftdata* next;
	};




	int struct_push(shiftdata** top) {
		shiftdata* nov;
		nov = new shiftdata;
		if (top == NULL) {
			*top = nov;
		}
		else {
			nov->next = *top;
			*top = nov;
		}
		return 0;
	}

int ReadShiftFile(char* name, shiftdata** top) {
	fstream signatfile;
	signatfile.open(name, ios::in);
	int i = 0;
	string Signatur;
	while (!signatfile.eof()) {
		getline(signatfile, Signatur);
		struct_push(top);
		i = Signatur.find(';');
		strncpy_s((*top)->signatt, Signatur.c_str(), i);
		(*top)->shift = Signatur.substr(i + 1);
	}
	return 0;
}

int main() {
	ifstream file;
	ifstream signatfile;
	char identyficator[2];
	WIN32_FIND_DATA ffd;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	TCHAR *dirpath = new TCHAR[MAX_PATH];
	if(!dirpath) { cout << "Error 6" << endl; return 6; }
	TCHAR *buff = new TCHAR[MAX_PATH];
	if(!buff) { cout << "Error 6" << endl; return 6; }
	INT Shift;
	char shiftpath[MAX_PATH];
	shiftdata* top = NULL;
	
	if(StrProcessing(dirpath, buff)) { cout << "Error 6" << endl; return 6; }
	if((hFile = FindFirstFileA(buff, &ffd)) == INVALID_HANDLE_VALUE) { cout << "Error 6" << endl; return 6; }

	cout << "Enter path to file with Shift and Signatur" << endl;
	if (cout.fail()) {
		cout.clear();
		cout << "Error 333" << endl;
		return 333;
	}
	string* inter = new string;
	if (!getline(cin, *inter, '\n')) {
		cout << "Eroro 15" << endl;
		return 15;
	}
	strncpy_s(shiftpath, (*inter).c_str(), (int)(*inter).length());
	delete inter;


	if (ReadShiftFile(shiftpath, &top)) { cout << "error" << endl; return 10; }

	shiftdata *intermediattopvl;
	int len, p;
	char buffy[MAX_PATH];
	for (int k = 0; k < MAX_PATH; k++) {
		buffy[k] = 0;
	}
	p = 0;

	if (hFile == INVALID_HANDLE_VALUE) { cout << "Error 11" << endl; return 11; }
	do {
		if (ffd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) { continue; }
		file.open((string)dirpath += ffd.cFileName, ios::binary);
		if (!file) { cout << "Error" << endl; return 1; }
		file.read(identyficator, sizeof(identyficator));


		if (identyficator[0] != 'M' || identyficator[1] != 'Z') { file.close(); continue; }

		intermediattopvl = top;
		while (intermediattopvl != NULL) {
			char* end;
			Shift = strtol((intermediattopvl->shift).c_str(), &end, 10);
			len = strlen(intermediattopvl->signatt);
			file.seekg(Shift);
			file.read(buffy, len);
			file.seekg(0, ios::beg);
			for (int j = 0; j < len; j++) {
				if (buffy[j] == 0) { buffy[j] = 32; }
				if (buffy[j] != (intermediattopvl->signatt)[j]) {
					p++;
				}
			}
			if (p == 0) {
				cout << intermediattopvl->signatt << " <-Signatur   File Name-> ";
				cout << ffd.cFileName << endl;
				intermediattopvl = intermediattopvl->next;
			}
			else {
				p = 0;
				intermediattopvl = intermediattopvl->next;
				continue;
			}
			
		}
		
		file.close();
		file.clear();

	} while (FindNextFileA(hFile, &ffd));
	
	delete[] buff;
	delete[] dirpath;
	std::system("pause");
	return 0;
}