# Application principale

=================================

## Presentation

Cette application est un projet embarque developpe sur un ATmega2560.

Elle permet à l'utilisateur d'apprendre et de manipuler les signes de la LSF (Langue des Signes Française) et de la BSL (British Sign Language) grâce à une interface composee de 3 ecrans, d'un clavier matriciel et d'un encodeur rotatif.

L'application possede trois fonctionnalites principales :

- Traduction
- Alphabet
- Jeu

## Fonctionnement general

Au demarrage, le microcontrôleur initialise les differents peripheriques :

1. Les ecrans
2. Le clavier matriciel
3. Le MCP23017 lie a l'encodeur rotatif
4. Le systeme de selection de langue
5. Les differents etats de l'application

Une fois l'initialisation terminee, le menu principal est affiche.

L'application fonctionne ensuite dans une boucle principale qui verifie continuellement :

- le changement de langue.
- la rotation de l'encodeur.
- l'appui sur le bouton de l'encodeur.
- l'activite du clavier.
- le systeme d'economie d'energie .
- l'etat actuel de l'application.

Le comportement de l'interface depend principalement de machines à etats.

## Materiel

Le projet utilise principalement :

- ATmega2560 : microcontrôleur principal

- ILI9488 : ecran principal

- 2 × GC9A01 : ecrans utilises pour les yeux

- MCP23017 : pour parler en i2c au rotary encoder

- encodeur rotatif : navigation et selection

- Clavier matriciel 4 × 10 : saisie de caracteres

- Carte SD : stockage des images

- Switch : selection de la langue

## Menu principal

> Le menu principal contient trois choix :

> - Traduction

> - Alphabet

> - Jeu

L'encodeur permet de deplacer le curseur.

Le bouton permet de selectionner l'element choisi.

Le choix actuel est stocke dans menu_choice.

### Traduction

La fonctionnalite de traduction permet de saisir un mot à l'aide du clavier.

**Fonctionnement :**

- Entrer dans Traduction

- Saisir un mot

- Valider avec l'encodeur

- Afficher le signe de la premiere lettre

- Appuyer pour passer à la lettre suivante

- Afficher le signe suivant

- Continuer jusqu'à la derniere lettre

- Revenir à une nouvelle saisie

Le mot est stocke dans word et sa longueur dans word_len.

La touche “delete” permet de supprimer le dernier caractere.

Lorsque le mot est valide, le programme recherche les images correspondant aux differentes lettres sur la carte SD.

Les images dependent de la langue selectionnee.

Pour la LSF, le programme recherche les images associees au suffixe LSF.

Pour la BSL, le programme recherche les images associees au suffixe BSL.

Les images sont ensuite affichees une par une sur l'ecran principal.

----------------------------------------------------------------------
### Alphabet

La fonctionnalite Alphabet permet de parcourir les caracteres disponibles.

L'utilisateur utilise l'encodeur pour se deplacer dans la liste.

Un appui sur le bouton permet d'afficher la lettre selectionnee.

Un nouvel appui permet de revenir à la liste.

**Les deux principaux etats sont :**

ALPHABET_LIST

ALPHABET_LETTER

----------------------------------------------------------------------
### Jeux

L'application possede deux jeux.

**Jeu 1 : trouver la bonne lettre**

Le premier jeu consiste à reconnaître une lettre à partir d'un signe.

Le programme choisit aleatoirement une lettre et affiche le signe correspondant.

L'utilisateur doit ensuite choisir la bonne lettre parmi trois propositions.

Deroulement :

Signe affiche

3 propositions

Selection avec l'encodeur

Validation avec le bouton

Affichage de OUI ou NON

Une seule des trois reponses correspond au signe affiche.

Si la reponse est correcte, une nouvelle question est generee.

Si la reponse est incorrecte, le jeu revient à l'ecran de depart du jeu.

**Jeu 2 : trouver le bon signe**

Le fonctionnement est inverse.

Le programme choisit une lettre et l'affiche.

Trois signes sont ensuite presentes successivement.

Deroulement :

Lettre affichee

Signe 1

Signe 2

Signe 3

Choix entre 1, 2 et 3

Validation avec le bouton

Affichage de OUI ou NON

Une seule des trois images correspond à la lettre donnee.

Les choix et les lettres sont generes aleatoirement.

----------------------------------------------------------------------
## Encodeur rotatif

L'encodeur est connecte au MCP23017, lui-meme relie à l'ATmega2560 par I²C.

Il possede trois signaux :

**CLK** : rotation

**DT** : direction

**SW** : bouton

La rotation permet de deplacer les curseurs dans les differents menus.

Le bouton permet de valider ou de passer à l'etape suivante.

Le comportement de l'encodeur depend de l'etat actuel de l'application.

Par exemple :

Dans le MENU, l'encodeur deplace le curseur du menu.

Dans l'ALPHABET, il permet de parcourir les lettres.

Dans le JEU, il permet de sectelionner une reponse.

Le bouton possede egalement un systeme d'anti-rebond afin d'eviter plusieurs detections pour un seul appui.

## Clavier

Le clavier est une matrice de 4 lignes et 10 colonnes, soit 38 touches.

La disposition utilisee est :

1 2 3 4 5 6 7 8 9 0

Q W E R T Y U I O P

A S D F G H J K L -

Z X C V B N M # /

Le programme selectionne chaque ligne successivement et lit les colonnes afin de detecter la touche appuyee.

Le tableau keymap permet ensuite de convertir la position de la touche en caractere.

## Gestion de la langue

Deux langues sont disponibles :

LANG_FR

LANG_EN

La langue est selectionnee grâce à un switch materiel.

La variable language permet de connaître la langue actuellement utilisee.

Les textes affiches et les images recherchees dependent de cette langue.

## Gestion de l'energie

L'application possede un systeme d'economie d'energie.

Lorsqu'aucune interaction n'est detectee pendant un certain temps, l'application passe en veille.

Le retroeclairage de l'ecran principal est alors desactive et les yeux changent d'affichage.

Une activite de l'utilisateur reactive le systeme.

Les principales actions considerees comme une activite sont :

- rotation de l'encodeur
- appui sur le bouton
- appui sur une touche du clavier
- changement de langue

La fonction power_save_activity() remet le systeme en activite.

La fonction power_save_update() gere le compteur d'inactivite, apres une 20aine de secondes on passe en economie d’energie.

### Architecture du code

Le projet est organise en plusieurs modules.

Le dossier app contient la logique principale de l'application :

- menu

- traduction

- alphabet

- game

Le dossier display contient la gestion des ecrans :

- ILI9488

- GC9A01

- affichage du texte

Le dossier input contient la gestion des entrees :

- clavier

- encodeur

- I²C / MCP23017

Le dossier system contient les elements communs au systeme :

- globals

- power_save

- Le dossier utils contient les fonctions utilitaires.

- Chaque module possede une responsabilite specifique.

- Les modules app gerent la logique de l'application.

- Les modules input gerent les interactions avec l'utilisateur.

- Les modules display gerent les ecrans.

- Les modules system regroupent les elements communs au fonctionnement du systeme.

- Machine à etats

- Le fonctionnement de l'application repose sur plusieurs niveaux d'etats.

- L'etat principal est app_state.

Il permet de savoir si l'utilisateur se trouve dans :

MENU

TRADUCTION

ALPHABET

JEU

Chaque fonctionnalite possede ensuite ses propres etats.

Par exemple, pendant un jeu :

- Afficher la question

- Afficher les propositions

- Selectionner une reponse

- Valider

- Afficher OUI ou NON

- Passer à la question suivante

Cette organisation permet au meme bouton ou au meme mouvement de l'encodeur d'avoir un comportement different selon la situation.

## Boucle principale

La boucle principale permet à l'application de surveiller en permanence les entrees utilisateur et de faire evoluer la machine à etats.

Elle gere notamment :

- la langue

- le generateur aleatoire 

- l'encodeur 

- le bouton de l'encodeur 

- l'economie d'energie 

- le clavier 

- la fonctionnalite actuellement selectionnee.

## Resume

L'application est organisee autour de trois elements principaux.

**Entrees :**

- Clavier matriciel

- Encodeur rotatif

- Bouton de l'encodeur

- Switch de langue

**Traitement :**

- Machines à etats

- Navigation dans les menus

- Gestion de la traduction

- Gestion de l'alphabet

- Gestion des jeux

- Generation aleatoire

- Gestion de l'economie d'energie

**Sorties :**

- Ecran ILI9488

- Deux ecrans GC9A01

- Images de signes stockees sur carte SD

Le systeme utilise principalement une architecture basee sur les etats. Cela permet de gerer les differentes fonctionnalites de l'application et d'adapter le comportement du clavier, de l'encodeur et du bouton en fonction de l'ecran ou de l'action en cours.