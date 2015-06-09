/*
  -Program i�erisinde pseudo kod kullan�c� taraf�ndan text dosyas�na girilecek komutlard�r.
  -Size verilen ek i�erisinde pseudo_carp, pseudo_fibonacci, pseudo_isaret dosyalar�ndaki 
  kodlar pseudo dosyas� �eklinde code dosyas�n�n yan�na yerle�tirilmedilidir.
  -bellek, tumbellek, etiket program taraf�ndan olu�turulan dosyalard�r.
  -Etiketler i�in virg�l'�n sa� ve sol taraf�nda bo�luk bulunmamal�d�r.
  -Komutlar proje bilgilendirmesindeki gibi yerle�tirilmelidir.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BOOL int
#define INDIS int


void etiketBul();/*Birinci tarama fonksiyonu prototipi*/
BOOL strword(const char *, char*);/*Kelime kar��la�r�ma fonksiyonu prototipi*/ 
int hextodec(char *);/*HEX adresi DEC edrese �eviren fonksiyon prototipi*/
int hextodecch(char);/*Hex karakteri Dec say�ya d�n��t�ren fonksiyon prototipi*/
void temizle();/*M[4096] Bellek g�zlerini 0'layan fonksiyon prototipi*/
void programYukle();/*Program� bellege y�kler*/
INDIS findInstruction(char *);/*Hangi komut oldu�unu ��zen fonksiyon prototipi*/
int findLabel(char *);/*Komutun adres k�sm�n� ��zen fonksiyon*/
void programiCalistir();/*Memory �zerindeki program� �al��t�ran fonksiyon prototipi*/

void Jns(void);/*Jns komutu fonksiyon prototipi*/
void Load(void);/*Load komutu fonksiyon prototipi*/
void Store(void);/*Store komutu fonksiyon prototipi*/
void Add(void);/*Add komutu fonksiyon prototipi*/
void Subt(void);/*Subt komutu fonksiyon prototipi*/
void Input(void);/*Input komutu fonksiyon prototipi*/
void Output(void);/*Output komutu fonksiyon prototipi*/
void Halt(void);/*Halt komutu fonksiyon prototipi*/
void Skipcond(void);/*Skipcond komutu fonksiyon prototipi*/
void Jump(void);/*Jump komutu fonksiyon prototipi*/
void Clear(void);/*Clear komutu fonksiyon prototipi*/
void AddI(void);/*AddI komutu fonksiyon prototipi*/
void JumpI(void);/*JumpI komutu fonksiyon prototipi*/
void StoreI(void);/*StoreI komutu fonksiyon prototipi*/
void LoadI(void);/*LoadI komutu fonksiyon prototipi*/




int AC = 0;/*Accumulator*/
int MAR = 0;/*Memory Address Register*/
int MBR = 0;/*Data Register*/
char IR[5] = {'\0', '\0', '\0', '\0', '\0'};/*Instruction Register*/
int PC = 0;/*Program Counter*/
int OutReg = 0;/*Output Register*/
int InReg = 0;/*Input Register*/
char M[4096][10];/*Memory*/
int LC = 0;/*Location Counter*/
void (*komutfonksiyonlari[15])(void) = {Jns, Load, Store, Add, Subt, Input, Output, Halt, Skipcond, Jump, Clear, AddI, JumpI, StoreI, LoadI};/*Fonksiyonlar�n adreslerini tutan */
char *komutlar[15] = {"Jns", "Load", "Store", "Add", "Subt", "Input", "Output", "Halt", "Skipcond", "Jump", "Clear", "AddI", "JumpI", "StoreI", "LoadI"};/*Komut Seti*/
int opcode[15] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};/*Komut Setinin Opcode kar��l���*/




int main (){

	int secim;

	printf("Lutfen bir secim yapiniz:\n");
	printf("1)Temizle\n");
	printf("2)Program Yukle\n");
	printf("3)Calistir\n");
	printf("4)Cikis\n");
	while (scanf("%d", &secim), !(secim > 0 && secim < 5))
		printf("Gecersiz bir secim yaptiniz!\n");
	
	while (secim != 4){
		switch(secim){
		case 1: temizle(); break;
		case 2: etiketBul(); programYukle(); break;
		case 3: programiCalistir();
		}
		printf("Lutfen bir secim yapiniz:\n");
		while (scanf("%d", &secim), !(secim > 0 && secim < 5))
			printf("Gecersiz bir secim yaptiniz!\n");
	}
	

	printf("AC     : %04x\n", AC);
	printf("MAR    : %03x\n", MAR);
	printf("MBR    : %04x\n", MBR);
	printf("IR     : %s\n", IR);
	printf("PC     : %03x\n", PC);
	printf("OutReg : %04x\n", OutReg);
	printf("InReg  : %04x\n", InReg);

	system("pause");
	return;
}


void etiketBul(){/*Birinci tarama fonksiyonu*/

	FILE *fps;/*Kaynak dosya*/
	FILE *fpd;/*Hedef dosya*/

	char tempsource[20];/*Kaynak buffer*/
	char tempdest[20];/*Hedef buffer*/

	char *tempstart = NULL;/*Ge�ici ba�lang�� adresi tutar*/
	char *tempgetstart = NULL;/*Ge�ici fgets fonksiyonunun geri d�n�� de�eridir*/
	int k = 0;

	if ((fps = fopen("pseudo.txt", "r")) == NULL){/*Kaynak dosya a�ma*/
		printf("Dosya acilamadi...");
		exit(EXIT_FAILURE);
	}

	if ((fpd = fopen("etiket.txt", "w")) == NULL){/*Hedef dosya yaratma*/
		printf("Dosya yaratilamadi...");
		exit(EXIT_FAILURE);
	}

	fprintf(fpd, "Etiket    Adresi\n");

	LC = 0;
	while ((tempgetstart = fgets(tempsource, 20, fps)) != NULL){/*T�m dosya i�erisinde traverse i�lemi*/
		LC++;
		if (strword(tempsource, "END")){
			LC--;
			fclose(fps);
			fclose(fpd);
			return;
		}
		else if (strword(tempsource, "ORG")){
			LC = hextodec(tempsource + 4);
			LC--;
		}
		else if ((tempstart = strchr(tempsource, ',')) != NULL){
			while (tempgetstart != tempstart){
				tempdest[k++] = *tempgetstart++;
			}
			tempdest[k] = '\0';
			k = 0;
			fprintf(fpd, "%s     %x\n",tempdest, LC);
		}
	}

	fclose(fps);
	fclose(fpd);

	return;
}

BOOL strword(char *source, char *find){/*Kelime kar��la�t�rma fonksiyonu*/

	int i,j;

	int sourcelen, findlen;

	sourcelen = strlen(source);
	findlen = strlen(find);


	for (i = 0; i < sourcelen; i++){
		for (j = 0; j < findlen; j++)
			if (*(source + i + j) != *(find + j))
				break;
		if (j == findlen)
			return 1;
	}

	return 0;
}

int hextodec(char *adres){/*HEX adresi DEC edrese �eviren fonksiyon*/

	int adreslen = 0;
	int i = 0;
	int DEC = 0;
	int BASE = 1;

	while (*(adres + i) != '\n' && *(adres + i) != '\0'){
		adreslen++;
		i++;
	}
	
	for (i = 0; i < adreslen; i++){
		if (*(adres - i + adreslen - 1) >= '0' && *(adres - i + adreslen - 1) <= '9'){
			DEC += (*(adres - i + adreslen - 1) - '0') * BASE;
			BASE *= 16;
		}
		else if (*(adres - i + adreslen - 1) >= 'A' && *(adres - i + adreslen - 1) <= 'F'){
			DEC += (*(adres - i + adreslen - 1) - 'A' + 10) * BASE;
			BASE *= 16;
		}
		else if (*(adres - i + adreslen - 1) >= 'a' && *(adres - i + adreslen - 1) <= 'f'){
			DEC += (*(adres - i + adreslen - 1) - 'a' + 10) * BASE;
			BASE *= 16;
		}
	}

		return DEC;
}

int hextodecch(char ch){/*Hex karakteri Dec say�ya d�n��t�ren fonksiyon prototipi*/
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	else if (ch >= 'A' && ch <= 'F')
		return ch - 'A' + 10;
	else if (ch >= 'a' && ch <= 'f')
		return ch - 'a' + 10;
	else
		return -1;
}


void temizle(){/*M[4096] Bellek g�zlerini 0'layan fonksiyon prototipi*/
	int i, j;

	srand(time(0));/*K�k de�er 01.01.1970'e g�re ayarland�*/

	for (i = 0; i < 4096; i++)
		for (j = 0; j < 10; j++){
			if (j == 0)
				itoa(2, M[i], 16);/*Tek basamakl� de�erler belle�e random �ekilde yerle�tirildi.*/
			else
				M[i][j] = '\0';
		}
	return;
}

void programYukle(){/*Program� bellege y�kler*/

	FILE *fps;/*Kaynak dosya*/
	FILE *fpd;/*Hedef Dosya*/

	char tempsource[20];/*Kaynak buffer*/
	char tempdest[20];/*Hedef buffer*/

	char *tempstart = NULL;/*Ge�ici ba�lang�� adresi tutar*/
	char *tempgetstart = NULL;/*Ge�ici fgets fonksiyonunun geri d�n�� de�eridir*/
	char *skiptemp = NULL;/*Skipcode i�in bo�luk tutar*/
	int k = 0;
	int temp = 0;
	int indis = 0;


	if ((fps = fopen("pseudo.txt", "r")) == NULL){/*Kaynak dosya a�ma*/
		printf("Dosya acilamadi...");
		exit(EXIT_FAILURE);
	}

	if ((fpd = fopen("bellek.txt", "w")) == NULL){/*Hedef Dosya yaratma*/
		printf("Dosya yaratilamadi...");
		exit(EXIT_FAILURE);
	}

	LC = 0;
	while ((tempgetstart = fgets(tempsource, 20, fps)) != NULL){
		LC++;
		if (strword(tempsource, "END")){
			LC--;
			fclose(fps);
			fclose(fpd);
			return;
		}
		else if (strword(tempsource, "ORG")){
			LC = hextodec(tempsource + 4);
			LC--;
		}
		else if (tempstart = strchr(tempsource, ',')){/*Etiketleri yerle�tir.*/
			if (strword(tempsource, "DEC")){
				while (*(tempstart + 5  + k) != '\n'){
					tempdest[k] = *(tempstart + 5  + k);
					k++;
				}
				tempdest[k] = '\n';
				k = 0;
				temp = atoi(tempdest);/*Decimal say�ya d�n��t�r�ld�*/
				fprintf(fpd, "%x %x\n", LC, temp);/*Hex kar��l�klar� yaz�ld�*/
			}
			else if (strword(tempsource, "HEX")){
				while (*(tempstart + 5  + k) != '\n'){
					tempdest[k] = *(tempstart + 5  + k);
					k++;
				}
				tempdest[k] = '\n';
				k = 0;
				temp = hextodec(tempdest);/*Decimal kar��l��� al�nd�*/
				fprintf(fpd, "%x %x\n", LC, temp);/*Hex kar��l�klar� yaz�ld�*/
			}
			else {
				goto komut;/*Jump etiketi*/
			}
		}
		else {/*Komut yerle�tirme*/
			komut:
			if ((indis = findInstruction(tempsource)) == 15)
				printf("Komut bulunamad� l�tfen assembly kodunuzu kontrol ediniz.");
			else {
				if (strchr(tempsource, 'I') != NULL){/*Indirect adresleme yap�l�rken kullan�l�r. ��nk� strword I's�z bulur I varsa do�ru opcode �retilir.*/
					if (indis == 3)
						indis = 11;
					else if (indis == 9)
						indis = 12;
					else if (indis == 2)
						indis = 13;
					else if (indis == 1)
						indis = 14;
				}
				if ((temp = findLabel(tempsource)) == 0){/*Etiketin adres kar��l���n� DEC olarak d�ner*/
					if (indis == 8){
						if (*(skiptemp = strchr(tempsource,' ') + 1) == '4')
						fprintf(fpd,"%x %x400\n", LC, opcode[indis]);
						else if(*(skiptemp = strchr(tempsource,' ') + 1) == '8')
						fprintf(fpd,"%x %x800\n", LC, opcode[indis]);
						else if (*(skiptemp = strchr(tempsource,' ') + 1) == '0')
						fprintf(fpd,"%x %x000\n", LC, opcode[indis]);
					}
					else
						fprintf(fpd,"%x %x000\n", LC, opcode[indis]);
				}
				else
					fprintf(fpd,"%x %x%x\n", LC, opcode[indis], temp);
			}
		}
		
	}
	
	fclose(fps);
	fclose(fpd);

	return;
}

INDIS findInstruction(char *source){/*Hangi komut oldu�unu ��zen fonksiyon*/

	int indis = 0;

	for (indis = 0; indis < 15; indis++)/*Burada strword ile komut aramas� yap�ls�n...*/
		if (strword(source, komutlar[indis]))
			return indis;
	return indis;
	
}

int findLabel(char *source){/*Komutun adres k�sm�n� ��zen fonksiyon*/

	FILE *fps;

	char tempsource[20];/*Dosyadan okuma yap�l�r*/
	char templabel[20];/*source label bulma*/

	char *tempgetstart = NULL;/*Adresin ba�lang�c�n� tutar*/
	char *tempspace = NULL;/*Bo�luk karakterinin adresini tutar*/
	char *tempstart = NULL;/*Ge�ici ba�lang�� de�i�keni*/

	int i = 0;

	if ((fps = fopen("etiket.txt", "r")) == NULL){/*Kaynak dosya a�ma*/
		printf("Dosya acilamadi...");
		exit(EXIT_FAILURE);
	}


	if ((tempspace = strchr(source, ' ')) == NULL)
		return 0;

	while (*(tempspace + i + 1) != '\n'){
		templabel[i] = *(tempspace + i + 1);/*Label parametreden diziye okundu*/
		i++;
	}
	templabel[i] = '\0';

	while ((tempgetstart = fgets(tempsource, 20, fps)) != NULL){
		if (strword(tempgetstart, templabel)){
			tempstart = strchr(tempgetstart, ' ');
			return hextodec(tempstart + 5);
		}
	}
	return 0;
}

void programiCalistir(){/*Memory �zerindeki program� �al��t�ran fonksiyon prototipi*/

	FILE *fps;
	FILE *fpf;

	char tempsource[20];
	char address[4] = {'\n', '\n', '\n', '\n'};
	char word[5] = {'\0', '\0', '\0', '\0', '\0'};
	int addressinfo;

	char *tempgetstart = NULL;

	int decodeopcode;
	int k = 0;

	if ((fps = fopen("bellek.txt", "r")) == NULL){/*Kaynak dosya a�ma*/
		printf("Dosya acilamadi...");
		exit(EXIT_FAILURE);
	}


	while ((tempgetstart = fgets(tempsource, 20, fps)) != NULL){/*Memory i�erisine program yerle�tirildi*/
		while (k < 3){/*Adres 3 HEX say� olarak ifade edilir.*/
			address[k] = tempsource[k];
			k++;
		}
		address[k] = '\n';
		k = 0;
		while (tempsource[k + 4] != '\n'){/*Geri kalan HEX say� Komut Format�n� ifade eder.*/
			word[k] = tempsource[k + 4];
			k++;
		}
		word[k] = '\0';
		k = 0 ;

		addressinfo = hextodec(address);

		strcpy(M[addressinfo], word);
		*(M[addressinfo] + 4) = '\0';/*strcopy null karakter '\0' koymaz*/
	}
	

	/*Program �al��maya ba�l�yor*/
	fseek(fps, 0, SEEK_SET);/*Dosyan�n ba��na ge�ildi*/
	fgets(tempsource, 20, fps);/*PC i�in ilk adres okundu*/
	while (k < 3){
			address[k] = tempsource[k];
			k++;
		}
	PC = hextodec(address);
	fclose(fps);/*Dosyadan belle�e okuma yapt�k*/


	while (1){
		MAR = PC;/*FETCH BA�LANGI�*/
		strcpy(IR, M[MAR]);
		PC++;/*FETCH B�T��*/
		decodeopcode = hextodecch(IR[0]);/*DECODE OpCode*/
		if (decodeopcode == 7)
			break;/*Halt komutu*/
		komutfonksiyonlari[decodeopcode]();/*Komut �cras�*/
		
	}

	if ((fpf = fopen("tumbellek.txt", "w")) == NULL){/*Kaynak dosya a�ma*/
		printf("Dosya acilamadi...");
		exit(EXIT_FAILURE);
	}

	for (k = 0; k < 4096; k++)
		fprintf(fps, "%x %s\n", k, M[k]);

	fclose(fpf);

	return;
}

void Jns(void){/*Jns komutu fonksiyon */
	MBR = PC;/*MBR <- PC*/
	MAR = hextodec(&M[MAR][1]);/*MAR <- X*/
	itoa(MBR, M[MAR], 16);/*M[MAR] <- MBR*/
	MBR = MAR;/*MBR <- X*/
	AC = 1;/*AC <- 1*/
	AC += MBR;/*AC <- AC + MBR*/
	PC = AC;/*PC <- AC*/
}

void Load(void){/*Load komutu fonksiyon*/
	MAR = hextodec(&M[MAR][1]);/*MAR <- X*/
	MBR = hextodec(M[MAR]);/*MBR <- M[MAR]*/
	AC = MBR;/*AC <- MBR*/
}

void Store(void){/*Store komutu fonksiyon*/
	MAR = hextodec(&M[MAR][1]);/*MAR <- X*/
	MBR = AC;/*MBR <- AC*/
	itoa(MBR, M[MAR], 16);/*M[MAR] <- MBR*/
}

void Add(void){/*Add komutu fonksiyon*/
	MAR = hextodec(&M[MAR][1]);/*MAR <- X*/
	MBR = hextodec(M[MAR]);/*MBR <- M[MAR]*/
	AC += MBR;/*AC <- AC + MBR*/
}

void Subt(void){/*Subt komutu fonksiyon*/
	MAR = hextodec(&M[MAR][1]);/*MAR <- X*/
	MBR = hextodec(M[MAR]);/*MBR <- M[MAR]*/
	AC -= MBR;/*AC <- AC - MBR*/
}

void Input(void){/*Input komutu fonksiyon*/
	AC = InReg;/*AC <- InReg*/
}

void Output(void){/*Output komutu fonksiyon*/
	OutReg = AC;/*OutReg <- AC*/
}

void Halt(void){/*Halt komutu fonksiyon*/
}

void Skipcond(void){/*Skipcond komutu fonksiyon*/

	int X = hextodecch(IR[1]);/*X <- IR(11-8)*/


	if (X == 0 && AC < 0)/*xxxx (00)00 0000 0000*/
		PC++;
	else if (X == 4 && AC == 0)/*xxxx (01)00 0000 0000*/
		PC++;
	else if (X == 8 && AC > 0)/*xxxx (10)00 0000 0000*/
		PC++;
	else 
		return;
}

void Jump(void){/*Jump komutu fonksiyon*/
	PC = hextodec(&IR[1]);/*PC <- IR(11-0)*/
}

void Clear(void){/*Clear komutu fonksiyon*/
	AC = 0;/*AC <- 0*/
}

void AddI(void){/*AddI komutu fonksiyon*/
	MAR = hextodec(&M[MAR][1]);/*MAR <- X*/
	MBR = hextodec(M[MAR]);/*MBR <- M[MAR]*/
	MAR = MBR;/*MAR <- MBR*/
	MBR = hextodec(M[MAR]);/*MBR <- M[MAR]*/
	AC += MBR;/*AC <- AC + MBR*/
}

void JumpI(void){/*JumpI komutu fonksiyon*/
	MAR = hextodec(&M[MAR][1]);/*MAR <- X*/
	MBR = hextodec(M[MAR]);/*MBR <- M[MAR]*/
	PC = MBR;/*PC <- MBR*/
}


void StoreI(void){/*StoreI komutu fonksiyonu*/
	MAR = hextodec(&M[MAR][1]);/*MAR <- X*/
	MBR = hextodec(M[MAR]);/*MBR <- M[MAR]*/
	MAR = MBR;/*MAR <- MBR*/
	MBR = AC;/*MBR <- AC*/
	itoa(MBR, M[MAR], 16);/*M[MAR] <- MBR*/
}

void LoadI(void){/*LoadI komutu fonksiyonu*/
	MAR = hextodec(&M[MAR][1]);/*MAR <- X*/
	MBR = hextodec(M[MAR]);/*MBR <- M[MAR]*/
	MAR = MBR;/*MAR <- MBR*/
	MBR = hextodec(M[MAR]);/*MBR <- M[MAR]*/
	AC = MBR;/*AC <- MBR*/
}