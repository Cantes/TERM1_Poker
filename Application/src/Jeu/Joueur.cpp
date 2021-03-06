#include "../../include/Jeu/Joueur.h"
#include "../../include/Jeu/Jeu.h" 

Joueur::Joueur(bool estDealer, int jetons, int position){
	this->dealer = estDealer;
	this->cave = jetons;
	this->position = position;
	this->misePlusHauteJoueur = 0;
    this->compteurActions = new int[3];
    for(int i =0; i<3;i++){
        this->compteurActions[i] = 0;
    }
}


Joueur::~Joueur(){

}

int Joueur::getCave() const{
	return this->cave;
}


int Joueur::getPosition() const{
	return this->position;
}


std::vector<Carte> Joueur::getMain() const{
	return this->main;
}


void Joueur::ajouteJetons(int jetons){
	this->cave = this->cave + jetons;
}


void Joueur::retireJetons(int jetons){
	this->cave = this->cave - jetons;
}


void Joueur::ajouteCarte(Carte carte){
	if(this->main.size() == 0){
		this->main.push_back(carte);
	}else{
		if(carte.getRang() == 1){
			Carte c = this->main.at(0);
			this->main.pop_back();
			this->main.push_back(carte);
			this->main.push_back(c);
		}else if(this->main.at(0).getRang() == 1){
			this->main.push_back(carte);
		}else if(this->main.at(0).getRang() < carte.getRang()){
			Carte c = this->main.at(0);
			this->main.pop_back();
			this->main.push_back(carte);
			this->main.push_back(c);
		}else{
			this->main.push_back(carte);
		}
	}
}


bool Joueur::estDealer(){
	return this->dealer;
}

void Joueur::resetMain(){
	this->main.clear();
}


void Joueur::setJeu(Jeu* j){
	this->jeu = j;
}


Jeu* Joueur::getJeu(){
	return this->jeu;
}

void Joueur::changeDealer(){
	if(this->dealer){
		this->dealer = false;
	}else{
		this->dealer = true;
	}
}

int Joueur::getMisePlusHauteJoueur(){
	return this->misePlusHauteJoueur;
}

void Joueur::setMisePlusHauteJoueur(int jetons){
	this->misePlusHauteJoueur = jetons;
}

void Joueur::videMain(){
	this->main.clear();
}

void Joueur::resetCompteurActions(){
    for(int i = 0 ; i <3; i++){
        this->compteurActions[i] = 0;
    }
}

int* Joueur::getCompteurActions(){
    return this->compteurActions;
}

int Joueur::getMiseTotaleJoueur(){
    return this->miseTotaleJoueur;
}

void Joueur::setMiseTotaleJoueur(int mise){
    this->miseTotaleJoueur = mise;
}

Profilage* Joueur::getProfil(){
    return this->profilJoueur;
}

void Joueur::setProfil(std::string pseudo){
    this->profilJoueur = new Profilage(pseudo);
}
