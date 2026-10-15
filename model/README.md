# Modèle Edge Impulse

Le modèle n'est pas stocké dans ce dépôt (fichiers générés et volumineux).
Il s'intègre au projet sous forme de **librairie Arduino** exportée depuis Edge Impulse.

## Informations à compléter

| Élément | Valeur |
|---|---|
| Projet Edge Impulse | (lien) |
| Type | Classification d'images / Détection d'objets |
| Classes | (liste) |
| Taille du dataset | (nombre d'images) |
| Résolution d'entrée | (ex. 96x96) |
| Précision | (ex. 92 %) |

## Exporter la librairie

1. Dans Edge Impulse : **Deployment > Arduino library > Build**.
2. Télécharger le `.zip`.
3. Arduino IDE : `Croquis > Inclure une bibliothèque > Ajouter la bibliothèque .ZIP`.

## Nom de la librairie

Le sketch inclut `<SECURTI_inferencing.h>`. Le nom du header dépend du nom du
projet Edge Impulse (`<nom_du_projet>_inferencing.h`). Si ta librairie porte un
autre nom (par exemple `IMAGE_inferencing.h`), modifie la ligne `#include`
dans le fichier `.ino` pour qu'elle corresponde.
