# Rapport de développement du projet NumberMatch

## Première itération (07/10 → 21/10)

- Création de la logique principale du jeu à l’aide de structures et de fonctions.
- Implémentation complète de toutes les règles du jeu.
- Développement d’une interface console permettant l’interaction avec l’utilisateur.
- Refonte de la structure du jeu afin de permettre l’ajout de nouveaux types d’interfaces sans supprimer les anciennes.
- Préparation pour permettre le choix du type d’interface via un paramètre passé au lancement du programme.

```
> numbermatch console
ou 
> numbermatch mlv
```

### Second iteration (04/11 → 18/11)
- Création d’une interface graphique en utilisant la bibliothèque **MLV** pour game_screen.
- Ajout d’animations dans l’interface du jeu pour rendre l’affichage plus dynamique.

### Troisième itération (19/11 → 25/11)

- Création de l’interface de menu et du tutoriel interactif.
- Refonte de `game_field` : remplacement du tableau statique par un tableau dynamique pour plus de flexibilité.
- Mise à jour des commentaires dans le code pour respecter le style **Doxygen**.
---

# NumberMatch Project Development Report

## First iteration (07/10 → 21/10)

- Created the core game logic using structures and functions.
- Fully implemented all gameplay rules.
- Built a console interface for user interaction.
- Modified the game structure to support adding new interface types without removing previous ones.
- Planned feature to allow selecting the interface type through a startup parameter.

```
> numbermatch console
ou 
> numbermatch mlv
```


## Second iteration (04/11 → 18/11)

- Developed a graphical game interface using the **MLV** library for game_screen.
- Added animations to the game screen interface to make the display more dynamic.
- Integrated a score saving system.

### Third iteration (19/11 → 25/11)

- Developed the interactive menu and tutorial interface.
- Refactored `game_field` structure: replaced static array with dynamic array for more flexibility.
- Updated code comments to follow **Doxygen** style.
