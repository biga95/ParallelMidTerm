Elaborato Mid Term – Parallel Computing

Questo progetto confronta una versione sequenziale e una versione parallela (OpenMP) per il calcolo di bigrammi e trigrammi, sia di parole che di caratteri, a partire da un file di testo.

L’obiettivo è misurare i tempi di esecuzione e calcolare lo speedup della versione parallela rispetto a quella sequenziale.

---

Struttura del progetto

Il repository contiene una solution Visual Studio con due progetti distinti:

ParallelMidTerm/
│
├─ SequentialProject/
│   ├─ sequential_bigrams.cpp
│   ├─ utility.cpp / utility.h
│   
│
├─ ParallelProject/
│   ├─ parallel_words.cpp
│   ├─ utility.cpp / utility.h
│   
│
├─ results/
│
├─ dataset/        (NON incluso nel repository)
│
├─ .gitignore
└─ ParallelMidTerm.sln



---

Descrizione del progetto

Il programma legge un corpus testuale in inglese (dataset Leipzig) e calcola:

Bigrammi di parole  
Trigrammi di parole 
Bigrammi di caratteri  
Trigrammi di caratteri 

Per ciascuna modalità vengono contati tutti i n-grammi, ordinati per frequenza e stampati i Top 10.  

Sia versione parallela che sequenziale confrontano i loro tempi e calcolano lo speedup, salvando tutti file txt nella cartella results.



---

Come eseguire

Aprire ParallelMidTerm.sln con Visual Studio 2022

Inserire il dataset nella cartella dataset/

Impostare come progetto di avvio:

SequentialProject per la versione sequenziale

ParallelProject per la versione parallela

Compilare ed eseguire

I risultati vengono mostrati su console e salvati nei file di output.

---

Dataset

Il dataset non è incluso nel repository per motivi di dimensione.

È stato utilizzato un corpus della Leipzig Corpora Collection, ad esempio:
eng_news_2024_100K-sentences.txt
eng_news_2024_1M-sentences.txt

il dataset va inserito nella cartella:
ParallelMidTerm/dataset/

---

 Nota sullo sviluppo
Nota sullo sviluppo

Durante lo sviluppo sono stati fatti vari test e riorganizzazioni.
La versione finale è stata raccolta in questa struttura con due progetti separati nella stessa solution, per rendere il confronto più chiaro e la consegna più ordinata.

(vecchia repo)
https://github.com/biga95/bigrams_trigrams

---

Conclusione

Implementate versione sequenziale e parallela con OpenMP

Misurati tempi reali di esecuzione

Calcolato automaticamente lo speedup





