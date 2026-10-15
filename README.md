# Reconnaissance faciale et détection d'objets embarquées (ESP32)

Système d'intelligence artificielle embarquée (TinyML) sur **ESP32-S3** : une caméra
capture des images, un modèle **Edge Impulse** s'exécute directement sur le
microcontrôleur, et le résultat (classe + niveau de confiance) s'affiche sur un
écran **LCD I2C 16x2**.

## Fonctionnalités

- Capture d'images en temps réel (caméra OV2640, QVGA 320x240)
- Inférence locale avec Edge Impulse (sans cloud, sans Wi-Fi)
- Affichage du label et de la confiance sur LCD 16x2
- Sortie des résultats sur le port série (115200 bauds)
- Gestion des deux modes : classification d'images et détection d'objets

## Matériel

| Composant | Détail |
|---|---|
| Carte | ESP32-S3 avec PSRAM OPI (brochage type ESP32-S3-EYE) |
| Caméra | OV2640 |
| Écran | LCD 16x2 I2C (adresse `0x27`) |

Voir [`hardware/bill_of_materials.md`](hardware/bill_of_materials.md) et
[`docs/wiring.md`](docs/wiring.md) pour le détail.

## Structure du dépôt

```
firmware/esp32_edge_ai_detection/   Code Arduino + configuration de la carte
model/                              Informations sur le modèle Edge Impulse
docs/                               Câblage et images
hardware/                           Liste du matériel
```

## Installation

1. Installer **Arduino IDE** et le package **esp32 by Espressif**.
2. Installer la librairie **LiquidCrystal I2C** (Gestionnaire de bibliothèques).
3. Importer la librairie Edge Impulse exportée :
   `Croquis > Inclure une bibliothèque > Ajouter la bibliothèque .ZIP`
   (voir [`model/README.md`](model/README.md)).
4. Sélectionner la carte **ESP32S3 Dev Module** avec **PSRAM : OPI PSRAM**
   (la configuration complète est dans `sketch.json`).
5. Ouvrir `firmware/esp32_edge_ai_detection/`, compiler et téléverser.
6. Ouvrir le moniteur série à **115200 bauds**.

## Fonctionnement

1. Initialisation du LCD et de la caméra.
2. Boucle principale : capture JPEG, conversion RGB888, redimensionnement à la taille d'entrée du modèle.
3. Exécution de `run_classifier()` (Edge Impulse).
4. Affichage du label et de la confiance (`%`) sur le LCD et sur le port série.

## Résultats

Ajouter ici des photos du montage et une vidéo de démonstration (`docs/images/`).

## Auteur

[Votre Nom] – [LinkedIn] – [Email]

## Licence

MIT, voir [`LICENSE`](LICENSE).
