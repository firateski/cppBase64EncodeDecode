// Base64Converter.cpp : Defines the entry point for the console application.
//
#include "stdafx.h" // Visual Studio ortamında gerekli olan kütüphane
#include <iostream> // input ve output işlemlerini yapmamız için gerekli kütüphane
#include "base64Converter.h" //base64 sınıfımız

base64Converter _base64;

int main()
{
	//Kullanıcıdan aldığımız girdiği tutacak değişken
	string girilenMetin;

	//Kullanıcıya metin girmesini söylüyoruz.
	cout << "Base64 turune dondurmek istediginiz metni giriniz:" << endl;

	//Kullanıcının girdiği metni 'girilenMetin' ismindeki değişkene aktarıyoruz.
	getline(cin, girilenMetin);
	
	//base64 türündeki kodlanmış verimizi almak için encodeString() fonksiyonumuzu çağırıyoruz.
	cout << ">>> Encode Edilen Metin: " << _base64.encodeString(girilenMetin) << endl << endl;

	// Üst kısımda kullanıcıdan metin aldık ve bunu Base64 Kodlanmış halini gösterdik.
	// Alt kısımda ise Base64 kodlanmış veriyi isteyip bunun kodunun çözülmüş halini gösterdik.

	//Kullanıcıya metin girmesini söylüyoruz.
	cout << "Base64 turundeki metni giriniz:" << endl;

	//Kullanıcının girdiği metni 'girilenMetin' ismindeki değişkene aktarıyoruz.
	getline(cin, girilenMetin);
	
	//base64 türündeki verimizi almak için encodeString() fonksiyonumuzu çağırıyoruz.
	cout << ">>> Decode Edilen Metin: " << _base64.decodeString(girilenMetin) << endl << endl;

	main();

	return 0;
}