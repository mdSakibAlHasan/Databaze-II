#include <iostream>
#include <fstream>

int main()
{
    // Open input file
    std::ifstream input_file("new.txt");
    if (!input_file.is_open())
    {
        std::cerr << "Error: Failed to open input file!\n";
        return 1;
    }

    // Open output file
    std::ofstream output_file("old.txt");
    if (!output_file.is_open())
    {
        std::cerr << "Error: Failed to open output file!\n";
        return 1;
    }

    // Read character by character from input file and save it to output file
    char c;
    while (input_file.get(c))
    {
        if(c==',')
            output_file.put(32);
        else
            output_file.put(c);
    }

    // Close files
    input_file.close();
    output_file.close();

    return 0;
}
