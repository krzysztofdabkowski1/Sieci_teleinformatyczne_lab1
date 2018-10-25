#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <time.h>
//#pragma warning(disable:4996)

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
		for (int j = 0; j < 7; j++) 
			if ((znak[i] &tab[j]) == tab[j]) suma++;
	}
	return suma%2;
}
/*int bit_par(vector <char> znak) {
	int i = 0, suma = 0;
	int bajt[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	for (i; i < znak.size();i++) {
		dec_to_bin(znak[i],bajt);
		for (int j = 0; j < 8; j++) {
			if (bajt[j] == 1) suma++;
		}
	}
	cout << "Suma:" << suma << endl;
	if (suma % 2 == 0) suma = 0;
	else suma = 1;
	//cout << "Bit parzystosci: " << suma << endl;
	return suma;
}*/
int suma_modulo(vector <char> znak) {
	int i = 0, suma = 0;
	int bajt[8] = {0,0,0,0,0,0,0,0};
	while (i<znak.size()) {

		suma += znak[i];
		i++;
	}
	suma = suma %128;
	dec_to_bin(suma,bajt);
	//cout << "Suma modulo: ";
	//for (int j = 7; j >= 0; j--)
		//cout << bajt[j];
	cout <<endl;
	return suma;
}
int CRC(vector<char> znak) {
	int i = 0,suma=0;
	int bajt[8];
	vector <int> v;
	vector <int> dzielnik{1,1,0,1,0,0,1,0};
	for (i; i < znak.size();i++) {

		dec_to_bin(znak[i],bajt);
		for (int j = 7; j >= 0; j--) {
			v.push_back(bajt[j]);
		}
	}
	cout <<endl;
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
	//cout << "CRC: ";
	for (int i = v.size() - dzielnik.size() + 1; i < v.size(); i++) {
		suma += v[i] * pow(2, v.size()-i);
		cout << v[i];
	}
	//cout << endl << suma << endl;
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
	cout << "ilosc bledow:" << wrong_bits << endl;
	int bajt, bit;
	vector <int> repeat_bajt,repeat_bit;
	srand(time(0));
	for (int i = 0; i < wrong_bits; i++) {
		
		do {
			bajt = random(znak.size());
			bit = pow(2, random(8));
		} while (is_repeated(repeat_bajt, repeat_bit, bajt, bit) == true);
		repeat_bajt.push_back(bajt);
		repeat_bit.push_back(bit);
		
		if ((char)bit != (znak[bajt]&(char)bit)) {
			znak[bajt] |= char(bit);
		}
		else {
			znak[bajt] &= (char)~bit;
		}
		
	}

	/*for (int i = 0; i < znak.size(); i++) {
		cout << znak[i];
	}*/
	cout << endl;
}
int main()
{
	vector <char> znak;
	ifstream is("book.txt", std::ifstream::binary);
	if (is) {
		is.seekg(0, is.end);
		int length = is.tellg();
		is.seekg(0, is.beg);
		char * buffer = new char[length + 1];
		is.read(buffer, length);
		for (int i = 0; i < length; i++) znak.push_back(buffer[i]);

		//cout.write(buffer, length);
		cout << endl;
		delete[] buffer;
	}
	is.close();
	
	
	
	ofstream os("plik2.txt", std::ofstream::binary);
	
	//znak.push_back(suma_modulo(znak));
	//znak.push_back(CRC(znak));
	znak.push_back(bit_par(znak));
	error_maker(znak);
	os.write(znak.data(), znak.size());
	os.close();
	
	
	vector <char> znak2;
	char last_letter;
	ifstream is2("plik2.txt", std::ifstream::binary);
	if (is2)
	{
		is2.seekg(0, is2.end);
		int length = is2.tellg();
		is2.seekg(0, is2.beg);
		char * buffer2 = new char[length + 1];
		is2.read(buffer2, length);
		//cout.write(buffer2, length);
		cout << endl;
		for (int i = 0; i < length; i++) {
			znak2.push_back(buffer2[i]);
			//cout << znak2[i];
		}
		last_letter = znak2[znak2.size() - 1];
		znak2.pop_back();
		delete[] buffer2;
		is2.close();
	}

	if (bit_par(znak2) == last_letter) cout << endl << "zgodny bit parzystosci" << endl;
	else cout << "bledny bit parzystosci" << endl;

	//if (suma_modulo(znak2) == last_letter) cout << endl << "zgodna suma modulo" << endl;
	//else cout << "bledna suma modulo" << endl;
	
	
	//if (CRC(znak2) == last_letter) cout << endl << "zgodne CRC" << endl;
	//else cout << "bledne CRC" << endl;
	system("pause");
	return 0;
	
}