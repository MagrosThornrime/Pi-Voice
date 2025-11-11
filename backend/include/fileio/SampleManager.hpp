#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <Types.hpp>


namespace fileio {
/// @brief Loads audio samples so they can be used later as oscillators
class SampleManager {
    /// @brief A directory where the samples are stored
    std::string _samplesDirectory;

    /// @brief All samples possible to use
    std::unordered_map<std::string, std::string> _samplePaths;

    /// @brief Reads the samples' directory and gets names of all usable samples
    void _loadSamplePaths();

public:
    /// @brief Constructor
    /// @param samplesDirectory a directory where the samples are stored
    SampleManager(const std::string& samplesDirectory);

    /// @brief Get list of samples
    std::vector<std::string> getSampleNames();

    /// @brief Loads a sample from given file
    /// @param sampleName name of the sample
    //std::vector<f32> loadSample(const std::string& sampleName);

    // todo:
    // 1. setter dla ścieżki w której będziemy trzymać samples (będzie jakaś wartość domyślna though)
    // setter powinien reloadować słownik z ścieżkami do sampli
    // 2. podczas czytania ścieżek do sampli w folderze, robimy coś takiego:
    //     - z każdej ścieżki wyciągamy jakoś nazwę pliku
    //     - wrzucamy do słownika: klucz jako nazwa, ścieżka jako wartość
    //     - jeśli klucz już istnieje, próbujemy dodać do nazwy liczbę od 2 w górę
    //     - dopóki nie znajdziemy wolnej
    //     - poza tym trzeba sprawdzić czy plik da się otworzyć i czy ma zadany samplerate i liczbę kanałów 1 lub 2

};

}