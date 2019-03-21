#pragma once
class Kitaplar
{
private:
	
public:
	static const int constLenght = 100;
	char isbn[100];
	char kitapAdi[100];
	char kitapYazari[100];
	char fiyat[100];
	Kitaplar(void);
	Kitaplar(const char *isbno, const char *kitap, const char *yazar, const char *ucret);
	void wr(fstream& of);
	void rd(fstream& inf);
	void txtOku(ifstream& inf);
	friend ostream& operator<<(ostream& out, Kitaplar& k) {// cout ile yazdýrmak için overload ettim
		out << "ISBN = " << k.isbn <<endl<<
			"Kitap Adi  = " << k.kitapAdi<<endl
			<< "Kitap Yazari = " << k.kitapYazari <<endl
			<< "Fiyat = "<< k.fiyat << endl;
		return out;
	}
	friend bool operator==(Kitaplar& k,Kitaplar& l) {// iki nesnenin eþit olduguna bakmak için overload ettim
		if (strcmp(k.isbn, l.isbn) == 0 && strcmp(k.kitapAdi, l.kitapAdi) == 0) {
			return true;
		}
		return false;
	}
};

