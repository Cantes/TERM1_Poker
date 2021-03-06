/*========================================================================
Nom: ChoixOptionsDialog.cpp       Auteur: Morgane Vidal
Maj: 11/03/2015            Creation: 11/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe
                                                        ChoixOptionsDialog.
=========================================================================*/

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include<iostream>
#include<QFormLayout>
#include<QFile>
#include<QTextStream>
#include "../../include/Interface/ChoixOptionsDialog.h"


using namespace std;


ChoixOptionsDialog::ChoixOptionsDialog() : QDialog(){

    //titre fenêtre
    setWindowTitle("Choix des options");

    //fenetre
    QVBoxLayout *boite = new QVBoxLayout;

    ajouterPseudosConnus();

    QLabel *calibrageDefaut = new QLabel("Rationalité : " + QString::number(rationaliteDefaut)
                                           + "%\nAgressivité : " + QString::number(agressiviteDefaut) + "%");

    // Rationalité
    QHBoxLayout *layoutRationalite = new QHBoxLayout;

    rationalite.setRange(0, 100);
    rationalite.setValue(rationaliteDefaut);
    rationalite.setOrientation(Qt::Horizontal);

    QLabel *valRationalite = new QLabel(QString::number(rationaliteDefaut));

    connect(&rationalite, SIGNAL(valueChanged(int)), valRationalite, SLOT(setNum(int)));

    layoutRationalite->addWidget(&rationalite);
    layoutRationalite->addWidget(valRationalite);


    // Agressivité
    QHBoxLayout *layoutAgressivite = new QHBoxLayout;

    agressivite.setRange(0, 100);
    agressivite.setValue(agressiviteDefaut);
    agressivite.setOrientation(Qt::Horizontal);

    QLabel *valAgressivite = new QLabel(QString::number(agressiviteDefaut));

    connect(&agressivite, SIGNAL(valueChanged(int)), valAgressivite, SLOT(setNum(int)));

    layoutAgressivite->addWidget(&agressivite);
    layoutAgressivite->addWidget(valAgressivite);


    //bouton valider
    QPushButton *bouton=new QPushButton("Valider");

    //Formulaire
    QFormLayout *formulaire = new QFormLayout;
    formulaire->addRow("Nouveau pseudo ",&nouveau);
    formulaire->addRow("Pseudo déjà existant",&pseudos);
    formulaire->addRow("Calibrage par défaut", calibrageDefaut);
    formulaire->addRow("Rationalité", layoutRationalite);
    formulaire->addRow("Agressivité", layoutAgressivite);

    //Ajout du formulaire et du bouton valider dans la fenetre
    boite->addLayout(formulaire);
    boite->addWidget(bouton);

    //setLayout(formulaire);
    setLayout(boite);

    //Connection avec l'interface graphique du jeu
    connect(bouton, SIGNAL(clicked()), this, SLOT(accept()));

}

void ChoixOptionsDialog::ajouterPseudosConnus(){

    //Ajout d'un pseudo vide :
    pseudos.addItem("");
    //Ouverture du fichier
    QFile fichierPseudos("../Application/ressources/Profilage/ProfilageStatique/pseudos.txt");

    if(!fichierPseudos.open(QIODevice::ReadOnly | QIODevice::Text)){
        cerr<<"Erreur lors de l'ouverture du fichier contenant la liste des pseudos"<<endl;
        return;
    }
    QTextStream flux(&fichierPseudos);
    QString ligne;
    while(! flux.atEnd()){
        ligne = flux.readLine();
        pseudos.addItem(ligne);
    }
}

Options ChoixOptionsDialog::getOptions(){

    Options options;

    if (exec() == QDialog::Accepted) {
        QString nouveauPseudo = nouveau.text();
        QString ancienPseudo = pseudos.currentText();

        if(ancienPseudo.isEmpty() && !nouveauPseudo.isEmpty()){
            options.pseudo = nouveauPseudo;
        }
        else if(!ancienPseudo.isEmpty()){
            options.pseudo = ancienPseudo;
        }
        else{
            cerr<<"Pas de pseudo"<<endl;
            options.pseudo = "";
        }

        options.rationaliteIA = rationalite.value();
        options.agressiviteIA = agressivite.value();
    }
    else {
        options.pseudo = "";
        options.rationaliteIA = static_cast<double>(rationaliteDefaut);
        options.agressiviteIA = static_cast<double>(agressiviteDefaut);
    }

    return options;
}
