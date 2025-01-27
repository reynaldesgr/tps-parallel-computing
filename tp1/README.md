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

- Boucle sur les lignes : car les lignes sont indépendantes entre elles
  Chaque ligne de A est utilisée uniquement pour une ligne spécifique de C, ce qui évite tout conflit d'accès aux données.
  
Exemple avec n = 3 : 

  $\begin{aligned}
  C[0][0] &= A[0][0] \times B[0][0] + A[0][1] \times B[1][0] + A[0][2] \times B[2][0] \\
  C[0][1] &= A[0][0] \times B[0][1] + A[0][1] \times B[1][1] + A[0][2] \times B[2][1] \\
  C[0][2] &= A[0][0] \times B[0][2] + A[0][1] \times B[1][2] + A[0][2] \times B[2][2]
  \end{aligned}$
- Boucle sur les colonnes si i est déjà parallélisée