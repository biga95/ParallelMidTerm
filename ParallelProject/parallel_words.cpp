#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <cctype>
#include <string>
#include <omp.h>
#include <algorithm>
#include "utility.h"

// ---- normalize ----------------------------------------------------
std::string normalize(const std::string& s) {
    std::string out;
    out.reserve(s.size());

    for (size_t i = 0; i < s.size(); i++) {
        unsigned char c = s[i];

        if (c < 32 || c > 126) {
            out.push_back(' ');
            continue;
        }

        char lower = std::tolower(c);

        // tolgo apostrofi
        if (lower == '\'') {
            continue;
        }

        // trattino interno (t-shirt)
        if (lower == '-') {
            if (i > 0 && i + 1 < s.size()
                && std::isalpha((unsigned char)s[i - 1])
                && std::isalpha((unsigned char)s[i + 1])) {
                continue;
            }
            out.push_back(' ');
            continue;
        }

        if (std::isalnum(lower)) {
            out.push_back(lower);
        }
        else {
            out.push_back(' ');
        }
    }

    return out;
}

// ---- read_words ----------------------------------------------------
std::vector<std::string> read_words(std::ifstream& file) {
    std::vector<std::string> words;
    std::string line;

    while (std::getline(file, line)) {

        int p = 0;
        while (p < (int)line.size() && std::isdigit((unsigned char)line[p])) p++;
        while (p < (int)line.size() && std::isspace((unsigned char)line[p])) p++;

        line = line.substr(p);
        std::string clean = normalize(line);

        std::string temp;
        for (char c : clean) {
            if (c == ' ') {
                if (!temp.empty()) {
                    words.push_back(temp);
                    temp.clear();
                }
            }
            else {
                temp.push_back(c);
            }
        }
        if (!temp.empty()) {
            words.push_back(temp);
        }
    }

    return words;
}


int main() {

    std::ifstream file("../dataset/eng_news_2024_100K-sentences.txt");
    if (!file) {
        std::cout << "Errore apertura file\n";
        return 1;
    }

    auto words = read_words(file);
    int N = (int)words.size();
    std::cout << "Parole lette: " << N << "\n";

    int num_threads = omp_get_max_threads();

    //BIGRAMMI PAROLE

    std::unordered_map<std::string, int> bigrams;
    std::vector<std::unordered_map<std::string, int>> local_big(num_threads);

    auto t_big_start = std::chrono::high_resolution_clock::now();

#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        auto& mp = local_big[tid];

#pragma omp for
        for (int i = 0; i < N - 1; i++) {
            std::string bg = words[i] + " " + words[i + 1];
            mp[bg]++;
        }
    }

    for (auto& mp : local_big) {
        for (auto& p : mp) {
            bigrams[p.first] += p.second;
        }
    }

    auto t_big_end = std::chrono::high_resolution_clock::now();
    double time_big = std::chrono::duration<double>(t_big_end - t_big_start).count();

    std::cout << "Tempo bigrammi parole (parallel): " << time_big << " s\n";
    std::cout << "Bigrammi distinti: " << bigrams.size() << "\n";

   
    // stampa 10 bigrammi parole
    std::vector<std::pair<std::string, int>> v1;
    for (auto& p : bigrams) {
        v1.push_back(p);
    }

    std::sort(v1.begin(), v1.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

    std::cout << "Bigrammi parole (top 10):\n";
    for (int i = 0; i < 10; i++) {
        std::cout << v1[i].first << " = " << v1[i].second << "\n";
    }

    //TRIGRAMMI PAROLE

    std::unordered_map<std::string, int> trigrams;
    std::vector<std::unordered_map<std::string, int>> local_tri(num_threads);

    auto t_tri_start = std::chrono::high_resolution_clock::now();

#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        auto& mp = local_tri[tid];

#pragma omp for
        for (int i = 0; i < N - 2; i++) {
            std::string tg = words[i] + " " + words[i + 1] + " " + words[i + 2];
            mp[tg]++;
        }
    }

    for (auto& mp : local_tri) {
        for (auto& p : mp) {
            trigrams[p.first] += p.second;
        }
    }

    auto t_tri_end = std::chrono::high_resolution_clock::now();
    double time_tri = std::chrono::duration<double>(t_tri_end - t_tri_start).count();

    std::cout << "\nTempo trigrammi parole (parallel): " << time_tri << " s\n";
    std::cout << "Trigrammi distinti: " << trigrams.size() << "\n";

    
    // stampa 10 trigrammi parole
    std::vector<std::pair<std::string, int>> v2;
    for (auto& p : trigrams) {
        v2.push_back(p);
    }

    std::sort(v2.begin(), v2.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

    std::cout << "Trigrammi parole (top 10):\n";
    for (int i = 0; i < 10; i++) {
        std::cout << v2[i].first << " = " << v2[i].second << "\n";
    }
    

    //BIGRAMMI CARATTERI

    std::unordered_map<std::string, int> big_char;
    std::vector<std::unordered_map<std::string, int>> local_big_char(num_threads);

    auto t_bigc_start = std::chrono::high_resolution_clock::now();

#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        auto& mp = local_big_char[tid];

#pragma omp for
        for (int i = 0; i < N; i++) {
            const std::string& w = words[i];
            int L = (int)w.size();
            for (int k = 0; k < L - 1; k++) {
                std::string bg;
                bg += w[k];
                bg += w[k + 1];
                mp[bg]++;
            }
        }
    }

    for (auto& mp : local_big_char) {
        for (auto& p : mp) {
            big_char[p.first] += p.second;
        }
    }

    auto t_bigc_end = std::chrono::high_resolution_clock::now();
    double time_big_char = std::chrono::duration<double>(t_bigc_end - t_bigc_start).count();

    std::cout << "\nTempo bigrammi caratteri (parallel): " << time_big_char << " s\n";
    std::cout << "Bigrammi di caratteri distinti: " << big_char.size() << "\n";

    // stampa 10 bigrammi caratteri
    std::vector<std::pair<std::string, int>> v3;
    for (auto& p : big_char) {
        v3.push_back(p);
    }

    std::sort(v3.begin(), v3.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

    std::cout << "Bigrammi caratteri (top 10):\n";
    for (int i = 0; i < 10; i++) {
        std::cout << v3[i].first << " = " << v3[i].second << "\n";
    }

    //TRIGRAMMI CARATTERI

    std::unordered_map<std::string, int> tri_char;
    std::vector<std::unordered_map<std::string, int>> local_tri_char(num_threads);

    auto t_tric_start = std::chrono::high_resolution_clock::now();

#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        auto& mp = local_tri_char[tid];

#pragma omp for
        for (int i = 0; i < N; i++) {
            const std::string& w = words[i];
            int L = (int)w.size();
            for (int k = 0; k < L - 2; k++) {
                std::string tg;
                tg += w[k];
                tg += w[k + 1];
                tg += w[k + 2];
                mp[tg]++;
            }
        }
    }

    for (auto& mp : local_tri_char) {
        for (auto& p : mp) {
            tri_char[p.first] += p.second;
        }
    }

    auto t_tric_end = std::chrono::high_resolution_clock::now();
    double time_tri_char = std::chrono::duration<double>(t_tric_end - t_tric_start).count();

    std::cout << "\nTempo trigrammi caratteri (parallel): " << time_tri_char << " s\n";
    std::cout << "Trigrammi di caratteri distinti: " << tri_char.size() << "\n";
    

    // stampa 10 trigrammi caratteri
    std::vector<std::pair<std::string, int>> v4;
    for (auto& p : tri_char) {
        v4.push_back(p);
    }

    std::sort(v4.begin(), v4.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

    std::cout << "Trigrammi caratteri (top 10):\n";
    for (int i = 0; i < 10; i++) {
        std::cout << v4[i].first << " = " << v4[i].second << "\n";
    }

    std::cout << "\nThread usati: " << num_threads << "\n";

	// Salvo i tempi su file
    std::ofstream out("par_times.txt");
    if (out) {
        out << "big_words "<< time_big << "\n";
        out << "tri_words "<< time_tri << "\n";
        out << "big_chars " << time_big_char << "\n";
        out << "tri_chars " << time_tri_char << "\n";
    }

    show_speedup("../results/seq_times.txt", "../results/par_times.txt");

    SpeedupData sp = read_speedup("../results/seq_times.txt", "../results/par_times.txt");
    write_speedup_to_file(sp, "../results/speedup.txt");
    std::cout << "\nSpeedup calcolato!\n";


}


