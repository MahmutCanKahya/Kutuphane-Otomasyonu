#pragma once
#include "Kitaplar.h"
class Hash
{
public:
	void hashTablosuOlustur();
	void hashTabloDoldur();
	bool tabloVeriEkle(Kitaplar yeniKayit);
	bool tabloVeriSil(const char * isbno);
	bool isbnGoreSorgu(const char * isbno);
	int yaziliKarakterSayisi(char dizi[]);
	float kitapSayisiBul(fstream & of);
	float ortalamaSorguSayisi(fstream& of, fstream& f, fstream& ff);
};