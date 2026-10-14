ENGLISH VERSION BELOW

# Application principale

=================================

## Présentation

Ce firmware est conçu pour être utilisé avec un appareil Lily's Pad utilisant un ATmega2560.

Le firmware prend la forme d'une application qui permet à l'utilisateur d'apprendre et de manipuler les signes de la LSF (Langue des Signes Française) et de la BSL (British Sign Language) grâce à une interface composée de 3 écrans, d'un clavier matriciel et d'un encodeur rotatif.

L'application possède trois fonctionnalités principales :

- Traduction
- Alphabet
- Jeu

## Fonctionnement général

Au démarrage, le microcontrôleur initialise les différents périphériques :

1. Les écrans
2. Le clavier matriciel
3. Le MCP23017 lié à l'encodeur rotatif
4. Le système de sélection de langue
5. Les différents états de l'application

Une fois l'initialisation terminée, le menu principal est affiché.

L'application fonctionne ensuite dans une boucle principale qui vérifie continuellement :

- le changement de langue.
- la rotation de l'encodeur.
- l'appui sur le bouton de l'encodeur.
- l'activité du clavier.
- le système d'économie d'énergie .
- l'état actuel de l'application.

Le comportement de l'interface dépend principalement de machines à états.

## Matériel

Le projet utilise principalement :

- ATmega2560 : microcontrôleur principal

- ILI9488 : écran principal

- 2 × GC9A01 : écrans utilisés pour les yeux

- MCP23017 : pour parler en i2c au rotary encoder

- Encodeur rotatif : navigation et sélection

- Clavier matriciel 4 × 10 : saisie de caractères

- Carte SD : stockage des images

- Switch : sélection de la langue

## Menu principal

> Le menu principal contient trois choix :

> - Traduction

> - Alphabet

> - Jeu

L'encodeur permet de déplacer le curseur.

Le bouton permet de sélectionner l'élément choisi.

Le choix actuel est stocké dans menu_choice.

### Traduction

La fonctionnalité de traduction permet de saisir un mot à l'aide du clavier.

**Fonctionnement :**

- Entrer dans Traduction

- Saisir un mot

- Valider avec l'encodeur

- Afficher le signe de la première lettre

- Appuyer pour passer à la lettre suivante

- Afficher le signe suivant

- Continuer jusqu'à la dernière lettre

- Revenir à une nouvelle saisie

Le mot est stocké dans `word` et sa longueur dans `word_len`.

La touche “delete” permet de supprimer le dernier caractère.

Lorsque le mot est validé, le programme recherche les images (stockées dans la SD) correspondant aux différentes lettres via une look up table.

Les images dépendent de la langue sélectionnée.

Pour la LSF, le programme recherche les images associées au suffixe LSF.

Pour la BSL, le programme recherche les images associées au suffixe BSL.

Les images sont ensuite affichées une par une sur l'écran principal.

---

### Alphabet

La fonctionnalité Alphabet permet de parcourir les caractères disponibles.

L'utilisateur utilise l'encodeur pour se déplacer dans la liste.

Un appui sur l'encodeur rotatif permet d'afficher la lettre selectionnée.

Un nouvel appui permet de revenir à la liste.

**Les deux principaux états sont :**

ALPHABET_LIST

ALPHABET_LETTER

---

### Jeux

L'application possède deux jeux.

**Jeu 1 : trouver la bonne lettre**

Le premier jeu consiste à reconnaître une lettre à partir d'un signe.

Le programme choisit aléatoirement une lettre et affiche le signe correspondant.

L'utilisateur doit ensuite choisir la bonne lettre parmi trois propositions.

Déroulement :

Signe affiché

3 propositions

Sélection avec l'encodeur

Validation avec le bouton

Affichage de OUI ou NON

Une seule des trois réponses correspond au signe affiche.

Si la réponse est correcte, une nouvelle question est générée.

Si la reéponse est incorrecte, le jeu revient à l'écran de depart du jeu.

**Jeu 2 : trouver le bon signe**

Le fonctionnement est inversé.

Le programme choisit une lettre et l'affiche.

Trois signes sont ensuite présentés successivement.

Déroulement :

Lettre affichée

Signe 1

Signe 2

Signe 3

Choix entre 1, 2 et 3

Validation avec le bouton

Affichage de OUI ou NON

Une seule des trois images correspond à la lettre donnée.

Les choix et les lettres sont générés aléatoirement.

---

## Encodeur rotatif

L'encodeur est connecté au MCP23017, lui-même relié à l'ATmega2560 par I²C.

Il possède trois signaux :

**CLK** : rotation

**DT** : direction

**SW** : bouton

La rotation permet de déplacer les curseurs dans les différents menus.

Le bouton permet de valider ou de passer à l'étape suivante.

Le comportement de l'encodeur dépend de l'état actuel de l'application.

Par exemple :

Dans le MENU, l'encodeur déplace le curseur du menu.

Dans l'ALPHABET, il permet de parcourir les lettres.

Dans le JEU, il permet de sélectionner une réponse.

Le bouton possède également un système d'anti-rebond afin d'éviter plusieurs détections pour un seul appui.

## Clavier

Le clavier est une matrice de 4 lignes et 10 colonnes, soit 38 touches.

La disposition utilisée est :

1 2 3 4 5 6 7 8 9 0

Q W E R T Y U I O P

A S D F G H J K L -

Z X C V B N M # /

Le '#' est pour le retour a l'ecran principal, '/' pour supprimer.

Le programme sélectionne chaque ligne successivement et lit les colonnes afin de détecter la touche appuyée.

Le tableau `keymap` permet ensuite de convertir la position de la touche en caractère.

## Gestion de la langue

Deux langues sont disponibles :

LANG_FR

LANG_EN

La langue est selectionnée grâce à un switch matériel.

La variable `language` permet de connaître la langue actuellement utilisée.

Les textes affichés et les images recherchées dependent de cette langue.

## Gestion de l'énergie

L'application possède un système d'économie d'énergie.

Lorsqu'aucune interaction n'est détectée pendant un certain temps, l'application passe en veille.

Le retro-éclairage de l'écran principal est alors désactivé et les yeux changent d'affichage.

Une activite de l'utilisateur reactive le système.

Les principales actions considérées comme une activité sont :

- rotation de l'encodeur
- appui sur le bouton
- appui sur une touche du clavier
- changement de langue

La fonction `power_save_activity()` remet le système en activité.

La fonction `power_save_update()` gère le compteur d'inactivité, après une vingtaine de secondes on passe en économie d’énergie.

### Architecture du code

Le projet est organisé en plusieurs modules.

Le dossier app contient la logique principale de l'application :

- menu

- traduction

- alphabet

- game

Le dossier display contient la gestion des écrans :

- ILI9488

- GC9A01

- affichage du texte

Le dossier input contient la gestion des entrées :

- clavier

- encodeur

- I²C / MCP23017

Le dossier système contient les éléments communs au système :

- globals

- power_save

- Le dossier utils contient les fonctions utilitaires.

- Chaque module possède une responsabilité spécifique.

- Les modules app gèrent la logique de l'application.

- Les modules input gèrent les interactions avec l'utilisateur.

- Les modules display gèrent les ecrans.

- Les modules système regroupent les éléments communs au fonctionnement du système.

- Machine à états

- Le fonctionnement de l'application repose sur plusieurs niveaux d'états.

- L'état principal est app_state.

Il permet de savoir si l'utilisateur se trouve dans :

MENU

TRADUCTION

ALPHABET

JEU

Chaque fonctionnalité possede ensuite ses propres états.

Par exemple, pendant un jeu :

- Afficher la question

- Afficher les propositions

- Sélectionner une réponse

- Valider

- Afficher OUI ou NON

- Passer à la question suivante

Cette organisation permet au même bouton ou au même mouvement de l'encodeur d'avoir un comportement différent selon la situation.

## Boucle principale

La boucle principale permet à l'application de surveiller en permanence les entrées utilisateur et de faire évoluer la machine à etats.

Elle gère notamment :

- la langue

- le générateur aleatoire

- l'encodeur

- le bouton de l'encodeur

- l'économie d'énergie

- le clavier

- la fonctionnalité actuellement sélectionnée.

## Résumé

L'application est organisée autour de trois éléments principaux.

**Entrées :**

- Clavier matriciel

- Encodeur rotatif

- Bouton de l'encodeur

- Switch de langue

**Traitement :**

- Machines à états

- Navigation dans les menus

- Gestion de la traduction

- Gestion de l'alphabet

- Gestion des jeux

- Generation aléatoire

- Gestion de l'économie d'énergie

**Sorties :**

- Ecran ILI9488

- Deux écrans GC9A01

- Images de signes stockées sur carte SD

Le système utilise principalement une architecture basée sur les états. Cela permet de gérer les differentes fonctionnalités de l'application et d'adapter le comportement du clavier, de l'encodeur et du bouton en fonction de l'écran ou de l'action en cours.

---

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

---

### Alphabet

The Alphabet functionality allows you to browse through the available characters.

The user uses the encoder to move into the list.

Pressing the button displays the selected letter.

A new tap allows you to return to the list.

**The two main states are:***

ALPHABET_LIST

ALPHABET_LETTER

---

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

---

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

'#' is for HOME and '/' is for delete.

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

**MENU**

**TRANSLATION**

**ALPHABET**

**GAME**

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

**Entries:**

- Matrix keyboard

- Rotary encoder

- Encoder button

- Switch language

**Treatment:**

- State machines

- Menu navigation

- Translation management

- Management of the alphabet

- Game management

- Random generation

- Energy saving management

**Outings:**

- Screen ILI9488

- Two GC9A01 screens

- Sign images stored on an SD card

The system mainly uses a state-based architecture. This allows to manage the different functionalities of the application and to adapt the behavior of the keyboard, the encoder and the button according to the screen or the current action.
