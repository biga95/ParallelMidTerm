#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cctype>
#include <chrono>
#include <algorithm>
#include "utility.h"




// normalizzo una riga
std::string normalize(const std::string& s) {
    std::string out;
    out.reserve(s.size());

    for (size_t i = 0; i < s.size(); i++) {
        unsigned char c = s[i];

		// tolgo caratteri non ASCII
        if (c < 32 || c > 126) {
            out.push_back(' ');
            continue;
        }

        char lower = std::tolower(c);

        // tolgo apostrofi, però mantengo la parola unica
        if (lower == '\'')
            continue;

		// tolgo il trattino tra parole senza spezzarle
        if (lower == '-') {
            if (i > 0 && i + 1 < s.size() &&
                std::isalpha((unsigned char)s[i - 1]) &&
                std::isalpha((unsigned char)s[i + 1]))
            {
                continue; 
            }
            else {
                out.push_back(' ');
                continue;
            }
        }

        if (std::isalnum((unsigned char)lower)) {
            out.push_back(lower);
        }
        else {
            out.push_back(' ');
        }
    }

    return out;
}


// legge il file riga per riga, normalizza e splitta
std::vector<std::string> read_words(std::ifstream& file) {
    std::vector<std::string> words;
    std::string line;

    while (std::getline(file, line)) {

        // rimuovo numerazione iniziale 
        int p = 0;
        while (p < (int)line.size() && std::isdigit((unsigned char)line[p]))
            p++;

        while (p < (int)line.size() && std::isspace((unsigned char)line[p]))
            p++;

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

        if (!temp.empty())
            words.push_back(temp);
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

    // bigrammi e trigrammi di parole
    std::unordered_map<std::string, int> bigrams_words;
    std::unordered_map<std::string, int> trigrams_words;

    auto t_big_word_start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < N - 1; i++) {
        bigrams_words[words[i] + " " + words[i + 1]]++;
    }
    auto t_big_word_end = std::chrono::high_resolution_clock::now();
    double time_bigram_words = std::chrono::duration<double>(t_big_word_end - t_big_word_start).count();

	auto t_tri_word_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N - 2; i++) {
        trigrams_words[words[i] + " " + words[i + 1] + " " + words[i + 2]]++;
    }
	auto t_tri_word_end = std::chrono::high_resolution_clock::now();
	double time_trigram_words = std::chrono::duration<double>(t_tri_word_end - t_tri_word_start).count();


    // bigrammi di caratteri
    std::unordered_map<std::string, int> bigrams_chars;
    auto t_big_char_start = std::chrono::high_resolution_clock::now();

    for (const auto& w : words) {
        int L = (int)w.size();

        for (int i = 0; i < L - 1; i++) {
            std::string bg;
            bg.push_back(w[i]);
            bg.push_back(w[i + 1]);
            bigrams_chars[bg]++;
        }
    }

    auto t_big_char_end = std::chrono::high_resolution_clock::now();
    double time_bigram_chars = std::chrono::duration<double>(t_big_char_end - t_big_char_start).count();

    // trigrammi di caratteri
    std::unordered_map<std::string, int> trigrams_chars;
    auto t_tri_char_start = std::chrono::high_resolution_clock::now();

    for (const auto& w : words) {
        int L = (int)w.size();

        for (int i = 0; i < L - 2; i++) {
            std::string tg;
            tg.push_back(w[i]);
            tg.push_back(w[i + 1]);
            tg.push_back(w[i + 2]);
            trigrams_chars[tg]++;
        }
    }

    auto t_tri_char_end = std::chrono::high_resolution_clock::now();
    double time_trigram_chars = std::chrono::duration<double>(t_tri_char_end - t_tri_char_start).count();




    //prova statistiche

    //bigrammi di parole
    std::vector<std::pair<std::string, int>> vec_big;

    for (auto& p : bigrams_words) {
        vec_big.push_back(p);
    }

    // ordino 
    std::sort(vec_big.begin(), vec_big.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second; 
        });

    std::cout << "\nPrimi 10 bigrammi di parole:\n";
    for (int i = 0; i < 10 && i < (int)vec_big.size(); i++) {
        std::cout << "BIGRAM WORD: " << vec_big[i].first << " = " << vec_big[i].second << "\n";
    }

    //trigrammi di parole
    std::vector<std::pair<std::string, int>> vec_trig;
    for (auto& p : trigrams_words) {
        vec_trig.push_back(p);
    }
    // ordino 
    std::sort(vec_trig.begin(), vec_trig.end(),
        [](const std::pair<std::string, int>& a,
            const std::pair<std::string, int>& b)
        {
            return a.second > b.second;
        });

    std::cout << "\nPrimi 10 bigrammi di parole:\n";
    for (int i = 0; i < 10 && i < (int)vec_trig.size(); i++) {
        std::cout << "TRIGRAM WORD: " << vec_trig[i].first << " = " << vec_trig[i].second << "\n";
    }


    //bigrammi di caratteri
    std::vector<std::pair<std::string, int>> vec_char_big;
    for (auto& p : bigrams_chars) vec_char_big.push_back(p);

    std::sort(vec_char_big.begin(), vec_char_big.end(),
        [](const std::pair<std::string, int>& a,
            const std::pair<std::string, int>& b)
        {
            return a.second > b.second;
        });

    std::cout << "\nPrimi 10 bigrammi di caratteri:\n";
    for (int i = 0; i < 10 && i < (int)vec_char_big.size(); i++)
        std::cout << "BIGRAM CHAR: " << vec_char_big[i].first << " = " << vec_char_big[i].second << "\n";

    
    //trigrammi di caratteri:
    std::vector<std::pair<std::string, int>> vec_char_tri;
    for (auto& p : trigrams_chars) vec_char_tri.push_back(p);

    std::sort(vec_char_tri.begin(), vec_char_tri.end(),
        [](const std::pair<std::string, int>& a,
            const std::pair<std::string, int>& b)
        {
            return a.second > b.second;
        });

    std::cout << "\nPrimi 10 trigrammi di caratteri:\n";
    for (int i = 0; i < 10 && i < (int)vec_char_tri.size(); i++)
        std::cout << "TRIGRAM CHAR: " << vec_char_tri[i].first << " = " << vec_char_tri[i].second << "\n";


    std::cout << "\nTEMPI:\n";
    std::cout << "Bigrammi parole:   " << time_bigram_words << " s\n";
    std::cout << "Trigrammi parole:  " << time_trigram_words << " s\n";
    std::cout << "Bigrammi char:     " << time_bigram_chars << " s\n";
    std::cout << "Trigrammi char:    " << time_trigram_chars << " s\n";




    std::ofstream out("seq_times.txt");
    if (out) {
        out << "big_words " << time_bigram_words << "\n";
        out << "tri_words " << time_trigram_words << "\n";
        out << "big_chars " << time_bigram_chars << "\n";
        out << "tri_chars " << time_trigram_chars << "\n";
    }
   
    show_speedup("../results/seq_times.txt", "../results/par_times.txt");

    SpeedupData sp = read_speedup("../results/seq_times.txt", "../results/par_times.txt");
    write_speedup_to_file(sp, "../results/speedup.txt");
    std::cout << "\nSpeedup calcolato correttamente!\n";

}

