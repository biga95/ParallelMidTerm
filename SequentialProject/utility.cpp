#include "utility.h"
#include <fstream>
#include <iostream>

// legge un file di tempi e li mette in una mappa
bool load_times(const std::string& filename,
    std::unordered_map<std::string, double>& out)
{
    std::ifstream f(filename);
    if (!f) {
        return false;
    }

    std::string label;
    double val;

    while (f >> label >> val) {
        out[label] = val;
    }

    return true;
}

// stampa tempi e calcola speedup se trova entrambi i file
void show_speedup(const std::string& seq_file,
    const std::string& par_file)
{
    std::unordered_map<std::string, double> seq;
    std::unordered_map<std::string, double> par;

    bool ok_seq = load_times(seq_file, seq);
    bool ok_par = load_times(par_file, par);

    if (!ok_seq && !ok_par) {
        std::cout << "Nessun file di tempi trovato.\n";
        return;
    }

    std::cout << "\nTEMPI SEQUENZIALI\n";
    if (ok_seq) {
        for (auto& p : seq) {
            std::cout << p.first << " = " << p.second << " s\n";
        }
    }
    else {
        std::cout << "(manca file " << seq_file << ")\n";
    }

    std::cout << "\nTEMPI PARALLELI\n";
    if (ok_par) {
        for (auto& p : par) {
            std::cout << p.first << " = " << p.second << " s\n";
        }
    }
    else {
        std::cout << "(manca file " << par_file << ")\n";
    }

    if (ok_seq && ok_par) {
        std::cout << "\nSPEEDUP\n";

        for (auto& p : seq) {
            const std::string& key = p.first;

            if (par.count(key)) {
                double s = seq[key];
                double pval = par[key];

                if (pval > 0.0) {
                    std::cout << key << " : " << s / pval << "\n";
                }
            }
        }
    }

    std::cout << "\n";
}

// legge i tempi dai due file e costruisce la struct SpeedupData
SpeedupData read_speedup(const std::string& seq_file,
    const std::string& par_file)
{
    std::unordered_map<std::string, double> seq;
    std::unordered_map<std::string, double> par;

    SpeedupData res{};
    if (!load_times(seq_file, seq)) return res;
    if (!load_times(par_file, par)) return res;

    // le label devono combaciare con quelle che scrivi nei txt
    // big_chars, big_words, tri_words, tri_chars
    if (par["big_chars"] > 0)  res.big_chars = seq["big_chars"] / par["big_chars"];
    if (par["big_words"] > 0)  res.big_words = seq["big_words"] / par["big_words"];
    if (par["tri_words"] > 0)  res.tri_words = seq["tri_words"] / par["tri_words"];
    if (par["tri_chars"] > 0)  res.tri_chars = seq["tri_chars"] / par["tri_chars"];

    return res;
}

// scrive gli speedup su file (es. ../results/speedup.txt)
void write_speedup_to_file(const SpeedupData& s,
    const std::string& filename)
{
    std::ofstream out(filename);
    if (!out) {
        std::cout << "Errore apertura " << filename << "\n";
        return;
    }

    out << "SPEEDUP\n";
    out << "big_chars : " << s.big_chars << "\n";
    out << "big_words : " << s.big_words << "\n";
    out << "tri_words : " << s.tri_words << "\n";
    out << "tri_chars : " << s.tri_chars << "\n";
}
