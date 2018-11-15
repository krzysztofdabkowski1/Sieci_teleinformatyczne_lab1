#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <time.h>
//https://docs.google.com/spreadsheets/d/1TEMAQipYx6IGz3JQt0zzMQnM_xoSjhrmMQVAMGUpcjk/edit?usp=sharing

using namespace std;

void dec_to_bin(int liczba, int bajt[8])
{
	for (int i = 0; i < 8; i++) bajt[i]=0;
	if (liczba < 0) {
		liczba = 256+liczba;
	}
	int i = 0;
	while (liczba)
	{
		bajt[i++] = liczba % 2;
		liczba /= 2;
	}
//	for (int i = 0; i < 8; i++) cout << bajt[i];
	//cout << endl;
}
void bit_par(vector <char> znak,int &wynik) {
	int suma = 0, tab[] = {1,2,4,8,16,32,64,128};
	for (int i = 0; i < znak.size(); i++) {
		for (int j = 0; j < 8; j++) {
			if ((znak[i] & tab[j]) == tab[j]) suma++;
		}
	}
	cout << endl <<"Suma bitow: "<< suma << endl;
	wynik= suma%2;
}
void suma_modulo(vector <char> znak,int &wynik) {
	int suma = 0;
	for (int i = 0; i < znak.size();i++) suma += znak[i];
	suma %=128;
	cout << endl <<"Suma modulo: "<< suma << endl;
	wynik= suma;
}
void CRC(vector<char> znak,int wynik[2]) {
	int i = 0,suma=0,bajt[8];
	vector <int> v;
	vector <int> dzielnik{1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,0 };
	for (i; i < znak.size();i++) {
		//cout <<i<<" "<<(int) znak[i]<<" ";
		dec_to_bin(znak[i],bajt);
		for (int j = 7; j >= 0; j--) {
			v.push_back(bajt[j]);
		}
	}
	int vec_size = v.size();
	for (int i = 0; i < dzielnik.size()-1; i++) v.push_back(0);
	i = 0;
	while (i <vec_size) {
		if (v[i] == 1) {
			for (int j = 0; j < dzielnik.size(); j++)
				v[i + j] = (v[i + j] + dzielnik[j]) % 2;
		}
		i++;
	}
	cout<<endl << "CRC: ";
	for (int i = 0; i < v.size(); i++) cout << v[i];
	for (int i = vec_size; i < vec_size+8; i++) {
		suma += v[i] * pow(2, v.size()-i-8);
		//cout << v[i];
	}
	wynik[0] = suma;

	suma = 0;
	for (int i = vec_size+8; i < v.size(); i++) {
		suma += v[i] * pow(2, v.size() - i);
		//cout << v[i];
	}
	wynik[1] = suma;
	
	cout <<endl;

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
	cout << endl << "Number of errors: " << wrong_bits << endl;
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
	int wynik[2];
	
	vector <char> tekst = Read("plik.txt");
	print(tekst);

	//suma_modulo(tekst,wynik[0]);
	CRC(tekst,wynik);
	//bit_par(tekst,wynik[0]);

	
	
	tekst.push_back(wynik[0]);
	tekst.push_back(wynik[1]);

	
	Write("plik2.txt", tekst);
	vector <char> tekst2 = Read("plik2.txt");
	//error_maker(tekst2);
	print(tekst2);
	Write("plik3.txt", tekst2);
	int last_letter=tekst2[tekst2.size() - 1];
	//tekst2.pop_back();

	//bit_par(tekst2, wynik[0]);
	//if ( wynik[0]== last_letter) cout << endl << "zgodny bit parzystosci" << endl;
	//else cout <<endl<< "bledny bit parzystosci" << endl;
	
	//suma_modulo(tekst2, wynik[0]);
	//if (wynik[0] == last_letter) cout << endl << "zgodna suma modulo" << endl;
	//else cout <<endl<< "bledna suma modulo" << endl;

	CRC(tekst2, wynik);
	if (wynik[0] == 0 &&wynik[1]==0) cout << endl << "zgodne CRC" << endl;
	else cout <<endl<< "bledne CRC" << endl;

	
	system("pause");
	return 0;
}