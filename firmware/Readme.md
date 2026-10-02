ENGLISH VERSION BELOW

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


-------------------------------------------------------------------------------------------------------

# Primary Application

=================================

## Presentation

This application is a project developed on an ATmega2560.

It allows the user to learn and manipulate the signs of LSF (French Sign Language) and BSL (British Sign Language) thanks to an interface composed of 3 screens, a matrix keyboard and a rotary encoder.

The application has three main functionalities:

- Translation
- Alphabet
- Game

## General operation

At startup, the microcontroller initializes the different devices:

1. The screens
2. The matrix keyboard
3. The MCP23017 is connected to the rotary encoder
4. The language selection system
5. The different states of the application

Once the initialization is complete, the main menu appears.

The application then runs in a main loop that continuously checks:

- the change of language.
- the encoder rotation.
- pressing the encoder button.
- the keyboard activity.
- the energy-saving system.
- the current status of the application.

The behavior of the interface mainly depends on state machines.

## Hardware

The project mainly uses:

- ATmega2560: Primary microcontroller

- ILI9488: main screen

- 2 × GC9A01: screens used for the eyes

- MCP23017: to speak in i2c at the rotary encoder

- rotary encoder: navigation and selection

- Matrix keyboard 4 × 10: character input

- SD card: image storage

- Switch: language selection

## Main Menu

> The main menu contains three choices:

> - Translation

> - Alphabet

> - Game

The encoder allows you to move the cursor.

The button allows you to select the chosen element.

The current choice is stored in menu_choice.

### Translation

Translation functionality allows you to enter a word using the keyboard.

**Operation:***

- Enter Translation

- Enter a word

- Validate with the encoder

- Display the sign of the first letter

- Press to move on to the next letter

- Display the following sign

- Continue until the last letter

- Return to a new entry

The word is stored in word and its length in word_len.

The key "delete" allows to delete the last character.

When the word is valid, the program searches for images corresponding to the different letters on the SD card.

The images dependent on the selected language.

For LSF, the program searches for images associated with the LSF suffix.

For the BSL, the program searches for images associated with the BSL suffix.

The images are then displayed one by one on the main screen.

----------------------------------------------------------------------
### Alphabet

The Alphabet functionality allows you to browse through the available characters.

The user uses the encoder to move into the list.

Pressing the button displays the selected letter.

A new tap allows you to return to the list.

**The two main states are:***

ALPHABET_LIST

ALPHABET_LETTER

----------------------------------------------------------------------
### Games

The app has two games.

**Game 1: find the right letter**

The first game is to recognize a letter from a sign.

The program randomly chooses a letter and displays the corresponding sign.

The user must then choose the correct letter from among three proposals.

Layout:

Poster sign

3 proposals

Selection with the encoder

Validation with the button

Display YES or NO

Only one of the three answers corresponds to the poster sign.

If the answer is correct, a new question is generated.

If the answer is incorrect, the game returns to the game’s start screen.

**Game 2: find the right sign**

The operation is reversed.

The program chooses a letter and displays it.

Three signs are then presented successively.

Layout:

Letter displayed

Sign 1

Sign 2

Sign 3

Choice between 1, 2, and 3

Validation with the button

Display YES or NO

Only one of the three images corresponds to the given letter.

Choices and letters are randomly generated.

----------------------------------------------------------------------
## Rotary encoder

The encoder is connected to the MCP23017, which in turn is connected to the ATmega2560 by I 2 C.

He has three signals:

**CLK** : rotation

**DT** : management

**SW** : button

Rotation allows you to move the cursors in different menus.

The button allows you to validate or move on to the next step.

The encoder behavior depends on the current state of the application.

For example:

In the MENU, the encoder moves the menu cursor.

In the ALPHABET, it allows you to move through the letters.

In the GAME, it allows you to select a response.

The button also has an anti-bounce system to avoid several detections for a single touch.

## Keyboard

The keyboard is a matrix of 4 rows and 10 columns, or 38 keys.

The layout used is:

1 2 3 4 5 6 7 8 9 0

Q W E R T Y U I O P

A S D F G H J K L

Z X C V B N M # /

The program selects each row successively and reads the columns in order to detect the key pressed.

The keymap table then allows you to convert the key position into a character.

## Language Management

Two languages are available:

LANG_FR

LANG_EN

The language is selected thanks to a hardware switch.

The language variable lets you know which language is currently in use.

The poster texts and images sought after in this language.

## Energy Management

The application has an energy-saving system.

When no interaction is detected for a period of time, the application goes into standby.

The backlighting of the main screen is then deactivated and the eyes change their display.

An activity of the user reactivates the system.

The main actions considered as an activity are:

- rotation of the encoder
- press the button
- press a key on the keyboard
- change of language

The power_save_activity() function restarts the system.

The power_save_update() function manages the inactivity counter, after about 20 seconds you switch to power saving.

### Code Architecture

The project is organized into several modules.

The app folder contains the main logic of the application:

- menu

- translation

- alphabet

- game

The display folder contains screen management:

- ILI9488

- GC9A01

- displaying the text

The input folder contains the management of entries:

- keyboard

- encoder

- I 2 C / MCP23017

The system folder contains the common elements of the system:

- globals

- power_save

- The utils folder contains the utility functions.

- Each module has a specific responsibility.

- The app modules manage the logic of the application.

- The input modules manage interactions with the user.

- The display modules handle the screens.

- The system modules bring together the common elements of the system’s operation.

- State machine

- The operation of the application is based on several levels of states.

- The main state is app_state.

It lets you know if the user is located in:

MENU

TRANSLATION

ALPHABET

GAME

Each functionality then has its own states.

For example, during a game:

- View the question

- View the proposals

- Select a response

- Validate

- Show YES or NO

- Move on to the next question

This organization allows the same button or the same movement of the encoder to have a different behavior depending on the situation.

## Main loop

The main loop allows the application to constantly monitor user inputs and to make the state machine evolve.

She manages in particular:

- the language

- the random generator 

- the encoder 

- the encoder button 

- energy saving 

- the keyboard 

- the functionality currently selected.

## Resume

The application is organized around three main elements.

**Entries:***

- Matrix keyboard

- Rotary encoder

- Encoder button

- Switch language

**Treatment:***

- State machines

- Menu navigation

- Translation management

- Management of the alphabet

- Game management

- Random generation

- Energy saving management

**Outings:***

- Screen ILI9488

- Two GC9A01 screens

- Sign images stored on an SD card

The system mainly uses a state-based architecture. This allows to manage the different functionalities of the application and to adapt the behavior of the keyboard, the encoder and the button according to the screen or the current action.