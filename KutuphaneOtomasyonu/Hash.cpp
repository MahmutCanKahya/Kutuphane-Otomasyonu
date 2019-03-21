#include "stdafx.h"
#include "Hash.h"
#include "Kitaplar.h"

void Hash::hashTablosuOlustur()
{
	fstream hashDosya("hash.bin", ios::binary | ios::out | ios::trunc);
	
	for (int i = 0; i < 205; i++) {		
		Kitaplar("", "", "", "").wr(hashDosya);//41*5lik tabloyu boþ bir kitap nesnesiyle dolduruyoruz 
	}											//Icinde kayit olmayan boþ bir dosya olusuyor
	hashDosya.close();
}
void Hash::hashTabloDoldur()
{
	fstream hashDosya("hash.bin",ios::binary|ios::in|ios::out);
	ifstream dosya("books.txt", ios::in);
	for (int i = 0;  i<205; i++) {
		Kitaplar kayit("", "", "", "");
		kayit.txtOku(dosya);//txt dosyasýnda sira sira kitaplarý okuyor
		long long muhtemelA = atoll(kayit.isbn)%41;//okudugu kitabýn isbn numarasýnýn 41 e göre modunu alýyor ve buluncagý index nosunu gösteriyor
		long long muhtemelDizin = muhtemelA * 2500;// Index numarasýný 2500 le çarparak buluncaðý bayt döndürür 
		hashDosya.seekg(muhtemelDizin,ios::beg);// Göstergeyi dosyanýn baþýndan istenen bayt'a öteleme yapýlýr
		bool bosYerBulundu = false;
		int k;
		while (!bosYerBulundu) {
			Kitaplar kitap[5];
			for (int j = 0; j < 5; j++) {
				kitap[j].rd(hashDosya);//Göstergenin bulundugu bayttan 5 tane kitabý kitap arrayine tanponlar
		
			}
			for (k = 0; k < 5; k++) {
				if (strcmp(kitap[k].isbn,"")==0) {
					bosYerBulundu = true; //Kitap arrayinde eðer boþ yer bulursa boþ yer bulundur true olur
					break;
				}
			}
			if(bosYerBulundu=false){
				muhtemelDizin = muhtemelDizin + 2500;//Boþ yer bulunmassa bi sonraki indexe gösterge ilerletilir
				hashDosya.seekg(muhtemelDizin, ios::beg);
			}
			else
			{
				bosYerBulundu = true;
			}
			
		}
		if (bosYerBulundu) {
			hashDosya.seekp(muhtemelDizin+(k*500),ios::beg);// boþyer bulundugunda eklenecek kayit boþ olan baytta yazdýrýlýr
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
	packingFactor2 = (kitapSayisiBul(dosya)) / 205 * 100;//packing factore gore kitapsayisini toplam kayit sayýsýna böldüm.
	dosya.close();
	if (packingFactor2<=80) {//eðer packing sayýsý %80 üstündeyse ekleme iþlemi yapamýcak
		fstream hashDosya("hash.bin", ios::binary | ios::in | ios::out);
		long long muhtemelA = atoll(yeniKayit.isbn) % 41;//tablo doldurdaki adýmlarýn aynýsý uygulanýyor sadece parametre olarak 
		long long muhtemelDizin = muhtemelA * 2500;		//kitap nesnesi alýyoruz.
		hashDosya.seekg(muhtemelDizin, ios::beg);
		bool bosYerBulundu = false;
		int k;
		while (!bosYerBulundu) {
			Kitaplar kitap[5];
			for (int j = 0; j < 5; j++) {
				kitap[j].rd(hashDosya);
			}
			for (k = 0; k < 5; k++) {
				if (strcmp(yeniKayit.isbn, kitap[k].isbn) == 0) {//kitap kovada varmý diye kontrol eder eðer varsa fonksiyondan cýkar false döndürür direk.
					return false;
				}
				if (strcmp(kitap[k].isbn, "") == 0 || yaziliKarakterSayisi(kitap[k].isbn) != 10) {// eðer boþ yer bulursa veya isbn numarasý 10dan farklýysa boþ yer açýlýr
					bosYerBulundu = true;														//eðer isbn 10dan farklýysa dosyadan veri silinmiþ demektir.
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
		packingFactor = (kitapSayisiBul(hash)) / 205 * 100;//packing factore gore kitapsayisini toplam kayit sayýsýna böldüm.
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
			if (kitap[k]==bos) {//eðer kitap nesnesi boþsa null geldi true döner
				nullGeldi = true;//bunu neden yaptým:Çünki eðerki aranacak sayýyý o kovada bulamassam ve kovada null deðer varsa
				break;			// demekki o kayit hiç eklenmemiþ demektir.Fazla arama yapmamak için böyle bir arama kullandým
			}
			if (strcmp(kitap[k].isbn, isbno) == 0) {//eðerki okunan kitabýn isbn no silincek isbn no na eþitse true döner
				
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
		if (veriBulundu==true) {// eðer kayýt bulunursa true döner ve kaydýn isbnin baþýna mezartaþý(#) koyar, bulunmassa false döner
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
			
			if (kitap[k] == bos) {// silmedeki olayýn aynýsý eðer dosyada arancak þey yoksa ve kovada boþ yer varsa boþuan arama yapmasýný önlicek
				nullGeldi = true;
				break;
			}
			if (strcmp(kitap[k].isbn, isbno) == 0) {//eðer isbni bulursa true dönüyor ve yazdýrýyor
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
	while (dizi[x]!=NULL) {//Bir dizide null a kadar kaç eleman tanýmladýgýmý bana döndüren bir fonksiyon
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
		if (!strcmp(temp.isbn,"")==0 && yaziliKarakterSayisi(temp.isbn)==10) {//kitap sayisini bulan fonksiyon eðer koþullara uyarsa
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
			
			if ((strcmp(temp2.isbn, "") == 1 || strcmp(temp2.isbn, "") == -1) && yaziliKarakterSayisi(temp2.isbn)==10) {// silmedeki olayýn aynýsý eðer dosyada arancak þey yoksa ve kovada boþ yer varsa boþuan arama yapmasýný önlicek
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

						if (kitap[k] == bos) {// silmedeki olayýn aynýsý eðer dosyada arancak þey yoksa ve kovada boþ yer varsa boþuan arama yapmasýný önlicek
							nullGeldi2 = true;
							break;
						}
						if (strcmp(kitap[k].isbn, temp.isbn) == 0) {//eðer isbni bulursa true dönüyor ve yazdýrýyor
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


