/*========================================================================
Nom: Fenetre.h       Auteur: Manuel CHATAIGNER
Maj: 26/03/2015          Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe contenant l'ensemble des propriétés de la fenêtre
               graphique.
=========================================================================*/

#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include <QPixmap>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QTextEdit>
#include <QLineEdit>

#include "ListeCartes.h"
#include "CompteurJetons.h"
#include "../Jeu/Jeu.h"

class Fenetre : public QWidget
{
    Q_OBJECT

    private:

        //Informations jeu
        QLineEdit actionEffectueeIA;
        QLabel resultatPartie;

        // Logs
        QTextEdit logs;
        QPushButton boutonLogs;

        // Liste des cartes
        ListeCartes layoutMain;
        ListeCartes layoutMainAdverse;
        ListeCartes layoutCartesCommunes;

        // Compteur du pot et des caves des joueurs
        CompteurJetons pot;
        CompteurJetons caveJoueur;
        CompteurJetons caveIA;

        // Boite de saisie du montant de la mise
        QSpinBox valeurMise;

        QPushButton boutonDemarrage;
        QCheckBox boutonChoixCartes;


        enum { CHECKER, MISER, SUIVRE, RELANCER, SE_COUCHER, NB_BOUTONS };

        // Boutons d'action de jeu
        QPushButton boutons[NB_BOUTONS];

        bool activationBoutons[NB_BOUTONS];

        Jeu *jeu;


        /**
         * @action Active ou désactive l'ensemble des boutons d'action du joueur
         * @param active Vrai s'il faut activer les boutons, faux pour les désactiver
         */
        void activeBoutons(bool active);

        /**
         * @action afficheTable Réupère l'ensemble des cartes de la table
         *                      à partir du jeu et les affiche
         */
        void afficheTable();

        /**
         * @action joueurCourant Active les boutons permettant au joueur d'effectuer une action
         */
        void joueurCourant();

        /**
         * @action jeuIA Lance la méthode de jeu de l'IA et actulaise l'interface en conséquence
         */
        void jeuIA();

    public:

        // Image contenant l'ensemble des cartes graphiques du jeu de carte
        static QPixmap *textureCartes;

        Fenetre(Jeu *j);
        ~Fenetre();

        /**
         * @action Ajoute dans les logs le contenu passé en paramètre
         * @param contenu Texte à ajouter aux logs
         */
        void ajoutLogs(QString contenu);

    signals:

        /**
         * @action Signal émis lorsque le tour d'un joueur (humain/IA) est terminé
         */
        void tourFini();

    public slots:

        /**
         * @action Affiche/cache la boite de logs lorsque le joueur appuie sur le bouton correspondant
         */
        void affichageLogs();

        /**
         * @action Distribue les cartes de pre-flop à tous les joueurs et lance le jeu
         */
        void demarragePartie();

        /**
         * @action Lance le tour du prochain joueur à partir du jeu et met à jour la table si besoin
         */
        void prochainJoueur();

        /**
         * @action Effectue les actions de fin de partie
         */
        void partieTermine();

        /** Méthodes d'action de jeu appelées une fois l'action choisie par le joueur courant **/

        void checker();

        void miser();

        void suivre();

        void relancer();

        void seCoucher();
};

#endif // FENETRE_H
