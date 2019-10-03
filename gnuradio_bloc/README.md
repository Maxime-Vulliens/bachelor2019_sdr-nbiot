## Manuel d'installation



- Logiciels requis :
  - GNU Radio : gr-nbiot est un module pour GNU Radio.
  - gr-lte : c'est une librairie pour GNU Radio disponible sur github à l'adresse suivante : ... cette librairie est nécessaire à l'exécution des diagrammes de ce "repository".



### Installation

Pour installer le module, exécuter les commandes suivantes dans un terminal:

```bash
# aller dans le repértoire gr-nbiot
cd gr-nbiot/

# Créer un répertoire pour le buil
mkdir build
cd build/

# configurer les fichiers pour le build
cmake ../

# compile est installe le module
sudo make install
sudo ldconfig

```



Une fois ces commandes exécutées, les blocs seront disponibles depuis l'interface GNU Radio.