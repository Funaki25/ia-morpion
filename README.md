# Projet SAE 2 : IA pour le jeu de Morpion

## **Description du projet**

Ce projet fait partie des activités de la **SAE 2 DEV**, qui vise à concevoir et développer une **IA capable de jouer au Morpion NxNxK**. Le but est d'aligner **K pions dans une grille NxN** en respectant les règles classiques du morpion. L'objectif principal était de **comparer différentes approches algorithmiques** pour évaluer leur qualité (taux de victoire) et leur performance (temps de calcul).

---

## **Fonctionnalités principales**

1. **Algorithme Minimax** :
   - Implémentation avec **élagage alpha-bêta**.
   - Limitation du temps d'exécution à **1 seconde** par décision.
   - Évaluation basée sur le potentiel de victoire (ou de blocage de l'adversaire).

2. **Détection des coups gagnants** :
   - Vérifie si un joueur peut gagner en un seul coup.
   - Priorise les actions permettant de **gagner** ou de **bloquer l'adversaire**.

3. **Flexibilité** :
   - Compatible avec des grilles de différentes tailles (**NxN**) et un nombre variable de pions à aligner (**K**).
   - Fonctionnement aussi bien pour les **joueurs humains** que pour l'IA.

---

## **Organisation du code**

- **`morpion.cpp`** : Contient les fonctions de base pour gérer la grille et les règles du jeu.
- **IA basée sur Minimax** :
  - **`jouerMachine()`** : Détermine le meilleur coup pour l'ordinateur.
  - **`minimax()`** : Évalue les scénarios futurs et choisit le coup optimal.

---

## **Méthodes algorithmiques**

### **Approches utilisées**

1. **Approche experte** :
   - Basée sur des règles déterministes simples (bloquer l'adversaire, aligner des pions, etc.).
   - Moins coûteuse en calcul mais plus limitée dans les scénarios complexes.

2. **Approche basée sur Minimax** :
   - Simulation exhaustive des possibilités à une profondeur limitée.
   - Permet des décisions stratégiques en évaluant les positions avantageuses.

---

## **Mesures de performance**

### **Temps de calcul**
- L'algorithme Minimax a été optimisé pour limiter son temps d'exécution grâce à :
  - Une profondeur maximale de recherche.
  - Un **élagage alpha-bêta** pour réduire le nombre de scénarios explorés.
  - Des mesures du **temps d'exécution** pour garantir une réactivité acceptable.


## **Instructions pour exécuter le projet**

1. **Compilation** :
   ```bash
   g++ -o morpion main.cpp morpion.cpp -std=c++11
