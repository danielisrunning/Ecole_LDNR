/* Projet de groupe LDNR: Daniel Goriounov, Thibault Conte, Said Hattiti, Nicolas Rault

Ce programme permet de gerer une ecole entiere a partir d'un menu.

*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//							***** STRUCTURES ************************************************************


//Nicolas
typedef struct
{
	int jour;
	int mois;
	int an;
}Date;

//Daniel. Stocke si c'est CP, CE1,...
// AgeMin = la date de naissance des élèves les plus jeunes
// AgeMAx = la date de naissance des élèves les plus vieux
typedef struct
{
	char nom[4] ;
	Date AgeMin ; // Date de naissance la plus proche
	Date AgeMax ; // Date de naissance la plus eloignee
} Age_De_Classe ;

// Daniel. Le prof. Nom et genre
typedef struct
{
	char nom[20] ;
	char genre[4] ; // On va stocker Mr ou Mme
	int classeAssignee;
} Prof;

// Daniel
typedef struct
{
	char nom[20];
	char prenom[15];
	float MoyenneGenerale;
	Date DateDeNaissance;
}Eleve;

// Thibault
typedef struct
{
	Prof Professeur; // A FAIRE : adapter les fonctions
	char NomDeClasse[20];
	int NbrEleves;
	Age_De_Classe AgeDeClasse ;
	Eleve Tabeleves[25];
}Classe;

//Said
typedef struct
{
	char nom[20];
	int nbreDeClasses;
	int nbreElevesParClasseMax;
	int AnneeEnCours;
	Classe TabClasses[30];
	int nbreDeProfs;
	Prof Professeur[20] ;
}Ecole;

//Daniel
typedef struct
{
	char nom[4];
	char description[30];
	char explication [200] ;
	int etat ;
}Commande ;


//							***** FONCTIONS ************************************************************

//							***** BASIQUES ************************************************************



// Nicolas  // met toutes les lettres d'un mot en minuscule (pour eviter les erreurs lors des comparaisons)
void Mettre_En_Minuscules(char *mot)
{
	int i = 0 ;
	for (i = 0; i < strlen(mot); i++)
	{
		mot[i] = tolower(mot[i]) ;
	}
}

// Nicolas Remet les majuscules au debut des mots (aussi apres un tiret), pour faire joli lors des affichages
void Remetttre_Majuscules(char *mot)
{
    int i = 0;
    for (i = 0; i < strlen(mot); i++)
    {
        if (mot[i] == '-')
        {
            mot[i+1] = toupper(mot[i+1]);
        }
    }

    mot[0] = toupper(mot[0]);
}

//Nicolas
// compare deux chaines de caracteres et indique si la chaine1 est devant dans l'ordre alphabetique (renvoie -1)
// renvoie 1 si chaine1 est derrire chaine2 dans l'ordre alphabetique
// renvoie 0 si les deux chaines sont identiques (comme strcmp)
int Comparer_Ordre_Alphabetique(const char *chaine1, const char *chaine2)
{
	int lg1 = strlen(chaine1);
	int lg2 = strlen(chaine2);

	int tiret = 0;
	int tiret2 = 0;

	for (int i = 0; i + tiret < lg1; i ++)
	{
		if (i + tiret2< lg2)
		{
			if (chaine1[i] == '-')  // si on rencontre un tiret ('-') on décale l'indice i pour ne pas comparer le tiret
				tiret ++;
			if (chaine2[i] == '-')
				tiret2 ++;

			if (chaine1[i + tiret] < chaine2[i + tiret2])
				return -1;
			else if (chaine1[i + tiret] > chaine2[i + tiret2])
				return 1;
		}
	}

	if (lg1 < lg2)
		return -1;
	else if (lg1 > lg2)
		return 1;
	else
		return 0;

}


// Demande si reponse oui ou non.
// renvoie 1 si c'est oui et 0 si non, -1 si erreur.
int Oui_Non()
{
	char res = 'u' ;
	while (res != 'O' && res != 'o'  && res != 'N' && res != 'n') {
		printf("Repondez par O ou N.\n") ;
		scanf("%c", &res);
		getchar() ;
	}

	switch (res)
	{
		case 'o' :
		case 'O' :
			{
				return 1 ;
			}

		case 'n' :
		case 'N' :
			{
				return 0 ;
			}
	}
	return -1 ;
}


// Daniel. Verifie si un nombre correspond a une valeur attendue
// Renvoie 1 si bon et 0 si c'est pas bon
// min et max doivent être en-dehors des valeurs attendues.
int Verifier_Nombre(int Nombre, int min, int max)
{
	if (Nombre>min && Nombre<max)
	{
		return 1 ;
	}

	return 0 ;
}



//							***** DATES ************************************************************


//Daniel && Nicolas. Verifier si une date correspond a une valeur attendue.
// Renvoie 0 si pas bon et 1 si pas de souci.
int Verifier_Date(Date MaDate)
{
	int resultat = 1 ;

    int jourDansChaqueMois[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

    // Verifier annee entre 1901 et 2030
	resultat = Verifier_Nombre(MaDate.an, 1900, 2031) ;
	if (resultat == 0)
	{
		printf("\nProbleme avec l'annee.\n") ;
		return resultat;
	}

	resultat = Verifier_Nombre(MaDate.mois, -1, 13) ;
	if (resultat == 0)
	{
		printf("\nProbleme avec le mois.\n") ;
		return resultat;
	}

	    // on verifie si l'annee est bissextile et on modifie le nombre de jours en fevrier
    if ((MaDate.an % 4 == 0 && MaDate.an % 400 != 0) || MaDate.an % 400 == 0)
        jourDansChaqueMois[1] = 28;
    else
        jourDansChaqueMois[1] = 29;


	resultat = Verifier_Nombre(MaDate.jour, -1, (jourDansChaqueMois[MaDate.mois-1] +1)) ;
	if (resultat == 0) // Si pas bon
	{
		printf("\nProbleme avec le jour.\n") ;
		return resultat;
	}

	return resultat;
}


//Thibault. Saisie de date
void Saisir_Date(Date *MaDate)
{
	int resultat = 0 ;
	do
	{
		printf("\nSaisir la date de naissance au format jj mm aaaa \n");
		scanf("%2d %2d %4d",&MaDate->jour, &MaDate->mois,&MaDate->an);
		getchar() ;
		resultat = Verifier_Date(*MaDate) ;
	} while (resultat == 0);
}


//Thibault
void Afficher_Date(Date MaDate)
{
	printf("%d/%d/%d",MaDate.jour, MaDate.mois, MaDate.an);
}


//Thibault.
void Copier_Date( Date *destination, Date origine)
{
	destination->jour = origine.jour;
	destination->mois = origine.mois;
	destination->an = origine.an;
}


/*Daniel. Reinitialiser les champs d'une date */
void Reinitialiser_Date(Date *MaDate)
{
	MaDate->jour = 0 ;
	MaDate->mois = 0 ;
	MaDate->an = 0 ;
}


// Nicolas
// renvoie -1 si date1 < date 2
// renvoie 1 si date1 > date 2
// renvoie 0 si date1 = date2
int Comparer_Dates(Date date1, Date date2)
{
	// Si l'an 1 est plus petit que l'an 2
	if (date1.an < date2.an)
		return -1;
	else if (date1.an > date2.an)
		return 1;
	else    // si l'anne est la meme on compare les mois
	{
		if (date1.mois < date2.mois)
			return -1;
		else if (date1.mois > date2.mois)
			return 1;
		else    // si les mois sont identiques on compare les jours
		{
			if (date1.jour < date2.jour)
				return -1;
			else if (date1.jour > date2.jour)
				return 1;
			else    // si les jours sont identiques, on renvoie 0
				return 0;
		}
	}
}



//									***** AGE DE CLASSE ************************************************************


// Daniel
void Reinitaliser_Age_De_Classe(Age_De_Classe *MonAgeDeClasse)
{
	strcpy(MonAgeDeClasse->nom, "") ;
	Reinitialiser_Date(&MonAgeDeClasse->AgeMin) ;
	Reinitialiser_Date(&MonAgeDeClasse->AgeMax) ;
}


// Daniel.
// Pour modifier le nom et les dates min et max d'une classe à la volée.
// etat commence à 0 et n'est modifié que si on identifie un bon nom de classe.
void Modifier_Age_De_Classe(Age_De_Classe *MonAgeDeClasse, int AnneeEnCours, int *etat)
{
	Date Age_Min = {1, 1, 0} ;
	Date Age_Max = {31, 12, 0} ;
	if (strcmp(MonAgeDeClasse->nom, "CP")== 0 )
	{
		Age_Min.an = AnneeEnCours-6 ;
		Copier_Date(&MonAgeDeClasse->AgeMin, Age_Min) ;
		Age_Max.an = AnneeEnCours-6 ;
		Copier_Date(&MonAgeDeClasse->AgeMax, Age_Max) ;
		*etat = 1 ;
	}
	if (strcmp(MonAgeDeClasse->nom, "CE1")== 0 )
	{
		Age_Min.an = AnneeEnCours-7 ;
		Copier_Date(&MonAgeDeClasse->AgeMin, Age_Min) ;
		Age_Max.an = AnneeEnCours-7 ;
		Copier_Date(&MonAgeDeClasse->AgeMax, Age_Max) ;
		*etat = 1 ;
	}
	if (strcmp(MonAgeDeClasse->nom, "CE2")== 0 )
	{
		Age_Min.an = AnneeEnCours-8 ;
		Copier_Date(&MonAgeDeClasse->AgeMin, Age_Min) ;
		Age_Max.an = AnneeEnCours-8 ;
		Copier_Date(&MonAgeDeClasse->AgeMax, Age_Max) ;
		*etat = 1 ;
	}
	if (strcmp(MonAgeDeClasse->nom, "CM1")== 0 )
	{
		Age_Min.an = AnneeEnCours-9 ;
		Copier_Date(&MonAgeDeClasse->AgeMin, Age_Min) ;
		Age_Max.an = AnneeEnCours-9 ;
		Copier_Date(&MonAgeDeClasse->AgeMax, Age_Max) ;
		*etat = 1 ;
	}
	if (strcmp(MonAgeDeClasse->nom, "CM2")== 0 )
	{
		Age_Min.an = AnneeEnCours-10 ;
		Copier_Date(&MonAgeDeClasse->AgeMin, Age_Min) ;
		Age_Max.an = AnneeEnCours-10 ;
		Copier_Date(&MonAgeDeClasse->AgeMax, Age_Max) ;
		*etat = 1 ;
	}
}


// Daniel. Rentrer l'age de classe et renvoyer l'age min et max.
void Saisir_Age_De_Classe(Age_De_Classe *MonAgeDeClasse, int AnneeEnCours)
{
	int etat = 0 ;
	do
	{
		printf("\nQuelle est la classe ? Tapez CP, CE1, CE2, CM1 ou CM2.\n") ;
		scanf("%s", MonAgeDeClasse->nom) ;
		getchar() ;

		// Fonction pour modifier l'age de classe.
		Modifier_Age_De_Classe(MonAgeDeClasse, AnneeEnCours, &etat) ;

	} while ( etat== 0 ) ;
}


//Daniel. Copie une age de classe vers une autre
void Copier_Age_De_Classe(Age_De_Classe *Destination, Age_De_Classe Origine)
{
	strcpy(Destination->nom, Origine.nom) ;
	Copier_Date(&Destination->AgeMin, Origine.AgeMin);
	Copier_Date(&Destination->AgeMax, Origine.AgeMax);
}


// Daniel. Afficher les dates de naissance max et min d'une classe
void Afficher_Age_De_Classe(Age_De_Classe MonAgeDeClasse)
{
	printf("Le %s prend les eleves nes entre le ", MonAgeDeClasse.nom) ;
	Afficher_Date(MonAgeDeClasse.AgeMin) ;
	printf(" et le ") ;
	Afficher_Date(MonAgeDeClasse.AgeMax) ;
	printf(".");
}




//									***** ELEVES ************************************************************



//Nicolas
void Saisir_Eleve(Eleve *_eleve)
{
	printf("Saisir nom prenom de l'eleve:\n");
	scanf("%s %s", _eleve->nom, _eleve->prenom);
	getchar() ;
	Saisir_Date(&_eleve->DateDeNaissance);

	//on met tous les caracteres en minuscule pour faciliter les comparaisons et eviter les erreurs:
	Mettre_En_Minuscules(_eleve->nom);
	Mettre_En_Minuscules(_eleve->prenom);
}

//Daniel
void Saisir_Eleve_Sans_age(Eleve *_eleve)
{
	printf("Saisir nom prenom de l'eleve:\n");
	scanf("%s %s", _eleve->nom, _eleve->prenom);
	getchar() ;
	
	//on met tous les caracteres en minuscule pour faciliter les comparaisons et eviter les erreurs:
	Mettre_En_Minuscules(_eleve->nom);
	Mettre_En_Minuscules(_eleve->prenom);
}


//Nicolas
void Afficher_Eleve(Eleve _eleve)
{
    Remetttre_Majuscules(_eleve.nom);
    Remetttre_Majuscules(_eleve.prenom);
	printf("%-15s %-12s ne le ", _eleve.nom, _eleve.prenom);
	Afficher_Date(_eleve.DateDeNaissance);
	printf("\n");
}


//Thibault.
void Copier_Eleves(Eleve *destination, Eleve origine)
{
	strcpy(destination->nom, origine.nom);
	strcpy(destination->prenom, origine.prenom);
	destination->MoyenneGenerale = origine.MoyenneGenerale;
	Copier_Date(&destination->DateDeNaissance, origine.DateDeNaissance);
}


//Daniel
void Reinitialiser_Eleve(Eleve *MonEleve)
{
	strcpy(MonEleve->nom, "");
	strcpy(MonEleve->prenom, "");
	MonEleve->MoyenneGenerale = 0;
	Reinitialiser_Date(&MonEleve->DateDeNaissance) ;
}


//Nicolas
//Compare deux eleves et renvoie -1 si evele1 devant eleve2 dans l'ordre alphabetique, 1 pour l'inverse
// si nom et prenom sont egaux, compare les date de naissance (-1 eleve1 plus jeune, 1 eleve1 plus vieux)
// renvoie 0 si les deux eleves sont completement identiques
int Comparer_Eleves(Eleve eleve1, Eleve eleve2)
{
    int comparatif = Comparer_Ordre_Alphabetique(eleve1.nom, eleve2.nom);

    if (comparatif == 0) // s'ils ont le meme nom on compare leurs prénoms
		comparatif = Comparer_Ordre_Alphabetique(eleve1.prenom, eleve2.prenom);

	if (comparatif == 0) // s'ils ont le meme prenom, on compare leurs dates de naissance
		comparatif = Comparer_Dates(eleve1.DateDeNaissance, eleve2.DateDeNaissance);

        return comparatif;
}

//Compare deux eleves et renvoie -1 si evele1 devant eleve2 dans l'ordre alphabetique, 1 pour l'inverse
int Comparer_Eleves_Sans_Date(Eleve eleve1, Eleve eleve2)
{
    int comparatif = Comparer_Ordre_Alphabetique(eleve1.nom, eleve2.nom);

    if (comparatif == 0) // s'ils ont le meme nom on compare leurs prénoms
		comparatif = Comparer_Ordre_Alphabetique(eleve1.prenom, eleve2.prenom);

        return comparatif;
}


// Said décale toutes les cases du tableau d'eleve vers la gauche depuis la case indexEleve et diminue le nombre d'eleve dans la classe
void Supprimer_Eleve(Classe *Maclasse,int indexEleve)
{
	for (int i=indexEleve; i < Maclasse -> NbrEleves -1; i++)
	{
		Copier_Eleves(&Maclasse -> Tabeleves[i],Maclasse -> Tabeleves[i+1]);
	}

	Maclasse -> NbrEleves --;
}



// Said, fonction rechercher 1 Eleve par rapport à toute la classe (eleve par eleve)
int Rechercher_Eleve_Classe(Classe Maclasse,Eleve eleve1)
{
	int i;
	int resultat =  -1 ;
	for(i=0;i<Maclasse.NbrEleves;i++)
	{
		// si l'eleve est egal a 0 elle est vrai sinon on retourne 1
		if(Comparer_Eleves(eleve1, Maclasse.Tabeleves[i]) == 0)
			resultat = i;
	}
	return resultat ;
}

int Rechercher_Eleve_Classe_Sans_Date(Classe Maclasse, Eleve eleve1)
{
	int i;
	int resultat =  -1 ;
	for(i=0;i<Maclasse.NbrEleves;i++)
	{
		// si l'eleve est egal a 0 elle est vrai sinon on retourne 1
		if(Comparer_Eleves_Sans_Date(eleve1, Maclasse.Tabeleves[i]) == 0)
			resultat = i;
	}
	return resultat ;
}



// Said, fonction rechercher 1 eleve par rapport a toute l'ecole.
//Renvoie position si trouvé et -1 si pas trouve.
int Rechercher_Eleve_Ecole(Ecole Monecole, Eleve eleve1)
{
	int resultat = -1 ;
	int i;
	for(i=0;i<Monecole.nbreDeClasses;i++)
	{
		if(Rechercher_Eleve_Classe(Monecole.TabClasses[i],eleve1) > -1)
		//si l'eleve est egal a 0 ou superieur a 1 alors on la retourne dans la case
			resultat = i;

	}
	return resultat ; // si > -1 alors trouvé eleve dans ecole. Si -1 pas trouvé
}

int Rechercher_Eleve_Ecole_Sans_Date(Ecole Monecole, Eleve eleve1)
{
	int resultat = -1 ;
	int i;
	for(i=0;i<Monecole.nbreDeClasses;i++)
	{
		if(Rechercher_Eleve_Classe_Sans_Date(Monecole.TabClasses[i],eleve1) > -1)
		//si l'eleve est egal a 0 ou superieur a 1 alors on la retourne dans la case
			resultat = i;

	}
	return resultat ; // si > -1 alors trouvé eleve dans ecole. Si -1 pas trouvé
}


// Thibault. // verifie si l'eleve à le bon age pour une classe. Renvoie 1 pour oui, 0 pour faux
int Verifier_Eleve(Date DateDeNaissance, Age_De_Classe AgeDeClasse )
{
	//Je compare date de naissance et age de classe (agemin). On veut -1
	int comparatif = Comparer_Dates(DateDeNaissance, AgeDeClasse.AgeMin);
	if (comparatif >= 0 )
	{
		// Si l'eleve est ne depuis l'age max, il rentre dans la classe
		comparatif = Comparer_Dates(DateDeNaissance, AgeDeClasse.AgeMax);
		if (comparatif == -1)
		{
			return 1;
		}
	}
	// Si rien trouve, on renvoie 0
	return 0 ;
}


// Said
void Tri_Eleves_Alphabetique(Classe *_classe)
{
     int en_cours,  plus_petit, j;
     Eleve temp;

     for ( en_cours = 0; en_cours < _classe->NbrEleves - 1; en_cours++)
     {
                plus_petit = en_cours;
         for (j = en_cours ; j < _classe->NbrEleves; j++)
              if ((strcmp(_classe->Tabeleves[j].nom,_classe->Tabeleves[plus_petit].nom)) < 0)
                  plus_petit = j;
          Copier_Eleves(&temp ,_classe->Tabeleves[en_cours]);
          Copier_Eleves(&_classe->Tabeleves[en_cours] ,_classe->Tabeleves[plus_petit]);
          Copier_Eleves(&_classe->Tabeleves[plus_petit],temp);
     }
}

// Nicolas. Eviter d'utiliser cette fonction en dehors de Assigner_Eleve_Dans_Classe. Ne verifie rien du tout
void Ajouter_Eleve(Eleve _eleve, Classe * _classe)
{
	Copier_Eleves(&_classe->Tabeleves[_classe->NbrEleves], _eleve);
	_classe->NbrEleves ++;
	Tri_Eleves_Alphabetique(_classe);
	printf("L'eleve a ete ajoute a la classe %s", _classe->NomDeClasse);
}

//Nicolas
void Assigner_Eleve_Dans_Classe(Eleve _eleve, Classe *_classe, int nbreEleveParClasseMax)
{
	//Verifier qu'il y a de la place dans la classe
	if (_classe->NbrEleves +1 <= nbreEleveParClasseMax)
	{
		// on vérifie si l'eleve est deja inscrit lors de la saisie de l'eleve, du coup, pas besoin de verifier ici. On vérifie donc son age:
		if (Verifier_Eleve(_eleve.DateDeNaissance, _classe->AgeDeClasse) == 0)
		{
			char reponse = 'z';
			printf("L'age de l'eleve ne correspond pas au niveau de la classe\n");
			while (reponse != 'N' && reponse != 'n' && reponse != 'O' && reponse != 'o')
			{
				printf("Inscrire quand meme l'eleve dans la classe? O pour oui N pour non:\n");
				scanf("%c", &reponse);
				getchar();
				if (reponse == 'O' || reponse == 'o')
				{
					Ajouter_Eleve(_eleve, _classe);
				}
				if (reponse == 'N' || reponse == 'n')
				{
					printf("\nL'eleve n'a pas ete inscrit");       //On passe à une nouvelle saisie de commande
				}
			}
		}
		else
			Ajouter_Eleve(_eleve, _classe);
	}
	else
		printf("\nLa classe est pleine\n");       //On passe à une nouvelle saisie de commande
}


// Thibault
void Quelle_Classe_Assigner(Date DateEleve,char *niveau, int AnneeEnCours)
{
    int i;
    for(i=6;i<11;i++)
    {
        if(DateEleve.an == AnneeEnCours - i)
        {
            switch (i)
            {
                case 6:
                    strcpy(niveau,"CP");
                    break;
                case 7:
                    strcpy(niveau,"CE1");
                    break;
                case 8:
                    strcpy(niveau,"CE2");
                    break;
                case 9:
                    strcpy(niveau,"CM1");
                    break;
                case 10:
                    strcpy(niveau,"CM2");
                    break;
              }
         }
    }
}


// A FAIRE : creer une fonction superieure qui prend un eleve, le trouve, identifie sa classe et demande la classe finale.
// Said. Transfert passe en parametre d'une classe a une autre.
void Transfert_Eleve_Classe(Eleve eleve1, Classe *destination, Classe *origine, int nbreEleveMaxParClasse)
{
	Assigner_Eleve_Dans_Classe(eleve1, destination, nbreEleveMaxParClasse) ;
	int i = Rechercher_Eleve_Classe(*origine, eleve1) ;
	Supprimer_Eleve(origine, i);
}

//Daniel. Fonction du menu.
// Demande si je veux creer un nouvel eleve ou pas. Le cree si l'eleve existant est vide.
void Creer_Nouvel_Eleve_Ou_Pas(Eleve *MonEleve)
{
	// Pour savoir si on reutilise l'eleve temporaire.
	int tmp = -1 ;
	if ((strlen(MonEleve->nom))!=0)
	{
		printf("Eleve en memoire :\n") ;
		Afficher_Eleve(*MonEleve) ;

		printf("\nSouhaitez-vous creer un nouvel eleve ?\n") ;
		if ((tmp = (Oui_Non()))==1)
		{
			Saisir_Eleve(MonEleve) ;
		}
		else {
			printf("\nNous reutilisons l'eleve :\n") ;
			Afficher_Eleve(*MonEleve) ;
			printf("\n") ;
		}
	}

	// Si eleve vide
	while ((strlen(MonEleve->nom))==0)
	{
		printf("Pas d'eleve existant.\n") ;
		Saisir_Eleve(MonEleve) ;
	}
}


//Nicolas
void Transferer_Eleve(Eleve monEleve, Ecole *monEcole)
{
	int indexClasse;
	int indexEleve;
	int i;

	int j;

	for(i=0;i<monEcole->nbreDeClasses;i++)
	{
		indexEleve = Rechercher_Eleve_Classe(monEcole->TabClasses[i], monEleve);
		if(indexEleve > -1)
		{
			indexClasse = i;
			break;
		}
	}

	printf("\nDans qu'elle classe voulez-vous transferer cet eleve?\n");

	while (j < 1 || j > monEcole->nbreDeClasses)
	{
		for (i = 0; i < monEcole->nbreDeClasses; i++)
		{
			printf("%d\t%s\n", i+1, monEcole->TabClasses[i].NomDeClasse);     // affiche le nom de chaque classe avec un chiffre devant
		}
		printf("\nSaisir le numero de la classe souhaitee:\n");

		scanf("%d", &j);
		getchar();
	}

	Transfert_Eleve_Classe(monEcole->TabClasses[indexClasse].Tabeleves[indexEleve], &monEcole->TabClasses[j-1], &monEcole->TabClasses[indexClasse], monEcole->nbreElevesParClasseMax);

}

//PROF									***** PROFESSEUR **************************************************************************




// Nicolas modif d'afficher prof pour avoir l'option de ne pas afficher sa classe
void Afficher_Prof_SansClasse(Prof MonProf)
{
	Remetttre_Majuscules(MonProf.nom);
	printf("%s ", MonProf.genre) ;
	printf("%s", MonProf.nom) ;

}

// Daniel & Nicolas. Afficher un prof
void Afficher_Prof(Prof MonProf, Classe *TabClasses)
{
	Remetttre_Majuscules(MonProf.nom);
	printf("%s ", MonProf.genre) ;

	if (strcmp(MonProf.genre, "Mr") == 0)
		printf("%-16s", MonProf.nom) ;
	else
		printf("%-15s", MonProf.nom) ;
	if (MonProf.classeAssignee > -1)
	{
		printf(" a la classe %s", TabClasses[MonProf.classeAssignee].NomDeClasse ) ;
	}
}

//Daniel
void Afficher_Tab_Prof(Prof TabProfs[], Classe *TabClasses, int NbreDeProfs)
{
	int i = 0;

	for (i = 0; i < 20 && i <NbreDeProfs; i++)
	{
		if (strlen(TabProfs[i].nom) > 0)
		{
			printf("\n");
			Afficher_Prof(TabProfs[i], TabClasses);
		}
		else
		{
			printf("\n");
			break;
		}
	}
}


// Daniel. Saisie du prof.
void Saisir_Professeur(Prof *MonProf)
{
	char Genre = 't';
	printf("\nQuel est le nom du Professeur ?\n") ;
	scanf("%s", MonProf->nom);
	getchar();
	Mettre_En_Minuscules(MonProf->nom) ;

    while (Genre != 'H' && Genre != 'h' && Genre != 'F' && Genre != 'f')
    {
        printf("\nQuel est le genre du Professeur ? Tapez H ou F.\n");
        scanf("%c", &Genre) ;
        getchar();

        switch (Genre)
        {
            case 'H':
            case 'h':
                strcpy(MonProf->genre, "Mr") ;
                break;
            case 'F':
            case 'f':
                strcpy(MonProf->genre, "Mme") ;
                break;
            default:
                printf("Le genre saisi n'a pas ete reconnu\n");
                break;
        }
	}
}

// Copier d'un prof vers un autre.
void Copier_Prof(Prof *destination, Prof origine )
{
    strcpy(destination->nom, origine.nom);
    strcpy(destination->genre, origine.genre);
    destination->classeAssignee = origine.classeAssignee;
}

//Daniel. remets tout a zero
void Reinitialiser_Prof(Prof *MonProf)
{
	strcpy(MonProf->nom, "");
	strcpy(MonProf->genre, "");
	MonProf->classeAssignee = -1;
}


//Daniel. Je compare 2 profs
// Si sont les mêmes, je renvoie 0. Sinon, 1.
int Comparer_Professeur(Prof MonProf, Prof Prof2)
{
	int resultat = 1 ;
	// Si le nom est le même, j'entre dans le if
	if (strcmp(MonProf.nom, Prof2.nom)==0)
	{
		//si le genre est le meme, ça renvoie 0. Sinon 1.
		if (strcmp(MonProf.genre, Prof2.genre)== 0)
		{
			resultat = 0 ;
		}
	}
	return resultat ;
}

//Nicolas. - 1 si pas trouvé et position si trouve
int Trouver_Prof_Dans_Ecole(Ecole _ecole, Prof prof)
{
    for (int i = 0; i < _ecole.nbreDeProfs; i++)
    {
        if (Comparer_Professeur(prof, _ecole.Professeur[i]) == 0)
        {
            return i;
        }
    }
	return -1 ;
}


// Nicolas
int Trouver_Prof_Libre(Prof *profsDeLecole, int nbreDeProfs)
{
	int i = -1 ;
	for (i = 0; i < nbreDeProfs; i++)
	{
		if (profsDeLecole[i].classeAssignee == -1)
			return i;
	}
	return -1 ;
}


//Daniel. Je compare un prof à un tableau de profs.
// Si existe, je renvoie la position dans le tableau de prof.
//Si n'existe pas, je renvoie 0. S'il existe je renvoie la position
int Verifier_Si_Prof_Existe(Prof MonProf, Prof TabProf[], int NbreDeProfs)
{
	int i = 0 ;
	
	if (NbreDeProfs == 0)
	{
		return -1 ;
	}
	for (i=0; i<NbreDeProfs ; i++)
	{
		// Si ce sont les mêmes profs
		if ((Comparer_Professeur(MonProf, TabProf[i]))==0)
		{
			return i ;
		}
	}
	return -1 ;
}




//Daniel.  Pour reorganiser dans le tableau de profs. Decale tout à gauche à partir du prof supprimé
//Position = position dans le tableau supprimée. Taille = nouvelle taille du tableau
void Trier_Tableau_Profs(Prof *TabProfs, int NbreDeProfs, int position)
{
	int i = 0 ;

	// Je commence à la position indiquée.
	for (i=position; i<NbreDeProfs && i <20 ; i++)
	{
		Copier_Prof(&TabProfs[i], TabProfs[i+1]) ;
	}
}


//Daniel : On fait toutes les vérifs et on ajoute un prof
void Ajouter_Prof_Au_Tableau(Prof MonProf, Prof *TabProfs, int *NbreDeProfs)
{
	*NbreDeProfs= *NbreDeProfs + 1 ;
	Copier_Prof(&TabProfs[*NbreDeProfs-1], MonProf) ;

}


// Daniel. On saisit un prof et on fait toutes les vérifications.
void Ajouter_Prof_A_Ecole(Ecole *MonEcole)
{
	Prof tmp_Prof ;
	if (MonEcole->nbreDeProfs >19)
	{
		printf("Il y a deja le nombre maximum de professeurs. Supprimez un professeur pour en ajouter un.") ;
		return ;
	}
	do
	{
		Saisir_Professeur(&tmp_Prof) ;
	}while ((Verifier_Si_Prof_Existe(tmp_Prof, MonEcole->Professeur, MonEcole->nbreDeProfs))>-1) ;
	tmp_Prof.classeAssignee = -1 ;

	Ajouter_Prof_Au_Tableau(tmp_Prof ,MonEcole->Professeur, &MonEcole->nbreDeProfs) ;
}


//Daniel. Supprime prof d'un tableau. Pas de verifications sur l'ecole. Elles doivent etre faites avant.
//Si le prof n'existe pas, ca ne fera rien
void Supprimer_Prof_Tableau(Prof *MonProf, Ecole *MonEcole)
{
	int position = -1 ;
	int i = 0 ;
	for (i = 0 ; i < MonEcole->nbreDeProfs ; i++)
	{
		// Si je trouve le prof
		if ((Comparer_Professeur(*MonProf, MonEcole->Professeur[i]))== 0)
		{
			Trier_Tableau_Profs(MonEcole->Professeur, MonEcole->nbreDeProfs, i) ;
			MonEcole->nbreDeProfs -- ;
			printf("Le prof a ete supprime.\n") ;
			return ;
		}
	}
	printf("Le prof n'a pas ete trouve. Pas de suppression.\n") ;
	return ;
}

//Daniel. Je demande quel prof remplacer et je le desassigne, demande par quel prof remplacer.
//En parametre, passer le prof temporaire du menu ou un vide.
void Remplacer_Prof( Ecole *Ecole_De_Menu )
{
	int tmp ;
	int i = 0 ;
	int j= -1 ;
	char tmp_cdc[20] ;
	Prof Prof_De_Menu ;
	
	// S'il n'est pas possible de remplacer un prof, j'arrête la fonction ici
	if((Ecole_De_Menu->nbreDeProfs == Ecole_De_Menu->nbreDeClasses && Ecole_De_Menu->nbreDeProfs >19 )|| Ecole_De_Menu->nbreDeClasses <1)
	{
		printf("Nous avons deja le nombre maximum de classes et de profs. Il n'est pas possible de remplacer un professeur.\n") ;
		return ;
	}
	
	// J'affiche la liste des profs pour choisir dedans.
	printf("Voici la liste des professeurs de l'ecole :\n") ;
	Afficher_Tab_Prof(Ecole_De_Menu->Professeur, Ecole_De_Menu->TabClasses, Ecole_De_Menu->nbreDeProfs) ;

	// Je saisis le nom d'un prof tant qu'il n'existe pas
	Reinitialiser_Prof(&Prof_De_Menu) ;
	tmp = -2 ;
	do 
	{
		if (tmp == -1)
		{
			printf("Le professeur n'a pas ete trouve.\n ") ;
		}
		Saisir_Professeur(&Prof_De_Menu) ; // Le prof a modifier.
		tmp = Verifier_Si_Prof_Existe(Prof_De_Menu, Ecole_De_Menu->Professeur, Ecole_De_Menu->nbreDeProfs) ;
		
		// Je saisis le nom d'un prof s'il n'a pas de classe.
		if ( tmp > -1 && Ecole_De_Menu->Professeur[tmp].classeAssignee <0)
		{
			// Ce professeur n'a pas de classe.
			printf("Ce professeur n'a pas de classe. Merci de saisir un autre professeur.\n") ;
			Saisir_Professeur(&Prof_De_Menu) ; // Le prof a modifier.
			tmp = Verifier_Si_Prof_Existe(Prof_De_Menu, Ecole_De_Menu->Professeur, Ecole_De_Menu->nbreDeProfs) ;
		}
		} while (tmp <0 ) ;
	
	
	


	//J'affiche les profs qui sont libres et qui ne sont pas le prof a supprimer.
	printf("\nVoici la liste des professeurs qui n'ont pas de classe :\n") ;
	for (i=0 ; i<Ecole_De_Menu->nbreDeProfs ; i++)
	{
		if (Ecole_De_Menu->Professeur[i].classeAssignee == -1 && i != tmp)
		{
			Afficher_Prof(Ecole_De_Menu->Professeur[i], Ecole_De_Menu->TabClasses) ;
		}
	}

	// Je prend un prof qui doit etre libre. Je boucle tant que je n'ai pas trouve un bon prof.
	printf("\nQui voulez-vous choisir pour remplacer ") ;
	Afficher_Prof_SansClasse(Prof_De_Menu) ;
	printf(" dans sa classe ?") ;
	do
	{
		// Si je n'ai pas trouve de prof dans la liste, je redemande
		if (j == -2)
		{
			printf("\nIl y a eu une erreur sur le professeur.\n") ;
		}
		// Je demande le prof a prendre en remplacement du modifié.
		Saisir_Professeur(&Prof_De_Menu) ; // Attention, on enregistre le nouveau professeur dans le prof du menu !
		// Le prof a modifier n'est plus stocke que dans tmp, qui est l'index dans le tableau Professeur de l'ecole
		j = -1 ; // indicateur si on a trouve le prof ou pas.
		for (i=0 ; i<Ecole_De_Menu->nbreDeProfs ; i++)
		{
			if ((Comparer_Professeur(Prof_De_Menu, Ecole_De_Menu->Professeur[i]))== 0) // Si ce sont les memes
			{
				// Si je trouve le prof, j enregistre la position du prof libre a prendre.
				//Je copie le prof trouve dans la classe du prof modifié
				Copier_Prof(&Ecole_De_Menu->TabClasses[Ecole_De_Menu->Professeur[tmp].classeAssignee].Professeur, Ecole_De_Menu->Professeur[i]) ;
				//Je modifie le prof qui etait libre et sa classe assignee.
				Ecole_De_Menu->Professeur[i].classeAssignee = Ecole_De_Menu->Professeur[tmp].classeAssignee ;
				j = i ;
				break ;
			}
			// Si je ne trouve pas le prof.
			else 
			{
				j = -2 ; // Permet d'afficher message d'erreur si pas trouve a la fin
			}
		}
	} while (j <0) ;
	
	// Je modifie le prof modifié pour le mettre sans classe.
	Ecole_De_Menu->Professeur[tmp].classeAssignee = -1 ;
	printf("Le prof a bien ete remplace.\n") ;

}

//Daniel.
void Supprimer_Prof_Ecole(Ecole *Ecole_De_Menu)
{
	int tmp ;
	int i = 0 ;
	int j=0 ;
	char tmp_cdc[20] ;
	Prof Prof_De_Menu ;
	//J'affiche la liste des profs
	printf("Voici la liste des professeurs de l'ecole :\n") ;
	Afficher_Tab_Prof(Ecole_De_Menu->Professeur, Ecole_De_Menu->TabClasses, Ecole_De_Menu->nbreDeProfs) ;

	// Je saisis le nom d'un prof tant qu'il n'existe pas
	Reinitialiser_Prof(&Prof_De_Menu) ;
	tmp = -2 ;
	do 
	{
		Saisir_Professeur(&Prof_De_Menu) ; // Le prof a supprimer.
		tmp = Verifier_Si_Prof_Existe(Prof_De_Menu, Ecole_De_Menu->Professeur, Ecole_De_Menu->nbreDeProfs) ;
	} while (tmp < 0) ;
	

	// Si le prof a une classe, je verifie si un prof de remplacement est libre
	if ((Ecole_De_Menu->Professeur[tmp].classeAssignee) > -1)
	{
		// Si pas de prof libre, je demande d'en creer un
		printf("\nLe professeur choisi enseigne une classe.") ;
		if (Ecole_De_Menu->nbreDeProfs<= Ecole_De_Menu->nbreDeClasses)
		{
			printf("\nIl n'y a pas de professseurs qui puissent le remplacer. Merci d'en creer un :\n") ;
			Ajouter_Prof_A_Ecole(Ecole_De_Menu) ;
		}
		//Quand j'ai un prof libre, je demande de le choisir.
		else
		{

			//J'affiche les profs qui sont libres et qui ne sont pas le prof a supprimer.
			printf("\nVoici la liste des professeurs qui n'ont pas de classe :\n") ;
			for (i=0 ; i<Ecole_De_Menu->nbreDeProfs ; i++)
			{
				if (Ecole_De_Menu->Professeur[i].classeAssignee == -1 && i != tmp)
				{
					Afficher_Prof(Ecole_De_Menu->Professeur[i], Ecole_De_Menu->TabClasses) ;
				}
			}

			// Je prend un prof qui doit etre libre. Je boucle tant que je n'ai pas trouve un bon prof.
			printf("\nQui voulez-vous choisir pour remplacer ") ;
			Afficher_Prof_SansClasse(Prof_De_Menu) ;
			printf(" dans sa classe ?") ;
			do
			{
				// Si je n'ai pas trouve de prof dans la liste, je redemande
				if (j == -2)
				{
					printf("\nIl y a eu une erreur sur le professeur.\n") ;
				}
				Saisir_Professeur(&Prof_De_Menu) ; // Attention, on enregistre le nouveau professeur dans le prof du menu !
				// Le prof a supprimer n'est plus stocke que dans tmp, qui est l'index dans le tableau Professeur de l'ecole
				j = -1 ; // indicateur si on a trouve le prof ou pas.
				for (i=0 ; i<Ecole_De_Menu->nbreDeProfs ; i++)
				{
					if ((Comparer_Professeur(Prof_De_Menu, Ecole_De_Menu->Professeur[i]))== 0) // Si ce sont les memes
					{
						printf("Prof trouve.\n") ;
						// Si je trouve le prof, j enregistre la position du prof libre a prendre.
						//Je copie le prof trouve dans la classe du prof a supprimer
						Copier_Prof(&Ecole_De_Menu->TabClasses[Ecole_De_Menu->Professeur[tmp].classeAssignee].Professeur, Ecole_De_Menu->Professeur[i]) ;
						//Je modifie le prof qui etait libre et sa classe assignee.
						Ecole_De_Menu->Professeur[i].classeAssignee = Ecole_De_Menu->Professeur[tmp].classeAssignee ;
						j = i ;
						break ;
					}
					// Si je ne trouve pas le prof.
					else 
					{
						j = -2 ; // Permet d'afficher message d'erreur si pas trouve a la fin
					}
				}
				printf("j = %d\n", j) ;
			} while (j <0) ;

		}
		// Je reprend le prof créé/libre choisi. Je l'attribue et je le modifie
	}
	// Je supprime le prof et modifie le Nbre deProfs
	Supprimer_Prof_Tableau(&Ecole_De_Menu->Professeur[tmp], Ecole_De_Menu) ;
}


//CLASSE									***** CLASSE *********************************************************************************




//Nicolas
void Copier_Classe(Classe *destination, Classe origine)
{
    Copier_Age_De_Classe(&destination->AgeDeClasse, origine.AgeDeClasse);
    destination->NbrEleves = origine.NbrEleves;
    strcpy(destination->NomDeClasse, origine.NomDeClasse);
    Copier_Prof(&destination->Professeur, origine.Professeur);

    for (int i = 0; i < origine.NbrEleves; i ++)
    {
        Copier_Eleves(&destination->Tabeleves[i], origine.Tabeleves[i]);
    }
}

//Nicolas
char Definir_Identifiant_Classe(const char *niveauDeClasse, Ecole *MonEcole)
{
    char lettres[7] = {'A', 'B', 'C', 'D', 'E', 'F'};
    int identifiant = 0;

    for (int i = 0; i < MonEcole->nbreDeClasses; i++)
    {
        if (strcmp(MonEcole->TabClasses[i].AgeDeClasse.nom, niveauDeClasse) == 0)
            identifiant ++;
    }

    return lettres[identifiant];
}


// Nicolas sert juste à pas répéter du code dans la fonction Creer_Classe_Vide Ne pas utliser cette fonction
void Ajouter_Classe_Vide(Ecole *MonEcole, const char *niveau, int AnneeEnCours)
{
	Classe classeVide;
	int etat = 0 ;
        strcpy(classeVide.NomDeClasse, niveau);

        char identifiant[2] = "";
        identifiant[0] = Definir_Identifiant_Classe(niveau, MonEcole);

        strcat(classeVide.NomDeClasse, identifiant);

        strcpy(classeVide.AgeDeClasse.nom, niveau);
        Modifier_Age_De_Classe(&classeVide.AgeDeClasse, AnneeEnCours, &etat);

        int indexProf = Trouver_Prof_Libre(MonEcole->Professeur, MonEcole->nbreDeProfs);
        Copier_Prof(&classeVide.Professeur, MonEcole->Professeur[indexProf]);
        MonEcole->Professeur[indexProf].classeAssignee = MonEcole->nbreDeClasses;

        MonEcole->nbreDeClasses ++;
        Copier_Classe(&MonEcole->TabClasses[MonEcole->nbreDeClasses], classeVide);
}


//Daniel et Nicolas Renvoie l'index de la case où est cree classe vide dans le tableau des classes de l'ecole sinon renvoie -1 si la classe
// n'a pas pu etre cree
int Creer_Classe_Vide(Ecole *MonEcole, const char *niveau)
{
    if (MonEcole->nbreDeClasses +1 > MonEcole->nbreDeProfs || Trouver_Prof_Libre(MonEcole->Professeur, MonEcole->nbreDeProfs) < 0)
    {
        char reponse = 'z';

        printf("\nIl n'y a pas assez de professeurs dans l'ecole pour creer une nouvelle classe.\n");
        while (reponse != 'N' && reponse != 'n' && reponse != 'O' && reponse != 'o')
        {
            printf("Ajouter un professeur? O pour oui N pour non:\n");
            scanf("%c", &reponse);
			getchar() ;
            if (reponse == 'O' || reponse == 'o')
            {
                Ajouter_Prof_A_Ecole(MonEcole);
                Ajouter_Classe_Vide(MonEcole, niveau, MonEcole->AnneeEnCours);     // On refait la fonction puisque maintenant on a assez de profs
                return MonEcole->nbreDeClasses -1;
            }
            if (reponse == 'N' || reponse == 'n')
            {      //On passe à une nouvelle saisie de commande
                return -1;
            }
        }
    }
    else
    {
        Ajouter_Classe_Vide(MonEcole, niveau, MonEcole->AnneeEnCours);
        return MonEcole->nbreDeClasses -1;
    }
	return -1 ;
}

// Souci, ne marche pas encore !
// Nicolas. Ajouter classe vide a ecole ecole
void Ajouter_Classe(Ecole *MonEcole)
{
	
	char niveau[4] = "";
	if (MonEcole->nbreDeProfs >19 && Trouver_Prof_Libre(MonEcole->Professeur, MonEcole->nbreDeProfs) < 0 )
	{
		printf("Il n'est pas possible de creer de nouveaux professeurs. Il n'y a pas de professeurs libres.\n") ;
		return  ;
	}
	
	while ((strcmp(niveau, "CP")) != 0 && (strcmp(niveau, "CE1")) != 0 && (strcmp(niveau, "CE2")) != 0 && (strcmp(niveau, "CM1")) != 0 && (strcmp(niveau, "CM2")) != 0)
	{
		printf("Saisir le niveau.\n");
		scanf("%s", niveau);
		printf("%s", niveau) ;
		getchar();
	}

	Creer_Classe_Vide(MonEcole, niveau);
}


//Thibault. (Daniel+Nicolas)
// Assigne un eleve automatiquement dans une ecole.
void Assigner_Eleve(Eleve _eleve, Ecole *monEcole)
{
	int i;
	int inscrit=0;

	if (Rechercher_Eleve_Ecole(*monEcole, _eleve) > 0) // modifie. Ne marchait pas (daniel)
	{
		printf("Cet eleve est deja inscrit\n");
		return;
	}

	for (i=0; i<monEcole->nbreDeClasses ;i++)
	{
		if (monEcole->TabClasses[i].NbrEleves < monEcole->nbreElevesParClasseMax)
		{
			int comparatif = Verifier_Eleve(_eleve.DateDeNaissance, monEcole->TabClasses[i].AgeDeClasse );
			if (comparatif == 1)
			{
			    int indexClasse = monEcole->TabClasses[i].NbrEleves;
				Copier_Eleves(&monEcole->TabClasses[i].Tabeleves[indexClasse], _eleve);
				monEcole->TabClasses[i].NbrEleves ++ ;
				Tri_Eleves_Alphabetique(&monEcole->TabClasses[indexClasse]);
				inscrit = 1;
				printf("L'eleve a ete inscrit dans la classe %s\n", monEcole->TabClasses[indexClasse].NomDeClasse);
				break;
			}
		}
	}
	if (inscrit == 0)
	{
		char niveau[4]="";
		Quelle_Classe_Assigner(_eleve.DateDeNaissance, niveau, monEcole->AnneeEnCours);
	    if (Creer_Classe_Vide(monEcole, niveau) != -1) // si on a reussi a creer une classe vide
        {
            Copier_Eleves(&monEcole->TabClasses[monEcole->nbreDeClasses-1].Tabeleves[0], _eleve);
            Tri_Eleves_Alphabetique(&monEcole->TabClasses[monEcole->nbreDeClasses -1]);
			printf("L'eleve a ete inscrit dans la classe %s\n", monEcole->TabClasses[monEcole->nbreDeClasses -1].NomDeClasse);
        }
		else
		printf("\nL'eleve n'a pas ete inscrit");       //On passe à une nouvelle saisie de commande
	}
}

// !! Ne verifie pas si l'eleve existe deja. Peut donc faire une erreur.
void Assigner_Eleve_Sans_rechercher(Eleve _eleve, Ecole *monEcole)
{
	int i;
	int inscrit=0;

	for (i=0; i<monEcole->nbreDeClasses ;i++)
	{
		if (monEcole->TabClasses[i].NbrEleves < monEcole->nbreElevesParClasseMax)
		{
			int comparatif = Verifier_Eleve(_eleve.DateDeNaissance, monEcole->TabClasses[i].AgeDeClasse );
			if (comparatif == 1)
			{
			    int indexClasse = monEcole->TabClasses[i].NbrEleves;
				Copier_Eleves(&monEcole->TabClasses[i].Tabeleves[indexClasse], _eleve);
				monEcole->TabClasses[i].NbrEleves ++ ;
				Tri_Eleves_Alphabetique(&monEcole->TabClasses[indexClasse]);
				inscrit = 1;
				break;
			}
		}
	}
	if (inscrit == 0)
	{
		char niveau[4]="";
		Quelle_Classe_Assigner(_eleve.DateDeNaissance, niveau, monEcole->AnneeEnCours);
	    if (Creer_Classe_Vide(monEcole, niveau) != -1) // si on a reussi a creer une classe vide
        {
            Copier_Eleves(&monEcole->TabClasses[monEcole->nbreDeClasses-1].Tabeleves[0], _eleve);
            Tri_Eleves_Alphabetique(&monEcole->TabClasses[monEcole->nbreDeClasses -1]);
        }
	}
}



//Daniel. Saisir Classe
void Saisir_Classe(Classe *MaClasse, int NombreElevesMax, int AnneeEnCours)
{
	int compteur = 0 ;
	//J'enregistre le type de classe, le nom de la classe et le prof.
	//Creer_Classe_Vide(MaClasse, AnneeEnCours) ;

	printf("\nCombien d'eleves voulez-vous inscrire a ce stade ?\n") ;
	scanf("%d", &MaClasse->NbrEleves) ;
	getchar() ;

	// Boucle d'enregistrement d'élèves. Je limite au nombre d'élèves max et au réel.
	if (MaClasse->NbrEleves> NombreElevesMax)
	{
		MaClasse->NbrEleves = NombreElevesMax ;
		printf("\nVous voulez rentrer plus d'eleves que le nombre max autorise. PAS BIEN.\n");
	}
	for (compteur= 0 ; compteur<MaClasse->NbrEleves && compteur <NombreElevesMax ; compteur++)
	{
		Saisir_Eleve(&MaClasse->Tabeleves[compteur]) ;
	}
	return ;
}


//Daniel. Afficher une classe et ses eleves.
void Afficher_Classe(Classe MaClasse)
{
	int compteur = 0 ;
	printf("\nLe %s a %d eleves et a pour prof %s %s.\n", MaClasse.NomDeClasse, MaClasse.NbrEleves, MaClasse.Professeur.genre, MaClasse.Professeur.nom) ;
	Afficher_Age_De_Classe(MaClasse.AgeDeClasse) ;

	// Affichage de tous les élèves.
	printf("\nLa liste des eleves est :\n") ;
	for (compteur= 0 ; compteur<MaClasse.NbrEleves; compteur++)
		{
			Afficher_Eleve(MaClasse.Tabeleves[compteur]) ;
		}
		return ;
}


//Daniel.
//Affiche l'en tete de la classe sans les eleves
void Afficher_Classe_Sans_Eleves(Classe MaClasse)
{
	int compteur = 0 ;
	printf("\nLa classe %s a %d eleves et a pour prof %s %s.\n", MaClasse.NomDeClasse, MaClasse.NbrEleves, MaClasse.Professeur.genre, MaClasse.Professeur.nom) ;
	Afficher_Age_De_Classe(MaClasse.AgeDeClasse) ;
}


// Daniel. Affiche les classes sans les eleves
void Afficher_Toutes_Les_Classes(Ecole MonEcole)
{
	int i;
	 for(i=0;i<MonEcole.nbreDeClasses;i++)
		 {
			 Afficher_Classe_Sans_Eleves(MonEcole.TabClasses[i]);
		 }
}

// Nicolas
void Supprimer_Classe(Ecole *monEcole, int indexClasse)
{
    int i;
    // on cherche le prof de la classe dans le tableau des profs
    monEcole->Professeur[Trouver_Prof_Dans_Ecole(*monEcole, monEcole->TabClasses[indexClasse].Professeur)].classeAssignee = -1;

    for (i = indexClasse; i < monEcole->nbreDeClasses -1; i++)
    {
		Copier_Classe(&monEcole->TabClasses[i], monEcole->TabClasses[i+1]);
		monEcole->Professeur[Trouver_Prof_Dans_Ecole(*monEcole, monEcole->TabClasses[i].Professeur)].classeAssignee = i;
    }

    monEcole->nbreDeClasses --;
}


// Nicolas Tri les classes par niveau de CP à CM2 (pour l'instant ne tri pas CPA, CPB...) et stocke ca dans un tableau de classes temporaire, et stocke le
// nombre de classe de chaque niveau dans TabNbreClassesParNiveau (en gros, combien de classes de CP, combien de classes de CE1...)
void Trier_Classes_Temp(Ecole _ecole, Classe *classes, int *TabNbreClassesParNiveau)
{
	int i;
	int nbreDeClassesTemp = 0;
	char niveaux[5][4] = {"CP", "CE1", "CE2", "CM1", "CM2"};

	for (i = 0; i < 5; i++)     // pour chaque niveau CP, CE1, CE2...
	{
		for (int j = 0; j < _ecole.nbreDeClasses; j++)  // on regarde toutes les classes de l'ecole
		{
			if (strcmp(_ecole.TabClasses[j].AgeDeClasse.nom, niveaux[i]) == 0)          // on regarde si AgeDeClasse.nom correspond à un niveau CP, CE1...
			{
				Copier_Classe(&classes[nbreDeClassesTemp],_ecole.TabClasses[j]);    // si oui, on copie la classe dans le tableau temporaire
				nbreDeClassesTemp ++;               // on augmente le nombre de classes que l'on a stocké dans le tableau de classes temporaire
				TabNbreClassesParNiveau[i] ++;      // on augmente le nombre de classes correspondant au niveau i
			}
		}
	}
}

void Trier_Classes(Ecole *_ecole, int *NbreDeClassesParNiveau)
{
	Classe TabClasseTemp[10];
	int _NbreDeClassesParNiveau[5] = {0,0,0,0,0};

	int i = 0;

	for (i = 0; i < 10; i++)
	{
		TabClasseTemp[i].NbrEleves = 0;
	}

	Trier_Classes_Temp(*_ecole, TabClasseTemp, _NbreDeClassesParNiveau);

	for (i = 0; i < _ecole->nbreDeClasses; i++)
	{
		Copier_Classe(&_ecole->TabClasses[i], TabClasseTemp[i]);
		_ecole->Professeur[Trouver_Prof_Dans_Ecole(*_ecole, TabClasseTemp[i].Professeur)].classeAssignee = i;
	}

	for (i = 0; i < 5; i++)
	{
		NbreDeClassesParNiveau[i] = _NbreDeClassesParNiveau[i];
	}
}


// Nicolas
void Equilibrer_Classes(Ecole *_ecole)
{
	int NbreDeClassesParNiveau[5] = {0,0,0,0,0};

	// d'abord on trie les classes par niveau (Toutes les classes de CP, puis les classes de CE1...)
	Trier_Classes(_ecole, NbreDeClassesParNiveau);

	// pour chaque niveau, on stocke les eleves dans un tableau temporaire
	Eleve tempEleves[150];

	int totalEleveTemp = 0;         // combien d'eleves on a stocké dans le tableau d'eleves temporaire
	int totalClassesTriees = 0;     // combien de classes on a traite
	int totalClassesCopiees = 0;
	int i = 0;
	int j = 0;
	int u = 0;

	for(i = 0; i < 5; i++)      // pour chaque niveau (CP, CE1...)
	{
		totalEleveTemp = 0;
		if (NbreDeClassesParNiveau[i] > 1)
		{
			for (j = totalClassesTriees; j < NbreDeClassesParNiveau[i] + totalClassesTriees; j++)
			{
				for (u = 0; u < _ecole->TabClasses[j].NbrEleves; u++)
				{
					Copier_Eleves(&tempEleves[totalEleveTemp], _ecole->TabClasses[j].Tabeleves[u]);
					totalEleveTemp ++;
				}
				_ecole->TabClasses[j].NbrEleves = 0;
			}
			totalClassesTriees += NbreDeClassesParNiveau[i];

			u = 0;
			int indiceEleve = 0;
            int nbreDeClassesRemplies = 1 + totalEleveTemp / 25;

			if (nbreDeClassesRemplies < NbreDeClassesParNiveau[i])
            {
                for (j = NbreDeClassesParNiveau[i] + totalClassesCopiees - 1; j > nbreDeClassesRemplies -1; j--)
                {
                    Supprimer_Classe(_ecole, j);
                }

                NbreDeClassesParNiveau[i] = nbreDeClassesRemplies;
            }

			for (j = 0; j < totalEleveTemp; j++)
			{
				if (u >= NbreDeClassesParNiveau[i])
				{
					u = 0;
					indiceEleve ++;
				}

                printf("\n u = %d", u+totalClassesCopiees);

				Copier_Eleves(&_ecole->TabClasses[totalClassesCopiees + u].Tabeleves[j/NbreDeClassesParNiveau[i]], tempEleves[j]);
				_ecole->TabClasses[totalClassesCopiees + u].NbrEleves ++;
				u++;
			}
			totalClassesCopiees += NbreDeClassesParNiveau[i];
		}
		printf("Les classes ont ete reequilibrees.\n") ;
	}



	// Enfin, une fois les classes equilibrees, on les trie:
	for (i = 0; i < _ecole->nbreDeClasses; i++)
	{
		Tri_Eleves_Alphabetique(&_ecole->TabClasses[i]);
	}
}

void Trouver_Niveau_Inferieur(char *niveauInferieur, const char *niveauActuel)
{
	if (strcmp(niveauActuel, "CP"))
		strcpy(niveauInferieur, "NON");
	if (strcmp(niveauActuel, "CE1"))
		strcpy(niveauInferieur, "CP");
	if (strcmp(niveauActuel, "CE2"))
		strcpy(niveauInferieur, "CE1");
	if (strcmp(niveauActuel, "CM1"))
		strcpy(niveauInferieur, "CE2");
	if (strcmp(niveauActuel, "CM2"))
		strcpy(niveauInferieur, "CM1");
}


//Nicolas + Said
void Redoubler_Eleve(Ecole *_Ecole, Eleve *tempEleve)
{
	//Eleve tempEleve;

	char niveauxInferieur[4] = "";

	int indexClasse;
	int indexEleve;
	int i;
	int inscrit = 0;


	Creer_Nouvel_Eleve_Ou_Pas(tempEleve);

	for(i=0;i<_Ecole->nbreDeClasses;i++)
	{
		indexEleve = Rechercher_Eleve_Classe(_Ecole->TabClasses[i], *tempEleve);
		if(indexEleve > -1)
		{
			indexClasse = i;
			break;
		}
	}

	Trouver_Niveau_Inferieur(niveauxInferieur, _Ecole->TabClasses[indexClasse].AgeDeClasse.nom);

	for (i=0; i<_Ecole->nbreDeClasses ;i++)
	{
		if (strcmp(niveauxInferieur, _Ecole->TabClasses[i].AgeDeClasse.nom) == 0)
		{
			if (_Ecole->TabClasses[i].NbrEleves < _Ecole->nbreElevesParClasseMax)
			{
				Transfert_Eleve_Classe(_Ecole->TabClasses[indexClasse].Tabeleves[indexEleve], &_Ecole->TabClasses[i], &_Ecole->TabClasses[indexClasse], _Ecole->nbreElevesParClasseMax);
				Tri_Eleves_Alphabetique(&_Ecole->TabClasses[i]);
				inscrit = 1;
				break;
			}
		}
	}

	if (inscrit != 1)
	{
		Creer_Classe_Vide(_Ecole, niveauxInferieur);
		Transfert_Eleve_Classe(_Ecole->TabClasses[indexClasse].Tabeleves[indexEleve], &_Ecole->TabClasses[_Ecole->nbreDeClasses], &_Ecole->TabClasses[indexClasse], _Ecole->nbreElevesParClasseMax);
	}
}

//									***** ECOLE ************************************************************



//Said. Affichage d'école avec les classes et les eleves
void Afficher_Ecole( Ecole MonEcole)
{
	int i;
	// Imprimer chaque classe. BOUCLE
		printf("Ecole primaire %s annee scolaire %d\n %d classes avec %d d'eleves par classe maximum\n", MonEcole.nom, MonEcole.nbreDeClasses, MonEcole.nbreElevesParClasseMax, MonEcole.AnneeEnCours);
	// Imprimer chaque classe. BOUCLE
	 for(i=0;i<MonEcole.nbreDeClasses;i++)
	 {
		 Afficher_Classe(MonEcole.TabClasses[i]);
	 }
}


// Nicolas.
void Afficher_Ecole_Sans_Eleves(Ecole MonEcole)
{
	int i;
	// Imprimer chaque classe. BOUCLE
		printf("Ecole primaire %s annee scolaire %d\n %d classes avec %d d'eleves par classe maximum\n", MonEcole.nom, MonEcole.AnneeEnCours, MonEcole.nbreDeClasses, MonEcole.nbreElevesParClasseMax);
		
	// Imprimer chaque classe. BOUCLE
	 for(i=0;i<MonEcole.nbreDeClasses;i++)
	 {
		 Afficher_Classe_Sans_Eleves(MonEcole.TabClasses[i]) ;
	 }
}


//Daniel : affiche la liste de tous les eleves d'une ecole
void Afficher_Ecole_Tous_Eleves(Ecole MonEcole)
{
	int compteur = 0 ;
	int i = 0 ;
	// Affichage de tous les élèves.
	printf("\nLa liste des eleves est :\n") ;
	// Boucle classe par classe
	for (i = 0 ; i<MonEcole.nbreDeClasses ; i ++)
	{
		//boucle eleve par eleve
		for (compteur= 0 ; compteur < MonEcole.TabClasses[i].NbrEleves; compteur++)
			{
				Afficher_Eleve(MonEcole.TabClasses[i].Tabeleves[compteur]) ;
			}
	}
	printf("\n") ;
	return ;
}

//Said. Fonction pour saisir tous les paramètres de l'école
void Saisir_Ecole( Ecole *MonEcole)
{
	int i;
	//
	printf("entrez le nom de de l'ecole, le nombre de classes et le nombre d'eleves maximum autorise par classe.\nEntrez au format : nomdecole 1 25\n");
	scanf("%s %d %d",MonEcole->nom,&MonEcole->nbreDeClasses,&MonEcole->nbreElevesParClasseMax);
	getchar() ;
	printf("Entrez l'annee en cours au format AAAA\n");
	scanf("%d",&MonEcole->AnneeEnCours);
	getchar() ;

	// Saisir chaque classe < nbreDeClasses. BOUCLE

	for(i=0;i<MonEcole->nbreDeClasses;i++)
	{
		Saisir_Classe(&MonEcole->TabClasses[i],MonEcole->nbreElevesParClasseMax,MonEcole->AnneeEnCours);
	}
}

//Daniel
void Saisir_Ecole_Sans_Remplir_Classes(Ecole *MonEcole)
{
	printf("entrez le nom de de l'ecole, le nombre de classes et le nombre d'eleves maximum autorise par classe.\nEntrez au format : nomdecole 1 25\n");
	scanf("%s %d %d",MonEcole->nom,&MonEcole->nbreDeClasses,&MonEcole->nbreElevesParClasseMax);
	getchar();
	printf("Entrez l'annee en cours au format AAAA\n");
	scanf("%d",&MonEcole->AnneeEnCours);
	getchar();
}


// Daniel. Remets a zero toute l'ecole
void Reinitialiser_Ecole(Ecole *MonEcole)
{
	strcpy(MonEcole->nom, "") ;
	MonEcole->nbreDeClasses = 0 ;
	MonEcole->nbreElevesParClasseMax = 0 ;
	MonEcole->AnneeEnCours = 0 ;
	// Reinitialiser tableau de classes
	MonEcole->nbreDeProfs = 0 ;
	// reinitialiser tableau de profs
}


//Daniel. Permet de remplir une ecole
void Preremplir_Ecole(Ecole *MonEcole)
{
	int i = 0 ;
	Classe Classe_Temp[30] = {
			{{"superman", "Mr", 0},"CPA", 2, {"CP", {1, 1, 2012}, {31,12,2012}}, {{"rault", "nicolas", 10, {3, 8, 2012}},{"conte", "thibault", 10, {19, 10, 2012}}}},
			{{"marvel", "Mme", 1},"CE1A", 2, {"CE1", {1, 1, 2011}, {31,12,2011}}, {{"hattiti", "said", 10, {3, 8, 2011}},{"goriounov", "daniel", 10, {19, 10, 2011}}}},
			{{"batman", "Mr", 2},"CE2A", 2, {"CE2", {1, 1, 2010}, {31,12,2010}}, {{"moro", "joel", 10, {3, 8, 2010}},{"charge", "sonia", 10, {19, 10, 2010}}}},
			{{"doom", "Mr", 4},"CM1A", 2, {"CM1", {1, 1, 2009}, {31,12,2009}}, {{"tardif", "arnaud", 10, {3, 8, 2009}},{"cannillo", "pauline", 10, {19, 10, 2009}}}},
			{{"gamora", "Mme", -1},"CM2A", 2, {"CM2", {1, 1, 2008}, {31,12,2008}},{{"tessier", "tom", 10, {3, 8, 2008}},{"oriez", "jeremy", 10, {19, 10, 2008}}}}
		} ;
	Prof Prof_Temp[20] =
		{
			 {"superman", "Mr", 0},{"marvel", "Mme", 1},{"batman", "Mr", 2},{"batgirl", "Mme", 3},{"doom", "Mr", 4},{"gamora", "Mme", -1},{"deadpool", "Mr", -1},{"rogue", "Mme", -1},{"alfred", "Mr", -1},{"fantomette", "Mme", -1},{"wolverine", "Mr", -1},{"hitgirl", "Mme", -1}, {"", "", -1}
		} ;
	strcpy(MonEcole->nom, "Ecole Marvel DC") ;
	MonEcole->nbreDeClasses = 5 ;
	MonEcole->nbreElevesParClasseMax = 25 ;
	MonEcole->AnneeEnCours = 2018 ;
	// Reinitialiser tableau de classes
	for (i = 0 ; i< MonEcole->nbreDeClasses ; i++)
	{
		Copier_Classe(&MonEcole->TabClasses[i], Classe_Temp[i]) ;
	}
 		MonEcole->nbreDeProfs = 12 ;
	// reinitialiser tableau de profs
	for (i = 0 ; i<MonEcole->nbreDeProfs; i++)
	{
		Copier_Prof(&MonEcole->Professeur[i], Prof_Temp[i]) ;
	}
}

void Changer_Nom_Classe(Classe *_classe, const char *niveau, Ecole *_ecole)
{
    char identifiant[2] = "";

    strcpy(_classe->AgeDeClasse.nom, niveau);
    strcpy(_classe->NomDeClasse, niveau);
    identifiant[0] = Definir_Identifiant_Classe(niveau, _ecole);
    strcat(_classe->NomDeClasse, identifiant);
}


// Daniel + Nicolas avance ou recule l'annee scolaire d'un an.
void Modifier_Annee_Scolaire(Ecole *MonEcole)
{
	int compteur = 0 ;
	char reponse = ' ' ;
	int i = 0;
	int nbreClassesCM2 = 0;
	int etat = 0;

	int classesParNiveau [5] = {0,0,0,0,0};

	// On avance d'une année en cours.
	MonEcole->AnneeEnCours = MonEcole->AnneeEnCours + 1 ;

	Trier_Classes(MonEcole, classesParNiveau);  // on trie les classes au cas où et on récupere le nombre de classes par niveau (surtout le CM2 qui nous
													// interesse)
	// Je libere les profs de CM2
	for (i = MonEcole->nbreDeClasses -1; i > MonEcole->nbreDeClasses - classesParNiveau[i] -1; i --)
	{
		MonEcole->Professeur[Trouver_Prof_Dans_Ecole(*MonEcole, MonEcole->TabClasses[i].Professeur)].classeAssignee = -1;
	}

	MonEcole->nbreDeClasses -= classesParNiveau[4];     // on enleve les classes de CM2

	for (i = 0; i < MonEcole->nbreDeClasses; i++)       // pour chaque classe restante dans l'ecole on change le nom, et le niveau
	{
		if (strcmp(MonEcole->TabClasses[i].AgeDeClasse.nom, "CP")== 0)
		{
				Changer_Nom_Classe(&MonEcole->TabClasses[i], "CE1", MonEcole);
		}
		if (strcmp(MonEcole->TabClasses[i].AgeDeClasse.nom, "CE1")== 0)
		{
				Changer_Nom_Classe(&MonEcole->TabClasses[i], "CE2", MonEcole);
		}
		if (strcmp(MonEcole->TabClasses[i].AgeDeClasse.nom, "CE2")== 0)
		{
				Changer_Nom_Classe(&MonEcole->TabClasses[i], "CM1", MonEcole);
		}
		if (strcmp(MonEcole->TabClasses[i].AgeDeClasse.nom, "CM1")== 0)
		{
				Changer_Nom_Classe(&MonEcole->TabClasses[i], "CM2", MonEcole);
		}

		Modifier_Age_De_Classe(&MonEcole->TabClasses[i].AgeDeClasse, MonEcole->AnneeEnCours, &etat) ;
	}
}




//								!!!!!!!!!!!!!!!! 	FONCTIONS POUR LE MENU PRINCIPAL 	!!!!!!!!!!!!!!!!!!******************
//
//
// DANIEL.





// Fonction simple pour faire une pause et repartir avec entree
void Pause_Entree()
{
	//Faire une pause pour voir le resultat avant de continuer
	printf("\nTapez ENTREE pour continuer.");
	char tmpc = ' ' ;
	while (tmpc != '\n')
	{
		scanf("%c", &tmpc) ;
	}
}

// Verifie si pas de classe, puis si pas d'ecole, propose de creer ecole, puis de creer une classe d'un niveau au choix.
void Verifier_Creer_Si_Ecole_Vide(Ecole *Ecole_De_Menu, char *tmp_cdc)
{
	// Verifie si classe existe
	Age_De_Classe AGDC_local ;
	while (Ecole_De_Menu->nbreDeClasses == 0)
	{
		//Si ecole vide, la creer
		if ((strlen(Ecole_De_Menu->nom))==0)
		{
			printf("\nPas d'ecole. Merci de creer une ecole.\n") ;
			Saisir_Ecole_Sans_Remplir_Classes(Ecole_De_Menu) ;
		}

		// Si toujours pas de classe, la crer en fonction du type de classe choisi.
		if (Ecole_De_Menu->nbreDeClasses == 0)
		{
			Saisir_Age_De_Classe(&AGDC_local,Ecole_De_Menu->AnneeEnCours) ;
			Creer_Classe_Vide(Ecole_De_Menu, AGDC_local.nom) ;
			strcpy(tmp_cdc, AGDC_local.nom) ;
		}
	}
}


//Daniel. Afficher seulement les noms des classes
void Afficher_Toutes_Classes_Noms(Ecole MonEcole)
{
	int i = 0 ;
	printf("\nListe des classes existantes :\n");
	for (i=0 ; i< MonEcole.nbreDeClasses ; i++ )
	{
		printf("%s  ", MonEcole.TabClasses[i].NomDeClasse) ;
	}
	printf("\n") ;
}



// choix des classes a afficher et parmi laquelle je choisis une
// tmp_cdc est le nom de la classe, et tmp sa position dans le tableau de classes.
void Choisir_Classe_Afficher(Ecole MonEcole, char *tmp_cdc, int *tmp)
{
	int i = 0 ;
	int trouve = 0;
	Afficher_Toutes_Classes_Noms(MonEcole) ;

	if (MonEcole.nbreDeClasses <1)
	{
		while (trouve == 0)
		{
			printf("\nQuelle Classe souhaitez-vous choisir ? Entrez son nom.\n") ;
			scanf("%s", tmp_cdc) ;
			getchar() ;

			for (i=0; i<MonEcole.nbreDeClasses ; i++ )
			{
				if (strcmp(MonEcole.TabClasses[i].NomDeClasse, tmp_cdc)== 0)
				{
					*tmp = i ;
					trouve = 1 ;
				}
			}
			if (trouve == 0)
			{
				printf("\nPas de classe trouvee. ") ;
			}
		}

	}
}


//Daniel.
// Fonction du menu. Permet de choisir un eleve, l'affiche, propose de le supprimer, propose de modifier nom-prenom, puis date de naissance.
// propose de reaffecter automatiquement selon la date de naissance. Sinon, permet de transferer l'eleve manuellement.
void Modifier_Eleve(Eleve *Eleve_De_Menu, Ecole *Ecole_De_Menu)
{
	int rep = -1 ;
	int rep2 = -1 ;
	int classe_tmp = -1 ;
	int classe_dest = -1 ; //Stocke la classe de destination eventuelle pour le transfert de l'eleve.
	int position = -1 ; //Stocke la position de l'eleve dans le tableau d'eleve.
	char tmp_cdc[20] = " " ;
	//Je demande quel eleve je cherche. S'il n'est pas en stock.

	Afficher_Ecole_Tous_Eleves(*Ecole_De_Menu) ;
	printf("Quel eleve voulez-vous modifier ?\n") ;
	Creer_Nouvel_Eleve_Ou_Pas(Eleve_De_Menu) ;

	// Boucle tant que je ne trouve pas l'eleve, et je stocke la classe de l'eleve dans le tmp.
	while ((classe_tmp = Rechercher_Eleve_Ecole(*Ecole_De_Menu, *Eleve_De_Menu))<0)
	{
		// On propose de ressaisir l'eleve
		printf("L'eleve n'a pas ete trouve. Souhaitez-vous ressaisir un eleve ?\n") ;
		if ((rep = Oui_Non())== 1)
		{
			Saisir_Eleve(Eleve_De_Menu) ;
		}
		else {
			printf("Abandon de la modification d'eleve.\n") ;
			return ;
		}
	}
	position = Rechercher_Eleve_Classe(Ecole_De_Menu->TabClasses[classe_tmp], *Eleve_De_Menu) ;

	//Suppression de l'eleve ?
	printf("Souhaitez-vous supprimer l'eleve ?\n") ;
		if ((rep = Oui_Non())== 1)
		{
			Supprimer_Eleve(&Ecole_De_Menu->TabClasses[classe_tmp], position) ;
		}

	//Modification des donnees de l'eleve
	printf("Souhaitez-vous Modifier les noms et prenoms de l'eleve ?\n") ;
	if ((rep2 = Oui_Non())== 1)
	{
		printf("Quel est le nouveau nom ?\n") ;
		do {
			scanf("%s", Eleve_De_Menu->nom) ;
			getchar();
		} while (strlen(Eleve_De_Menu->nom)< 2 ) ;

		printf("Quel est le nouveau prenom ?\n") ;
		do {
			scanf("%s", Eleve_De_Menu->prenom) ;
			getchar() ;
		} while (strlen(Eleve_De_Menu->prenom)< 2 ) ;
	}

	rep = Eleve_De_Menu->DateDeNaissance.an ; // Je stocke l'annee de naissance.
	// je demande si je saisis la date de naissance
	printf("Souhaitez-vous Modifier la date de naissance de l'eleve ?\n") ;
	if ((rep2 = Oui_Non())== 1)
	{
		Saisir_Date(&Eleve_De_Menu->DateDeNaissance) ;
		// Je propose d'assigner l'eleve directement a une nouvelle classe si l'annee de naissance a change
		if (rep != Eleve_De_Menu->DateDeNaissance.an)
		{
			printf("Souhaitez-vous Assigner automatiquement l'eleve a une nouvelle classe ?") ;
			if (Oui_Non() == 1)
			{
				Assigner_Eleve_Sans_rechercher(*Eleve_De_Menu, Ecole_De_Menu) ;
				//Il faut supprimer l'eleve precedent.
				Supprimer_Eleve(&Ecole_De_Menu->TabClasses[classe_tmp], position) ;
				printf("Eleve deplace.\n") ;
				// comme j'ai deplace l'eleve, je considere que j'ai tout fait avec, donc je sors de ma fonction.
				return ;
			}
		}
	}

	// Si je n'ai pas deplace l'eleve, je demande si je veux le deplacer
	printf("Souhaitez-vous transferer l'eleve dans une autre classe ?\n") ;
	if (Oui_Non() == 1)
	{
		classe_dest = -1 ;
		Choisir_Classe_Afficher(*Ecole_De_Menu, tmp_cdc, &classe_dest) ;

		Transfert_Eleve_Classe(Ecole_De_Menu->TabClasses[classe_tmp].Tabeleves[position], &Ecole_De_Menu->TabClasses[classe_dest], &Ecole_De_Menu->TabClasses[classe_tmp], Ecole_De_Menu->nbreElevesParClasseMax) ;
		printf("L'eleve a ete deplace.\n") ;
	}

}



//								!!!!!!!!!!!!!!!! 	FONCTIONS SPECIFIQUES AU MENU PRINCIPAL 	!!!!!!!!!!!!!!!!!!******************
//menu et fonctions de menu par Daniel


// Affiche une commande en fonction d'afficher_menu
// A FAIRE : limiter la longueur d'affichage
void Afficher_Commande(int afficher_menu, Commande MaCommande)
{
	char explication_tmp[] = " " ;
	if (afficher_menu == 0 || afficher_menu == 1 || afficher_menu == 2 || afficher_menu == 3 || afficher_menu == 4)
	{
		printf("%-10s", MaCommande.nom ) ;

		if (afficher_menu == 1 || afficher_menu == 2 || afficher_menu == 3 || afficher_menu == 4)
		{
			printf("%-35s", MaCommande.description) ;
			if (afficher_menu == 2 || afficher_menu == 3)
			{
				printf("%s", MaCommande.explication) ;
				printf("\n") ;
			}
			if (afficher_menu == 4 || afficher_menu == 1) {
				printf("\n") ;
			}
		}
	}
}


// En fonction d'afficher_menu, afficher ou pas les commandes a partir du tableau
void Afficher_Toutes_Commandes(int afficher_menu, Commande TabCommandes[])
{
	if (afficher_menu == 0 || afficher_menu == 1 || afficher_menu == 2 || afficher_menu == 3 || afficher_menu == 4)
	{
		int i = 0;
		printf("\t\t**** LISTE DES COMMANDES ****\n") ;
		for(i=0; (strlen(TabCommandes[i].nom))!= 0; i++)
		{
			Afficher_Commande(afficher_menu, TabCommandes[i]) ;
		}
		printf("\n") ;
	}
}


// en fonction d'afficher_menu, afficher ou pas le menu.
void Afficher_Menu_Description(int etat, int afficher_menu)
{
	printf("\n\t\t**** MENU PRINCIPAL ****\n");
	if (afficher_menu == 5 || afficher_menu == 3 || afficher_menu == 4)
	{
		printf("\nVous etes dans le menu du programme MEGAECOLE 2030.\n");
		printf("Ce Programme permet de gerer une ecole entiere. La liste des commandes qui suit vous permet d'interagir avec le programme.\n") ;
	}

	// a afficher si les commandes ne sont pas affichees, comme rappel
	if (afficher_menu == 0 || afficher_menu == 5 || afficher_menu == 6 )
	{
		printf("Tapez AFM pour modifier l'affichage du menu et des commandes\n") ;
	}
	printf("Tapez FIN pour sortir du Programme et tout reinitialiser.\n\n") ;
}


// Modifier afficher_menu pour modifier ce qu'on affiche du menu et des commandes
void Modifier_Affichage_Menu(int *afficher_menu)
{
	*afficher_menu = -1 ;
	printf("\n\t\t****MODIFICATION DE L'AFFICHAGE DU MENU ET DES COMMANDES****\n") ;
	do
	{
		printf("Entrer une des commandes qui suit :\n") ;
		printf("0\tListe de noms de commandes. Pas de menu\n") ;
		printf("1\tCommandes sans l'explication longue. Pas de menu.\n") ;
		printf("2\tCommandes completes. Pas de menu.\n") ;
		printf("3\tAFFICHE TOUT.\n") ;
		printf("4\tCommandes sans l'explication. Menu complet.\n") ;
		printf("5\tPas de commandes. Menu complet.\n") ;
		printf("6\tN'AFFICHE RIEN.\n") ;
		printf("\nVotre choix :\n") ;
		scanf("%d", &*afficher_menu) ;
		getchar() ;
	} while (*afficher_menu < 0 ||  *afficher_menu > 6) ;
	printf("\nVotre choix d'affichage %d a bien ete selectionne.\n", *afficher_menu) ;
}


// Scanf de la commande
void Rentrer_Commande(char CDE[4])
{
	printf("Rentrez une commande en majuscules parmi la liste des commandes et tapez entree.\n") ;
	scanf("%s", CDE) ;
	getchar() ;
}


// DANIEL. 					!!!!!!!!!!!!!! 			MENU PRINCIPAL D'ACCÈS  AU PROGRAMME 		!!!!!!!!!!!!!!!!!!!!
void Menu_Principal()
{
	// 												Variables de stockage du Menu
	Date Date_De_Menu ;
	Reinitialiser_Date(&Date_De_Menu) ;
	Age_De_Classe AgeDC_De_Menu ;
	Prof Prof_De_Menu ;
	Prof TabProfs[20] ;
	Eleve Eleve_De_Menu ;
	Reinitialiser_Eleve(&Eleve_De_Menu) ;
	Eleve TabEleves[25] ;
	Classe Classe_De_Menu ;
	Classe TabClasses[20] ;
	Ecole Ecole_De_Menu ;
	Prof TabProfesseurs[20] =
	{
		 {"Superman", "Mr", -1},{"Marvel", "Mme", -1},{"Batman", "Mr", -1},{"Batgirl", "Mme", -1},{"Doom", "Mr", -1},{"Gamora", "Mme", -1},{"Deadpool", "Mr", -1},{"Rogue", "Mme", -1},{"Alfred", "Mr", -1},{"Fantomette", "Mme", -1},{"Wolverine", "Mr", -1},{"Hitgirl", "Mme", -1}, {"", "", -1}
	} ;
	// Tableau des commandes au format Commande. NE PAS MODIFIER !
	Commande TabCdes[30] =
	{
		{ "REE", "Reinitialiser tout", "Reinitialise toute l'ecole et relance le menu a neuf.", -3},
		{ "MEN", "Revenir au menu", "Retour au menu. Affiche a nouveau le menu complet et toutes les commandes.", -2},
		{ "PRE", "Reremplir tout", "Revient a l'ecole preremplie standard et relance le menu a neuf.", -1},
		{ "FIN", "Sortir du menu", "Met fin a l'ensemble du programme du menu. Reinitialise tout.", 0},
		{ "AFM" , "Modifier l'affichage du menu" , "Si vous voulez afficher ou faire disparaitre l'affichage du menu et des commandes" , 1 },
		{ "INS" , "Inscrire un eleve"  , "Selectionne automatiquement la bonne classe et en cree une si besoin."  , 2},
		{ "INC" , "Inscrire eleve dans X classe" , "Choisir manuellement une classe specifique pour inscrire l'eleve"  , 3 },
		{ "AFE", "Afficher l'ecole" , "Afficher l'ecole entiere et tous ses composants." , 4 },
		{ "AFC", "Afficher une classe" , "Afficher la classe entiere et tous ses composants." , 5 },
		{ "AFP" , "Afficher les profs" , "Affiche la liste complete des professeurs de l'ecole."  , 6 },
		{ "RCH", "Rechercher un eleve", "Cherche si un eleve est deja enregistre dans l'ecole et indique ou.", 7},
		{ "AJP", "Ajouter un prof", "Ajoute un professeur a l'ecole.", 8},
		{ "RTP", "Retirer un prof", "Retire un professeur de la liste des professeurs de l'ecole", 9},
		{ "RMP", "Remplacer un prof", "Remplace un prof dans une classe par un autre", 10},
		{ "CAS", "Avancer d'une annee" , "Passe a l'annee scolaire suivante et change toute l'ecole pour s'adapter" , 11},
		{ "DSE", "Desinscrire un eleve", "Desinscrit un eleve de l'ecole et le supprime.", 12},
		{ "MDE", "Modifier un eleve", "Permet de supprimer un eleve, changer le nom, le prenom, la date de naissance et la classe d'un eleve, le reaffecter automatiquement ou manuellement.", 13} ,
		{ "AJC", "Ajouter une classe", "Ajoute une nouvelle classe a l'ecole", 14} ,
		{ "SCL", "Supprimer une classe", "Supprime une classe de l'ecole", 15},
		{ "REQ", "Reequilibrer les classes", "Repartit les eleves des classes de chaque niveau de maniere equilibree.", 16},
		{ "RDB", "Faire redoubler un eleve", "Fais redoubler un eleve en l'envoyant dans une classe plus petite.", 17},
		{ "TRE", "Transferer un eleve", "Transfere un eleve de sa classe a une classe au choix.", 18},
		{ "AFT", "Afficher tous les eleves", "Affche tous les eleves de l'ecole sous forme de liste", 20} ,
		//{ ,  ,  , },
		//{ ,  ,  , },
		//{ ,  ,  , },
	} ;

	/* Stockage de la commande. */
	char CDE[4] = "" ;
	/*Stockage de la bonne commande. > 0 = oui */
	int commmande_trouvee = 0 ;
	// Etat en cours du menu.
	int etat = -1 ;
	// Etat d'affichage du menu.
	int afficher_menu = 3 ;
	// temporaire
	int tmp ;
	char tmp_cdc[20] ;
	int i = 0 ;
	int j = 0 ;
	char tmpc = ' ' ;

	// 																	BOUCLE 0 : MENU TOUT
	// BOUCLE 0. Si FIN ou etat 0, fin du menu et sortie du code
	while(strcmp(CDE, "FIN")!= 0 && etat != 0 )
	{

		// Reinitialisation de l'ecole si etat -1
		if (etat == -3)
		{
			printf("\nEcole Reinitialisee a zero.\n") ;
			Reinitialiser_Ecole(&Ecole_De_Menu) ;
			Afficher_Ecole_Sans_Eleves(Ecole_De_Menu) ;
			printf("\n") ;
			Pause_Entree() ;
			etat = -2 ; // remise a neuf de l'etat
		}


		// Preremplissage de l'etat
		if (etat == -1)
		{
			printf("\n....Ecole Preremplie chargee") ;
			Preremplir_Ecole(&Ecole_De_Menu) ;
			//Afficher_Ecole_Sans_Eleves(Ecole_De_Menu) ;
			printf("\n") ;
			//Pause_Entree() ;
			etat = -2 ; // remise a neuf de l'etat
		}


		// 																	BOUCLE 1 : SELECTION DE L'ETAT
		// BOUCLE 1 qui vérifie si une bonne commande a ete entree.
		// !! Bien classer par etat (du plus petit au plus grand. !!
		// Ici, on attribue l'etat en fonction de la commande rentree. etape de tri.
		do
		{
			// Affichage conditionnel des commandes. Redemande tant que bonne commande non rentree.

			if (etat == -1 || etat == -2 || etat == -3)
			{
				// Affichage conditionnel du menu.
				Afficher_Menu_Description(etat, afficher_menu) ;
			}
			Afficher_Toutes_Commandes(afficher_menu, TabCdes) ;
			Rentrer_Commande(CDE) ;

			// reinitialisation de tout
			if (strcmp(CDE, "REE")== 0 )
			{
				etat = -3 ;
				afficher_menu = 2 ;
			}
			//Retour au menu
			if (strcmp(CDE, "MEN")== 0 )
			{
				etat = -2 ;
				afficher_menu = 2 ;
			}
			//Preremplit l'ecole et efface la precedente.
			if (strcmp(CDE, "PRE")== 0 )
			{
				etat = -1 ;
			}
			// Met fin au programme
			if (strcmp(CDE, "FIN")== 0 )
			{
				etat = 0 ;
			}
			//Modifier l'affichage
			if (strcmp(CDE, "AFM")== 0 )
			{
				etat = 1 ;
			}
			// Inscrit un eleve automatiquement
			if (strcmp(CDE, "INS")== 0 )
			{
				etat = 2 ;
			}
			// Inscrit un eleve dans une classe precise
			if (strcmp(CDE, "INC")== 0 )
			{
				etat = 3 ;
			}
			//Afficher l'ecole
			if (strcmp(CDE, "AFE")== 0 )
			{
				etat = 4 ;
			}

			//Afficher une classe
			if (strcmp(CDE, "AFC")== 0 )
			{
				etat = 5 ;
			}

			//Afficher la liste des profs
			if (strcmp(CDE, "AFP")== 0 )
			{
				etat = 6 ;
			}
			// Recherche un eleve et indique sa classe s'il existe
			if (strcmp(CDE, "RCH")== 0 )
			{
				etat = 7 ;
			}
			//Ajouter un professeur.
			if (strcmp(CDE, "AJP")== 0 )
			{
				etat = 8 ;
			}
			if (strcmp(CDE, "RTP")== 0 )
			{
				etat = 9 ;
			}
			//Remplace un prof par un autre dans une classe.
			if (strcmp(CDE, "RMP")== 0 )
			{
				etat = 10 ;
			}
			//Avance d'une annee
			if (strcmp(CDE, "CAS")== 0)
			{
				etat = 11;
			}
			
			//said desinscrire un eleve
			if (strcmp(CDE, "DSE")== 0)
			{
				etat = 12 ;
			}

			//Modifie un eleve
			if (strcmp(CDE, "MDE")== 0)
			{
				etat = 13 ;
			}
			
			//rajoute une classe
			if (strcmp(CDE, "AJC")== 0)
			{
				etat = 14;
			}
			
			// Supprime une classe
			if (strcmp(CDE, "SCL")== 0)
			{
				etat = 15 ;
			}
			
			//Reequilibre les classes
			if (strcmp(CDE, "REQ")== 0)
			{
				etat = 16 ;
			}
			
			// redoubler eleve
			if (strcmp(CDE, "RDB")== 0)
			{
				etat = 17 ;
			}

			//Transferer eleve
			if (strcmp(CDE, "TRE")== 0)
			{
				etat = 18;
			}
			//Affiche une liste des eleves de l'ecole
			if (strcmp(CDE, "AFT")== 0)
			{
				etat = 20 ;
			}
			


		// 																	FIN BOUCLE 1
		// FIN BOUCLE 1. Verifie si une bonne commande a ete rentree.
		} while (etat < -3 || etat == -2) ;


		// APPELS DE FONCTIONS UNE FOIS UNE COMMAND SELECTIONNEE.
		// !! BIEN ORDONNER PAR ETAT, DU PLUS PETIT AU PLUS GRAND
		// DECLENCHEMENT EN FONCTION DES ETATS/ Penser a bien remettre l'etat a -2 apres chaque appel de fonction
		// BOUCLE 2
		// 																	BOUCLE 2 : APPELS DE FONCTIONS
		do
		{
//AFF = modifer l'affichage du menu.
			if (etat == 1)
			{
				Modifier_Affichage_Menu(&afficher_menu) ;
				etat = -2 ;
			}

//Inscrire un eleve automatiquement.
			if (etat == 2)
			{
				Creer_Nouvel_Eleve_Ou_Pas(&Eleve_De_Menu) ;

				Assigner_Eleve(Eleve_De_Menu, &Ecole_De_Menu) ;
				etat = -2 ;
			}

//Inscrire un eleve dans une classe specifique.
			if (etat == 3)
			{
				Creer_Nouvel_Eleve_Ou_Pas(&Eleve_De_Menu) ;

				strcpy(tmp_cdc, "") ;
				Verifier_Creer_Si_Ecole_Vide(&Ecole_De_Menu, tmp_cdc) ;

				// si pas eu besoin de creer ecole ni classe, je choisis la classe
				if (strcmp(tmp_cdc, "")== 0)
				{
					Choisir_Classe_Afficher(Ecole_De_Menu, tmp_cdc, &tmp) ;
				}
				// sinon, je prend en index la premiere classe du tableau de classes
				else
				{
					tmp = 0 ;
				}

				// A FAIRE
				// Verifier nombre d'eleves
				// Verifier age
				Assigner_Eleve_Dans_Classe(Eleve_De_Menu, &Ecole_De_Menu.TabClasses[tmp], Ecole_De_Menu.nbreElevesParClasseMax) ;

				etat = -2 ;
			}

//Affiche l'ecole
			if (etat == 4)
			{
				Afficher_Ecole_Sans_Eleves(Ecole_De_Menu) ;
				etat = -2 ;
			}

//Affiche Classe
			if (etat == 5)
			{
				printf("\nQuelle classe souhaitez-vous choisir ? Entrez son nom au choix parmi les suivantes.\n") ;
				//Afficher_Toutes_Les_Classes(Ecole_De_Menu) ;

				tmp = -1 ;
				Choisir_Classe_Afficher(Ecole_De_Menu, tmp_cdc, &tmp) ;

				if (tmp > 0)
				{
					Afficher_Classe(Ecole_De_Menu.TabClasses[tmp]) ;
				}
				else
				{
					printf("Il n'y a pas de Classe a afficher.\n") ;
				}			}

//Affiche la liste des profs
			if (etat == 6)
			{
				printf("\nL'ecole %s a %d professeurs :\n", Ecole_De_Menu.nom, Ecole_De_Menu.nbreDeProfs) ;
				Afficher_Tab_Prof(Ecole_De_Menu.Professeur, Ecole_De_Menu.TabClasses, Ecole_De_Menu.nbreDeProfs) ;
				etat = -2 ;
			}

//Recherche un eleve dans l'ecole
			if (etat == 7)
			{
				Creer_Nouvel_Eleve_Ou_Pas(&Eleve_De_Menu) ;
				// Je stocke l'adresse de la classe de l'eleve
				tmp = Rechercher_Eleve_Ecole(Ecole_De_Menu, Eleve_De_Menu) ;

				//Si on trouve un eleve, on imprime sa classe
				if (tmp > -1)
				{
					printf("\n L'eleve existe deja dans la classe %s.\n", Ecole_De_Menu.TabClasses[tmp].NomDeClasse) ;
				}
				// Si on ne trouve pas l'eleve, on propose de l'inscrire
				else
				{
					printf("\nL'eleve n'a pas ete trouve. Il n'est donc pas encore inscrit.\nSouhaitez-vous l'enregistrer dans une classe adaptee ?\n") ;
					tmp = Oui_Non() ;
					if ((tmp = Oui_Non() )== 1)
					{
						Assigner_Eleve(Eleve_De_Menu, &Ecole_De_Menu) ;
					}
					else
					{
						printf("\nL'eleve est stocke dans la memoire tampon du menu. Vous pouvez le reutiliser a la prochaine commande.\n") ;
					}
				}
				etat = -2 ;
			}

// Ajouter un prof a l'ecole
			if (etat == 8)
			{
				Ajouter_Prof_A_Ecole(&Ecole_De_Menu) ;
				etat = -2 ;
			}

// RTP Je retire un prof de l'ecole
			if (etat == 9)
			{
				Supprimer_Prof_Ecole(&Ecole_De_Menu) ;
				etat = -2 ;
			}
			
//Remplacer prof
			if (etat == 10)
			{
				Remplacer_Prof(&Ecole_De_Menu) ;
				etat = -2 ;
			}
			
//Nicolas. Changer d'annee
			if (etat == 11)
			{
				printf("\nVoulez-vous passer a l\'annee suivante (annee %d)?\n", Ecole_De_Menu.AnneeEnCours +1);
				if (Oui_Non() == 1)
				{
					Modifier_Annee_Scolaire(&Ecole_De_Menu);
					printf("\nNouvelle annee scolaire: %d - %d", Ecole_De_Menu.AnneeEnCours, Ecole_De_Menu.AnneeEnCours +1);
				}
				else
					printf("L'annee n\'a pas ete changee. Annee scolaire en cours: %d - %d", Ecole_De_Menu.AnneeEnCours, Ecole_De_Menu.AnneeEnCours +1);

				etat = -2;
			}

			//SAID Desinscrire un eleve dans une classe de l'ecole
			if (etat == 12)
				// si lutilisateur veut abandonné on ne lexecute pas
				// mais si on a un bon eleve a ce stade il faut que je trouve sa classe et sa position index dans leleve je les stocke dans les bonnes variable temporaire dans le menu
				// jappelle ma fonction supprimer eleve sur ECOLE DU MENU ATTENTION
			{    // dermande daffichage a desinscrire
				printf("quel eleve voulez-vous désinscrire ?\n ");
				 // on appel la foncvtion du menu cree eleve ou pas
				// condition  boucle si leleve nexiste pas demande a lutilisateur demande si saisir un eleve ou de passer a la suite (par printf)
				Creer_Nouvel_Eleve_Ou_Pas(&Eleve_De_Menu) ;// sa renvoie -1 si on trouve pas l'eleve
				while ( (tmp = Rechercher_Eleve_Ecole(Ecole_De_Menu,Eleve_De_Menu)) == -1)
				{
					Saisir_Eleve(&Eleve_De_Menu);
				}
				i =  Rechercher_Eleve_Classe(Ecole_De_Menu.TabClasses[tmp], Eleve_De_Menu);

				Supprimer_Eleve(&Ecole_De_Menu.TabClasses[tmp],i);

			}

// Modifier un eleve
			//Trouver un eleve, Afficher ses donnees, proposer de modifier et laisser ou reaffecter dans la classe, ou supprimer.
			if (etat == 13)
			{
				// Tant que je ne trouve pas l'eleve, je continue.
				Modifier_Eleve(&Eleve_De_Menu, &Ecole_De_Menu) ;

				etat = -2 ;
			}
			
// Rajouter une classe			
			if (etat == 14)
			{
				Ajouter_Classe(&Ecole_De_Menu) ;
				etat = -2 ;
			}
			
// Supprimer une classe		
			if (etat == 15)
			{
				tmp = -1 ;
				while (tmp<0)
				{
					printf("Quelle classe souhaitez-vous supprimer parmi la liste des classes suivantes :\n");
					Afficher_Toutes_Classes_Noms(Ecole_De_Menu) ;
					scanf("%s", tmp_cdc) ;
					getchar() ;
					for (i = 0 ; i < Ecole_De_Menu.nbreDeClasses; i++)
					{
						if (strcmp(Ecole_De_Menu.TabClasses[i].NomDeClasse, tmp_cdc) == 0)
						{
							tmp = i ;
							break ;
						}
					}
				}
				
				Supprimer_Classe(&Ecole_De_Menu, tmp) ;
				printf("Classe Supprimee.\n") ;
				etat = -2 ;
			}
			

// Reequlibre les classes  de l'ecole en repartissant tous les eleves bien
			if (etat == 16)
			{
				Equilibrer_Classes(&Ecole_De_Menu) ;
				etat = -2 ;
			}

// Fais redoubler un eleve.
			if (etat == 17)
			{
				Redoubler_Eleve(&Ecole_De_Menu, &Eleve_De_Menu) ;
				etat = -2 ;
			}

// Transferer un eleve			
			if (etat == 18)
			{
				Afficher_Ecole_Tous_Eleves(Ecole_De_Menu) ;
				Creer_Nouvel_Eleve_Ou_Pas(&Eleve_De_Menu);
				Transferer_Eleve(Eleve_De_Menu, &Ecole_De_Menu);

				etat = -2;
			}

//Affiche tous les eleves de l'ecole
			if (etat == 20)
			{
				Afficher_Ecole_Tous_Eleves(Ecole_De_Menu) ;
				etat = -2 ;
			}

		// FIN BOUCLE 2
		} while (etat >0 ) ;

		//Faire une pause pour voir le resultat avant de continuer
		if (etat != 0 && etat != -3 && etat != -1) // Enlever pour etat 1 : modification de menu. Mais comment ?
		{
			getchar() ;
			Pause_Entree() ;
		}
		//BOUCLE 0 : FIN
	}

	// Impression de fin de programme et sortie
	printf("\n\nMerci d'avoir utilise MEGAECOLE 2030\n") ;
	printf("\nProgramme code par :\nNicolas Rault\nThibault Conte\nSaid Hattiti\nDaniel Goriounov.\n\n") ;
	return ;

}

/*INSTRUCTIONS MENU :
ETAT.
A passer à chaque fonction interne au menu en modifiable. TOUJOURS !
Témoin de l'état du menu (où il en est) qui controle l'execution des fonctions et autre
-2	A la sortie de l'execution d'un etat, remet a -2. Permet de relancer la boucle principale
-1	Début du programme. Ou lors de reinitialisation de tout
0	FIN	 	fin du programme et sortie de la fonction menu
1	AFM	 	Selection de l'affichage du menu.
2	INS		Inscrire un eleve
3	INC		Inscrire un eleve dans une classe precise
4 	AFE		Afficher l'ecole
5	AFC		Afficher une classe
6	AFP		Afficher la liste des professeurs
7	RCH		Rechercher un eleve

AFFICHAGE_MENU.
Temoin qui permet a l'utilisateur de selectionner s'il veut afficher le menu/commandes ou pas
0	Liste de commandes
1	commandes partielles sans menu.
2	commandes complètes sans menu
3	tout afficher
4	menu et commandes sans l'explication
5 	menu sans commandes
6	rien

*/



//CODE PRINCIPAL
int main()
{
    Menu_Principal() ;
}
