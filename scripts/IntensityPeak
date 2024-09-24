#include <TFile.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>  // For file output

int FindMaxAbsIrrad() {
    // Open the ROOT file
    TFile *file = TFile::Open("output.root");
    if (!file || file->IsZombie()) {
        std::cerr << "Error: Cannot open file 'output.root'" << std::endl;
        return -1;
    }

    // Open a text file to save the results in a row and column format
    std::ofstream outFile("results.txt");
    if (!outFile) {
        std::cerr << "Error: Cannot open file 'results.txt' for writing" << std::endl;
        return -1;
    }

    // Optional: Write the header
    outFile << "HistogramIndex\tMaxIrradiance\tMaxWavelength\n";

    // Variables for graph
    std::vector<double> maxValues;
    std::vector<double> maxWavelengths;

    // Loop over histograms with prefix "abs_irrad_"
    int histogramCount = 0;
    while (true) {
        // Construct the histogram name
        std::string histName = "abs_irrad_" + std::to_string(histogramCount);

        // Get the histogram from the file
        TH1D *histogram = dynamic_cast<TH1D*>(file->Get(histName.c_str()));
        if (!histogram) {
            // If histogram not found, we assume we've reached the end of the available histograms
            break;
        }

        // Find the bin with the maximum value
        int maxBin = histogram->GetMaximumBin();
        double maxValue = histogram->GetBinContent(maxBin);
        double maxWavelength = histogram->GetXaxis()->GetBinCenter(maxBin);

        // Write the result to the text file
        outFile << histogramCount << "\t" << maxValue << "\t" << maxWavelength << "\n";

        // Store the data for the plot
        maxValues.push_back(maxValue);
        maxWavelengths.push_back(maxWavelength);

        // Increment the histogram counter
        histogramCount++;
    }

    // Close the text file
    outFile.close();

    // Create a canvas for plotting
    TCanvas *canvas = new TCanvas("canvas", "Max Irradiance vs Wavelength", 800, 600);

    // Create a graph to plot maxValue vs maxWavelength
    TGraph *graph = new TGraph(histogramCount, &maxWavelengths[0], &maxValues[0]);
    graph->SetTitle("Intensity vs Wavelength; Wavelength (nm); Intensity(uWatt/cm^2/nm)");
    
    // Set the marker style and color
    graph->SetMarkerStyle(20);  // Marker style 20 is a solid circle
    graph->SetMarkerColor(kBlue);  // Set marker color to blue
    graph->SetMarkerSize(1);  // Optional: adjust marker size

    // Draw the graph with markers (circles)
    graph->Draw("AP");  // "A" = axis, "P" = points

    // Save the plot as an image
    canvas->SaveAs("MaxIrradiance_vs_Wavelength.png");

    // Close the ROOT file
    file->Close();

    return 0;
}
