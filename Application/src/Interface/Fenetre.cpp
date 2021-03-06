/*========================================================================
Nom: Fenetre.cpp       Auteur: Manuel CHATAIGNER
Maj: 11/02/2015            Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Fenetre.
=========================================================================*/

#include "../../include/Interface/Fenetre.h"
#include "../../include/Interface/CarteGraphique.h"
#include "../../include/Interface/CartesDialog.h"
#include "../../include/Interface/ChoixOptionsDialog.h"
#include "../../include/Interface/Logger.h"

#include<QString>
#include <QVBoxLayout>
#include <iostream>
#include <QScrollBar>
using namespace std;

QPixmap *Fenetre::textureCartes = 0;

Fenetre::Fenetre(Jeu *j) : QWidget()
{

    move(0,0);
    setWindowTitle(tr("Poker"));
    resize(1280,400);

    // Couleur de fond
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(20, 127, 20));
    setPalette(pal);

    this->jeu = j;

    for (int i = 0; i < NB_BOUTONS; i++) {
        activationBoutons[i] = true;
    }


    // Layout principal
    QHBoxLayout *layout = new QHBoxLayout;


    // ////////////////////////////////////////////////////
    // Logs
    // ////////////////////////////////////////////////////

    QVBoxLayout *layoutLogs = new QVBoxLayout;

    QLabel *titreTableauDeBord = new QLabel("Tableau de bord :");
    titreTableauDeBord->setFixedSize(300, 50);
    titreTableauDeBord->setStyleSheet("QLabel {color : #89DF57; text-align:center; font-weight:bold;}");

    logs.setReadOnly(true);
    logs.setMaximumSize(300, 300);


    boutonLogs.setText("Afficher/Cacher");
    boutonLogs.setFixedWidth(300);

    layoutLogs->setAlignment(Qt::AlignTop);
    layoutLogs->setAlignment(Qt::AlignHCenter);
    layoutLogs->setSpacing(20);

    layoutLogs->addWidget(titreTableauDeBord);
    layoutLogs->addWidget(&logs);
    layoutLogs->addWidget(&boutonLogs);
    layoutLogs->addWidget(&resultatPartie);



    connect(&boutonLogs, SIGNAL(clicked()), this, SLOT(affichageLogs()));


    // ////////////////////////////////////////////////////
    // Cartes
    // ////////////////////////////////////////////////////

    QVBoxLayout *layoutJeu = new QVBoxLayout;

    // Chargement de l'image
    if (!textureCartes){
        textureCartes = new QPixmap("../Application/ressources/Interface/deck.png");
    }

    QHBoxLayout *layoutAdversaire =new QHBoxLayout;

    //Sous-layout IA
    actionEffectueeIA.setReadOnly(true);
    actionEffectueeIA.setFixedWidth(80);
    actionEffectueeIA.setPlaceholderText("Action");

    layoutAdversaire->addLayout(&layoutMainAdverse);
    layoutAdversaire->addWidget(&actionEffectueeIA);

    layoutJeu->setSpacing(150);
    layoutJeu->setAlignment(Qt::AlignTop);

    layoutJeu->addLayout(layoutAdversaire);
    layoutJeu->addLayout(&layoutCartesCommunes);
    layoutJeu->addLayout(&layoutMain);


    // ////////////////////////////////////////////////////
    // Compteurs
    // ////////////////////////////////////////////////////

    caveIA.display(jeu->getJoueur(1).getCave());
    pot.display(0);
    caveJoueur.display(jeu->getJoueur(0).getCave());


    // ////////////////////////////////////////////////////
    // Boutons d'action
    // ////////////////////////////////////////////////////

    QHBoxLayout *layoutDemarrage = new QHBoxLayout;

    boutonDemarrage.setText("Démarrage partie");
    boutonDemarrage.setMaximumWidth(150);
    connect(&boutonDemarrage, SIGNAL(clicked()), this, SLOT(demarragePartie()));

    boutonChoixCartes.setChecked(false);
    boutonChoixCartes.setText("Choix des cartes");
    boutonChoixCartes.setObjectName(QString("Checkbox"));
    boutonChoixCartes.setStyleSheet("QWidget#Checkbox { background-color: rgb(255, 255, 255); border-style: solid; border-color: black; border-width: 1px; padding : 3px; }");

    layoutDemarrage->setAlignment(Qt::AlignLeft);
    layoutDemarrage->setSpacing(10);
    layoutDemarrage->addWidget(&boutonDemarrage);
    layoutDemarrage->addWidget(&boutonChoixCartes);


    QVBoxLayout *layoutBoutons = new QVBoxLayout;

    layoutBoutons->setSpacing(10);
    layoutBoutons->setAlignment(Qt::AlignTop);

    boutons[CHECKER].setText("Checker");
    boutons[MISER].setText("Miser");
    boutons[SUIVRE].setText("Suivre");
    boutons[RELANCER].setText("Relancer");
    boutons[SE_COUCHER].setText("Se coucher");

    layoutBoutons->addWidget(&boutons[CHECKER]);
    layoutBoutons->addWidget(&boutons[MISER]);
    layoutBoutons->addWidget(&boutons[SUIVRE]);
    layoutBoutons->addWidget(&boutons[RELANCER]);
    layoutBoutons->addWidget(&boutons[SE_COUCHER]);

    connect(&boutons[CHECKER], SIGNAL(clicked()), this, SLOT(checker()));
    connect(&boutons[MISER], SIGNAL(clicked()), this, SLOT(miser()));
    connect(&boutons[SUIVRE], SIGNAL(clicked()), this, SLOT(suivre()));
    connect(&boutons[RELANCER], SIGNAL(clicked()), this, SLOT(relancer()));
    connect(&boutons[SE_COUCHER], SIGNAL(clicked()), this, SLOT(seCoucher()));

    activeBoutons(false);


    // ////////////////////////////////////////////////////
    // Fenetre
    // ////////////////////////////////////////////////////

    QHBoxLayout *layoutJoueur = new QHBoxLayout;

    valeurMise.setMaximum(10000);

    layoutJoueur->setAlignment(Qt::AlignRight);
    layoutJoueur->setSpacing(50);

    layoutJoueur->addWidget(&caveJoueur);
    layoutJoueur->addWidget(&valeurMise);
    layoutJoueur->addLayout(layoutBoutons);


    QVBoxLayout *layoutOptions = new QVBoxLayout;

    layoutOptions->setSpacing(100);
    layoutOptions->setAlignment(Qt::AlignHCenter);

    layoutOptions->addLayout(layoutDemarrage);
    layoutOptions->addWidget(&caveIA);
    layoutOptions->addWidget(&pot);
    layoutOptions->addLayout(layoutJoueur);


    // Layout principal
    layout->setSpacing(100);
    layout->addLayout(layoutLogs);
    layout->addLayout(layoutJeu);
    layout->addLayout(layoutOptions);

    setLayout(layout);

    connect(this, SIGNAL(tourFini()), this, SLOT(prochainJoueur()));

    //Récupération des options du jeu
    ChoixOptionsDialog fenetreOptions;
    Options options = fenetreOptions.getOptions();

    QString pseudoJoueur = options.pseudo;
    jeu->setPseudo(pseudoJoueur.toStdString() );
    jeu->setAgressiviteIA(options.agressiviteIA);
    jeu->setRationaliteIA(options.rationaliteIA);
}

Fenetre::~Fenetre()
{
    Logger::supprimerInstance();
}

void Fenetre::affichageLogs()
{
    bool visible = !logs.isHidden();

    logs.setHidden(visible);
}

void Fenetre::ajoutLogs(QString contenu)
{
    logs.setText(logs.toPlainText() + contenu + "\n");
    QScrollBar *sb=logs.verticalScrollBar();
    sb->setValue(sb->maximum());
}

void Fenetre::demarragePartie()
{
    actionEffectueeIA.clear();
    resultatPartie.setText("");
    boutonDemarrage.hide();
    layoutCartesCommunes.vider();

    Logger::getInstance()->ajoutLogs("Distribution des cartes");

    // Sélection des cartes par l'utilisateur

    if (boutonChoixCartes.isChecked()) {
        CartesDialog fenetreCartes(this);
        std::vector<int> ids = fenetreCartes.choixCartes();

        if (!ids.empty()) {
            jeu->affectationCarte(ids);
        }
    }

    jeu->distributionMain();
    
    pot.display(jeu->getPot());
    caveJoueur.display(jeu->getJoueur(0).getCave());
    caveIA.display(jeu->getJoueur(1).getCave());


    // Affichage de la main adverse dans les logs

    std::vector<Carte> jeuAdverse = jeu->getJoueur(1).getMain();

    Logger::getInstance()->ajoutLogs("Jeu adverse : ");
    for (int i = 0; i < jeuAdverse.size(); i++) {
        Logger::getInstance()->ajoutLogs("-> " + QString::number(jeuAdverse.at(i).getRang())
                  + " " + CarteGraphique::couleurs[jeuAdverse.at(i).getCouleur()]);
    }


    // Main du joueur

    layoutMain.vider();
    layoutMain.ajoutCartes(jeu->getJoueur(0).getMain());

    // Cartes communes

    layoutCartesCommunes.vider();

    for (int i = 0; i < 5; i++) {
        CarteGraphique *dos = new CarteGraphique(0, 0);
        layoutCartesCommunes.addWidget(dos);
    }

    // Main adverse

    CarteGraphique *dos = new CarteGraphique(0, 0);
    CarteGraphique *dos2 = new CarteGraphique(0, 0);

    layoutMainAdverse.vider();
    layoutMainAdverse.addWidget(dos);
    layoutMainAdverse.addWidget(dos2);


    valeurMise.setMinimum(jeu->getBlind());

    if (jeu->getJoueurCourant() == 0) {     // Joueur humain
        joueurCourant();
    }
    else {                                  // Intelligence artificielle
        jeuIA();
    }
}

void Fenetre::afficheTable()
{
    std::vector<Carte> table = jeu->getTable();

    layoutCartesCommunes.vider();
    layoutCartesCommunes.ajoutCartes(table);

    // On complète la table avec des dos de carte
    for (int i = 0; i < 5 - table.size(); i++) {
        CarteGraphique *dos = new CarteGraphique(0, 0);
        layoutCartesCommunes.addWidget(dos);
    }

    Logger::getInstance()->ajoutLogs("Ajout de cartes sur la table");
}

void Fenetre::activeBoutons(bool active)
{
    // Pour chaque bouton, soit on le désactive ou on regarde son état si activation

    for (int i = 0; i < NB_BOUTONS; i++) {
        bool enable = active ? activationBoutons[i] : active;
        boutons[i].setEnabled(enable);
    }
}

void Fenetre::joueurCourant()
{
    activeBoutons(true);
}

void Fenetre::jeuIA()
{
    static_cast<IntelligenceArtificielle>(jeu->getJoueur(jeu->getJoueurCourant())).jouer();

    switch (jeu->getAction()) {
        case TYPES::ACTION_LIST::CHECKER:
            actionEffectueeIA.setText("Check");
            Logger::getInstance()->ajoutLogs("IA check");
            break;


        case TYPES::ACTION_LIST::MISER:
            actionEffectueeIA.setText("Mise : "+QString::number(jeu->getMise()));
            activationBoutons[MISER] = false;
            activationBoutons[CHECKER] = false;

            valeurMise.setMinimum(2 * jeu->getMise());

            caveIA.display(jeu->getJoueur(1).getCave());
            pot.display(jeu->getPot());

            Logger::getInstance()->ajoutLogs("IA mise " + QString::number(jeu->getMise()));
            break;


        case TYPES::ACTION_LIST::SUIVRE:
            actionEffectueeIA.setText("Suit");
            caveIA.display(jeu->getJoueur(1).getCave());
            pot.display(jeu->getPot());

            Logger::getInstance()->ajoutLogs("IA suit");
            break;


        case TYPES::ACTION_LIST::RELANCER:
            actionEffectueeIA.setText("Relance : "+QString::number(jeu->getMise()));
            valeurMise.setMinimum(2 * jeu->getMise());

            caveIA.display(jeu->getJoueur(1).getCave());
            pot.display(jeu->getPot());

            Logger::getInstance()->ajoutLogs("IA relance " + QString::number(jeu->getMise()));
            break;
        case TYPES::ACTION_LIST::SE_COUCHER:
            actionEffectueeIA.setText("Se couche");
            Logger::getInstance()->ajoutLogs("IA se couche");
            partieTermine();
            return;
            break;
        default:
            break;
    }

    emit tourFini();
}

void Fenetre::prochainJoueur()
{
    activeBoutons(false);

    if (!jeu->prochainJoueur()){
        partieTermine();
        return;
    }

    if (jeu->debutTour()) {
        afficheTable();
    }

    if (jeu->getJoueurCourant() == 0) {     // Joueur humain
        joueurCourant();
    }
    else {                                  // Intelligence artificielle
        jeuIA();
    }
}

void Fenetre::checker()
{
    jeu->checker(0);

    Logger::getInstance()->ajoutLogs("Joueur 1 check");

    emit tourFini();
}

void Fenetre::miser()
{
    int montant = valeurMise.value();

    jeu->miser(0, montant);

    caveJoueur.display(jeu->getJoueur(0).getCave());
    pot.display(jeu->getPot());

    Logger::getInstance()->ajoutLogs("Joueur 1 mise " + QString::number(montant));

    emit tourFini();
}

void Fenetre::suivre()
{
    jeu->suivre(0);

    caveJoueur.display(jeu->getJoueur(0).getCave());
    pot.display(jeu->getPot());

    activationBoutons[CHECKER] = true;
    activationBoutons[MISER] = true;
    valeurMise.setMinimum(jeu->getBlind());

    Logger::getInstance()->ajoutLogs("Joueur 1 suit");

    emit tourFini();
}

void Fenetre::relancer()
{
    int montant = valeurMise.value();

    jeu->relancer(0, montant);

    caveJoueur.display(jeu->getJoueur(0).getCave());
    pot.display(jeu->getPot());

    activationBoutons[CHECKER] = true;
    activationBoutons[MISER] = true;
    valeurMise.setMinimum(jeu->getBlind());

    Logger::getInstance()->ajoutLogs("Joueur 1 relance " + QString::number(montant));

    emit tourFini();
}

void Fenetre::seCoucher()
{
    jeu->seCoucher(0);

    Logger::getInstance()->ajoutLogs("Joueur 1 se couche");

    partieTermine();
}

void Fenetre::partieTermine()
{
    Logger::getInstance()->ajoutLogs("Partie terminée !");
    activeBoutons(false);

    layoutMainAdverse.vider();
    layoutMainAdverse.ajoutCartes(jeu->getJoueur(1).getMain());


    int gagne = jeu->nouvelleMain();

    if(gagne==GAGNE){
        resultatPartie.setStyleSheet("QLabel {color : #89DF57; font-size : 40px; text-align:center; padding-left:80px; font-weight:bold;}");
        resultatPartie.setText("Gagné !");
    }
    else if(gagne==EGALITE){
        resultatPartie.setStyleSheet("QLabel {color : #23BDFE; font-size : 40px; text-align:center; padding-left:80px; font-weight:bold;}");
        resultatPartie.setText("Egalité");
    }
    else{
        resultatPartie.setStyleSheet("QLabel {color : #FE0000; font-size : 40px; text-align:center; padding-left:70px; font-weight:bold;}");
        resultatPartie.setText("Perdu !");
    }


    boutonDemarrage.setText("Rejouer");
    boutonDemarrage.setHidden(false);
}
