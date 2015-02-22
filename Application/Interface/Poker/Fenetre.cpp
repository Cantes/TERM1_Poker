#include "Fenetre.h"
#include "CarteGraphique.h"
#include <QVBoxLayout>
#include <QVBoxLayout>

QPixmap *Fenetre::textureCartes = 0;

Fenetre::Fenetre(Jeu *j) : QWidget()
{
    setWindowTitle(tr("Poker"));
    resize(800,600);

    // Couleur de fond
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(20, 127, 20));
    setAutoFillBackground(true);
    setPalette(pal);

    this->jeu = j;

    QHBoxLayout *layout = new QHBoxLayout;

    QVBoxLayout *layoutJeu = new QVBoxLayout;
    QVBoxLayout *layoutOptions = new QVBoxLayout;

    QHBoxLayout *layoutJoueur = new QHBoxLayout;


    // ////////////////////////////////////////////////////
    // Cartes
    // ////////////////////////////////////////////////////


    // Chargement de l'image
    if (!textureCartes){
        textureCartes = new QPixmap("deck.png");
    }


    // ////////////////////////////////////////////////////
    // Compteurs
    // ////////////////////////////////////////////////////

    pot.setMaximumSize(100, 50);
    pot.setSegmentStyle(QLCDNumber::Filled);
    pot.display(0);

    caveJoueur.setMaximumSize(100, 50);
    caveJoueur.setSegmentStyle(QLCDNumber::Filled);
    caveJoueur.display(jeu->getJoueur(0).getCave());

    caveIA.setMaximumSize(100, 50);
    caveIA.setSegmentStyle(QLCDNumber::Filled);
    caveIA.display(jeu->getJoueur(1).getCave());


    // ////////////////////////////////////////////////////
    // Boutons d'action
    // ////////////////////////////////////////////////////

    QVBoxLayout *layoutBoutons = new QVBoxLayout;

    layoutBoutons->setSpacing(10);
    layoutBoutons->setAlignment(Qt::AlignTop);

    boutonChecker.setText("Checker");
    boutonMiser.setText("Miser");
    boutonSuivre.setText("Suivre");
    boutonRelancer.setText("Relancer");
    boutonSeCoucher.setText("Se coucher");

    layoutBoutons->addWidget(&boutonChecker);
    layoutBoutons->addWidget(&boutonMiser);
    layoutBoutons->addWidget(&boutonSuivre);
    layoutBoutons->addWidget(&boutonRelancer);
    layoutBoutons->addWidget(&boutonSeCoucher);


    connect(&boutonChecker, SIGNAL(clicked()), this, SLOT(checker()));
    connect(&boutonMiser, SIGNAL(clicked()), this, SLOT(miser()));
    connect(&boutonSuivre, SIGNAL(clicked()), this, SLOT(suivre()));
    connect(&boutonRelancer, SIGNAL(clicked()), this, SLOT(relancer()));
    connect(&boutonSeCoucher, SIGNAL(clicked()), this, SLOT(seCoucher()));

    activeBoutons(false);

    // ////////////////////////////////////////////////////
    // Fenetre
    // ////////////////////////////////////////////////////

    next.setText("Next");
    next.setMaximumSize(70, 30);
    connect(&next, SIGNAL(clicked()), this, SLOT(demarragePartie()));


    layoutJoueur->setAlignment(Qt::AlignRight);
    layoutJoueur->setSpacing(50);

    layoutJoueur->addWidget(&caveJoueur);
    layoutJoueur->addWidget(&valeur);
    layoutJoueur->addLayout(layoutBoutons);

    layoutJeu->setSpacing(150);
    layoutJeu->setAlignment(Qt::AlignTop);

    layoutJeu->addLayout(&layoutMainAdverse);
    layoutJeu->addLayout(&layoutCartesCommunes);
    layoutJeu->addLayout(&layoutMain);

    layoutOptions->setSpacing(150);
    layoutOptions->setAlignment(Qt::AlignHCenter);

    layoutOptions->addWidget(&next);
    layoutOptions->addWidget(&caveIA);
    layoutOptions->addWidget(&pot);
    layoutOptions->addLayout(layoutJoueur);


    layout->addSpacing(200);
    layout->addLayout(layoutJeu);
    layout->addLayout(layoutOptions);

    setLayout(layout);

    connect(this, SIGNAL(tourFini()), this, SLOT(prochainJoueur()));
}

Fenetre::~Fenetre()
{

}

void Fenetre::demarragePartie()
{
    jeu->distributionMain();

    // Main du joueur

    std::vector<Carte> mainCourante = jeu->getJoueur(0).getMain();

    for (int i = 0; i < mainCourante.size(); i++){
        CarteGraphique *c = new CarteGraphique(mainCourante.at(i));
        layoutMain.addWidget(c);
    }

    // Main adverse

    CarteGraphique *dos = new CarteGraphique(0, 0);
    CarteGraphique *dos2 = new CarteGraphique(0, 0);

    layoutMainAdverse.addWidget(dos);
    layoutMainAdverse.addWidget(dos2);

    disconnect(&next, SIGNAL(clicked()), this, SLOT(demarragePartie()));

    if (jeu->getJoueurCourant() == 0) {     // Joueur humain
        joueurCourant();
    }
    else {                                  // Intelligence artificielle
        static_cast<IntelligenceArtificielle>(jeu->getJoueur(jeu->getJoueurCourant())).jouer();

        switch (jeu->getAction()) {
            case TYPES::ACTION_LIST::MISER:

                break;
            case TYPES::ACTION_LIST::RELANCER:

                break;
            case TYPES::ACTION_LIST::SE_COUCHER:

                break;
            default:
                break;
        }
    }

    disconnect(&next, SIGNAL(clicked()), this, SLOT(demarragePartie()));
    connect(&next, SIGNAL(clicked()), this, SLOT(joueurCourant()));
}

void Fenetre::afficheTable()
{
    QLayoutItem *item;

    while ((item = layoutCartesCommunes.takeAt(0)) != 0) {
        delete item->widget();
        delete item;
    }

    std::vector<Carte> table = jeu->getTable();

    for (int i = 0; i < table.size(); i++){
        CarteGraphique *c = new CarteGraphique(table.at(i));
        layoutCartesCommunes.addWidget(c);
    }

    //joueurCourant();
}

void Fenetre::activeBoutons(bool active)
{
    boutonChecker.setEnabled(active);
    boutonMiser.setEnabled(active);
    boutonRelancer.setEnabled(active);
    boutonSuivre.setEnabled(active);
    boutonSeCoucher.setEnabled(active);
}

void Fenetre::joueurCourant()
{
    activeBoutons(true);
    next.setEnabled(false);
}

void Fenetre::prochainJoueur()
{
    activeBoutons(false);

    jeu->prochainJoueur();

    if (jeu->debutTour()) {
        afficheTable();
    }

    if (jeu->getJoueurCourant() == 0) {     // Joueur humain
        joueurCourant();
    }
    else {                                  // Intelligence artificielle
        static_cast<IntelligenceArtificielle>(jeu->getJoueur(jeu->getJoueurCourant())).jouer();

        switch (jeu->getAction()) {
            case TYPES::ACTION_LIST::MISER:

                break;
            case TYPES::ACTION_LIST::RELANCER:

                break;
            case TYPES::ACTION_LIST::SE_COUCHER:

                break;
            default:
                break;
        }
    }

    next.setEnabled(true);
}

void Fenetre::checker()
{
    jeu->checker(0);

    emit tourFini();
}

void Fenetre::miser()
{
    jeu->miser(0, valeur.value());

    caveJoueur.display(jeu->getJoueur(0).getCave());
    pot.display(jeu->getPot());

    emit tourFini();
}

void Fenetre::suivre()
{
    jeu->suivre(0);

    emit tourFini();
}

void Fenetre::relancer()
{
    // TODO Vérifier que la valeur est supérieure à la mise courante

    jeu->relancer(0, valeur.value());

    emit tourFini();
}

void Fenetre::seCoucher()
{
    //jeu->seCoucher(0);

    emit tourFini();
}