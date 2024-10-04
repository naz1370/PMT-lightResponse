#include <TH1D.h>
#include <TFile.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath> // Include the cmath library for mathematical operations

int readInInten() {
    // List of new file names
    std::string fileNames[] = {"F23-893.txt", "C23-1170.txt", "B24-1145.txt"};
    const int maxFileNumber = 3;  // We now have three files

    // Number of bins and range
    const double xMin = 175.65; // Updated to minimum wavelength found in the files
    const double xMax = 356.46; // Updated to maximum wavelength found in the files
    double binSize = 0.57;
    double NBinsCalc = (xMax - xMin) / binSize;
    const int nBins = NBinsCalc; // Number of bins

    // Loop over file names
    std::cout << "Looping over files" << std::endl;
    for (int fileNumber = 0; fileNumber < maxFileNumber; ++fileNumber) {
        std::string fileName = fileNames[fileNumber];
        std::cout << "Opening file " << fileName << std::endl;

        // Open the file
        std::ifstream file(fileName);
        if (!file.is_open()) {
            std::cerr << "Could not open file: " << fileName << std::endl;
            continue;
        }

        // Skip the first line of the header
        std::string discard;
        std::getline(file, discard);

        // Create histograms for each file using the file name as part of the histogram name
        TH1D* histogram1 = new TH1D(("abs_irrad_" + fileName).c_str(),
                                    ("Absolute Irradiance " + fileName).c_str(),
                                    nBins, xMin, xMax);
        
        TH1D* histogram2 = new TH1D(("PhotonCount_" + fileName).c_str(),
                                    ("Photon Count " + fileName).c_str(),
                                    nBins, xMin, xMax);
        
        TH1D* histogram3 = new TH1D(("scopeCount_" + fileName).c_str(),
                                    ("Sample Count " + fileName).c_str(),
                                    nBins, xMin, xMax);

        // Variables to hold the data
        double wavelength, sampleCount, darkCount, referenceCount, absoluteIrradiance, photonCount;
        std::string line;
        
        // Read and store data
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            if (iss >> wavelength >> sampleCount >> darkCount >> referenceCount >> absoluteIrradiance >> photonCount) {
                
                // Fill histograms
                int binX1 = histogram1->GetXaxis()->FindBin(wavelength);
                histogram1->Fill(wavelength, absoluteIrradiance);
                histogram1->SetBinError(binX1, 0.1); // Assuming constant error
                
                int binX2 = histogram2->GetXaxis()->FindBin(wavelength);
                histogram2->Fill(wavelength, photonCount);
                histogram2->SetBinError(binX2, 0.1); // Assuming constant error
                
                int binX3 = histogram3->GetXaxis()->FindBin(wavelength);
                histogram3->Fill(wavelength, sampleCount);
                histogram3->SetBinError(binX3, 0.1); // Assuming constant error
            }
        }

        // Close the file
        file.close();

        // Save the histograms to a ROOT file
        TFile output("output.root", "UPDATE");
        if (!output.IsZombie()) {
            histogram1->Write();
            histogram2->Write();
            histogram3->Write();
            output.Close();
            delete histogram1;
            delete histogram2;
            delete histogram3;
        } else {
            std::cerr << "Could not open output file." << std::endl;
        }
    }
    
    std::cout << "Processing complete." << std::endl;
    return 0;
}

