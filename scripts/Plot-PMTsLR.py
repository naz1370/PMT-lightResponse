import numpy as np

# Ask for user information and Dark value at the beginning
Dark = float(input("Enter the Dark value: "))

def read_first_file(file_path, dark_value):
    data = []
    with open(file_path, 'r') as file:
        for line in file:
            if line.strip() == "":
                continue
            row = [float(x) for x in line.strip().split(',')]
            current = sum(row) / len(row)
            row.append(current)
            row = [x - dark_value for x in row]
            data.append(row)
    return np.array(data)

def read_second_file(file_path):
    data = []
    with open(file_path, 'r') as file:
        next(file)  # Skip the header line
        for line in file:
            row = line.strip().split('\t')
            data.append([float(row[1]), float(row[2])])  # MaxIrradiance and Wavelength
    return np.array(data)

def combine_data(data1, data2):
    min_rows = min(data1.shape[0], data2.shape[0])
    combined = np.column_stack((data1[:min_rows], data2[:min_rows]))
    
    current = combined[:, 2]
    max_irradiance = combined[:, 3]
    light_response = -current / max_irradiance
    
    combined = np.column_stack((combined, light_response))
    
    return combined

def save_combined_data_to_file(matrix, file_name="combined_data.txt"):
    with open(file_name, 'w') as txtfile:
        # Write the header
        txtfile.write("Index\tcurr1\tcurr2\tCurrent(A)\tIntensity(uWatt/cm^2/nm)\tWavelength(nm)\tLight Response(A/(uWhatt/cm^2/nm)\n")
        # Write the data rows using format() instead of f-strings
        for i, row in enumerate(matrix):
            txtfile.write("{}\t{:.4e}\t{:.4e}\t{:.4e}\t{:.4f}\t{:.4f}\t{:.4e}\n".format(i, row[0], row[1], row[2], row[3], row[4], row[5]))
    print("\nData has been written to {}".format(file_name))

# Read and process the files
file_path1 = "13Sepetember_F24-1250_cable3_picoammeter_A.txt"
file_path2 = "results.txt"
matrix1 = read_first_file(file_path1, Dark)
matrix2 = read_second_file(file_path2)

# Combine the data and calculate Light Response
result_matrix = combine_data(matrix1, matrix2)

# Print the combined data to the console
print("Combined Matrix:")
print("Index | curr1 | curr2 | Current(A) | Intensity(uWhatt/cm^2/nm) | Wavelength(nm) | Light Response(A/uWhatt/cm^2/nm)")
print("-" * 100)
for i, row in enumerate(result_matrix):
    print("{:5d} | {:11.4e} | {:12.4e} | {:11.4e} | {:12.4f} | {:10.4f} | {:14.4e}".format(i, row[0], row[1], row[2], row[3], row[4], row[5]))

print("\nMatrix shape:", result_matrix.shape)

# Save the combined data to a text file
save_combined_data_to_file(result_matrix)
