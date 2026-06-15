
# Plusieurs fichiers pour plusieurs tests:

### Utils:

uart.c -> les fonctions pour afficher en uart

### Test bouton unique:

unique-buttontest.c -> voir si je peux detecter quand un bouton est presse ou pas, et ecrire quelque chose associe a ce bouton

### Test plusieurs bouton:

test-2x3-keyboard.c -> Voir si je peux faire une matrice de bouton fonctionnelle
4x10-keyboard.c -> Code adapte a un clavier en 4x10, plus facile a readapter au besoin, est code avec les pins choisis dans le pinout mais qui ne sont pas compatible avec la arduino mega qu'on utilise pour le prototypage
ard-4x10-keyboard.c -> Code adapte a un clavier en 4x10, mais avec les pin adequates pour le prototypage  avec le dev kit du lab

1   2   3   4   5   6   7   8   9   0
Q   W   E   R   T   Y   U   I   O   P
A   S   D   F   G   H   J   K   L   \N
Z   X   C   V   B   N   M   /   /   /