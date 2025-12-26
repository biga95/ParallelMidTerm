#pragma once

#include <string>
#include <unordered_map>

// legge un file di tempi (label valore) e li mette in una mappa
bool load_times(const std::string& filename,
    std::unordered_map<std::string, double>& out);

// stampa tempi e calcola speedup se trova entrambi i file
void show_speedup(const std::string& seq_file,
    const std::string& par_file);

// struct per tenere gli speedup in modo ordinato
struct SpeedupData {
    double big_chars;
    double big_words;
    double tri_words;
    double tri_chars;
};

// legge i due file di tempi e costruisce la struct SpeedupData
SpeedupData read_speedup(const std::string& seq_file,
    const std::string& par_file);

// scrive gli speedup su un file di testo (es. ../results/speedup.txt)
void write_speedup_to_file(const SpeedupData& s,
    const std::string& filename);
