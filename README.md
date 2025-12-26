Elaborato Mid Term — Parallel Computing
Calcolo e confronto di bigrammi e trigrammi (di parole e caratteri) tra una versione sequenziale e una parallela con OpenMP.

Obiettivo del progetto: misurare il tempo di esecuzione delle due versioni e valutarne l’accelerazione (speedup).

---

Struttura del progetto

Il repository contiene una solution Visual Studio con due progetti distinti:

ParallelMidTerm/
│
├─ ParallelProject/ ← versione parallela (OpenMP)
│ ├─ parallel_words.cpp ← main parallelo
│ ├─ utility.cpp / utility.h ← funzioni condivise
│ └─ par_times.txt ← tempi registrati in esecuzione
│
├─ SequentialProject/ ← versione sequenziale
│ ├─ sequential_bigrams.cpp ← main sequenziale
│ ├─ utility.cpp / utility.h
│ └─ seq_times.txt ← tempi registrati in esecuzione
│
├─ results/
│ └─ speedup.txt ← confronto prestazioni seq vs par
│
├─ dataset/ ← file testo NON incluso nel repo
│
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
Entrambe le versioni misurano i tempi di esecuzione e li salvano in:
SequentialProject/seq_times.txt
ParallelProject/par_times.txt


La versione parallela inoltre confronta i tempi con la sequenziale
e genera il file:
results/speedup.txt


contenente lo speedup S = T_seq / T_par.

---

Come eseguire

1. Aprire `ParallelMidTerm.sln` con Visual Studio (testato con VS2022)
2. Caricare il dataset in `./dataset/` (es: `eng_news_2024_100K-sentences.txt`)
3. Impostare uno dei due progetti come avvio:

| Progetto | Modalità | Output file tempi |
|--------|----------|-----------------|
| `SequentialProject` | sequenziale | `seq_times.txt` |
| `ParallelProject`   | OpenMP       | `par_times.txt` + `speedup.txt` |

4. Compilare → Eseguire  
5. I risultati (Top 10 n-grammi, tempi, speedup) compariranno su console + file

---

Speedup ottenuto (esempio reale)

| Categoria | Speedup |
|----------|---------|
| Bigrammi caratteri | ≈ 2.8x |
| Trigrammi parole   | ≈ 2.3x |
| Trigrammi caratteri| ≈ 1.17x |
| Bigrammi parole    | < 1x (overhead > guadagno) |

💡 Interpretazione  
- Quando il lavoro è grande e il costo del merge è basso → buon speedup  
- Per operazioni leggere come bigrammi parole → overhead dei thread incide di più

---

Dataset

Il corpus non è incluso per dimensione.  
Per riprodurre i test:

1. Scaricare da Leipzig Corpora Collection
   https://wortschatz.uni-leipzig.de/en/download/English  
   *(file usato nei test: `eng_news_2024_100K-sentences.txt`)*

2. Inserire in:  ParallelMidTerm/dataset/


3. Il programma lo leggerà automaticamente.

---

 Nota sullo sviluppo

Il progetto inizialmente nasce in una repo separata con vari test incrementali.  
Alla maturazione della versione stabile ho riorganizzato tutto in questa struttura più pulita con 2 progetti dentro la stessa solution, per consegna e confronto più chiaro.

(vecchia repo)
https://github.com/biga95/bigrams_trigrams

---

Conclusione

Implementate versioni sequenziale + parallela OpenMP
Misurati tempi e generato confronto speedup automatico
Struttura pulita e replicabile  
Repository pronto per presentazione e dimostrazione orale





