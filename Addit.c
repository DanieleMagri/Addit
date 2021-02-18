#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct prod{
	int 	code;
	char 	name[100];
	float 	price;
	int 	quantity;
	struct prod *nextPtr;
}Product;

typedef Product *ProductPtr;

void menuchoice	(ProductPtr lPtr, size_t totproducts, int thresholdvar);
void modquantity(ProductPtr lPtr, size_t totproducts, int thresholdvar);
void modprice	(ProductPtr lPtr, size_t totproducts, int thresholdvar);
void newproduct	(ProductPtr lPtr, size_t totproducts, int thresholdvar);
void deleteprod	(ProductPtr lPtr, size_t totproducts, int thresholdvar);
void searchprint(ProductPtr lPtr, size_t totproducts, int thresholdvar);
void threshold	(ProductPtr lPtr, size_t totproducts, int thresholdvar);
void exitfun	(ProductPtr lPtr, size_t totproducts, int thresholdvar);

ProductPtr search	(ProductPtr lPtr, int insertcode);
size_t printreverse 	(ProductPtr lPtr, int threshold);
void print			(ProductPtr correntePtr);

int main(void){
	ProductPtr lPtr = NULL;
	FILE *fPtr;
	srand(10);
	size_t totproducts=0;
	int thresholdvar=10;
	
	if ((fPtr = fopen("elencoProdotti.csv", "r")) == NULL){
		puts("Impossibile aprire file");
		exit(EXIT_FAILURE);
	}else{
		while (!feof(fPtr)){
			ProductPtr nuovoPtr = malloc(sizeof(Product));
			fscanf(fPtr, "%d,%[^,],%f\n", &nuovoPtr -> code, nuovoPtr -> name, &nuovoPtr -> price);
			nuovoPtr -> quantity = 0 + rand() % (100000 - 0 + 1);
			nuovoPtr -> nextPtr = NULL;
			
				ProductPtr correntePtr = lPtr;
				ProductPtr precedentePtr = NULL;
				while (correntePtr != NULL && nuovoPtr -> code > correntePtr -> code){
					precedentePtr = correntePtr;
					correntePtr = correntePtr -> nextPtr;
				}
				if (precedentePtr == NULL){
					nuovoPtr -> nextPtr = lPtr;
					lPtr = nuovoPtr;
				}else{
					precedentePtr -> nextPtr = nuovoPtr;
					nuovoPtr -> nextPtr = correntePtr;
				}
			
			totproducts++;
		}
		fclose(fPtr);
		menuchoice(lPtr,totproducts,thresholdvar);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

void threshold	(ProductPtr lPtr, size_t totproducts, int thresholdvar){
	puts("Inserire il valore soglia:");
	while (scanf("%d", &thresholdvar)<1 || thresholdvar <0){
		printf("Errore! Inserire un intero maggiore o uguale a 0.\nInserire nuovamente il valore soglia:\n");
		scanf("%*[^\n]%*c");
	}
	if (printreverse (lPtr, thresholdvar) != 1){
		printf("Non è stato trovato nessun prodotto con quantità minore di %d.\n\n",thresholdvar);
	}
	menuchoice(lPtr,totproducts,thresholdvar);
}


size_t printreverse (ProductPtr lPtr, int threshold){
	if (lPtr == NULL){
		puts("\n");
		return 0;
	}
	ProductPtr nuovoPtr = lPtr;
	if (nuovoPtr -> nextPtr != NULL){
		printreverse(nuovoPtr -> nextPtr, threshold);
	}
	if (nuovoPtr -> quantity < threshold){
		print(nuovoPtr);
		return 1;
	}
	return 0;
}
//----------------------------------------------------------------------------------------------------------------------

void deleteprod (ProductPtr lPtr, size_t totproducts, int thresholdvar){
	size_t insertcode;
	puts("Inserire il codice del prodotto da eliminare:");
	while (scanf("%zd", &insertcode)<1 || insertcode <0){
		printf("Errore! Inserire un intero maggiore o uguale a 0.\nInserire nuovamente il codice del prodotto da eliminare:\n");
		scanf("%*[^\n]%*c");
	}
	if (insertcode == lPtr -> code){
		totproducts--;
		ProductPtr init = lPtr;
		lPtr = init -> nextPtr;
		print(init);
		free(init);
		puts("Prodotto eliminato!\n\n");
		menuchoice (lPtr, totproducts, thresholdvar);
	}else{
		ProductPtr precedentePtr = lPtr;
		ProductPtr correntePtr = lPtr -> nextPtr;
		while (correntePtr != NULL && correntePtr -> code != insertcode){
			precedentePtr = correntePtr;
			correntePtr = correntePtr -> nextPtr;
		}
		if (correntePtr != NULL) {
			totproducts--;
 			ProductPtr tempPtr = correntePtr;
 			precedentePtr -> nextPtr = correntePtr -> nextPtr;
 			print(tempPtr);
 			free(tempPtr);
 			puts("Prodotto eliminato!\n\n");
 			menuchoice (lPtr, totproducts, thresholdvar);
		}else{
			printf("Prodotto non trovato!\n\n");
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void newproduct (ProductPtr lPtr, size_t totproducts, int thresholdvar){
	ProductPtr nuovoPtr = malloc(sizeof(Product));
	nuovoPtr -> nextPtr = NULL;
	
	puts("Inserire il codice del nuovo prodotto:");
	while (scanf("%d", &(nuovoPtr -> code))<1 || (nuovoPtr -> code) <0){
		printf("Errore! Inserire un intero maggiore o uguale a 0.\nInserire nuovamente il codice del nuovo prodotto:\n");
		scanf("%*[^\n]%*c");
	}
	ProductPtr correntePtr = lPtr;
	ProductPtr precedentePtr = NULL;
	
	while (correntePtr -> nextPtr != NULL && nuovoPtr -> code > correntePtr -> code){
		precedentePtr = correntePtr;
		correntePtr = correntePtr -> nextPtr;
	}
	if (nuovoPtr -> code == correntePtr -> code){
		puts("Prodotto già esistente!");
		print(correntePtr);
		free(nuovoPtr);
		return;
	}else if (precedentePtr == NULL){
		nuovoPtr -> nextPtr = lPtr;
		lPtr = nuovoPtr;
	}else{
		precedentePtr -> nextPtr = nuovoPtr;
		nuovoPtr -> nextPtr = correntePtr;
	}
	
	puts("Inserire il nome del nuovo prodotto:");
	while (scanf("%s", (nuovoPtr -> name))<1){
		printf("Errore! Inserire nuovamente il nome del nuovo prodotto:\n");
		scanf("%*[^\n]%*c");
	}
	puts("Inserire il prezzo del nuovo prodotto:");
	while (scanf("%f", &(nuovoPtr -> price))<1 || nuovoPtr -> price <0){
		printf("Errore! Inserire un prezzo maggiore o uguale a 0.\nInserire nuovamente il prezzo del nuovo prodotto:\n");
		scanf("%*[^\n]%*c");
	}
	nuovoPtr -> quantity = 0 + rand() % (100000 - 0 + 1);
	totproducts++;
	print(nuovoPtr);
	menuchoice (lPtr, totproducts, thresholdvar);
}

//----------------------------------------------------------------------------------------------------------------------

void modprice (ProductPtr lPtr, size_t totproducts, int thresholdvar){
	int insertcode;
	puts("Inserire il codice del prodotto:");
	while(scanf ("%d", &insertcode)<1 || insertcode<0){
		printf("Errore! Inserire un intero maggiore o uguale a 0.\nInserire nuovamente il codice del prodotto:\n");
		scanf("%*[^\n]%*c");
	}
	ProductPtr	Product = search (lPtr, insertcode);
	
	if ( Product != NULL ){
		puts ("Inserire il nuovo prezzo:");
		while (scanf("%f", &(Product -> price))<1 || (Product -> price)<0){
			printf("Errore! Inserire un prezzo maggiore o uguale a 0.\nInserire nuovamente il nuovo prezzo del prodotto:\n");
			scanf("%*[^\n]%*c");
		}
		print(Product);
		menuchoice (lPtr, totproducts, thresholdvar);
	}else{
		printf ("Prodotto non trovato!\n\n");
	}
}

//----------------------------------------------------------------------------------------------------------------------

void modquantity (ProductPtr lPtr, size_t totproducts, int thresholdvar){
	int insertcode;
	puts("Inserire il codice del prodotto:");
	while(scanf ("%d", &insertcode)<1 || insertcode<0){
		printf("Errore! Inserire un intero maggiore o uguale a 0.\nInserire nuovamente il codice del prodotto:\n");
		scanf("%*[^\n]%*c");
	}
	ProductPtr	Product = search (lPtr, insertcode);
	
	if ( Product != NULL ){
		puts("Inserire la nuova quantità del prodotto:");
		while (scanf("%d", &(Product -> quantity))<1 || (Product -> quantity)<0){
			printf("Errore! Inserire un intero maggiore o uguale a 0.\nInserire nuovamente la nuova quantità del prodotto:\n");
			scanf("%*[^\n]%*c");
		}
		print(Product);
		menuchoice (lPtr, totproducts, thresholdvar);
	}else{
		printf ("Prodotto non trovato!\n\n");
	}
}

//----------------------------------------------------------------------------------------------------------------------

void print (ProductPtr Productprint){
	printf("%s\n%14s %20s %11s %12s\n", "Dettagli prodotto:", "Codice", "Prodotto", "Prezzo", "Quantità");
	printf("%14d %20s %10.2f€ %11d\n\n",Productprint -> code, Productprint -> name, Productprint -> price, Productprint -> quantity);
}

//----------------------------------------------------------------------------------------------------------------------

void searchprint (ProductPtr lPtr, size_t totproducts, int thresholdvar){
	int insertcode;
	puts("Inserire il codice del prodotto da cercare:");
	while(scanf ("%d", &insertcode)<1 || insertcode<0){
		printf("Errore! Inserire un intero maggiore o uguale a 0.\nInserire nuovamente il codice del prodotto:\n");
	}
	ProductPtr	Product = search (lPtr, insertcode);
	
	if ( Product != NULL )
		print (Product);
	else
		printf ("Prodotto non trovato!\n\n");
}

//----------------------------------------------------------------------------------------------------------------------

ProductPtr search (ProductPtr lPtr, int insertcode){
	ProductPtr correntePtr = lPtr;
	while (correntePtr != NULL && correntePtr -> code != insertcode){
			correntePtr = correntePtr -> nextPtr;
	}
	if (correntePtr == NULL)
		return NULL;
	else
		return correntePtr;
}

//----------------------------------------------------------------------------------------------------------------------

void menuchoice (ProductPtr lPtr, size_t totproducts, int thresholdvar){
	int choice;
	void (*menuselect[7])(ProductPtr lPtr, size_t totproducts, int thresholdvar) = {modquantity, modprice, newproduct, deleteprod, searchprint, threshold, exitfun};
	printf("Scegliere un’opzione:\n1 - Variazione della quantità di un prodotto\n2 - Variazione del prezzo di un prodotto\n3 - Inserimento di un nuovo prodotto\n4 - Eliminazione di un prodotto\n5 - Ricerca di un prodotto\n6 - Stampa prodotti sotto la soglia\n7 - Esci\n\n");
	while(scanf("%d",&choice)<1 || choice<1 || choice>7){
		printf("Errore! Inserire un intero da 1 a 7:");
		scanf("%*[^\n]%*c");
	}
	(*menuselect[choice-1])(lPtr,totproducts,thresholdvar);
	menuchoice (lPtr, totproducts, thresholdvar);
}

//----------------------------------------------------------------------------------------------------------------------

void exitfun (ProductPtr lPtr, size_t totproducts, int thresholdvar){
	FILE *WfPtr;
	ProductPtr correntePtr = lPtr;
	size_t cont=0,order=0;
	WfPtr = fopen("daOrdinare.txt", "w");
	fprintf(WfPtr, "%20s%20s%20s\n", "Codice", "Prodotto", "Da ordinare");
	while (correntePtr != NULL){
		if (correntePtr -> quantity < thresholdvar){
			order = thresholdvar - (correntePtr -> quantity);
			fprintf(WfPtr, "%20d%20s%20zd\n", correntePtr -> code, correntePtr -> name, order+1);
			cont++;
		}
		correntePtr = correntePtr -> nextPtr;
	}
	fclose(WfPtr);
	WfPtr = fopen("numeroTotale.txt", "w");
	fprintf(WfPtr, "%s%zd%s\n", "I prodotti disponibili in magazzino sono ",totproducts,".");
	fprintf(WfPtr, "%s%d%s%zd%s\n", "I prodotti disponibili in magazzino con quantità inferiore a ",thresholdvar," sono ",cont,".");
	fclose(WfPtr);
	printf("I prodotti disponibili in magazzino sono %zd.\n", totproducts);
	printf("I prodotti disponibili in magazzino con quantità inferiore a %d sono %zd.\n", thresholdvar, cont);
	exit(puts("Bye bye!\n"));
}
