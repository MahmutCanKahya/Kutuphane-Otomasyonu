#include "stdafx.h"
#include "Kitaplar.h"

Kitaplar::Kitaplar(void)
{
	strcpy_s(isbn, "");//boþ bir constructor tanýmladým
	strcpy_s(kitapAdi, "");
	strcpy_s(kitapYazari, "");
	strcpy_s(fiyat, "");
}

Kitaplar::Kitaplar(const char * isbno, const char * kitap, const char * yazar, const char * ucret)
{
	strcpy_s(isbn,isbno);// istediðimi parametreleri yazýcagým bir constructor tanýmladým
	strcpy_s(kitapAdi, kitap);
	strcpy_s(kitapYazari, yazar);
	strcpy_s(fiyat, ucret);
}

void Kitaplar::wr(fstream & of)
{
	char bosluk[100];//bir pointer deðiþkenini parametre alarak o deðiþkene yazma yaptýrdým
	of.write(isbn, constLenght);
	of.write(kitapAdi, constLenght);
	of.write(kitapYazari, constLenght);
	of.write(fiyat, constLenght);
	of.write(bosluk, constLenght);

}

void Kitaplar::rd(fstream & inf)
{
	char bosluk[100];//bir pointer deðiþkenini parametre alarak o deðiþkene okuma yaptýrdým
	inf.read(isbn, constLenght);
	inf.read(kitapAdi, constLenght);
	inf.read(kitapYazari, constLenght);
	inf.read(fiyat, constLenght);
	inf.read(bosluk, constLenght);
}
void Kitaplar::txtOku(ifstream & inf)
{
	string temp;// burda txt dosyasýndan verileri okumak için ayrý bir fonksiyon olusturdum
	char bosluk[100];
	getline(inf, temp);
	strcpy_s(isbn,temp.c_str());
	getline(inf, temp);
	strcpy_s(kitapAdi, temp.c_str());
	getline(inf, temp);
	strcpy_s(kitapYazari, temp.c_str());
	getline(inf, temp);
	strcpy_s(fiyat, temp.c_str());
	getline(inf, temp);
	strcpy_s(bosluk, temp.c_str());

}
