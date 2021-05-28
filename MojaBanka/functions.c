#include "functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int brojKorisnika = 0;
static KORISNIK* trazeni;
static KORISNIK* poljeKorisnika = NULL;

void izbornik(const char* nazivDatoteke) {
	FILE* dat = fopen(nazivDatoteke, "rb");

	if (dat == NULL) {
		dat = fopen(nazivDatoteke, "wb");
	}
	fclose(dat);



	int opcija;
	while (1) {
		printf("Dobrodosli u glavni izbornik banke\n");
		printf("Odaberite jednu od ponudenih opcija:\n");
		printf("1. Kreiranje korisnika\n");
		printf("5. Ispis svih korisnika\n");
		printf("6. Pretraga\n");
		printf("8. Brisanje korisnika\n");
		printf("9. Izlazak iz programa\n");
		printf("Odabir: ");



		scanf("%d", &opcija);
		switch (opcija) {
		case 1:
			kreiranjeKorisnika(nazivDatoteke);
			break;
		case 5:
			ispisSvihKorisnika(nazivDatoteke);
			break;
		case 6:
			trazeni = pretragaKorisnika(nazivDatoteke);
			break;
		case 8:
			trazeni = pretragaKorisnika(nazivDatoteke);
			brisanjeKorisnika(&trazeni, nazivDatoteke);
			break;
		case 9:
			exit(EXIT_SUCCESS);
			break;
		}
	}
}

void kreiranjeKorisnika(const char* nazivDatoteke) {
	FILE* dat = fopen(nazivDatoteke, "rb");
	if (dat == NULL) {
		perror("Kreiranje korisnika");
		return;
	}

	KORISNIK korisnici = {0};


	fread(&brojKorisnika, sizeof(int), 1, dat);
	fclose(dat);

	dat = fopen(nazivDatoteke, "rb+");
	if (dat == NULL) {
		perror("Kreiranje korisnika");
		return;
	}

	brojKorisnika++;
	korisnici.id = brojKorisnika;

	fwrite(&brojKorisnika, sizeof(int), 1, dat);
	printf("Unesite ime korisnika: \n");
	scanf("%24s", korisnici.ime);
	printf("Unesite prezime korisnika: \n");
	scanf("%24s", korisnici.prezime);
	getchar();
	printf("Unesite adresu korisnika: \n");
	fgets(korisnici.adresa, sizeof(korisnici.adresa), stdin);
	printf("Unesite postanski broj adrese korisnika: \n");
	scanf("%5s", korisnici.postanski_broj);
	printf("Unesite prebivaliste korisnika: \n");
	getchar();
	fgets(korisnici.prebivaliste, sizeof(korisnici.prebivaliste), stdin);

	//printf("\nUnesite datum rodenja korisnika");

	fseek(dat, sizeof(int) + sizeof(KORISNIK) * brojKorisnika - sizeof(KORISNIK), SEEK_SET);
	fwrite(&korisnici, sizeof(KORISNIK), 1, dat);
	fclose(dat);

}



/*void unosImenaPrezime(KORISNIK* korisnici) {
	char pomocnoPolje[50] = { 0 };
	int duljina = 0;
	printf("\nUnesite ime korisnika: ");
	scanf("%49s", pomocnoPolje);
	duljina = strlen(pomocnoPolje);
	korisnici->ime = (char*)calloc(duljina + 1, sizeof(char));
	if (korisnici->ime == NULL) {
		return;
	}
	strcpy(korisnici->ime, pomocnoPolje);

	printf("\nUnesite prezime korisnika: ");
	scanf("%49s", pomocnoPolje);
	duljina = strlen(pomocnoPolje);
	korisnici->prezime = (char*)calloc(duljina + 1, sizeof(char));
	if (korisnici->prezime == NULL) {
		return;
	}
	strcpy(korisnici->prezime, pomocnoPolje);
}*/

/* void unosAdrese(KORISNIK* korisnici) {
	char pomocnoPolje[50] = { 0 };
	int duljina = 0;
	printf("\nUnesite adresu korisnika: ");
	getchar();
	fgets(pomocnoPolje, sizeof(pomocnoPolje), stdin);
	duljina = strlen(pomocnoPolje);
	korisnici->adresa = (char*)calloc(duljina + 1, sizeof(char));
	if (korisnici->adresa == NULL) {
		return;
	}
	strcpy(korisnici->adresa, pomocnoPolje);
} */

/*void unosPrebivalista(KORISNIK* korisnici) {
	char pomocnoPolje[50] = { 0 };
	int duljina = 0;
	printf("\nUnesite mjesto prebivalista korisnika: ");
	scanf("%49s", pomocnoPolje);
	duljina = strlen(pomocnoPolje);
	korisnici.prebivaliste = ;
	if (korisnici->prebivaliste == NULL) {
		return;
	}
	strcpy(korisnici->prebivaliste, pomocnoPolje);
}*/

KORISNIK* pretragaKorisnika(const char* nazivDatoteke) {
	poljeKorisnika = ucitajKorisnike(nazivDatoteke);
	if (poljeKorisnika == NULL) {
		printf("Nema korisnika.");
		return NULL;
	}

	int id; 
	printf("Unesite ID korisnika: \n");
	scanf("%d", &id);

	int i;
	for (i = 0; i < brojKorisnika; i++) {
		if ((poljeKorisnika+i)->id == id) {
			return poljeKorisnika + i;
		}
	}
}

KORISNIK* ucitajKorisnike(const char* nazivDatoteke) {
	FILE* dat = fopen(nazivDatoteke, "rb");
	if (dat == NULL) {
		perror("Ucitavanje korisnika");
		return NULL;
	}
	fread(&brojKorisnika, sizeof(int), 1, dat);
	printf("Broj korisnika: %d\n", brojKorisnika);
	poljeKorisnika = (KORISNIK*)calloc(brojKorisnika, sizeof(KORISNIK));
	if (poljeKorisnika == NULL) {
		perror("Zauzimanje memorije za korisnike");
		return NULL;
	}
	fread(poljeKorisnika, sizeof(KORISNIK), brojKorisnika, dat);
	if (poljeKorisnika == NULL) {
		printf("Nema korisnika");
		return NULL;
	}
	int i;
	for (i = 0; i < brojKorisnika; i++) {

	}

	return poljeKorisnika;
}

void ispisSvihKorisnika(const char* nazivDatoteke) {
	poljeKorisnika = ucitajKorisnike(nazivDatoteke);
	if (poljeKorisnika == NULL) {
		perror("Zauzimanje memorije za korisnike");
		return;
	}
	int i;
	for (i = 0; i < brojKorisnika; i++) {
		printf("Ime: %s\tPrezime: %s\t", (poljeKorisnika + i)->ime, (poljeKorisnika + i)->prezime);
	}
}

void brisanjeKorisnika(KORISNIK** trazeniKorisnik, const char* nazivDatoteke) {
	FILE* dat = fopen(nazivDatoteke, "rb+");
	if (dat == NULL) {
		perror("Brisanje korisnika iz datoteke data.bin");
		exit(EXIT_FAILURE);
	}

	poljeKorisnika = ucitajKorisnike(nazivDatoteke);
	if (poljeKorisnika == NULL) {
		perror("Zauzimanje memorije za korisnike");
		return;
	}

	fseek(dat, sizeof(int), SEEK_SET);
	int i;
	int noviBrojacKorisnika = 0;
	for (i = 0; i < brojKorisnika; i++)
	{
		printf("Trazeni: %p Petlja: %p", *trazeniKorisnik, poljeKorisnika + i);
		if (*trazeniKorisnik != (poljeKorisnika + i)) {
			fwrite((poljeKorisnika + i), sizeof(KORISNIK), 1, dat);
			noviBrojacKorisnika++;
		}
	}
	rewind(dat);
	fwrite(&noviBrojacKorisnika, sizeof(int), 1, dat);
	fclose(dat);
	printf("Korisnik je uspjesno obrisan\n");
	*trazeniKorisnik = NULL;
}