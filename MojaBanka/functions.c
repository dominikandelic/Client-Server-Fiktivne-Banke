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

	char trazeniIban[22];

	int opcija;
	while (1) {
		printf("__________________________________\n");
		printf("Dobrodosli u glavni izbornik banke\n");
		printf("Odaberite jednu od ponudenih opcija:\n");
		printf("1. Kreiranje korisnika\n");
		printf("2. Dodavanje racuna\n");
		printf("3. Upravljanje racunima\n");
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
		case 2:
			trazeni = pretragaKorisnika(nazivDatoteke);
			dodajRacun(trazeni, nazivDatoteke);
			break;
		case 3:
			printf("Unesite IBAN racuna kojim zelite upravljati: ");
			scanf("%21s", trazeniIban);
			pretragaPoIbanu(nazivDatoteke, trazeniIban);
		case 5:
			ispisSvihKorisnika(nazivDatoteke);
			break;
		case 6:
			trazeni = pretragaKorisnika(nazivDatoteke);
			break;
		case 8:
			printf("BRISANJE KORISNIKA\n");
			trazeni = pretragaKorisnika(nazivDatoteke);
			brisanjeKorisnika(trazeni, nazivDatoteke);
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
	printf("Upisite broj po cemu zelite pretraziti korisnika:\n1) ID-u\n2) OIB-u\nOdabir: ");
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
	fseek(dat, 2 * sizeof(int), SEEK_SET);
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
	printf("Sto zelite promijeniti?\n1) Tip racuna\n2) Saldo\nOdabir: ");
	int odabir;
	int tip_racuna;
	double saldo;
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
	default:
		printf("Opcija ne postoji.");
		break;
	}

	int pozicija_korisnika = 0, i;

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