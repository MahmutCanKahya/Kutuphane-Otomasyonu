#include "stdafx.h"
#include "Hash.h"
#include "Kitaplar.h"

void Hash::hashTablosuOlustur()
{
	fstream hashDosya("hash.bin", ios::binary | ios::out | ios::trunc);
	
	for (int i = 0; i < 205; i++) {		
		Kitaplar("", "", "", "").wr(hashDosya);//41*5lik tabloyu bo� bir kitap nesnesiyle dolduruyoruz 
	}											//Icinde kayit olmayan bo� bir dosya olusuyor
	hashDosya.close();
}
void Hash::hashTabloDoldur()
{
	fstream hashDosya("hash.bin",ios::binary|ios::in|ios::out);
	ifstream dosya("books.txt", ios::in);
	for (int i = 0;  i<205; i++) {
		Kitaplar kayit("", "", "", "");
		kayit.txtOku(dosya);//txt dosyas�nda sira sira kitaplar� okuyor
		long long muhtemelA = atoll(kayit.isbn)%41;//okudugu kitab�n isbn numaras�n�n 41 e g�re modunu al�yor ve buluncag� index nosunu g�steriyor
		long long muhtemelDizin = muhtemelA * 2500;// Index numaras�n� 2500 le �arparak bulunca�� bayt d�nd�r�r 
		hashDosya.seekg(muhtemelDizin,ios::beg);// G�stergeyi dosyan�n ba��ndan istenen bayt'a �teleme yap�l�r
		bool bosYerBulundu = false;
		int k;
		while (!bosYerBulundu) {
			Kitaplar kitap[5];
			for (int j = 0; j < 5; j++) {
				kitap[j].rd(hashDosya);//G�stergenin bulundugu bayttan 5 tane kitab� kitap arrayine tanponlar
		
			}
			for (k = 0; k < 5; k++) {
				if (strcmp(kitap[k].isbn,"")==0) {
					bosYerBulundu = true; //Kitap arrayinde e�er bo� yer bulursa bo� yer bulundur true olur
					break;
				}
			}
			if(bosYerBulundu=false){
				muhtemelDizin = muhtemelDizin + 2500;//Bo� yer bulunmassa bi sonraki indexe g�sterge ilerletilir
				hashDosya.seekg(muhtemelDizin, ios::beg);
			}
			else
			{
				bosYerBulundu = true;
			}
			
		}
		if (bosYerBulundu) {
			hashDosya.seekp(muhtemelDizin+(k*500),ios::beg);// bo�yer bulundugunda eklenecek kayit bo� olan baytta yazd�r�l�r
			kayit.wr(hashDosya);
		}
	}
	dosya.close();
	hashDosya.close();
}
bool Hash::tabloVeriEkle(Kitaplar yeniKayit)
{	
	fstream dosya("hash.bin", ios::binary | ios::in);
	float packingFactor2;
	packingFactor2 = (kitapSayisiBul(dosya)) / 205 * 100;//packing factore gore kitapsayisini toplam kayit say�s�na b�ld�m.
	dosya.close();
	if (packingFactor2<=80) {//e�er packing say�s� %80 �st�ndeyse ekleme i�lemi yapam�cak
		fstream hashDosya("hash.bin", ios::binary | ios::in | ios::out);
		long long muhtemelA = atoll(yeniKayit.isbn) % 41;//tablo doldurdaki ad�mlar�n ayn�s� uygulan�yor sadece parametre olarak 
		long long muhtemelDizin = muhtemelA * 2500;		//kitap nesnesi al�yoruz.
		hashDosya.seekg(muhtemelDizin, ios::beg);
		bool bosYerBulundu = false;
		int k;
		while (!bosYerBulundu) {
			Kitaplar kitap[5];
			for (int j = 0; j < 5; j++) {
				kitap[j].rd(hashDosya);
			}
			for (k = 0; k < 5; k++) {
				if (strcmp(yeniKayit.isbn, kitap[k].isbn) == 0) {//kitap kovada varm� diye kontrol eder e�er varsa fonksiyondan c�kar false d�nd�r�r direk.
					return false;
				}
				if (strcmp(kitap[k].isbn, "") == 0 || yaziliKarakterSayisi(kitap[k].isbn) != 10) {// e�er bo� yer bulursa veya isbn numaras� 10dan farkl�ysa bo� yer a��l�r
					bosYerBulundu = true;														//e�er isbn 10dan farkl�ysa dosyadan veri silinmi� demektir.
					break;
				}
			}
			if (bosYerBulundu == false) {
				muhtemelDizin = muhtemelDizin + 2500;
				hashDosya.seekg(muhtemelDizin, ios::beg);
			}
			else
			{
				bosYerBulundu = true;
			}
		}
		
		if (bosYerBulundu) {
			hashDosya.seekp(muhtemelDizin + (k * 500), ios::beg);
			yeniKayit.wr(hashDosya);
		}
		hashDosya.close();
		fstream hash("hash.bin", ios::binary | ios::in);
		float packingFactor;
		packingFactor = (kitapSayisiBul(hash)) / 205 * 100;//packing factore gore kitapsayisini toplam kayit say�s�na b�ld�m.
		cout << "%" << packingFactor << endl;
		hash.close();
		return true;
	}
	return false;
}
bool Hash::tabloVeriSil(const char * isbno)
{
	fstream hashDosya("hash.bin", ios::binary | ios::in | ios::out);
	long long muhtemelA = atoll(isbno) % 41;
	long long muhtemelDizin = muhtemelA * 2500;
	hashDosya.seekg(muhtemelDizin, ios::beg);
	bool veriBulundu = false;
	bool nullGeldi = false;
	int k;
	Kitaplar bos("", "", "", "");
	while (!veriBulundu&&!nullGeldi) {
		Kitaplar kitap[5];
		for (int j = 0; j < 5; j++) {

			kitap[j].rd(hashDosya);
			
		}
		for (k = 0; k < 5; k++) {
			if (kitap[k]==bos) {//e�er kitap nesnesi bo�sa null geldi true d�ner
				nullGeldi = true;//bunu neden yapt�m:��nki e�erki aranacak say�y� o kovada bulamassam ve kovada null de�er varsa
				break;			// demekki o kayit hi� eklenmemi� demektir.Fazla arama yapmamak i�in b�yle bir arama kulland�m
			}
			if (strcmp(kitap[k].isbn, isbno) == 0) {//e�erki okunan kitab�n isbn no silincek isbn no na e�itse true d�ner
				
				veriBulundu = true;
				
				break;
			}
		}
		if (veriBulundu == false) {
			muhtemelDizin = muhtemelDizin + 2500;
			hashDosya.seekg(muhtemelDizin, ios::beg);
		}
		else
		{
			veriBulundu = true;
		}
	}
		if (veriBulundu==true) {// e�er kay�t bulunursa true d�ner ve kayd�n isbnin ba��na mezarta��(#) koyar, bulunmassa false d�ner
			char tmp[100] = "#";
			strcat_s(tmp, isbno);
			Kitaplar temp("","","","");
			hashDosya.seekg(muhtemelDizin + (k * 500), ios::beg);
			temp.rd(hashDosya);
			strcpy_s(temp.isbn,tmp);
			hashDosya.seekp(muhtemelDizin + (k * 500), ios::beg);
			temp.wr(hashDosya);
			hashDosya.close();
			return true;
		}
		hashDosya.close();
		return false;
	
	
}
bool Hash::isbnGoreSorgu(const char * isbno)
{
	fstream hashDosya("hash.bin", ios::binary | ios::in | ios::out);
	long long muhtemelA = atoll(isbno) % 41;
	long long muhtemelDizin = muhtemelA * 2500;
	hashDosya.seekg(muhtemelDizin, ios::beg);
	bool veriBulundu = false;
	bool nullGeldi =false;
	int k;
	int sorguSayisi=0;
	Kitaplar bos("","","","");
	Kitaplar kitap[5];
	while (!veriBulundu&&!nullGeldi) {
		sorguSayisi++;
		for (int j = 0; j < 5; j++) {
			kitap[j].rd(hashDosya);
		}
		for (k = 0; k < 5; k++) {
			
			if (kitap[k] == bos) {// silmedeki olay�n ayn�s� e�er dosyada arancak �ey yoksa ve kovada bo� yer varsa bo�uan arama yapmas�n� �nlicek
				nullGeldi = true;
				break;
			}
			if (strcmp(kitap[k].isbn, isbno) == 0) {//e�er isbni bulursa true d�n�yor ve yazd�r�yor
				veriBulundu = true;
				break;
			}
		}
		if (veriBulundu == false) {
			muhtemelDizin = muhtemelDizin + 2500;
			hashDosya.seekg(muhtemelDizin, ios::beg);
		}
		else
		{
			veriBulundu = true;
		}
	}
	if (veriBulundu == true) {
		hashDosya.seekg(muhtemelDizin+(k*500),ios::beg);
		kitap[k].rd(hashDosya);
		cout << kitap[k];
		hashDosya.close();
		cout << "Sorgu Sayisi=:" << sorguSayisi<<endl;
		return true;
	}
	hashDosya.close();
	return false;
}
int Hash::yaziliKarakterSayisi(char dizi[])
{
	int x = 0;
	while (dizi[x]!=NULL) {//Bir dizide null a kadar ka� eleman tan�mlad�g�m� bana d�nd�ren bir fonksiyon
		x++;
	}
	return x;
}
float Hash::kitapSayisiBul(fstream & of)
{
	float x=0;
	Kitaplar temp("", "", "", "");
	while (!of.eof()) {
		temp.rd(of);
		if (!strcmp(temp.isbn,"")==0 && yaziliKarakterSayisi(temp.isbn)==10) {//kitap sayisini bulan fonksiyon e�er ko�ullara uyarsa
			x++;																//yani kayit varsa x 1 artar
		}
		
	}
	return x;
	of.close();
}
float Hash::ortalamaSorguSayisi(fstream & of,fstream& f, fstream& ff)
{
	float sorguTemp=0;
	float sorguToplam = 0;
	bool nullGeldi = false;
	int devam = 0;
	Kitaplar temp2("","","","");
	Kitaplar temp("","","","");
	Kitaplar bos("", "", "", "");
	for (int i = 0; i < 205; i++) {
		if (devam==102500) {
			break;
		}
		of.seekg(devam,ios::beg);
		
			
			temp2.rd(of);
			
			if ((strcmp(temp2.isbn, "") == 1 || strcmp(temp2.isbn, "") == -1) && yaziliKarakterSayisi(temp2.isbn)==10) {// silmedeki olay�n ayn�s� e�er dosyada arancak �ey yoksa ve kovada bo� yer varsa bo�uan arama yapmas�n� �nlicek
					nullGeldi = true;
					strcpy_s(temp.isbn, temp2.isbn);
					strcpy_s(temp.kitapAdi, temp2.kitapAdi);
					strcpy_s(temp.kitapYazari, temp2.kitapYazari);
					strcpy_s(temp.fiyat, temp2.fiyat);
					devam = devam + 500;
					
					
			}
			else {
				devam = devam + 500;
				nullGeldi = false;
			}
				
		
			if (nullGeldi) {
				long long muhtemelA = atoll(temp.isbn) % 41;
				long long muhtemelDizin = muhtemelA * 2500;
				f.seekg(muhtemelDizin, ios::beg);
				Kitaplar kitap[5];
				bool veriBulundu = false, nullGeldi2 = false;
				int k;
				while (!veriBulundu && !nullGeldi2) {
					sorguTemp++;
					for (int j = 0; j < 5; j++) {
						kitap[j].rd(f);
					}
					for (k = 0; k < 5; k++) {

						if (kitap[k] == bos) {// silmedeki olay�n ayn�s� e�er dosyada arancak �ey yoksa ve kovada bo� yer varsa bo�uan arama yapmas�n� �nlicek
							nullGeldi2 = true;
							break;
						}
						if (strcmp(kitap[k].isbn, temp.isbn) == 0) {//e�er isbni bulursa true d�n�yor ve yazd�r�yor
							veriBulundu = true;
							break;
						}
					}
					if (veriBulundu == false) {
						muhtemelDizin = muhtemelDizin + 2500;
						f.seekg(muhtemelDizin, ios::beg);
					}
					else
					{
						veriBulundu = true;
					}
				}
				sorguToplam += sorguTemp;
				sorguTemp = 0;
			}
		
	}
	f.close();
	f.close();
	of.open("hash.bin",ios::binary|ios::in);
	float x = kitapSayisiBul(ff) ;
	
	return sorguToplam/x;
}


