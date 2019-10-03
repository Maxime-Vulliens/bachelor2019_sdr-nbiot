# bachelor2019_sdr-nbiot

## ·      Structure du répertoire

Travail de Bachelor 2019 réalisé par Maxime Vulliens



-   **Aduino_sketch** : ce répertoire contient les sketchs utilisés pour l’environnement de développement « Arduino IDE ». Ces sketchs ont été exécuté sur la carte de développement Arduino MK NRB 1500.

  - **NBRATConfig.ino :** Ce document est le sketch qui permet de configurer le mode opérationnel de la carte de développement Arduino. Ce sketch est mis à disposition comme exemple par la librairie <MKNRB>.
  - **NB_cmdHandler.ino :** Ce document est le sketch réalisé pour envoyer des commandes AT au modem à travers la console série. Ce sketch permet d’envoyer les commandes AT unitairement et de récupérer le résultat qui y est lié. Ce sketch a permis de tester différentes commandes pour analyser l’état du réseau.

- **Fipy_firmware :** ce répertoire contient le firmware python développé pour la carte FiPy. Ce firmware donne la possibilité d’exécuter deux séries de commandes AT : 

  - La première pour tenter de se connecter à un réseau
  - La seconde pour récupérer un maximum d’informations de statuts

  Ce firmware donne également la possibilité d’exécuter les commandes AT unitairement. Ce firmware est chargé sur la carte avec le logiciel « Atom » et le plugin « Pymakr » installé.

- **Gnuradio_bloc :** ce répertoire contient le module développé pour réaliser le système de détection d’interférence sur GNU Radio. Ce module est composé de plusieurs blocs :

  - **Npss_sync :** ce bloc permet de réaliser la synchronisation sur le signal NPSS.
  - **Npss_advanced_sync :** ce bloc n’est pas fonctionnel pour le moment. Son but est de réaliser la synchronisation sur le signal NPSS d’une manière optimisée.
  - **Remove_cp :** ce bloc permet l’extraction des Cycles Prefix (CP).
  - **Custom_fft :** ce bloc permet de réaliser un FFT.
  - **Nsss_decode :** ce bloc permet de décoder l’identifiant de l’antenne et le numéro de frame.
  - **Global.h :** ce fichier contient les définitions communes aux différents blocs.

- **Gunradio_grc :** ce répertoire contient les diagrammes réalisés pour GNU Radio. Les différents diagrammes réalisés sont :

  -  **Synchronisation :** Ce diagramme a été utilisé pour développer le système. Il permet de réaliser la synchronisation des signaux NPSS et NSSS.
  - **Barrage Jamming :** Ce diagramme permet d’ajouter une interférence de type BJ au flux d’entrée. Ce répertoire contient également le script python pour automatiser l’exécution de ce diagramme.
  - **Tone Jamming :** Ce diagramme permet d’ajouter une interférence de type STJ ou MTJ au flux d’entrée. Ce répertoire contient également le script python pour automatiser l’exécution de ce diagramme.
  -  **Sync_test :** Ce diagramme a été utilisé pour réaliser des tests de synchronisation avec un flux d’entrée décimé, ou avec un filtre de type « Bandpass » au lieu de « Lowpas ».

- **Records_used** : ce répertoire contient les captures réalisées pour différent test. Les enregistrements sont de très courte durée (1-2 secondes) pour répondre à la limite de taille imposée par github. Les enregistrements sont réalisés à 10MSps, c'est pourquoi ils sont relativement lourd. Les captures sont :
  - cell_id_test : contient un enregistrement réalisé lorsque l'identifiant physique de l'antenne est de 133
  - standard : contient un enregistrement réalisé lorsque l'identifiant physique de l'antenne est de 1

