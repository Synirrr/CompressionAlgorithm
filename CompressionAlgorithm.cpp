// CompressionAlgorithm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
using namespace std;


typedef uint8_t BYTE;

unsigned int byte_compress(BYTE* data_ptr, unsigned int size)
{
    // Read in first byte and set temp variables
    vector<BYTE> vTemp;


    // Check to see if the next  byte repeats the current one
    for (BYTE index = 0; index <= size - 1;)
    {
        BYTE current = data_ptr[index];
        BYTE length;
        // If the next byte is different copy just current byte to new array
        if (data_ptr[index + 1] != current)
        {
            vTemp.push_back(current);
            cout << "index is: " << (uint64_t)index << endl;
            index++;
        }
        // If the next byte is the same check the next byte until a different one is found. Record the length of the series.
        else
        {
            for (BYTE i = index + 1; i <= size - 1; i++)
            {
                if (data_ptr[i] != data_ptr[i + 1])
                {
                    // Add series number to vector after flipping MSB to indicate series has been detected
                    current = current | 0x80;
                    vTemp.push_back(current);
                    // Calculate length of series
                    length = (i + 1) - index;
                    if (length > 255)
                    {
                        cerr << "Size of series greater than 255. Algorithm is unable to handle this." << endl;
                        return 0;
                    }
                    vTemp.push_back(length);
                    index = i + 1;
                    break;
                }
            }
        }
    }
    // Overwrite input array with vector for output
    data_ptr = vTemp.data();
    
    for (BYTE i = 0; i < vTemp.size() - 1; i++)
        cout << (uint64_t)data_ptr[i] << endl;
    return (unsigned int)vTemp.size();
}


int main()
{
    std::cout << "Hello World!\n";
    //Initialize input data
    // Data before the call
    BYTE data_ptr[] = { 0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64, 0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09 };
    // 0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64, 0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x56, 0x45, 0x56, 0x56, 0x56, 0x89, 0x03
    unsigned int data_size = 24;

    // Call byte_compress
    unsigned int new_size = byte_compress(data_ptr, data_size);
    cout << "The new buffer has a size of: " << new_size << endl << "Compared to a previous size of: " << data_size << endl;

    for (BYTE i = 0; i < new_size - 1; i++)
        cout << (uint64_t)data_ptr[i] << endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
