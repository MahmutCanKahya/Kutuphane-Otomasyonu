// KutuphaneOtomasyonu.cpp: Konsol uygulamasının giriş noktasını tanımlar.
//
#include "stdafx.h"
#include "Kitaplar.h"
#include "Hash.h"

int main()
{
	Hash kitap;// hasteki fonksiyonlara ulasmak için nesne olusturdum.
	//kitap.hashTablosuOlustur();
	//kitap.hashTabloDoldur();
	bool cıkıs = true;
	while (cıkıs) {
		cout << "\t\t*****************************Kutuphane Sistemi***************************** " 
			<< endl << "1.Kitap Ekle " << endl<< "2.Kitap Ara" << endl 
			<< "3.Kitap Sil" << endl << "4.Ortalama Sorgu Sayisi" << endl << "0.Cikis" << endl;
		int input;
		cin >> input;
		switch (input)
		{
		case 1: {
			bool kontrol = true;
			char a[100];
			char b[100];
			char c[100];
			char d[100];
			cout << "ISBN giriniz:";
			cin >> a;
			while (kontrol)
			{
				if (kitap.yaziliKarakterSayisi(a) == 10) {
					kontrol = false;
				}else {
					cout << "Lutfen 10 haneli bir ISBN numarasi giriniz!" << endl;
					cin >> a;
				}
			}	
			cout << "Kitap Adini giriniz:";
			cin >> b;
			cout << "Yazar Adini giriniz:";
			cin >> c;
			cout << "Fiyat giriniz:";
			cin >> d;
			Kitaplar kayit(a, b, c, d);
			if (kitap.tabloVeriEkle(kayit)==true) {
				cout << "***************" << endl << "Kayit basariyla eklenmistir" << endl << "***************" << endl;
			}else {
				cout << "***************" << endl << "Packing Factor %80 in ustunde veya kitap sistemde zaten var !!!" 
					<< endl << "***************" << endl;
			}
		}	
				break;
		case 2: {

			cout << "Bulunacak kaydin ISBN ini giriniz:" << endl;
			char bulunacakKayit[100];
			cin >> bulunacakKayit;
			bool x = kitap.isbnGoreSorgu(bulunacakKayit);
			if (x == true) {
				cout << "***************" << endl << "Kayit basariyla bulunmustur." << endl << "***************" << endl;
			}else {
				cout << "***************" << endl << "Aranan kayit sistemde bulunmadi." << endl << "***************" << endl;
			}
		}
				break;
		case 3:
		{
			cout << "Silincek kaydin ISBN ini giriniz:" << endl;
			char silincekISBN[100];
			cin >> silincekISBN;
			bool x = kitap.tabloVeriSil(silincekISBN);
			if (x == true) {
				cout << "***************" << endl << "Kayit basariyla silinmistir." << endl << "***************" << endl;
			}
			else {cout << "***************" << endl << "Silinecek kayit sistemde bulunmadi." << endl << "***************" << endl;
			}
		}
				break;
		case 4: {
			fstream dosya("hash.bin", ios::binary | ios::in);
			fstream dosya2("hash.bin", ios::binary | ios::in);
			fstream dosya3("hash.bin", ios::binary | ios::in);
			float x = kitap.ortalamaSorguSayisi(dosya, dosya2, dosya3);
			dosya.close();
			dosya2.close();
			dosya3.close();
			cout << "***************" << endl << "Ortalama Sorgu Sayisi =: " <<x<< endl << "***************" << endl;
		}
			break;
		case 0:
			cıkıs = false;
				break;
		default:
				break;
		}
	}
	return 0;
}