/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	list->firstElement = NULL;
	list->activeElement = NULL;
	list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {

	while (list->firstElement != NULL) {

       DLLElementPtr tmp = list->firstElement;
        while(tmp->nextElement != NULL)
        {
            tmp = tmp->nextElement;
            free(tmp->previousElement);
        }
        free(tmp);
		DLL_Init(list);
    }
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {

	//vytovrime novy prvok a skontrolujeme ci sme ho spravne alokovali
	DLLElementPtr new_element = malloc (sizeof(struct DLLElement));

    if(new_element == NULL){
        DLL_Error();
		return;
	}

    //nastavime mu potrebne parametre
    new_element->data = data;
	new_element->previousElement = NULL;
    new_element->nextElement = list->firstElement;

    if(list->firstElement != NULL){
    	list->firstElement->previousElement = new_element;
    }else{
        list->lastElement = new_element;
    }

    list->firstElement = new_element;
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {

	//vytovrime novy prvok a skontrolujeme ci sme ho spravne alokovali
	DLLElementPtr new_element = malloc (sizeof(struct DLLElement));

	if(new_element == NULL){
		DLL_Error();
		return;
	}

	//nastavime mu potrebne parametre
	new_element->data = data;
	new_element->previousElement = list->lastElement;
	new_element->nextElement = NULL;

	if(list->lastElement != NULL){
		list->lastElement->nextElement = new_element;
	}else{
		list->firstElement = new_element;
	}
	list->lastElement = new_element;
	
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {

	list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {

	list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {

	//kontrolujeme ci list a aktivny element neni NULL
	if(list != NULL && list->firstElement != NULL){
		*dataPtr = list->firstElement->data;
	}else{
		DLL_Error();
		return;
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {

	//kontrolujeme ci list a aktivny element neni NULL
	if(list != NULL && list->lastElement != NULL){
		*dataPtr = list->lastElement->data;
	}else{
		DLL_Error();
		return;
	}
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {

	DLLElementPtr del_element;

	 // ak je prazdny, nic nerobime
    if (list->firstElement == NULL) {
        return;
    }

    //ak je posledny prvok aktivny, tak mu tuto aktivitu rusime
    if (list->activeElement == list->firstElement) {
        list->activeElement = NULL;
    }

	//do vytvorenej premennej hodime prvy prvok
    del_element = list->firstElement;
	//na poziciu noveho prveho prvky hodime prvok, ktory je za nasim 
    list->firstElement = del_element->nextElement;
   
    if (list->firstElement != NULL) {
        list->firstElement->previousElement = NULL;
    }

    // ak je prvok, ktory mazeme aj poslednym tak first, aj last element nastavime na NULL
    if (del_element == list->lastElement) {
        list->firstElement = NULL;
        list->lastElement = NULL;
    }
	//uvolnime pamat
    free(del_element);
	
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {

	DLLElementPtr del_element;

	// ak je prazdny, nic nerobime
    if (list->lastElement == NULL) {
        return;
    } 

    //ak je posledny prvok aktivny, tak mu tuto aktivitu rusime
    if (list->activeElement == list->lastElement) {
            list->activeElement = NULL;
    }

    del_element = list->lastElement;

    // kontrolujeme ci prvok nebol prvym prvkom ak ano, menime first aj last element na NULL
    if (del_element == list->firstElement) {
        list->firstElement = NULL;
        list->lastElement = NULL;
    } else {
		//ak bol tak nastavime na posledny element, prvok predchadzajuci ten co mazeme
        list->lastElement = del_element->previousElement;
		//nasleduju prvok za poslednym je null
        list->lastElement->nextElement = NULL;
    }
	//uvolnime pamat
    free(del_element);
    
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	DLLElementPtr deleteted_elem;

	//kontrolujeme ci list, aktivny element alebo prvok za aktinvym elementom neni NULL
	if(list != NULL && list->activeElement != NULL && list->activeElement->nextElement != NULL){
		deleteted_elem = list->activeElement->nextElement->nextElement;

		//kontrolujeme ci docasny prvok je NULL
		if(deleteted_elem == NULL){
			list->lastElement = list->activeElement;
		}else{
			deleteted_elem->previousElement = list->activeElement;
		}
		//uvolnujeme pamat
		free(list->activeElement->nextElement);
		list->activeElement->nextElement = deleteted_elem; 
	}
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	
	DLLElementPtr deleteted_elem;

	//kontrolujeme ci list, aktivny element alebo prvok za aktinvym elementom neni NULL
	if(list != NULL && list->activeElement != NULL && list->activeElement->previousElement != NULL){
		deleteted_elem = list->activeElement->previousElement->previousElement;

		//kontrolujeme ci docasny prvok je NULL
		if(deleteted_elem == NULL){
			list->firstElement = list->activeElement;
		}else{
			deleteted_elem->nextElement = list->activeElement;
		}
		//uvolnujeme pamat
		free(list->activeElement->previousElement);
		list->activeElement->previousElement = deleteted_elem; 
	}
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {

	// aktivny element neni NULL
	if(list->activeElement != NULL){
		//vytvorime novy prvok a skontrolujeme ci sa podaril vytvorit
		DLLElementPtr new_element = malloc(sizeof(struct  DLLElement));

		if(new_element == NULL){
			DLL_Error();
			return;
		}
		//nastavime ukazovatele na dalsi a predchadzajuci prvok
		new_element->nextElement = list->activeElement->nextElement;
		new_element->previousElement = list->activeElement;
		new_element->data = data;

		//kontrolujeme ci je aktivny prvok zaroven aj poslednym
		if(list->activeElement == list->lastElement){
			list->lastElement = new_element;
		}else{
			list->activeElement->nextElement->previousElement = new_element;
		}
			list->activeElement->nextElement = new_element;

	}
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {

	// aktivny element neni NULL
	if(list->activeElement != NULL){
		//vytvorime novy prvok a skontrolujeme ci sa podaril vytvorit
		DLLElementPtr new_element = malloc(sizeof(struct  DLLElement));

		if(new_element == NULL){
			DLL_Error();
			return;
		}

		//nastavime ukazovatele na dalsi a predchadzajuci prvok
		new_element->nextElement = list->activeElement;
		new_element->previousElement = list->activeElement->previousElement;
		new_element->data = data;

		//kontrolujeme ci je aktivny prvok zaroven aj prvym 
		if(list->activeElement == list->firstElement){
			//ak ano, tak prvy prvok bude novo vytvoreny 
			list->firstElement = new_element;
		}else{
			list->activeElement->previousElement->nextElement = new_element;
		}
			list->activeElement->previousElement = new_element;
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {

	//kontrolujeme ci list a aktivny element neni NULL
	if(list != NULL && list->activeElement != NULL){
        *dataPtr = list->activeElement->data;
    }else{
        DLL_Error();
		return;
    }
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {

	//kontrolujeme ci list a aktivny element neni NULL
	if(list != NULL && list->activeElement != NULL){
        list->activeElement->data = data;
    }
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {

	//kontrolujeme ci list a aktivny element neni NULL
	if(list != NULL && list->activeElement !=NULL){
		//kontrolujeme ci za aktivnym prvkom sa nieco nachadza
		if(list->activeElement->nextElement == NULL){
		//ak nie tak active element je NULL
		list->activeElement = NULL;
		}else{
			//ak nie tak aktvivny prvok bude prvok za aktualne aktivnym prvkom
			list->activeElement = list->activeElement->nextElement;
		}
	}
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {

	//kontrolujeme ci list a aktivny element neni NULL
	if(list != NULL && list->activeElement !=NULL){
		//kontrolujeme ci pred aktivnym prvkom nieco je, ak nie tak NULL ak je tak nastavime na aktivny prvok predchadzajuci na ktorom sme 
		if(list->activeElement->previousElement == NULL){
		list->activeElement = NULL;
		}else{
			list->activeElement = list->activeElement->previousElement;
		}
	}
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	
	return (list->activeElement != NULL) ? 1 : 0;
}

/* Konec c206.c */
