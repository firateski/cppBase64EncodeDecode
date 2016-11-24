// Base64Converter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h" // Visual Studio ortamında gerekli olan kütüphane
#include <iostream> // input ve output işlemlerini yapmamız için gerekli kütüphane
#include <string>   // string değişkenini kullanmak için gerekli kütüphane
#include <math.h>   // Matematiksel işlem fonksiyonları için gereken kütüphane

//cout ve endl gibi komutların başına sürekli std yazmamak için namespace bildirimi yapıyoruz.
using namespace std;

//Encode ve Decode işlemlerinde kullanacağımız bazı ekstra fonksiyonların prototipleri
string decimalToBinary(int, int);
int binaryToDecimal(string);
int getIndexOfBase64Char(char);

//Bunlarda encode ve decode fonksiyonlarının prototipleri
string encodeString(string);
string decodeString(string);

// Bir alt satırdaki Base64 karakter dizisinin uzunluğu
const int lengthOfBase64Chars = 64;

//BASE64 Karakterleri
const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 


int main()
{
	//Kullanıcıdan aldığımız girdiği tutacak değişken
	string girilenMetin;

	//Kullanıcıya metin girmesini söylüyoruz.
	cout << "Base64 turune dondurmek istediginiz metni giriniz:" << endl;

	//Kullanıcının girdiği metni 'girilenMetin' ismindeki değişkene aktarıyoruz.
	getline(cin, girilenMetin);
	
	//base64 türündeki kodlanmış verimizi almak için encodeString() fonksiyonumuzu çağırıyoruz.
	cout << encodeString(girilenMetin) << endl << endl << endl; 

	
	// Üst kısımda kullanıcıdan metin aldık ve bunu Base64 Kodlanmış halini gösterdik.
	// Alt kısımda ise Base64 kodlanmış veriyi isteyip bunun kodunun çözülmüş halini gösterdik.


	//Kullanıcıya metin girmesini söylüyoruz.
	cout << "Base64 turundeki metni giriniz:" << endl << endl;

	//Kullanıcının girdiği metni 'girilenMetin' ismindeki değişkene aktarıyoruz.
	getline(cin, girilenMetin);
	
	//base64 türündeki verimizi almak için encodeString() fonksiyonumuzu çağırıyoruz.
	cout << decodeString(girilenMetin) << endl << endl;

	main();

	return 0;
}

//Veriyi Base64 koduna çeviren fonksiyonumuz
string encodeString(string input) {

	// 1- Değişkenler ve kullanıcıdan verinin alımı

	string girilenMetin = input; //Kullanıcıdan alınan ve Base64 türüne çevrilecek metni tutacağımız değişken

	string binaryData; //girilen metnin ASCII karşılılarının ikili sisteme çevrilmiş halini tutacak değişken

	int requiredExtraByte = 0; //baytların tam bölünmesi için eklenmesi gereken byte sayısını tutacak değişken

	int * asciiKarsiliklari = (int*)malloc(sizeof(int) * girilenMetin.length()); //Girilen metindeki karakterlerin ASCII karşılığını tutmak için girilen metin uzunluğunda int dizisi oluşturuyoruz.
																				 
	
	// 2- her karakterin ASCII karşılığının diziye aktarılması

	for (int i = 0; i < girilenMetin.length(); i++) //metin içindeki harflerin ascii karşılıklarını diziye aktarıyoruz.
	{
		asciiKarsiliklari[i] = girilenMetin[i]; //Karakterin ASCII karşılığını alıyoruz.
	}


	// 3- binary verinin sonuna eklenmesi gereken extra byte sayısının hesaplanması

	//Metin uzunluğu 3'e tam bölünüyorsa extra bayt 0 oluyor.
	//Metin uzunluğu 3'e tam bölünmüyorsa 3 sayısından metin uzunluğunun 3'e bölümünden kalanı çıkarıyoruz. Bu işlemin sonucu bize gereken extra byte sayısını verir.
	requiredExtraByte = girilenMetin.length() % 3 == 0 ? 0 : 3 - girilenMetin.length() % 3;

	// 4- ASCII karşılıklarını binary türüne çevirip binary verimizi oluşturuyoruz.

	//Alttaki döngü ile ilk olarak girilen metnin ASCII karşılıkları binary türüne çevriliyor.
	//Ardından extra byte miktarında istenen kadar sıfır ekliyoruz.
	//Örneğin, 2 tane extra byte gerekiyorsa aşağıdaki fonksiyon ile 0000 0000 0000 0000 verisi ekleniyor.

	for (int i = 0; i < girilenMetin.length() + requiredExtraByte; i++)
	{
		if (girilenMetin.length() > i) {
			binaryData += decimalToBinary(asciiKarsiliklari[i], 8);
		}
		else//alt kısımda extra byte miktarı ekleniyor.
		{
			//2. değişken verinin 8 hanelik olarak döndürülmesini sağlıyor.
			binaryData += decimalToBinary(0, 8);
		}
	}

	// 5- Son aşamada encode edilmiş verimizi oluşturuyoruz ve ekrana yazdırıyoruz.

	string encodedData;

	for (int i = 0; i < binaryData.length(); i += 6) //her altılık Biti okuyacağımız için altışar altışar okuyoruz.
	{
		string get6Digit = binaryData.substr(i, 6); //her altılık Biti alıyoruz.

		int toDecimal = binaryToDecimal(get6Digit); //alınan altlık Bitin onluk karşılığını alıyoruz.

		//Burada verinin içine eklenmiş ekstra Byte miktarı kadar eşittir(=) eklemek için basit bir matematiksel ifade kullandım.
		if (binaryData.length() - (requiredExtraByte * 8) > i)
		{
			//Base64 dizisinden karşılık gelen karakteri verimize ekliyoruz.
			encodedData += base64chars[toDecimal];
		}
		else
		{
			//Verinin sonuna eşittir ekliyoruz.
			encodedData += "=";
		}

	}

	return encodedData;
}

//Base64 kodunu çözen fonksiyonumuz
string decodeString(string input) { // TWE=

	//Kodu çözülmüş veriyi tutacak değişken
	string decodedData = "";

	//Kodu çözerken oluşacak ikili veriyi tutacak değişken
	string binaryData = "";

	//Girilen kodlanmış veri 4'e tam bölünmüyorsa geçerli bir Base64 kodlamsı değildir.
	if (input.length() % 4 != 0) return "!...Girilen Veri Gecerli Base64 Formatinda degil...!";
	
	//Kodlanmış verinin son karakterinin bulunduğu index
	int lastIndexCharOfInputStr = input.length() - 1; 

	//Verinin sadece son 2 karakterini eşittir(=) sembolüne eşit mi diye kontrol ediyoruz.
	//Çünkü kodlanmış Base64 verisinde maksimum 2 tane eşittir(=) sembolü olabilir.
	if (input[lastIndexCharOfInputStr] == '=') {
		
		//Verinin son karakteri silen kod
		input.erase(lastIndexCharOfInputStr, 1);

		//Bir karakter sildik diye verinin son karakterinin bulunduğu indexi 1 azaltıyoruz.
		lastIndexCharOfInputStr--;

		//Verinin ilk haline göre sondan ikinci karakterin eşittir(=) sembolüne eşit mi diye kontrol ediyoruz.
		if (input[lastIndexCharOfInputStr] == '=')
		{
			//Verinin sondan ikinci karakterini silen kod
			input.erase(lastIndexCharOfInputStr, 1);
		}
	}

	// 2- Kodlanmış verideki her karakterin base64 kodlarında denk geldiği indeksin binary verisine çevrilmesi
	for (int i = 0; i < input.length(); i++)
	{
		//Karakterin Base64 dizesinde denk gelen indeksini alıyoruz.
		int index = getIndexOfBase64Char(input[i]);

		//Denk gelen indeksin 6 Bitlik halde binary halini verimize ekliyoruz.
		binaryData += decimalToBinary(index, 6); 
	}


	// 3- Binary verisini 8'lik parçalara ayırıp her parçanın ifade ettiği sayının ASCII karakter karşılığı diziye ekleniyor.

	//Bu döngü veriyi 8'er olarak atlıyor.
	for (int i = 0; i < binaryData.length(); i += 8)
	{
		//Bu kod ile 8 bitlik veri onluk tabana çevrilip ASCII karşılığı olan karakteri buluyoruz.
		char c = binaryToDecimal(binaryData.substr(i, 8));

		//Kodu çözülmüş verinin sonuna bulduğumuz karakteri ekliyoruz.
		decodedData += c;
	}

	return decodedData;
}

//Bu fonksiyon basit bir matematikle İkilik tabandaki veriyi Onluk tabana çevirir.
int binaryToDecimal(string value) {

	int result = 0;

	for (int i = 0; i < value.length(); i++)
	{
		int digit = value[value.length() - i - 1] - '0';

		result = result + (digit * (pow(2, i)));
	}

	return result;
}

//Bu fonksiyon parametre aldığı value değerini 2. parametrede belirtilen formatta ikili veriye çevirir.
string decimalToBinary(int value, int digitFormat) {

	//divisonResult bir nevi temp değişkeni gibi görev alıyor ve ilk değerini value değerine eşitliyoruz.
	int divisionResult = value;

	//Her bölümden kalan bize binary verisinin hanesini verecektir.
	int calculatedDigit = divisionResult % 2;

	//Kalansız bölme işlemi yapıyoruz.
	divisionResult = (divisionResult - (divisionResult % 2)) / 2;

	//bulunan ilk hane binary verisini tutan stringe atanıyor.
	string binaryDataStr = to_string(calculatedDigit);

	//bölme işlemi bölüm değeri sıfıra eşit olana kadar devam ediyor
	while (divisionResult > 0) {
		
		calculatedDigit = divisionResult % 2; 
		divisionResult = (divisionResult - (divisionResult % 2)) / 2;

		//sona doğru giderken bulduğumuz her binary verisinin hanesini stringin başına ekliyoruz.
		binaryDataStr = to_string(calculatedDigit) + binaryDataStr;
	}

	//Bu kod parçası ile verinin kaç Bitlik formatta istenmişse ona göre başına sıfır(0) ekliyoruz.
	if (digitFormat != -1 && binaryDataStr.length() < digitFormat)
	{
		int requiredZeros = digitFormat - binaryDataStr.length();

		for (int i = 0; i < requiredZeros; i++)
		{
			binaryDataStr = "0" + binaryDataStr;
		}
	}

	return binaryDataStr; //string türünde oluşturduğumuz binary verisini döndürüyoruz.
}

//Bu fonksiyon parametre aldığı karakterin Base64 dizesinde denk geldiği indeksi döndürür.
int getIndexOfBase64Char(char c) {

	for (int i = 0; i < lengthOfBase64Chars; i++)
	{
		if (base64chars[i] == c) {
			return i;
		}
	}

	return -1;
}