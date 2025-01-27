# TP1
## OpenMP
OpenMP est une bibliothèque qui facilite l’écriture des programmes parallèles multi-threading pour
des architectures parallèles à mémoire partagée. 

Elle fournit des fonctions de gestion de threads (ex.
`omp_get_num_threads`) et des directives de compilation (`#pragma`) qui permet la création et la
gestion des régions parallèles à partir d’un programme séquentiel.

L’exécution d’un programme OpenMP entraîne l’exécution simultanée de plusieurs threads. Les
threads d’un même programme peuvent avoir des variables partagées (i.e. une seule copie mémoire
par variable) ou des variables privées (le même nom, mais une copie mémoire par variable et par
thread). La gestion des variables partagées / privées est primordiale pour la justesse des résultats du
programme et de sa performance. La performance du programme est aussi grandement affectée par
la gestion de régions parallèles.

L’utilisation de l’option `-fopenmp` permet à gcc de compiler vos
programmes OpenMP.

## Exécution du script `hello_omp.cpp`

```
Before PARALLEL REGION TID 0:
There are 1 threads on CPU 0x79c814313ac0 of rsegerie-Inspiron-15-3520
In the PARALLEL REGION TID 0:
There are 4 threads in process
Hello World from TID 0 / 4 on CPU 8 of rsegerie-Inspiron-15-3520!
Hello World from TID 1 / 4 on CPU 9 of rsegerie-Inspiron-15-3520!
Hello World from TID 3 / 4 on CPU 7 of rsegerie-Inspiron-15-3520!
Hello World from TID 2 / 4 on CPU 6 of rsegerie-Inspiron-15-3520!
After PARALLEL REGION TID -1:
There are 1 threads
```

1. Que se passe-t-il si vous remplacez `firstprivate` par `private` ?
    
La variable `tid` n'est pas préservée.

1ère exécution :
```
Before PARALLEL REGION TID 0:
 There are 1 threads on CPU 0x77a409713ac0 of rsegerie-Inspiron-15-3520
In the PARALLEL REGION TID 0:
 There are 4 threads in process
Hello World from TID 0 / 4 on CPU 5 of rsegerie-Inspiron-15-3520!
In the PARALLEL REGION TID 2:
 There are 4 threads in process
Hello World from TID 0 / 4 on CPU 7 of rsegerie-Inspiron-15-3520!
In the PARALLEL REGION TID 3:
 There are 4 threads in process
Hello World from TID 0 / 4 on CPU 10 of rsegerie-Inspiron-15-3520!
In the PARALLEL REGION TID 1:
 There are 4 threads in process
Hello World from TID 0 / 4 on CPU 8 of rsegerie-Inspiron-15-3520!
After PARALLEL REGION TID -1:
 There are 1 threads
 ```

2ème exécution : 
```
Before PARALLEL REGION TID 0:
 There are 1 threads on CPU 0x760b56313ac0 of rsegerie-Inspiron-15-3520
In the PARALLEL REGION TID 0:
 There are 4 threads in process
Hello World from TID 0 / 4 on CPU 6 of rsegerie-Inspiron-15-3520!
In the PARALLEL REGION TID 1:
 There are 4 threads in process
Hello World from TID 0 / 4 on CPU 7 of rsegerie-Inspiron-15-3520!
In the PARALLEL REGION TID 2:
 There are 4 threads in process
Hello World from TID 0 / 4 on CPU 1 of rsegerie-Inspiron-15-3520!
In the PARALLEL REGION TID 3:
 There are 4 threads in process
Hello World from TID 0 / 4 on CPU 3 of rsegerie-Inspiron-15-3520!
After PARALLEL REGION TID -1:
 There are 1 threads
```

2. Que constatez-vous si vous supprimer la clause `private` ?
```
Before PARALLEL REGION TID 0:
 There are 1 threads on CPU 0x7590d2713ac0 of rsegerie-Inspiron-15-3520
In the PARALLEL REGION TID 0:
 There are 4 threads in process
Hello World from TID 3 / 4 on CPU 9 of rsegerie-Inspiron-15-3520!
Hello World from TID 3 / 4 on CPU 11 of rsegerie-Inspiron-15-3520!
Hello World from TID 2 / 4 on CPU 4 of rsegerie-Inspiron-15-3520!
Hello World from TID 3 / 4 on CPU 5 of rsegerie-Inspiron-15-3520!
After PARALLEL REGION TID 3:
 There are 1 threads
```
La variable `tid` devient alors partagée et tous les threads risquent d'écraser la valeur.

## Calcul matriciel 
Soient A, B deux matrices carrées d’ordre n, on demande d’écrire un programme qui calcule
C=AxB

1. **Implémentation séquentielle**

Lecture de la taille de la matrice depuis la ligne de commande et produit matriciel classique avec 3 boucles.

2. **Parties parallélisables**

- Boucle sur les lignes : car les lignes sont indépendantes entre elles.

  Chaque ligne de A est utilisée uniquement pour une ligne spécifique de C, ce qui évite tout conflit d'accès aux données.

- Boucle sur les colonnes si i est déjà parallélisée

3. **Variables**

| Variable        | Portée   | Justification                                                                                    |
|----------------|----------|--------------------------------------------------------------------------------------------------|
| `A`             | `shared`  | Utilisée en **lecture seule** par tous les threads. Pas de risque de conflits.                   |
| `B`             | `shared`  | Utilisée en **lecture seule** par tous les threads. Pas de risque de conflits.                   |
| `C`             | `shared`  | Chaque élément est modifié indépendamment. Gestion des conflits nécessaire (ex. `critical`).     |
| `i, j, k`       | `private` | Variables d'indexation des boucles, propres à chaque thread pour éviter les conflits d'écriture. |
| `thread_num`    | `private` | Identifiant du thread utilisé uniquement pour l'affichage, propre à chaque thread.               |
| `schedule_type` | `shared`  | Constante en entrée du programme, utilisée par tous les threads sans modification.               |


4. **Résultats sequential. VS parallel.**

Les résultats obtenus sont cohérents :

```
./tp1 5 :

Matrice A :
  0.10   0.40   2.30   5.60   1.30 
  0.50   5.60   1.60   2.10   8.10 
  4.60   9.90   5.80   6.80   5.70 
  9.20   5.40   2.40   2.50   5.90 
  6.10   5.70   2.80   7.60   3.10 

Matrice B :
  1.50   4.30   6.80   9.40   4.20 
  5.00   9.50   4.60   7.40   0.30 
  1.10   3.10   1.10   8.00   5.30 
  9.30   2.60   0.40   5.10   9.40 
  1.30   4.30   0.00   3.70   6.80 

Temps d'exécution séquentiel: 3.048e-06 secondes
Temps d'exécution parallèle: 0.0265903 secondes
* Accélération obtenue: 0.000114628x 

Matrice C (résultat séquentielle) :
 58.45  31.51   7.29  55.67  74.21 
 60.57 100.60  31.76  99.62  87.08 
133.43 174.00  85.92 218.67 155.71 
 74.36 130.17  91.04 180.22 116.60 
115.44 122.15  73.82 172.15 134.69 

Matrice C (résultat parallèle) :
 58.45  31.51   7.29  55.67  74.21 
 60.57 100.60  31.76  99.62  87.08 
133.43 174.00  85.92 218.67 155.71 
 74.36 130.17  91.04 180.22 116.60 
115.44 122.15  73.82 172.15 134.69 

Les résultats sont corrects.

```

5. **Répartition des tâches & Static VS Dynamic**

Les clauses ``schedule(static)`` et ``schedule(dynamic)`` contrôlent la manière dont les itérations d'une boucle parallèle sont réparties entre les threads.

Avec l'**ordonnancement statique**, les itérations sont réparties à l'avance entre les threads, de manière équitable et prévisible.

Avec l'**ordonnancement dynamique**, les itérations sont assignées aux threads à la demande, c'est-à-dire qu'un thread termine une itération et prend la suivante disponible.

6. **Temps d'exécution sur des matrices 100x100**
```
Temps d'exécution séquentiel: 0.0086048 secondes
Temps d'exécution parallèle: 0.00238909 secondes
* Accélération obtenue: 3.60171x 
```