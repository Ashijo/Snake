#include <iostream>
#include <windows.h>
#include <conio.h>	// pour _kbhit() et _getch()
#include <time.h>	// pour time(int)
#include <stdlib.h>	// pour srand(int) et rand()
#include "RockUtiles/RockUtiles.h"// pour la gestion de l'écran
using namespace std;


/**********************************************	declaration des constante	*****************************************************/

const int debutLigne = 5, debutCol = 5, nbLignes = 30, nbColonnes = 70, couleur = 15;//Definition du debut de ligne , de colonne et la longueur des colonnes et des lignes
const int flecheHaut = 72, flecheBas = 80, flecheDroite = 77, flecheGauche = 75;//Definition codes ascii des fleches
const int directionDroite = 0, directionGauche = 1, directionHaut = 2, directionBas = 3;
const int tailleSerp = 150;


/**************************************************	Declaration des fonctions *********************************************/
void afficherTerrain(int nbLignes, int nbColonnes);
void afficherScore(int colo, int timerScore, int tailleSerpent);
int recupererTouche();
int calculerDirectionTouche(int touche);
void positionAleatoire(int nbLignes, int nbColonnes, int &posX, int &posY);
void afficherGameOver(int nbLignes, int nbColonnes);
bool testerCollisionFatal(int serpentX[], int serpentY[], int tailleSerpent, int nbColonnes, int nbLignes);
//void deplacerSerpentI(int direction, int &posX, int &posY); //fonction deplacer serpent I en suspent pour la suite du TP
int saisirNiveau();
int incrementerNiveau(int niveau, int &nbSouris, int &nbSourisGold, int &tailleSerpent, int serpentX[], int serpentY[], int sourisX[], int sourisY[],int sourisGoldX[], int sourisGoldY[]);
void creerSouris(int nbLignes, int nbColonnes, int sourisX[], int sourisY[], int nbSouris);
void creerSourisGold(int nbLignes, int nbColonnes, int sourisGoldX[], int sourisGoldY[], int nbSourisGold);
void afficherSouris(int sourisX[], int sourisY[], int nbSouris);
void afficherSourisGold(int sourisX[], int sourisY[], int nbSourisGold);// fonction supplementaire pour des souris bonus
void deplacerSerpentII(int direction, int serpentX[], int serpentY[], int &tailleSerpent, int sourisX[], int sourisY[], int &nbSouris);
bool testerCollision(int posX, int posY, int sourisX[], int sourisY[], int &nbSouris);
bool testerCollisionGold(int serpentX[], int serpentY[], int sourisGoldX[], int sourisGoldY[], int &nbSourisGold);



/********************************************************* fonction principale  **********************************************************/

int main()
{
	int posX, posY;
	int touche;
	int nbSouris, nbSourisGold;
	int niveau;
	int timerScore = 00;
	bool alive = true;
	int colo;
	int tempsX = clock() / 1000;
	int tempsY = 0;
	int direction = rand() % 4;
	int serpentX[tailleSerp], serpentY[tailleSerp], sourisX[50], sourisY[50], sourisGoldX[50], sourisGoldY[50];
	int tailleSerpent = 1;


	curseurVisible(false);//effacer le curseur 
	setDimensionFenetre(0, 2, nbColonnes + 40, nbLignes + 10); // dimentionner la fenetre
	afficherTerrain(nbLignes, nbColonnes);// afficher le terain
	afficherScore(10, timerScore, tailleSerpent);//afficher le tableau des score



	//Selection du premier niveau creation du nombre de souris approprié
	niveau = saisirNiveau();
	nbSouris = niveau * 2 + 2;
	nbSourisGold = nbSouris / 5;

	//Selection de la position aleatoir pour la tete
	positionAleatoire(nbLignes, nbColonnes, posX, posY);

	//donner la position aleatoir de la tete au tableau serpent
	serpentX[0]=posX;
	serpentY[0]=posY;


	//creation des souris grises
	creerSouris(nbLignes, nbColonnes, sourisX, sourisY, nbSouris);
	afficherSouris(sourisX, sourisY, nbSouris);

	//creation des souris dorée
	creerSouris(nbLignes, nbColonnes, sourisGoldX, sourisGoldY, nbSourisGold);
	afficherSourisGold(sourisGoldX, sourisGoldY, nbSourisGold);

	//Debut de la boucle principale
	do {

		touche = recupererTouche();
		int nouvelleDirection = calculerDirectionTouche(touche);

		if (nouvelleDirection != -1) {
			direction = nouvelleDirection;
		}

		if (nbSouris < 1) {
			incrementerNiveau( niveau,  nbSouris, nbSourisGold, tailleSerpent, serpentX, serpentY, sourisX, sourisY, sourisGoldX, sourisGoldY);
		}


		//lancement du serpent
		deplacerSerpentII(direction, serpentX, serpentY, tailleSerpent, sourisX, sourisY, nbSouris);


		//test de la collision avec les souris dorées
		if (testerCollisionGold(serpentX, serpentY, sourisGoldX, sourisGoldY, nbSourisGold)) {
			tailleSerpent+=2;
		}

		//attribution des coordonée de tete du serpent
		posX = serpentX[0];
		posY = serpentY[0];

		//test de la colision des souris grises
		if (testerCollision(posX, posY, sourisX, sourisY, nbSouris)) {
			tailleSerpent++;
		}



		//appel de la fonction qui affiche le score et le timer
		if (tempsX != clock() / 1000) {
			tempsX = clock() / 1000;
			tempsY++;
			colo = rand() % 14 + 1;
			afficherScore(colo, tempsY, tailleSerpent);

		}

		//verifie que le serpen soit toujours vivant
		alive = testerCollisionFatal(serpentX, serpentY, tailleSerpent, nbColonnes, nbLignes);

	} while (alive);//fin de la boucle principal

	//affichage du game over
	afficherGameOver(nbLignes, nbColonnes);

	return 0;
}



/*********************************************************** DEBUT PARTIE I ********************************************************************/

void afficherTerrain(int nbLignes, int nbColonnes)
/*
Tâche: afficher le contour du terrain
Paramètres: le nombre de lignes et de colonnes du terrain rectangulaire
*/
{
	//affichage du terrain
	int finLignes, finColonne;
	finLignes = debutLigne + nbLignes;
	finColonne = debutCol + nbColonnes;
	cadre(debutLigne, debutCol, finColonne, finLignes, couleur);



}

void afficherScore(int colo, int timerScore, int tailleSerpent) {
	//affichage du tableau de score
	cadre(80, 5, 105, 20, couleur);

	int timerSec = timerScore % 60;
	int timerMin = timerScore / 60;
	int pts = 0;

	color(colo);
	gotoXY(81, 6);
	cout << "#### #  #  ##  #  # ####";

	color(colo);
	gotoXY(81, 7);
	cout << "#    ## # #  # # #  #";

	color(colo);
	gotoXY(81, 8);
	cout << "#### #### #### ##   ##";

	color(colo);
	gotoXY(81, 9);
	cout << "   # # ## #  # # #  #";

	color(colo);
	gotoXY(81, 10);
	cout << "#### #  # #  # #  # ####";

	color(15);
	gotoXY(91, 12);
	cout << "SCORE";

	//Decompte et affichage des points

	pts = tailleSerpent * 10 - 10;
	gotoXY(93, 14);
	cout << pts;

	//Le timer
	if (timerSec < 10) {
		gotoXY(92, 18);
		cout << timerMin << ":0" << timerSec;
	}

	else {
		gotoXY(92, 18);
		cout << timerMin << ":" << timerSec;
	}
}

int recupererTouche()
/*
Tâche: tester si le joueur a appuyé sur une touche
Retour: retourner le code ASCII de la touche pressée, -1 sinon
*/
{
	char touche = -1;

	if (_kbhit() != 0) {
		touche = _getch();
	}

	return touche;
}

int calculerDirectionTouche(int touche)
/*
Tâche: calculer la direction correspondant à une touche ou -1
Paramètre: le code d'une touche (w, a, s ou d)
Retour: la direction qui correspond à la touche
(0: droite, 1: gauche, 2: haut, 3: bas)
*/
{
	int direction = -1;


	switch (touche) {
	case 100: direction = 0; break;
	case 97:direction = 1; break;
	case 119:direction = 2; break;
	case 115:direction = 3; break;


	}
	return direction;
}

void positionAleatoire(int nbLignes, int nbColonnes, int &posX, int &posY)
/*
Tâche: calculer une position aléatoire sur le terrain
Paramètres: les dimensions du terrain en entrée et les coordonnées de la 				position aléatoire en sortie
*/

{
	srand((int)time(NULL));
	posX = (int) rand() % (nbColonnes - 1) + (debutCol + 1); // donne une position x comprise entre debut colonne tableau et fin colonne tableau
	posY = (int) rand() % (nbLignes - 1) + (debutLigne + 1);// donne une position y comprise entre debut ligne tableau et fin ligne tableau

															 //affichage de serpent
	gotoXY(posX, posY);
	color(12);
	cout << (char)'w';
}


/*********************************************************** FIN PARTIE I ********************************************************************/


/*********************************************************** DEBUT PARTIE II ********************************************************************/

int saisirNiveau()
{

	int niveau;

	color(10);
	gotoXY(30, 10);
	cout << "#### #  #  ##  #  # ####";

	color(10);
	gotoXY(30, 11);
	cout << "#    ## # #  # # #  #";

	color(10);
	gotoXY(30, 12);
	cout << "#### #### #### ##   ##";

	color(10);
	gotoXY(30, 13);
	cout << "   # # ## #  # # #  #";

	color(10);
	gotoXY(30, 14);
	cout << "#### #  # #  # #  # ####";

	color(15);
	gotoXY(20, 20);
	cout << "Selectionnez votre niveau puis PRESS ENTER";

	gotoXY(40, 22);
	cin >> niveau;


	//pour effacer le logo et la selection du niveau precedemment faite
	gotoXY(30, 10);
	cout << "                          ";

	gotoXY(30, 11);
	cout << "                          ";

	gotoXY(30, 12);
	cout << "                          ";

	gotoXY(30, 13);
	cout << "                          ";

	gotoXY(30, 14);
	cout << "                          ";

	gotoXY(20, 20);
	cout << "                                           ";

	gotoXY(39, 22);
	cout << "    ";

	return niveau;

}

int incrementerNiveau(int niveau, int &nbSouris, int &nbSourisGold, int &tailleSerpent, int serpentX[], int serpentY[],int sourisX[], int sourisY[], int sourisGoldX[], int sourisGoldY[]) {

	niveau++;

	for (int i = 1; i <= tailleSerpent; i++) {
		gotoXY(serpentX[i], serpentY[i]);
		cout << " ";
	}

	nbSouris = niveau * 2 + 2;
	nbSourisGold = nbSouris / 5;

	tailleSerpent = 1;
	creerSouris(nbLignes, nbColonnes, sourisX, sourisY, nbSouris);
	creerSourisGold(nbLignes, nbColonnes, sourisGoldX, sourisGoldY, nbSourisGold);
	afficherSouris(sourisX, sourisY, nbSouris);
	afficherSourisGold(sourisGoldX, sourisGoldY, nbSourisGold);


	return niveau;
}

void creerSouris(int nbLignes, int nbColonnes, int sourisX[], int sourisY[], int nbSouris)
{
	//donne une valeur aleatoir a chaque souris
	srand((unsigned int)time(NULL));
	for (int i = 0; i <= nbSouris; i++)
	{
		sourisX[i] = rand() % (nbColonnes - 1) + (debutCol + 1);
		sourisY[i] = rand() % (nbLignes - 1) + (debutLigne + 1);
	}
}

void creerSourisGold(int nbLignes, int nbColonnes, int sourisGoldX[], int sourisGoldY[], int nbSourisGold)
{
	//donne une valeur aleatoir a chaque souris
	srand((unsigned int)time(NULL));
	for (int i = 0; i <= nbSourisGold; i++)
	{
		sourisGoldX[i] = rand() % (nbColonnes - 1) + (debutCol + 1);
		sourisGoldY[i] = rand() % (nbLignes - 1) + (debutLigne + 1);
	}
}

void afficherSouris(int sourisX[], int sourisY[], int nbSouris)
/*
Tâche: afficher les souris
Paramètres: les tableaux de coordonnées et le nombre de souris
*/
{

	for (int i = 0; i <= nbSouris; i++)
	{

		gotoXY(sourisX[i], sourisY[i]);
		color(7);
		cout << char('Q');
	}

}

void afficherSourisGold(int sourisGoldX[], int sourisGoldY[], int nbSourisGold) {

	//transforme certaines souris en souris dorées
	for (int i = 0; i <= nbSourisGold; i++)
	{
		gotoXY(sourisGoldX[i], sourisGoldY[i]);
		color(14);
		cout << char('Q');
	}


}

/*void deplacerSerpentI(int direction, int &posX, int &posY)

Tâche: déplacer le serpent d'une seule case dans la direction donnée. Le serpent est à l'écran avant l'appel et au retour de la fonction
Paramètres: la direction du serpent en entrée, et la position du serpent en entrée / sortie

{
	int posPrecedX = 0, posPrecedY = 0, touche;


	touche = recupererTouche();
	direction = calculerDirectionTouche(touche);




	while (!_kbhit()) {

		if (tempsSerp != clock() / 200) {
			tempsSerp = clock() / 200;

			if (direction == directionDroite && posX < nbColonnes + debutCol - 1)
			{
				gotoXY(posX, posY);
				posPrecedX = posX;
				color(10);
				cout << 'o';
				posX++;
				gotoXY(posX, posY);
				color(12);
				cout << (char)'<';
				posPrecedY = posY;
			}


			else if (direction == directionGauche && posX > debutCol + 1)
			{
				gotoXY(posX, posY);
				posPrecedX = posX;
				color(10);
				cout << 'o';
				posX--;
				gotoXY(posX, posY);
				color(12);
				cout << (char)'>';
				posPrecedY = posY;
			}
			else if (direction == directionHaut && posY > debutLigne + 1)
			{
				gotoXY(posX, posY);
				posPrecedY = posY;
				color(10);
				cout << 'o';
				posY--;
				gotoXY(posX, posY);
				color(12);
				cout << (char)'W';
				posPrecedX = posX;
			}

			else if (direction == directionBas && posY < nbLignes + debutLigne - 1)
			{
				gotoXY(posX, posY);
				posPrecedY = posY;
				color(10);
				cout << 'o';
				posY++;
				gotoXY(posX, posY);
				color(12);
				cout << (char)'M';
				posPrecedX = posX;
			}



			if (direction == -1)
			{
				if (posPrecedX < posX && posX < nbColonnes + debutCol - 1)
				{
					gotoXY(posX, posY);
					posPrecedX = posX;
					color(10);
					cout << 'o';
					posX++;
					gotoXY(posX, posY);
					color(12);
					cout << (char)'<';
				}


				else if (posPrecedX > posX && posX > debutCol + 1)
				{
					gotoXY(posX, posY);
					posPrecedX = posX;
					color(10);
					cout << '0';
					posX--;
					gotoXY(posX, posY);
					color(12);
					cout << (char)'>';
				}
				else if (posPrecedY > posY && posY > debutLigne + 1)
				{
					gotoXY(posX, posY);
					posPrecedY = posY;
					color(10);
					cout << 'o';
					posY--;
					gotoXY(posX, posY);
					color(12);
					cout << (char)'W';
				}

				else if (posPrecedY < posY && posY < nbLignes + debutLigne - 1)
				{
					gotoXY(posX, posY);
					posPrecedY = posY;
					color(10);
					cout << 'o';
					posY++;
					gotoXY(posX, posY);
					color(12);
					cout << (char)'M';
				}


			}



		}

		serpentX[0] = posX;
		serpentY[0] = posY;

	}
}*/

void deplacerSerpentII(int direction, int serpentX[], int serpentY[], int &tailleSerpent, int sourisX[], int sourisY[], int &nbSouris) {


	char tete;

	//pour ralentir legerement le serpent
	Sleep(200);

	//le switch case permettant le changement de direction 
	switch (direction) {
	case 0:
		serpentX[1] = serpentX[0];
		serpentY[1] = serpentY[0];
		serpentX[0] ++;
		tete = '<';
		break;

	case 1:
		serpentX[1] = serpentX[0];
		serpentY[1] = serpentY[0];
		serpentX[0] --;
		tete = '>';
		break;

	case 2:
		serpentY[1] = serpentY[0];
		serpentX[1] = serpentX[0];
		serpentY[0]--;
		tete = 'W';
		break;

	case 3:
		serpentY[1] = serpentY[0];
		serpentX[1] = serpentX[0];
		serpentY[0]++;
		tete = 'M';
		break;
	}

	//affichage de la tete
	gotoXY(serpentX[0], serpentY[0]);
	color(12);
	cout << tete;

	//affichage du corp
	gotoXY(serpentX[1], serpentY[1]);
	color(10);
	cout << "o";

	//Effacement du corp innutile
	gotoXY(serpentX[tailleSerpent], serpentY[tailleSerpent]);
	cout << " ";

	//deplacement du corp dans le tableau
	for (int i = tailleSerpent; i > 0; i--) {
		serpentX[i] = serpentX[i - 1]; 
		serpentY[i] = serpentY[i - 1];
	}
}

/*void deplacerSerpentII(int direction, int serpentX[], int serpentY[], int &indiceTete, int &indiceQueue, int sourisX[], int sourisY[], int &nbSouris) {

	int indicePres = indiceTete;
	indiceTete = (indicePres + 1) % tailleSerp;


	if (direction == flecheGauche)
	{
		serpentX[indiceTete] = serpentX[indicePres % tailleSerp] + 1; //sur X on increment l'indice
		serpentY[indiceTete] = serpentX[indicePres % tailleSerp];
		gotoXY(serpentX[indiceTete], serpentY[indiceTete]);
		cout << char('>');

	}
	else if (direction == flecheDroite)
	{
		serpentX[indiceTete] = serpentX[indicePres % tailleSerp] - 1; //sur X on decrement l'indice
		serpentY[indiceTete] = serpentX[indicePres % tailleSerp];
		gotoXY(serpentX[indiceTete], serpentY[indiceTete]);
		cout << char('<');
	}
	else if (direction == flecheHaut)
	{
		serpentX[indiceTete] = serpentX[indicePres % tailleSerp];
		serpentY[indiceTete] = serpentX[indicePres % tailleSerp] - 1; //sur Y on decrement l'indice
		gotoXY(serpentX[indiceTete], serpentY[indiceTete]);
		cout << char('W');

	}

	else if (direction == flecheDroite)
	{
		serpentX[indiceTete] = serpentX[indicePres % tailleSerp];
		serpentY[indiceTete] = serpentX[indicePres % tailleSerp] + 1; //sur Y on increment l'indice
		gotoXY(serpentX[indiceTete], serpentY[indiceTete]);
		cout << char('M');
	}

	if (!testerCollision(serpentX[indicePres], serpentY[indicePres], sourisX, sourisY, nbSouris))
	{
		gotoXY(serpentX[indiceQueue], serpentY[indiceQueue]);
		cout << char(' ');
	}

}*/

bool testerCollision(int posX, int posY, int sourisX[], int sourisY[], int &nbSouris) {

	bool collision = false;

	for (int s = 0; s <= nbSouris; s++) // collision parcour du tableau des souris
	{
		if (sourisX[s] == posX && sourisY[s] == posY)
		{

			collision = true;//Si la tete du serpent se trouve aux memes cohordonnées qu'une souris la collision renvoie la valeur vrai.
			sourisX[s] = sourisX[nbSouris];
			sourisY[s] = sourisY[nbSouris];

			nbSouris--;

		}
	}
	return collision;
}

bool testerCollisionGold(int serpentX[], int serpentY[], int sourisGoldX[], int sourisGoldY[], int &nbSourisGold) {
	bool collision = false;

	for (int s = 0; s < nbSourisGold; s++) // collision parcour du tableau des souris sachant que seul les premiere sont doré
	{
		if (sourisGoldX[s] == serpentX[0] && sourisGoldY[s] == serpentY[0])
		{
			collision = true;//Si la tete du serpent se trouve aux memes cohordonnées qu'une souris doré la collision renvoie la valeur vrai.
			sourisGoldX[s] = sourisGoldX[nbSourisGold];
			sourisGoldY[s] = sourisGoldY[nbSourisGold];
			
			nbSourisGold--;
		}
	}
	return collision;
}

bool testerCollisionFatal(int serpentX[], int serpentY[], int tailleSerpent, int nbColonnes, int nbLignes) {
	bool retour = true;

	//verification que le serpent n'entre pas en colosion avec une bordure droite ou gauche du cadre
	if ((serpentX[0] == 5) || (serpentX[0] == (5 + nbColonnes))) {
		retour = false;
	}
	//verification que le serpent n'entre pas en colosion avec une bordure haute ou basse du cadre
	if ((serpentY[0] == 5) || (serpentY[0] == (5 + nbLignes))) {
		retour = false;
	}

	//verification que le serpent n'entre pas en collision avec lui meme
	if (tailleSerpent >= 2) {
		for (int i = 2; i < tailleSerpent; i++) {

			if ((serpentX[i] == serpentX[0]) && (serpentY[i] == serpentY[0])) {
				retour = false;
			}
		}
	}
	return retour;
}

void afficherGameOver(int nbLignes, int nbColonnes) {
	int l = 6;
	color(15);
	gotoXY(6, l);

	//effacer le coté droit
	for (int i = 51; l < 23; i++) {
		gotoXY(i, l);
		cout << " ";
		if (i == 74 && l != 23) {
			i = 51;
			l++;
		}
	}
	l = 6;

	//effacer le coté gauche
	for (int i = 6; i <= 28; i++) {
		gotoXY(i, l);
		cout << " ";
		if (i == 28 && l != 34) {
			i = 5;
			l++;
		}
	}
	l = 6;

	//faire apparaitre le GAME OVER
	for (int i = 6; i <= 74; i++) {
		gotoXY(i, l);
		cout << " ";

		if ((i > 28 && i < 49) && (l > 10 && l <= 21)) {
			color(12);

			if (l == 11) {
				gotoXY(i, l);
				cout << " ###    #   #   # #####";
				i = 29;
				l = 12;
			}
			if (l == 12) {
				gotoXY(i, l);
				cout << "#      # #  ## ## #    ";
				i = 29;
				l = 13;
			}
			if (l == 13) {
				gotoXY(i, l);
				cout << "# ### #   # # # # #### ";
				i = 29;
				l = 14;
			}
			if (l == 14) {
				gotoXY(i, l);
				cout << "#  ## ##### #   # #    ";
				i = 29;
				l = 15;
			}
			if (l == 15) {
				gotoXY(i, l);
				cout << " ###  #   # #   # #####";
				i = 29;
				l = 16;
			}
			if (l == 16) {
				gotoXY(i, l);
				cout << "                       ";
				i = 29;
				l = 17;
			}
			if (l == 17) {
				gotoXY(i, l);
				cout << " ###  #   # #####  ### ";
				i = 29;
				l = 18;
			}
			if (l == 18) {
				gotoXY(i, l);
				cout << "#   # #   # #     #   #";
				i = 29;
				l = 19;
			}
			if (l == 19) {
				gotoXY(i, l);
				cout << "#   #  # #  ####  #### ";
				i = 29;
				l = 20;
			}
			if (l == 20) {
				gotoXY(i, l);
				cout << "#   #  # #  #     #   #";
				i = 29;
				l = 21;
			}
			if (l == 21) {
				gotoXY(i, l);
				cout << " ###    #   ##### #   #";
				i = 6;
				l = 22;
			}
		}

		if (i == 74 && l != 34) {
			i = 5;
			l++;
		}

	}
}