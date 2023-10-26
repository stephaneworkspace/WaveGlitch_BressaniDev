# WaveGlitch par bressani.dev

Bienvenue dans WaveGlitch, une application Mac et Linux développée en C++ avec l'ui en JUCE.

![WaveGlitch on MacOS](./Assets/screen_macos.png)
![WaveGlitch on Linux Ubuntu/Debian](./Assets/screen_linux.png)

## À propos
WaveGlitch est un outil conçu spécialement pour les producteurs musicaux. Il facilite les conversions d'échantillons et les organise dans un répertoire, prêt à être intégré dans votre station audionumérique favorite, telle qu'Ableton Live, Cubase, et bien d'autres.

## Caractéristiques
- **Création d'échantillons audio** : Transformez vos audios en échantillons subdivisés (1/2, 1/4, 1/8, 1/16, 1/32, 1/64, 1/128) pour des effets glitchs.
- **Lecture des échantillons audio**: Écoutez les échantillons depuis la même fenêtre une fois la conversion effectuée.

## Fonctionnement
Avec **WaveGlitch**, créer des échantillons glitché n'a jamais été aussi simple. 

1. **Sélectionnez votre répertoire d'export** :  
   Choisissez le dossier principal où vous souhaitez exporter vos échantillons. WaveGlitch crée automatiquement une structure pour vos exports : `(RootFolder)/bpm/tone/year/song/`.

2. **Préparez votre fichier** :  
   Sélectionnez le fichier sonore à traiter et renseignez les informations requises. Une fois cette étape complétée, le bouton "Processing" devient accessible.

3. **Traitement** :  
   Cliquez sur "Processing". Une fenêtre apparaîtra pour confirmer la création de votre échantillon.

4. **Écoute et utilisation** :  
   Vous pouvez soit écouter immédiatement les glitches (fonctionnalité non disponible sur Linux), soit fermer le programme. Retrouvez ensuite vos échantillons nouvellement créés dans votre logiciel d'édition musicale de prédilection.

## Licence
WaveGlitch est publié sous la licence open source GNU General Public License v3.0. Vous êtes libre de modifier, distribuer et utiliser le logiciel en accord avec les termes de cette licence.

Pour plus de détails, veuillez consulter le fichier `LICENSE` inclus dans le dépôt.

L'image de fond a été utilisé de GarryKillian on Freepik. Une partie du design du logo a été crée à partir d'une modification de l'image de onfocus sur Freepik.

## Compilation

### MacOs / ~~Windows (ne fonctionne pas)~~ / Linux
Nécessite cmake pour compiler.

```
cd build
cmake ..
```

## Bugs

La lecture des échantillons après le traitement ne fonctionne pas dans mon test sur Ubuntu 22.04, mais la création des échantillons fonctionne.
L'application compile sous windows, mais le programme plante quand on ajouter un fichier wav, donc le programme est inutilisable sous Microsoft Windows.

## Contact
Pour toute question ou suggestion, n'hésitez pas à me contacter sur [bressani.dev](http://bressani.dev).
