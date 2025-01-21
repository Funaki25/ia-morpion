#include <iostream>

#include <limits>
#include <ctime>
#include <chrono>
#include "morpion.h" // Inclusion des fonctions spécifiques au jeu Morpion

using namespace std;

// Constantes globales pour limiter le temps d'exécution et définir l'infini
const int TEMPS_LIMITE_MS = 1000; // 1 seconde
const int INF = numeric_limits<int>::max();

/**
 * Fonction pour évaluer la position actuelle du plateau.
 * @param tab : le plateau de jeu.
 * @param taille : taille de la grille.
 * @param joueur : symbole du joueur (ex : 'X' ou 'O').
 * @param adversaire : symbole de l'adversaire.
 * @param nbPion : nombre de pions nécessaires pour gagner.
 * @return Un score évaluant la position actuelle.
 */
int evaluerPosition(char** tab, int taille, char joueur, char adversaire, int nbPion) {
    int score = 0;
    // Directions à explorer (horizontale, verticale, diagonale et anti-diagonale)
    int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

    // Parcourt toutes les cases de la grille
    for (int x = 0; x < taille; ++x) {
        for (int y = 0; y < taille; ++y) {
            // Ignore les cases déjà occupées
            if (tab[x][y] == joueur || tab[x][y] == adversaire) continue;

            // Évalue chaque direction
            for (int d = 0; d < 4; ++d) {
                int countJoueur = 0, countAdversaire = 0;
                for (int i = 1; i < nbPion; ++i) {
                    int nx = x + i * directions[d][0];
                    int ny = y + i * directions[d][1];
                    if (nx >= 0 && ny >= 0 && nx < taille && ny < taille) {
                        if (tab[nx][ny] == joueur) countJoueur++;
                        if (tab[nx][ny] == adversaire) countAdversaire++;
                    }
                }
                // Calcule un score basé sur les pions alignés
                if (countAdversaire == 0) score += (countJoueur * countJoueur);
                if (countJoueur == 0) score -= (countAdversaire * countAdversaire);
            }
        }
    }
    return score;
}

/**
 * Vérifie si un joueur peut gagner en un seul coup.
 * @param tab : plateau de jeu.
 * @param taille : taille de la grille.
 * @param x, y : coordonnées du coup gagnant (si trouvé).
 * @param joueur : symbole du joueur.
 * @param nbPion : nombre de pions nécessaires pour gagner.
 * @return true si un coup gagnant est trouvé, false sinon.
 */
bool coupGagnant(char** tab, int taille, int& x, int& y, char joueur, int nbPion) {
    for (int i = 0; i < taille; ++i) {
        for (int j = 0; j < taille; ++j) {
            if (estLibre_morpion(tab, taille, i, j)) {
                tab[i][j] = joueur; // Place temporairement un pion
                if (victoire_morpion(tab, taille, nbPion, joueur)) {
                    x = i; y = j; // Enregistre le coup gagnant
                    tab[i][j] = ' '; // Réinitialise la case
                    return true;
                }
                tab[i][j] = ' '; // Réinitialise la case
            }
        }
    }
    return false;
}

/**
 * Implémentation de l'algorithme Minimax avec élagage alpha-bêta et limite de temps.
 */
int minimax(char** tab, int taille, int profondeur, bool maximiser, char joueur, char adversaire, int nbPion, int alpha, int beta, auto debut) {
    // Vérifie les conditions de victoire ou la profondeur maximale
    if (victoire_morpion(tab, taille, nbPion, joueur)) {
        return 1000 - profondeur;
    }
    if (victoire_morpion(tab, taille, nbPion, adversaire)){
         return -1000 + profondeur;
    }
    if (profondeur == 0 || chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - debut).count() > TEMPS_LIMITE_MS){
        return evaluerPosition(tab, taille, joueur, adversaire, nbPion);
    } 

    int meilleurScore;
    if (maximiser) {
        meilleurScore = -INF;
    } else {
        meilleurScore = INF;
    }

    // Parcourt toutes les cases pour tester les coups possibles
    for (int i = 0; i < taille; ++i) {
        for (int j = 0; j < taille; ++j) {
            if (estLibre_morpion(tab, taille, i, j)) {
                if (maximiser) {
                    tab[i][j] = joueur;
                } else {
                    tab[i][j] = adversaire;
                }// Simule un coup
                int score = minimax(tab, taille, profondeur - 1, !maximiser, joueur, adversaire, nbPion, alpha, beta, debut);
                tab[i][j] = ' '; // Réinitialise la case

                // Met à jour le score et effectue l'élagage alpha-bêta
                if (maximiser) {
                    meilleurScore = max(meilleurScore, score);
                    alpha = max(alpha, score);
                } else {
                    meilleurScore = min(meilleurScore, score);
                    beta = min(beta, score);
                }

                // Arrête si l'élagage est possible
                if (beta <= alpha) return meilleurScore;
            }
        }
    }
    return meilleurScore;
}

/**
 * Joue un coup pour la machine en fonction de l'algorithme Minimax.
 */
void jouerMachine(char** tab, int taille, int& x, int& y, int nbPl, int nbPion) {
    char joueur = 'O';
    char adversaire = 'X';

    // Chronométrer le temps de l'algorithme
    auto debut = chrono::high_resolution_clock::now();

    // Priorité : trouver un coup gagnant pour soi ou bloquer l'adversaire
    if (coupGagnant(tab, taille, x, y, joueur, nbPion)) {
        auto fin = chrono::high_resolution_clock::now();
        cout << "Temps pour trouver le coup gagnant : " 
             << chrono::duration_cast<chrono::milliseconds>(fin - debut).count() 
             << " ms" << endl;
        return;
    }
    if (coupGagnant(tab, taille, x, y, adversaire, nbPion)) {
        auto fin = chrono::high_resolution_clock::now();
        cout << "Temps pour bloquer l'adversaire : " 
             << chrono::duration_cast<chrono::milliseconds>(fin - debut).count() 
             << " ms" << endl;
        return;
    }

    // Utilisation de Minimax pour trouver le meilleur coup
    int meilleurScore = -INF;
    for (int i = 0; i < taille; ++i) {
        for (int j = 0; j < taille; ++j) {
            if (estLibre_morpion(tab, taille, i, j)) {
                tab[i][j] = joueur; // Simule un coup
                int score = minimax(tab, taille, 4, false, joueur, adversaire, nbPion, -INF, INF, debut);
                tab[i][j] = ' '; // Réinitialise la case

                // Met à jour le meilleur coup
                if (score > meilleurScore) {
                    meilleurScore = score;
                    x = i; y = j;
                }
            }
        }
    }

    auto fin = chrono::high_resolution_clock::now();
    cout << "Temps d'exécution du Minimax : " 
         << chrono::duration_cast<chrono::milliseconds>(fin - debut).count() 
         << " ms" << endl;
}

/**
 * Fonction principale du jeu.
 */
int main() {
    srand(time(NULL)); // Initialisation pour les tirages aléatoires
    char** tab; // Plateau de jeu
    int taille, nbPion;

    // Initialisation de la grille
    cout << "Entrez la taille de la grille : ";
    cin >> taille;
    cout << "Entrez le nombre de pions pour gagner : ";
    cin >> nbPion;

    init_morpion(tab, taille); // Prépare la grille
    bool alignement = false; // Indique si une victoire a été détectée
    bool tourHumain = true; // Gère le tour des joueurs
    int nbPl = 0; // Compteur des coups joués

    // Boucle principale du jeu
    while (!alignement && nbPl < taille * taille) {
        affichage_morpion(tab, taille); // Affiche la grille
        char pion;
        if (tourHumain) {
            pion = 'X';
        } else {
            pion = 'O';
        }
        int x = -1, y = -1;

        if (tourHumain) {
            // Saisie du joueur humain
            do {
                cout << "Entrez la ligne (0-" << taille - 1 << "): ";
                cin >> x;
                cout << "Entrez la colonne (0-" << taille - 1 << "): ";
                cin >> y;
            } while (!estLibre_morpion(tab, taille, x, y));
        } else {
            // Coup de la machine
            jouerMachine(tab, taille, x, y, nbPl, nbPion);
        }

        placer_morpion(tab, taille, x, y, pion); // Place le pion sur la grille
        affichage_morpion(tab, taille); // Affiche la grille mise à jour
        alignement = victoire_morpion(tab, taille, nbPion, pion); // Vérifie la victoire
        nbPl++; // Incrémente le nombre de coups
        tourHumain = !tourHumain; // Change le tour
    }

    // Affichage du résultat
    if (!alignement) {
        cout << "Match nul! Aucun joueur n'a aligne " << nbPion << " pions." << endl;
    } else {
        cout << "Le joueur '" << (tourHumain ? 'O' : 'X') << "' a gagne!" << endl;
    }

    // Libère la mémoire
    for (int i = 0; i < taille; ++i) delete[] tab[i];
    delete[] tab;

    return 0;
}
