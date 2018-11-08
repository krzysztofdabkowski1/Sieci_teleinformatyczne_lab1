#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <time.h>
//https://docs.google.com/spreadsheets/d/1TEMAQipYx6IGz3JQt0zzMQnM_xoSjhrmMQVAMGUpcjk/edit?usp=sharing

using namespace std;

void dec_to_bin(char znak,int bajt[8])
{
	int liczba = znak;
	int i = 0;
	while (liczba)
	{
		bajt[i++] = liczba % 2;
		liczba /= 2;
	}
}
int bit_par(vector <char> znak) {
	int suma = 0, tab[] = {1,2,4,8,16,32,64};
	for (int i = 0; i < znak.size(); i++) {
		for (int j = 0; j < 7; j++) {
			if ((znak[i] & tab[j]) == tab[j]) suma++;
		}
	}
	return suma%2;
}
int suma_modulo(vector <char> znak) {
	int suma = 0;
	for (int i = 0; i < znak.size();i++) suma += znak[i];
	cout <<endl<< suma<<endl;
	suma %=128;
	return suma;
}
int CRC(vector<char> znak) {
	int i = 0,suma=0;
	int bajt[8] = {0,0,0,0,0,0,0,0};
	vector <int> v;
	vector <int> dzielnik{1,1,0,1,0,0,1};
	for (i; i < znak.size();i++) {
		//cout << znak[i];
		dec_to_bin(znak[i],bajt);
		for (int j = 7; j >= 0; j--) {
			v.push_back(bajt[j]);
		}
	}
	int vec_size = v.size();
	for (int i = 0; i < dzielnik.size()-1; i++) v.push_back(0);
	i = 0;
	while (i < vec_size) {
		if (v[i] == 1) {
			for (int j = 0; j < dzielnik.size(); j++)
				v[i + j] = (v[i + j] + dzielnik[j]) % 2;
		}
		i++;
	}
	cout << "CRC: ";
	for (int i = v.size() - dzielnik.size() +1; i < v.size(); i++) {
		suma += v[i] * pow(2, v.size()-i);
		cout << v[i];
	}
	cout <<endl;
	return suma;
}

int random(int zakres) {
	return rand() % zakres;
}
bool is_repeated(vector <int> repeat_bajt, vector <int> repeat_bit, int bajt, int bit) {
	bool repeated = false;
	for (int i = 0; i < repeat_bajt.size(); i++) {
		if (repeat_bajt[i] == bajt && repeat_bit[i] == bit) {
			repeated = true;
			break;
		}
		}
	return repeated;
}

void error_maker(vector <char> &znak) {
	int wrong_bits = znak.size()*8*0.01;

	cout << endl<<wrong_bits << endl;
	int byte, bit;
	vector <int> repeat_byte,repeat_bit;
	srand(time(0));
	for (int i = 0; i < wrong_bits; i++) {
		do {
			byte = random(znak.size());
			bit = pow(2, random(8));
		} while (is_repeated(repeat_byte, repeat_bit, byte, bit) == true);
		repeat_byte.push_back(byte);
		repeat_bit.push_back(bit);
		
		if (bit != (znak[byte]&bit)) {
			znak[byte] |= bit;
		}
		else {
			znak[byte] &= ~bit;
		}
		
	}

	/*for (int i = 0; i < znak.size(); i++) {
		cout << znak[i];
	}*/
	cout << endl;
}
vector <char> Read(string name) {
	vector <char> znak;
	ifstream is(name, std::ifstream::binary);
	if (is) {
		is.seekg(0, is.end);
		int length = is.tellg();
		is.seekg(0, is.beg);
		char * buffer = new char[length + 1];
		is.read(buffer, length);
		for (int i = 0; i < length; i++) znak.push_back(buffer[i]);
	//	cout.write(buffer, length);
		cout << endl;
		delete[] buffer;
	}
	is.close();
	return znak;
}
void Write(string name,vector <char> znak) {
	ofstream os(name, std::ofstream::binary);
	os.write(znak.data(), znak.size());
	os.close();
}
void print(vector <char> tekst) {
	for (int i = 0; i < tekst.size(); i++)cout << tekst[i];
}
int main()
{
	vector <char> tekst = Read("plik.txt");

	//tekst.push_back(suma_modulo(tekst));
	tekst.push_back(CRC(tekst));
	//tekst.push_back(bit_par(tekst));

	print(tekst);
	Write("plik2.txt", tekst);
	vector <char> tekst2 = Read("plik2.txt");
	error_maker(tekst2);
	print(tekst2);
	int last_letter=tekst2[tekst2.size() - 1];
	tekst2.pop_back();

	//if (bit_par(tekst2) == last_letter) cout << endl << "zgodny bit parzystosci" << endl;
	//else cout <<endl<< "bledny bit parzystosci" << endl;
	
	//if (suma_modulo(tekst2) == last_letter) cout << endl << "zgodna suma modulo" << endl;
	//else cout << "bledna suma modulo" << endl;

	if (CRC(tekst2) == last_letter) cout << endl << "zgodne CRC" << endl;
	else cout << "bledne CRC" << endl;

	system("pause");
	return 0;
}