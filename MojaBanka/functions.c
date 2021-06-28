#include "functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#define LOZINKA_N 9
void unos(void);

static int brojKorisnika = 0;
static KORISNIK* trazeni;
static KORISNIK* poljeKorisnika = NULL;
static TECAJ tecaj = { 0 };
static int korisnik_id;

void izbornik(const char* nazivDatoteke) {

	FILE* dat = fopen(nazivDatoteke, "rb");
	if (dat == NULL) {
		dat = fopen(nazivDatoteke, "wb");
	}
	fclose(dat);

	int ret;
	char lozinka[9];
	char trazeniIban[22];
	char* placanja = "placanja.txt";

	int opcija;
	while (1) {
		printf("__________________________________\n");
		printf("Dobrodosli u glavni izbornik banke\n");
		printf("Odaberite jednu od ponudenih opcija:\n");
		printf("1. Kreiranje korisnika\n");
		printf("2. Dodavanje racuna\n");
		printf("3. Upravljanje racunima\n");
		printf("4. Dodavanje/ispis tecajne liste\n");
		printf("5. Brisanje tecajne liste\n");
		printf("6. Ispis svih korisnika\n");
		printf("7. Pretraga\n");
		printf("8. Brisanje korisnika\n");
		printf("9. Izlazak iz programa\n");
		printf("10. Klijent sucelje\n");
		printf("11. Ispis svih transakcija\n");
		printf("12. Sortiranje imena korisnika\n");
		printf("Odabir: ");



		scanf("%d", &opcija);
		switch (opcija) {
		case 1:
			kreiranjeKorisnika(nazivDatoteke);
			break;
		case 2:
			trazeni = pretragaKorisnika(nazivDatoteke);
			if (trazeni == NULL) {
				printf("Korisnik nije pronaden.\n");
				break;
			}
			dodajRacun(trazeni, nazivDatoteke);
			break;
		case 3:
			printf("Unesite IBAN racuna kojim zelite upravljati: ");
			scanf("%21s", trazeniIban);
			pretragaPoIbanu(nazivDatoteke, trazeniIban);
			break;
		case 4:
			upravljanjeTecajnomListom();
			break;
		case 5:
			ret = brisanjeTecajneListe();
			if (ret == -1) {
				printf("Dogodila se pogreska prilikom brisanja datoteke, postoji li uopce?\n");
			}
			else if (ret == -2) {
				break;
			} else {
				printf("Uspjesno obrisana tecajna lista.\n");
			}
			break;
		case 6:
			ispisSvihKorisnika(nazivDatoteke);
			break;
		case 7:
			trazeni = pretragaKorisnika(nazivDatoteke);
			if (trazeni == NULL) {
				printf("Korisnik nije pronaden.\n");
				break;
			}
			break;
		case 8:
			printf("BRISANJE KORISNIKA\n");
			trazeni = pretragaKorisnika(nazivDatoteke);
			if (trazeni == NULL) {
				printf("Korisnik nije pronaden.\n");
				break;
			}
			brisanjeKorisnika(trazeni, nazivDatoteke);
			break;
		case 9:
			if (poljeKorisnika != NULL) {
				free(poljeKorisnika);
				poljeKorisnika = NULL;
			}
			exit(EXIT_SUCCESS);
			break;

		case 10:
			// Korisnik se prijavljuje pomocu ID-a ili OIB-a i lozinke
			trazeni = pretragaKorisnika(nazivDatoteke);
			if (trazeni == NULL) {
				printf("Korisnik nije pronaden.\n");
				break;
			}
			printf("Unesite Vasu lozinku: ");
			int i;
			for (i = 0; i < LOZINKA_N - 1; i++) {
				lozinka[i] = _getch();

				printf("*");
			}
			lozinka[i] = '\0';
			printf("\n");
			if (strcmp(trazeni->lozinka, lozinka) == 0) {
				printf("Uspjesna prijava!\n");
				klijentSucelje(trazeni, nazivDatoteke);
			}
			else {
				printf("Neuspjeh.\n");
			}
			break;
		case 11:
			// ISPIS SVIH TRANSAKCIJA I UZLAZNO SORTIRANJE POJEDINOG IZNOSA TRANSAKCIJE - SELECTION SORT
			ispisSvihTransakcija(placanja);
			break;
		case 12:
			sortiranjeImenaKorisnika(nazivDatoteke);
			break;
		case 13:
			unos();
			break;
		default:
			printf("Unijeli ste opciju koja ne postoji.\n");
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

	KORISNIK korisnici = { 0 };

	int id;

	fread(&brojKorisnika, sizeof(int), 1, dat);
	fread(&id, sizeof(int), 1, dat);
	fclose(dat);

	dat = fopen(nazivDatoteke, "rb+");
	if (dat == NULL) {
		perror("Kreiranje korisnika");
		return;
	}

	brojKorisnika++;

	if (id < brojKorisnika) {
		id = brojKorisnika;
	}
	else {
		id++;
	}

	korisnici.id = id;
	korisnici.broj_racuna = 0;
	
	printf("Unesite ime korisnika: \n");
	scanf("%24s", korisnici.ime);
	printf("Unesite prezime korisnika: \n");
	scanf("%24s", korisnici.prezime);
	getchar();
	printf("Unesite datum rodenja korisnika (dd.mm.gggg.): \n");
	scanf("%hd.%hd.%hd.", &korisnici.datum_rodenja.dan, &korisnici.datum_rodenja.mjesec, &korisnici.datum_rodenja.godina);
	printf("Unesite adresu korisnika: \n");
	getchar();
	fgets(korisnici.adresa, sizeof(korisnici.adresa), stdin);
	printf("Unesite postanski broj adrese korisnika: \n");
	scanf("%5s", korisnici.postanski_broj);
	printf("Unesite prebivaliste korisnika: \n");
	getchar();
	fgets(korisnici.prebivaliste, sizeof(korisnici.prebivaliste), stdin);
	printf("Unesite OIB korisnika: \n");
	fgets(korisnici.oib, sizeof(korisnici.oib), stdin);

	char* lozinka = generiranjeLozinke();
	strcpy(korisnici.lozinka, lozinka);

	free(lozinka);
	lozinka = NULL;

	printf("\nSAMO JEDNOM CE SE PRIKAZATI!\nLozinka za prijavu klijenta u sustav: %s\n", korisnici.lozinka);

	fwrite(&brojKorisnika, sizeof(int), 1, dat);
	fwrite(&id, sizeof(int), 1, dat);
	fseek(dat, 2 * sizeof(int) + sizeof(KORISNIK) * brojKorisnika - sizeof(KORISNIK), SEEK_SET);
	fwrite(&korisnici, sizeof(KORISNIK), 1, dat);
	fclose(dat);



}

KORISNIK* pretragaKorisnika(const char* nazivDatoteke) {
	poljeKorisnika = ucitajKorisnike(nazivDatoteke);
	if (poljeKorisnika == NULL) {
		printf("Nema korisnika.");
		return NULL;
	}


	int odabir, i, id;
	char oib[12];
	printf("Upisite broj po kojem zelite pretraziti korisnika:\n1) ID-u\n2) OIB-u\nOdabir: ");
	scanf("%d", &odabir);
	switch (odabir) {
	case 1:
		printf("Unesite ID korisnika: \n");
		scanf("%d", &id);
		for (i = 0; i < brojKorisnika; i++) {
			if ((poljeKorisnika + i)->id == id) {
				return poljeKorisnika + i;
			}
		}
		break;
	case 2:
		printf("Unesite OIB korisnika: \n");
		scanf("%11s", oib);
		for (i = 0; i < brojKorisnika; i++) {
			if (strcmp((poljeKorisnika + i)->oib, oib) == 0) {
				return poljeKorisnika + i;
			}
		}
		break;
	default:
		printf("Opcija nije pronadena.");
		return NULL;
	}

	return NULL;

}

KORISNIK* ucitajKorisnike(const char* nazivDatoteke) {
	FILE* dat = fopen(nazivDatoteke, "rb");
	if (dat == NULL) {
		perror("Ucitavanje korisnika");
		return NULL;
	}
	fread(&brojKorisnika, sizeof(int), 1, dat);
	poljeKorisnika = (KORISNIK*)calloc(brojKorisnika, sizeof(KORISNIK));
	if (poljeKorisnika == NULL) {
		perror("Zauzimanje memorije za korisnike");
		return NULL;
	}
	fseek(dat, 2 * sizeof(int), SEEK_SET);
	fread(poljeKorisnika, sizeof(KORISNIK), brojKorisnika, dat);
	if (poljeKorisnika == NULL) {
		printf("Nema korisnika");
		return NULL;
	}
	/*int i;
	for (i = 0; i < brojKorisnika; i++) {
	}*/

	fclose(dat);
	return poljeKorisnika;
}

void ispisSvihKorisnika(const char* nazivDatoteke) {
	poljeKorisnika = ucitajKorisnike(nazivDatoteke);
	if (poljeKorisnika == NULL) {
		perror("Zauzimanje memorije za korisnike");
		return;
	}
	int i, j;
	for (i = 0; i < brojKorisnika; i++) {
		printf("Ime: %s  Prezime: %s  OIB: %s  Datum: %d.%d.%d. Broj racuna: %d ID: %d\n", (poljeKorisnika + i)->ime, (poljeKorisnika + i)->prezime, (poljeKorisnika + i)->oib, (poljeKorisnika + i)->datum_rodenja.dan, (poljeKorisnika + i)->datum_rodenja.mjesec, (poljeKorisnika + i)->datum_rodenja.godina, (poljeKorisnika + i)->broj_racuna, (poljeKorisnika + i)->id);
		for (j = 0; j < (poljeKorisnika + i)->broj_racuna; j++) {
			printf("IBAN: %s\nSaldo: %.2lf\n", (poljeKorisnika + i)->trasankc_racuni[j].iban, (poljeKorisnika + i)->trasankc_racuni[j].saldo);
		}
	}
}

void brisanjeKorisnika(KORISNIK* trazeniKorisnik, const char* nazivDatoteke) {
	printf("Jeste li sigurni? (Upisite \"da\" za potvrdu): ");
	char potvrda[3];
	scanf("%2s", potvrda);
	if (strcmp(potvrda, "da") != 0) {
		return;
	}


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

	fseek(dat, 2 * sizeof(int), SEEK_SET);
	int i, f = -1;
	int noviBrojacKorisnika = 0;
	for (i = 0; i < brojKorisnika; i++)
	{
		//DEBUG printf("Trazeni: %d Petlja: %d", trazeniKorisnik->id, (poljeKorisnika + i)->id);
		if ((poljeKorisnika + i)->id != trazeniKorisnik->id) {
			f = 1;
		}
		if ((poljeKorisnika + i)->id != trazeniKorisnik->id) {
			fwrite((poljeKorisnika + i), sizeof(KORISNIK), 1, dat);
			noviBrojacKorisnika++;
		}
	}
	rewind(dat);
	fwrite(&noviBrojacKorisnika, sizeof(int), 1, dat);
	fclose(dat);
	if (f == 1) {
		printf("Korisnik je uspjesno obrisan.\n");
	}
	else {
		printf("Korisnik nije pronaden.\n");
	}
	trazeniKorisnik = NULL;
}

void dodajRacun(KORISNIK* trazeniKorisnik, const char* nazivDatoteke) {
	printf("Odaberite koji tip racuna zelite otvoriti klijentu: \n1) Ziro racun\n2) Tekuci racun\n3) Devizni racun\nOdabir: ");
	int pozicija = 0, i;

	for (i = 0; i < brojKorisnika; i++) {
		if ((poljeKorisnika + i)->id == trazeniKorisnik->id) {
			break;
		}
		pozicija++;
	}

	FILE* dat = fopen(nazivDatoteke, "rb+");
	if (dat == NULL) {
		perror("Dodavanje racuna");
		return;
	}


	int odabir;
	int broj_racuna = trazeniKorisnik->broj_racuna;
	double depozit;
	scanf("%d", &odabir);
	getchar();
	printf("Je li korisnik uplatio depozit? Ako je, unesite iznos, inace upisite '0': ");
	scanf("%lf", &depozit);
	switch (odabir) {
	case 1:
		trazeniKorisnik->trasankc_racuni[broj_racuna].tip_racuna = 1;
		break;
	case 2:
		trazeniKorisnik->trasankc_racuni[broj_racuna].tip_racuna = 2;
		break;
	case 3:
		trazeniKorisnik->trasankc_racuni[broj_racuna].tip_racuna = 3;
		break;
	default:
		printf("Opcija ne postoji.");
		break;
	}

	generiranjeIbana(trazeniKorisnik, broj_racuna);
	trazeniKorisnik->broj_racuna++;
	trazeniKorisnik->trasankc_racuni[broj_racuna].saldo = depozit;
	fseek(dat, 2 * sizeof(int) + sizeof(KORISNIK) * pozicija, SEEK_SET);
	fwrite(trazeniKorisnik, sizeof(KORISNIK), 1, dat);

	fclose(dat);
}

void pretragaPoIbanu(const char* nazivDatoteke, char* trazeniIban) {
	poljeKorisnika = ucitajKorisnike(nazivDatoteke);
	if (poljeKorisnika == NULL) {
		printf("Nema korisnika.");
		return;
	}

	int i, j, f = -1, pozicija = 0;

	for (i = 0; i < brojKorisnika; i++) {
		pozicija = 0;
		for (j = 0; j < (poljeKorisnika + i)->broj_racuna; j++) {
			if (strcmp((poljeKorisnika + i)->trasankc_racuni[j].iban, trazeniIban) == 0) {
				f = 1;
				upravljajRacunom((poljeKorisnika + i), pozicija, nazivDatoteke);
			}
			pozicija++;
		}
	}
	if (f == -1) {
		printf("IBAN nije pronaden.\n");
		return;
	}
}

void upravljajRacunom(KORISNIK* trazeniKorisnik, int pozicija, const char* nazivDatoteke) {
	printf("Sto zelite promijeniti?\n1) Tip racuna\n2) Saldo\n3) Obrisi racun\nOdabir: ");
	int odabir;
	int tip_racuna;
	double saldo;
	int i, f = -1;
	char potvrda[3];
	scanf("%d", &odabir);

	switch (odabir) {
	case 1:
		printf("U koji tip racuna zelite promijeniti ovaj IBAN? ");
		scanf("%d", &tip_racuna);
		trazeniKorisnik->trasankc_racuni[pozicija].tip_racuna = tip_racuna;
		break;
	case 2:
		printf("Unesite novi saldo ovog racuna: ");
		scanf("%lf", &saldo);
		trazeniKorisnik->trasankc_racuni[pozicija].saldo = saldo;
		break;
	case 3:
		printf("Jeste li sigurni?\nda/ne: ");
		scanf("%2s", potvrda);
		if (strcmp(potvrda, "da") == 0) {
			for (i = 0; i < trazeniKorisnik->broj_racuna; i++) {
				if (i == pozicija) {
					// Ako iduci racun nije prazan, prebaci taj u trenutni
					if (trazeniKorisnik->trasankc_racuni[i + 1].iban != NULL) {
						trazeniKorisnik->trasankc_racuni[i].saldo = trazeniKorisnik->trasankc_racuni[i + 1].saldo;
						trazeniKorisnik->trasankc_racuni[i].tip_racuna = trazeniKorisnik->trasankc_racuni[i + 1].tip_racuna;
						strcpy(trazeniKorisnik->trasankc_racuni[i].iban, trazeniKorisnik->trasankc_racuni[i + 1].iban);
						trazeniKorisnik->broj_racuna--;
						// Postavi zastavicu koja prepisuje sad sve buduce vrijednosti za +1
						f = 1;
						continue;
					}
					// Inace samo stavi sve vrijednosti na 0
					else {
						trazeniKorisnik->trasankc_racuni[i].saldo = 0;
						trazeniKorisnik->trasankc_racuni[i].tip_racuna = 0;
						strcpy(trazeniKorisnik->trasankc_racuni[i].iban, "");
						trazeniKorisnik->broj_racuna--;
						// Postavi zastavicu koja prepisuje sad sve buduce vrijednosti za +1
						f = 1;
						continue;
					}
				}
				if (f == 1) {
					if (trazeniKorisnik->trasankc_racuni[i + 1].iban != NULL) {
						trazeniKorisnik->trasankc_racuni[i].saldo = trazeniKorisnik->trasankc_racuni[i + 1].saldo;
						trazeniKorisnik->trasankc_racuni[i].tip_racuna = trazeniKorisnik->trasankc_racuni[i + 1].tip_racuna;
						strcpy(trazeniKorisnik->trasankc_racuni[i].iban, trazeniKorisnik->trasankc_racuni[i + 1].iban);
						trazeniKorisnik->broj_racuna--;
					}
					else {
						trazeniKorisnik->trasankc_racuni[i].saldo = 0;
						trazeniKorisnik->trasankc_racuni[i].tip_racuna = 0;
						strcpy(trazeniKorisnik->trasankc_racuni[i].iban, "");
						trazeniKorisnik->broj_racuna--;
						f = 1;
						continue;
					}
				}
			}
		}
	default:
		printf("Opcija ne postoji.");
		break;
	}

	int pozicija_korisnika = 0;

	for (i = 0; i < brojKorisnika; i++) {
		if ((poljeKorisnika + i)->id == trazeniKorisnik->id) {
			break;
		}
		pozicija_korisnika++;
	}

	FILE* dat = fopen(nazivDatoteke, "rb+");
	if (dat == NULL) {
		perror("Upravljanje racunom");
		return;
	}


	fseek(dat, 2 * sizeof(int) + sizeof(KORISNIK) * pozicija_korisnika, SEEK_SET);
	fwrite(trazeniKorisnik, sizeof(KORISNIK), 1, dat);
	fclose(dat);


}

void generiranjeIbana(KORISNIK *trazeniKorisnik, int broj_racuna) {
	char iban_pocetna[] = { 'H','R', '8', '9', '2', '4', '8', '4', '0', '0', '8' };
	trazeniKorisnik->trasankc_racuni[broj_racuna].iban[21] = '\0';
	int i;

	for (i = 0; i < 11; i++) {
		trazeniKorisnik->trasankc_racuni[broj_racuna].iban[i] = iban_pocetna[i];
	}

	for (i = 11; i < 21; i++) {
		trazeniKorisnik->trasankc_racuni[broj_racuna].iban[i] = 48 + (float)rand() / RAND_MAX * (57 - 48);
	}

	return;
}

void upravljanjeTecajnomListom(void) {
	FILE* tecaj_dat = fopen("tecaj.bin", "rb+");
	int f = -1;
	if (tecaj_dat == NULL) {
		tecaj_dat = fopen("tecaj.bin", "wb");
		if (tecaj_dat == NULL) {
			perror("Otvaranje tecajne liste");
			return;
		}
	}

	fread(&tecaj, sizeof(TECAJ), 1, tecaj_dat);
	if (tecaj.aud == 0.00) {
		printf("Unesite srednji tecaj za Australski dolar: ");
		scanf("%f", &tecaj.aud);
		printf("Unesite srednji tecaj za Americki dolar: ");
		scanf("%f", &tecaj.usd);
		printf("Unesite srednji tecaj za Europski eur: ");
		scanf("%f", &tecaj.eur);
		printf("Unesite srednji tecaj za Svicarski franak: ");
		scanf("%f", &tecaj.chf);
		f = 1;
	}
	if (f == 1) {
		rewind(tecaj_dat);
		fwrite(&tecaj, sizeof(TECAJ), 1, tecaj_dat);
	}


	fclose(tecaj_dat);

	printf("Tecajna lista na danasnji dan: \n");
	printf("1 USD: %.2fHRK\n", tecaj.usd);
	printf("1 EUR: %.2fHRK\n", tecaj.eur);
	printf("1 CHF: %.2fHRK\n", tecaj.chf);
	printf("1 AUD: %.2fHRK\n", tecaj.aud);
	return;
}

int brisanjeTecajneListe(void) {
	char odabir[3];

	printf("Jeste li sigurni? (da/ne)\nOdabir: ");
	scanf("%2s", odabir);
	if (strcmp(odabir, "da") == 0) {
		if (remove("tecaj.bin") == 0) {
			tecaj.aud = 0;
			tecaj.usd = 0;
			tecaj.chf = 0;
			tecaj.eur = 0;
			return 0;
		}
		else {
			return -1;
		}
	}
	else {
		return -2;
	}
}

char* generiranjeLozinke(void) {
	int i;
	char* lozinka = (char*)calloc(LOZINKA_N, sizeof(char));
	if (lozinka == NULL) {
		return NULL;
	}
	int nasumicno;

	for (i = 0; i < LOZINKA_N; i++) {
		if (i == (LOZINKA_N - 1)) {
			*(lozinka + i) = '\0';
			continue;
		}
		nasumicno = 1 + (float)rand() / RAND_MAX * (4 - 1);
		switch (nasumicno) {
		case 1:
			*(lozinka + i) = 48 + (float)rand() / RAND_MAX * (57 - 48);
			break;
		case 2:
			*(lozinka + i) = 65 + (float)rand() / RAND_MAX * (90 - 65);
			break;
		case 3:
			*(lozinka + i) = 97 + (float)rand() / RAND_MAX * (122 - 97);
			break;
		default:
			break;
		}
	}

	return lozinka;
}

void klijentSucelje(KORISNIK* korisnik, const char *nazivDatoteke) {
	int odabir;
	int f = 1;
	while (f != -1) {
		printf("\nKLIJENT SUCELJE\n\nDobrodosli, %s\nUnesite jednu od zeljenih radnji:\n1) Ispis podataka\n2) Promjena lozinke\n3) Transakcije / Placanja\n4) POVRATAK U ADMIN DIO\nOdabir: ", korisnik->ime);
		scanf("%d", &odabir);
		switch (odabir) {
		case 1:
			printf("Ime: %s  Prezime: %s  OIB: %s  Datum: %d.%d.%d. Broj racuna: %d ID: %d\n", korisnik->ime, korisnik->prezime, korisnik->oib, korisnik->datum_rodenja.dan, korisnik->datum_rodenja.mjesec, korisnik->datum_rodenja.godina, korisnik->broj_racuna, korisnik->id);
			break;
		case 2:
			promjenaLozinkeKlijenta(korisnik, nazivDatoteke);
			break;
		case 3:
			upravljanjePlacanjemKlijenta(korisnik, "placanja.txt");
			break;
		case 4:
			f = -1;
			break;
		default:
			printf("Nepoznat odabir.");
			break;
		}
	}
}

void promjenaLozinkeKlijenta(KORISNIK* korisnik, const char* nazivDatoteke) {
	puts(nazivDatoteke);
	printf("Unesite vasu trenutnu lozinku: ");
	char lozinka[LOZINKA_N];
	int i;
	for (i = 0; i < LOZINKA_N - 1; i++) {
		lozinka[i] = _getch();

		printf("*");
	}
	lozinka[i] = '\0';
	printf("\n");

	if (strcmp(korisnik->lozinka, lozinka) == 0) {
		printf("\nUnesite novu lozinku od 8 znakova: ");
		for (i = 0; i < LOZINKA_N - 1; i++) {
			lozinka[i] = _getch();

			printf("*");
		}
		lozinka[i] = '\0';
		strcpy(korisnik->lozinka, lozinka);
	}
	else {
		printf("Neuspjeh.\n");
	}

	int pozicija_korisnika = 0;
	for (i = 0; i < brojKorisnika; i++) {
		if ((poljeKorisnika + i)->id == korisnik->id) {
			break;
		}
		pozicija_korisnika++;
	}

	FILE* dat = fopen(nazivDatoteke, "rb+");
	if (dat == NULL) {
		perror("Upravljanje racunom");
		return;
	}


	fseek(dat, 2 * sizeof(int) + sizeof(KORISNIK) * pozicija_korisnika, SEEK_SET);
	fwrite(korisnik, sizeof(KORISNIK), 1, dat);
	fclose(dat);
	printf("\nUspjesno promijenjena lozinka.\n");
}

void upravljanjePlacanjemKlijenta(KORISNIK* korisnik, const char* placanja) {
	FILE* placanja_dat = fopen(placanja, "r+");
	if (placanja_dat == NULL) {
		placanja_dat = fopen(placanja, "w");
		if (placanja_dat == NULL) {
			perror("Otvaranje datoteke za placanja");
			return;
		}
	}
	fclose(placanja_dat);
	char trazeniIban[22];
	int odabir, pozicija, f;
	printf("1) Isplata na racun\n2) Ispis transakcija\nUnesite zeljenu radnju: ");
	scanf("%d", &odabir);

	switch (odabir) {
	case 1:
		printf("Unesite IBAN racuna kojim zelite izvrsiti isplatu: ");
		scanf("%21s", trazeniIban);
		pozicija = pretragaPoIbanuKlijent(korisnik, trazeniIban);
		if (pozicija >= 0) {
			if ((f = placanjeKlijenta(pozicija, placanja, korisnik, trazeniIban)) == 1) {
				printf("Placanje je uspjesno izvrseno.\n");
				break;
			}
			else {
				printf("Placanje nije uspjesno izvrseno. \n");
				break;
			}
		}
		else {
			break;
		}
		break;
	case 2: 
		ispisTransakcijaKlijent(korisnik, placanja);
		break;
	default: 
		printf("Opcija nije pronadena.\n");
		break;
	}
}


int pretragaPoIbanuKlijent(KORISNIK* korisnik, char* trazeniIban) {

	int i, f = -1, pozicija = 0;

	pozicija = 0;
	for (i = 0; i < korisnik->broj_racuna; i++) {
		if (strcmp(korisnik->trasankc_racuni[i].iban, trazeniIban) == 0) {
			f = 1;
			break;
		}
		pozicija++;
	}
	if (f == -1) {
		printf("IBAN nije pronaden.\n");
		pozicija = -1;
		return pozicija;
	}

	return pozicija;
}

int placanjeKlijenta(int pozicija, const char* placanja, KORISNIK* korisnik, char* trazeniIban) {
	char odredisniIban[22];
	int f = -1;
	double odredisniNovac;

	FILE *placanja_dat = fopen(placanja, "a+");
	if (placanja_dat == NULL) {
		perror("Otvaranje transakcijske datoteke");
		return f;
	}


	printf("IBAN: %s\tSaldo: %lf", korisnik->trasankc_racuni[pozicija].iban, korisnik->trasankc_racuni[pozicija].saldo);

	printf("\nUnesite IBAN kojem zelite izvrsiti uplatu: ");
	scanf("%21s", odredisniIban);

	printf("Koliko novca zelite prebaciti: ");
	scanf("%lf", &odredisniNovac);

	if (odredisniNovac > korisnik->trasankc_racuni[pozicija].saldo) {
		printf("Nemate dovoljan iznos na racunu!");
		return f;
	}
	else {
		korisnik->trasankc_racuni[pozicija].saldo -= odredisniNovac;

		// SADA SPREMAMO VRIJEDNOSTI UMANJENOG SALDA NA KORISNICKI RACUN U DATA.BIN
		int pozicija_korisnika = 0, i;
		for (i = 0; i < brojKorisnika; i++) {
			if ((poljeKorisnika + i)->id == korisnik->id) {
				break;
			}
			pozicija_korisnika++;
		}

		FILE* dat = fopen("data.bin", "rb+");
		if (dat == NULL) {
			perror("Upravljanje racunom");
			f = -1;
			return f;
		}


		fseek(dat, 2 * sizeof(int) + sizeof(KORISNIK) * pozicija_korisnika, SEEK_SET);
		fwrite(korisnik, sizeof(KORISNIK), 1, dat);
		fclose(dat);
		// FORMAT ZA ISPIS U PLACANJA.TXT CE BITI IZVOR - ODREDISTE - IZNOS - ID IZVRSITELJA
		fprintf(placanja_dat, "%s %s %lf %d\n", trazeniIban, odredisniIban, odredisniNovac, korisnik->id);
		fclose(placanja_dat);


		int x = provjeraOdredisnogIbana(odredisniNovac, odredisniIban, "data.bin");
		// ODREDISNI KLIJENT NIJE KLIJENT NASE BANKE AKO JE X RAZLICIT OD 1, AKO JE X JEDAN, USPJESNO SMO IZVRSILI UPLATU NA ODREDISNI IBAN, SADA TREBAMO IZVORNI IBAN UMANJITI ZA IZNOS
		x == 1 ? printf("Klijent nase banke.\n") : printf("Nije klijent nase banke.\n");
		f = 1;
	}
	return f;
}


int provjeraOdredisnogIbana(double iznosUplate, char* trazeniIban, const char * nazivDatoteke) {
	poljeKorisnika = ucitajKorisnike("data.bin");
	if (poljeKorisnika == NULL) {
		printf("Nema korisnika.");
		return -1;
	}

	int i, j, f = -1, pozicija = 0;
	int korisnik_i, korisnik_j;

	for (i = 0; i < brojKorisnika; i++) {
		for (j = 0; j < (poljeKorisnika + i)->broj_racuna; j++) {
			if (strcmp((poljeKorisnika + i)->trasankc_racuni[j].iban, trazeniIban) == 0) {
				f = 1;
				korisnik_i = i;
				korisnik_j = j;
				(poljeKorisnika + korisnik_i)->trasankc_racuni[j].saldo += iznosUplate;
				break;
			}
		}
		if (f == 1) {
			// POSTAVLJANJE ID-A NA PRONADENOG KORISNIKA
			korisnik_id = (poljeKorisnika + korisnik_i)->id;
			// DEBUG printf("Pronasli smo korisnika, ID je %d\n", korisnik_id);
			// DEBUG printf("Saldo: %lf\n", (poljeKorisnika + korisnik_i)->trasankc_racuni[korisnik_j].saldo);
			break;
		}
	}
	if (f == -1) {
		printf("IBAN nije pronaden.\n");
		return f;
	}

	// KORISNIK JE KLIJENT NASE BANKE - SPREMAMO VRIJEDNOST SALDA ODREDISNOG KORISNIKA
	int pozicija_korisnika = 0;
	for (i = 0; i < brojKorisnika; i++) {
		if ((poljeKorisnika + i)->id == korisnik_id) {
			break;
		}
		pozicija_korisnika++;
	}

	KORISNIK* korisnik = (poljeKorisnika + korisnik_i);

	// DEBUG printf("Saldo: %lf\n", korisnik->trasankc_racuni[korisnik_j].saldo);

	FILE* dat = fopen("data.bin", "rb+");
	if (dat == NULL) {
		perror("Upravljanje racunom");
		f = -1;
		return f;
	}


	fseek(dat, 2 * sizeof(int) + sizeof(KORISNIK) * pozicija_korisnika, SEEK_SET);
	fwrite(korisnik, sizeof(KORISNIK), 1, dat);
	fclose(dat);


	return f;

}

void ispisTransakcijaKlijent(KORISNIK* korisnik, const char* placanja) {
	FILE* placanja_dat = fopen(placanja, "r");
	if (placanja_dat == NULL) {
		perror("Ispis transakcija kod klijenta");
		return;
	}
	char izvorniIban[22], odredisniIban[22];
	double iznosUplate;
	int status, id_korisnika;
	while ((status = fscanf(placanja_dat, "%s %s %lf %d\n", izvorniIban, odredisniIban, &iznosUplate, &id_korisnika)) >= 0) {
		if (id_korisnika == korisnik->id) {
			printf("Izvor: %s Odrediste: %s Iznos uplate: %.2lf\n", izvorniIban, odredisniIban, iznosUplate);
		}
	}

	return;
}

void ispisSvihTransakcija(const char* placanja) {
	FILE* placanja_dat = fopen(placanja, "r");
	if (placanja_dat == NULL) {
		perror("Ispis transakcija");
		return;
	}
	char izvorniIban[22], odredisniIban[22];
	double iznosUplate;
	int status, id_korisnika, br_transakcija = 0, i;
	while ((status = fscanf(placanja_dat, "%s %s %lf %d\n", izvorniIban, odredisniIban, &iznosUplate, &id_korisnika)) >= 0) {
		printf("Izvor: %s Odrediste: %s Iznos uplate: %.2lf\tID korisnika: %d\n", izvorniIban, odredisniIban, iznosUplate, id_korisnika);
		br_transakcija++;
	}

	
	double *poljeIznosa = (double*)calloc(br_transakcija, sizeof(double));
	if (poljeIznosa == NULL) {
		exit(EXIT_FAILURE);
	}

	rewind(placanja_dat);
	for (i = 0; i < br_transakcija; i++) {
		fscanf(placanja_dat, "%s %s %lf %d\n", izvorniIban, odredisniIban, (poljeIznosa + i ), &id_korisnika);
	}

	sortiranje(poljeIznosa, br_transakcija, 1);

	printf("Iznosi transakcija sortirani uzlazno: \n");
	for (i = 0; i < br_transakcija; i++) {
		printf("%.2lfHRK\n", *(poljeIznosa + i));
	}

	free(poljeIznosa);
	poljeIznosa = NULL;
	fclose(placanja_dat);

}

void zamjena(double* const veci, double* const manji) {
	double temp = 0;
	temp = *manji;
	*manji = *veci;
	*veci = temp;
}

void unos(void) {
	char znak;
	if ((znak = getchar()) != '\n') {
		unos();
	}
	putchar(znak);
}


void sortiranje(double *polje, const int n, const int mod_upravljanja) {
	int min = -1;
	for (int i = 0; i < n - 1; i++)
	{
		min = i;
		for (int j = i + 1; j < n; j++)
		{
			if (polje[j] < polje[min]) {
				min = j;
			}
		}
		zamjena(&polje[i], &polje[min]);
	}
}

void sortiranjeImenaKorisnika(const char * nazivDatoteke) {
	poljeKorisnika = ucitajKorisnike(nazivDatoteke);
	KORISNIK temp = { 0 };
	strcpy(temp.ime, "A");
	if (poljeKorisnika == NULL) {
		perror("Zauzimanje memorije za korisnike");
		return;
	}
	int i, j;


	// PRIMJENA BUBBLE SORT ALGORITMA ZA SORTIRANJE, USPOREDUJU SE SUSJEDNA POLJA
	for (i = 0; i < brojKorisnika - 1; i++) {
		for (j = i + 1; j < brojKorisnika; j++) {
			if (_strcmpi((poljeKorisnika + i)->ime, (poljeKorisnika + j)->ime) > 0) {
				temp = poljeKorisnika[i];
				poljeKorisnika[i] = poljeKorisnika[j];
				poljeKorisnika[j] = temp;
			}
		}
	}


	for (i = 0; i < brojKorisnika; i++) {
		printf("%s\n", (poljeKorisnika + i)->ime);
	}
}