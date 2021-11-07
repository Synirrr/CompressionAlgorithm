// CompressionAlgorithm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
using namespace std;


typedef uint8_t BYTE;



unsigned int byte_compress(BYTE *data_ptr, unsigned int size)
{
    // Read in first byte and set temp variables
    vector<BYTE> vTemp;


    // Check to see if the next  byte repeats the current one
    for (unsigned int index = 0; index <= size - 1;)
    {
        BYTE current = data_ptr[index];
        unsigned int length;
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
            for (unsigned int i = index + 1; i <= size - 1; i++)
            {
                if (data_ptr[i] != data_ptr[i + 1])
                {
                    // Add series number to vector after flipping MSB to indicate series has been detected
                    current = current | 0x80;
                    vTemp.push_back(current);
                    // Calculate length of series
                    unsigned int length = (i + 1) - index;
                    // Determine if we need more bytes for length
                    if (length > 8)
                    {
                        unsigned int numBytes = ceil(((double)length - 8) / 255);
                        // Shift the bits to fill a byte containing 4 bits (7-4) telling us how many bytes are used for length and 4 bits (3-0) telling us the length
                        BYTE controlByte = (numBytes << 4) | 8;
                        // Remove already allocated length
                        length = length - 8;
                        vTemp.push_back(controlByte);
                        // Add remaining length to the next required bytes
                        for (unsigned int t = 0; t < numBytes; t++)
                        {
                            if (length > 255)
                            {
                                vTemp.push_back(255);
                                length = length - 255;
                            }
                            else
                                vTemp.push_back(length);
                        }
                    }
                    else
                        vTemp.push_back(length);
                    index = i + 1;
                    break;
                }
            }
        }
    }
    // Overwrite input array with vector for output
    copy(vTemp.begin(), vTemp.end(), data_ptr);
    return (unsigned int)vTemp.size();
}


int main()
{
    //Initialize input data
    // Data before the call
    BYTE data_ptr[] = { 0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64, 0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09 };
    unsigned int data_size = 36;

    // Call byte_compress
    unsigned int new_size = byte_compress(data_ptr, data_size);
    cout << "The new buffer has a size of: " << new_size << endl << "Compared to a previous size of: " << data_size << endl;

    for (unsigned int i = 0; i < new_size; i++)
        cout << (uint64_t)data_ptr[i] << endl;
}