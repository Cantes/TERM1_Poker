#ifndef Jeu_h
#define Jeu_h

#include <vector>
#include <string>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
#include "Carte.h"
#include "Joueur.h"
#include "../Constantes.h"
#include "IntelligenceArtificielle.h"
#include "../Evaluateur/Evaluateur.h"
#include "../Profilage/CalculDonneesProfilage.h"
#include "../Profilage/Profilage.h"
#include "EstimationProba.h"

class Joueur;
class IntelligenceArtificielle;

class Jeu{

	//Liste des attributs
	private :
        std::vector<Joueur>                 positionnement;
        std::vector<Carte>                  deck;
        std::vector<Carte>                  table;
        std::vector<TYPES::ACTION_LIST>	    actions;
        int                                 blind;
        int                                 joueurCourant;
        int                                 pot;
        int                                 nombreDeCoup;
        int                                 mise;
        int                                 dealer;
        double                              agressiviteIA;
        double                              rationaliteIA;
        ETAPE_JEU                           etape;
        std::vector<Carte>                  tableTmp;
		
	//Constructeur et destructeur
	public:
		/**
		*@param  : Le nombre de joueur, le montant de la blind de depart, la cave de depart des joueurs et le type de proffiling de l'IA
		*@action : Initialise un nouveau jeu
		**/
        Jeu(int nbJoueur, int blindDepart, int cave, double agressivite, double rationalite);
		
		/**
		*@action : Destructeur de la classe Jeu
		**/
		~Jeu();
		
	//Accesseur
	
		/**
		*@action : Permet d'obtenir le montant de la petite blind
		*@return : Le montant de la petite blind
		**/
		int 			getBlind() const;
		
		/**
		*@action : Permet d'obtenir le joueur devant jouer
		*@return : Le joueur courant
		**/
		int 			getJoueurCourant() const;
		
		/**
		*@action : Permet d'obtenir le joueur en i-eme position
		*@return : Le joueur en i-eme position
		**/
		Joueur& 		getJoueur(int i);
        	
        	/**
		*@action : Permet d'ajouter un joueur a la partie
		*@param  : Le joueur a ajouter a la partie
		**/
		void 			setJoueur(Joueur joueur);
		
		/**
		*@action : Permet d'obtenir les carte communes
		*@return : L'ensemble des cartes commune
		**/
		std::vector<Carte>	getTable() const;
		
		/**
		*@action : Permet d'obtenir le pot de la partie
		*@return : Le pot de la partie en cours
		**/
		int			getPot() const;
		
		/**
		*@action : Permet de modifier le pot de la partie courante
		*@param  : Un entier representant la nouvelle valeur du pot
		**/
		void			setPot(int jetons);
		
		/**
		*@action  : Permet de connaitre l'action effectué par le joueur courant
		*@return  : L'action effectué par le joueur courant
		**/
		TYPES::ACTION_LIST	getAction() const;
		
		/**
		*@action : Permet d'obtenir la position du dealer
		*@return : La position du dealer
		**/
		int 			getDealer();
		
		/**
		*@action : Permet d'obtenir la mise de la partie courante
		*@return : Un entier représentant la mise courante
		**/
		int			getMise();

		/**
		*@action : Permet d'obtenir le deck
		*@return : Un vecteur représentant le deck
		**/
		std::vector<Carte>			getDeck();
		
		/**
		*@action : Permet d'obtenir l'ensemble des actions
		*@return : L'ensemble des actions
		**/
		std::vector<TYPES::ACTION_LIST> getListeActions() const;

		/**
		*@action : Permet d'obtenir l'agressivite de l'IA
		*@return : L'agressivite de l'IA
		**/
		double getAgressiviteIA() const;

		/**
		*@action : Permet d'obtenir la rationnalite de l'IA
		*@return : La rationnalite de l'IA
		**/
		double getRationaliteIA() const;

		/**
		*@action : Permet d'affecter l'agressivite de l'IA
		*@param : L'agressivite de l'IA
		**/
		void setAgressiviteIA(double agressivite);

		/**
		*@action : Permet d'affecter la rationnalite de l'IA
		*@param : La rationnalite de l'IA
		**/
		void setRationaliteIA(double rationalite);

        /**
        *@action : Permet d'affecter le pseudo du joueur
        *@param : Le pseudo choisi par le joueur
        **/
        void setPseudo(std::string pseudo);

        /**
        *@action : Permet d'obtenir l'etape courante
        *@return : L'etape courante
        **/
        ETAPE_JEU getEtape() const;


	
	//Methodes	
	//private:
		/**
		*@action : Distribue a chaque joueur ses cartes
		**/
		void distributionMain();
		
		/**
		*@action : Distribue les trois premieres carte commune : le Flop, tirees aleatoirement dans le deck
		**/
		void distributionFlop();
		
		/**
		*@action : Distribue la quatrieme carte : le Turn, tiree aleatoirement dans le deck
		**/
		void distributionTurn();
		
		/**
		*@action : Distribue la cinquieme carte : la River, tiree aleatoirement dans le deck
		**/
		void distributionRiver();
		
		/**
		*@action : Distribue les blinds en debut de partie
		**/
		void distributionBlind();
		
		/**
		*@action : Augmente le montant de la petite blind
		**/
		void miseAJourBlind();
		
		/**
		*@action : Melange le jeu de carte
		**/
		void melange();
		
		/**
		*@param  : Le nombre de joueur et le montant de depart d leur cave
		*@action : Cree les joueurs et les affectent au jeu
		*@return : L'ensemble des joueurs de la partie
		**/
		void initialisationTable(int nbJoueur, int cave);
		
		/**
		*@action : Cree l'ensemble des cartes utilisees dans le jeu
		*@return : Le "deck", l'ensemble du jeu de carte
		**/
		std::vector<Carte> nouveauDeck();
		
		/**
		*@action : Permet de savoir si le joueur a la possibilite checker
		*@param  : la position du joueur dont on veut savoir s'il peut checker
		*@return : vrai si le joueur peut checker, faux sinon 
		**/
		bool peutChecker(int posJoueur);

		/**
		*@action : Permet de savoir si le joueur a la possibilite de relancer
		*@param  : la position du joueur dont on veut savoir s'il peut relancer
		*@return : vrai si le joueur peut relancer, faux sinon
		**/
		bool peutRelancer(int posJoueur);

		/**
		*@action : Permet de savoir si le joueur a la possibilite de miser
		*@param  : la position du joueur dont on veut savoir s'il peut miser
		*@return : vrai si le joueur peut miser, faux sinon
		**/
		bool peutMiser(int posJoueur);


		/**
		*@action : Permet de savoir si le joueur a la possibilite de suivre
		*@param  : la position du joueur dont on veut savoir s'il peut suivre
		*@return : vrai si le joueur peut suivre, faux sinon
		**/
		bool peutSuivre(int posJoueur);

		
		/**
		*@action : Commande permettant a un joueur de miser
		*@param  : La position du joueur effectuant l'action ainsi que le montant de la mise
		*@return : Retourne vrai si l'action est possible, faux sinon
		**/
		bool miser(int posJoueur, int jetons);
		
		/**
		*@action : Commande permettant a un joueur de relancer
		*@param  : La position du joueur effectuant l'action ainsi que le montant de la relance
		*@return : Retourne vrai si l'action est possible, faux sinon
		**/
		bool relancer(int posJoueur, int jetons);
		
		/**
		*@action : Commande permettant a un joueur de faire "tapis"
		*@param  : La position du joueur effectuant l'action
		*@return : Retourne vrai si l'action est possible, faux sinon
		**/
		bool tapis(int posJoueur);
		
		/**
		*@action : Commande permettant a un joueur de suivre
		*@param  : La position du joueur effectuant l'action
		*@return : Retourne vrai si l'action est possible, faux sinon
		**/
		bool suivre(int posJoueur);
		
		/**
		*@action : Commande permettant a un joueur de checker
		*@param  : La position du joueur effectuant l'action
		*@return : Retourne vrai si l'action est possible, faux sinon
		**/
		bool checker(int posJoueur);
		
		/**
		*@action : Commande permettant a un joueur de se coucher
		*@param  : Le joueur effectuant l'action
		*@return : Retourne vrai si l'action est possible, faux sinon
		**/
       		bool seCoucher(int posJoueur);
       		
       		/**
		*@action  : Affecte le joueur suivant en tant que joueur courant et renvoi vrai s'il existe
		*@return : vrai s'il y a un joueur suivant, faux sinon
		**/
		bool prochainJoueur();
		
		/**
		*@action  : Permet de savoir si l'on est en début de tour
		*@return  : Vrai si l'on se trouve en debut de tour, faux sinon
		**/
		bool debutTour();
		
	
		/**
		*@action  : Permet de savoit si le tour est terminé
		*@return  : Vrai si le tour est termnié, faux sinon
		**/
		bool finDuTour();
		
		/**
		*@action  : Permet de reinitialiser le tableau des actions
		**/		
		void resetActions();
		
		/**
        *@action  : Relance une nouvelle main
        *@return  : Gagnant de la partie (GAGNE, PERDU, EGALITE)
		**/			
        int nouvelleMain();

		/**
		*@action  : Affecte les cartes choisies via l'interface
		*@param   : Un vecteur d'entier correspondant a la liste des ids des cartes
		**/
		void affectationCarte(std::vector<int> listeId);

        /**
        *@action  : Rempli le tableau de profilage du joueur
        *@param   : Le profil du joueur
        **/
        void remplissageTableau(Profilage &profil);
};

#endif
