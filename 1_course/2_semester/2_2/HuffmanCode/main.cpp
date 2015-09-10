#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include "HuffmanCode.h"

int main()
{
    HuffmanCode h;
/*
    std::ifstream is ("20150530142655.jpg", std::ifstream::binary);
    if (is)
    {
        // get length of file:
        is.seekg (0, is.end);
        int length = is.tellg();
        is.seekg (0, is.beg);

        char * buffer = new char [length];

        std::cout << "Reading " << length << " characters... ";
        // read data as a block:
        is.read (buffer,length);

        if (is)
          std::cout << "all characters read successfully.";
        else
          std::cout << "error: only " << is.gcount() << " could be read";
        is.close();

        // ...buffer contains the entire file...

   //*************************************************************************************************
        std::ofstream os ("output_code.txt", std::ofstream::binary);
        if (os)
        {
            std::vector<char> text;
            for(int i = 0; i < length; ++i)
            {
                text.push_back(buffer[i]);
            }

            std::cout << "Writing characters... ";
            std::vector<char> code = h.Code(text);
            std::string buf;

            for(auto it = code.begin(), end = code.end(); it != end; ++it)
            {
                buf.push_back(*it);
            }
            os.write (buf.c_str(),buf.size());
            os.close();
        }

    delete[] buffer;

//**********************************************************************************************

        std::ofstream oos ("output_decoder.txt", std::ofstream::binary);
        if (oos)
        {

            std::cout << "Writing characters... ";
            std::vector<char> decoder = h.GetDecoder();
            std::string buf;

            for(auto it = decoder.begin(), end = decoder.end(); it != end; ++it)
            {
                buf.push_back(*it);
            }
            oos.write (buf.c_str(),buf.size());
            oos.close();
        }
    }
*/


    std::ifstream is ("output_decoder.txt", std::ifstream::binary);
    if (is.is_open())
    {
        // get length of file:
        is.seekg (0, is.end);
        int length = is.tellg();
        is.seekg (0, is.beg);

        char * buffer = new char [length];

        std::cout << "Reading " << length << " characters... ";
        // read data as a block:
        is.read (buffer,length);

        if (is)
          std::cout << "all characters read successfully.";
        else
          std::cout << "error: only " << is.gcount() << " could be read";
        is.close();

        std::vector<char> decoder;
        for(int i = 0; i < length; ++i)
        {
            decoder.push_back(buffer[i]);
        }

        h.SetDecoder(decoder);

        delete[] buffer;

        // ...buffer contains the entire file...

//*************************************************************************************************
        is.open("output_code.txt", std::ifstream::binary);
        if (is)
        {
            // get length of file:
            is.seekg (0, is.end);
            length = is.tellg();
            is.seekg (0, is.beg);

            char * buffer = new char [length];

            std::cout << "Reading " << length << " characters... ";
            // read data as a block:
            is.read (buffer,length);

            if (is)
              std::cout << "all characters read successfully.";
            else
              std::cout << "error: only " << is.gcount() << " could be read";
            is.close();
//*************************************************************************************************
            std::vector<char> code;
            for(int i = 0; i < length; ++i)
            {
                code.push_back(buffer[i]);
            }


            std::ofstream os ("output_text.jpg", std::ofstream::binary);
            if (os)
            {

                std::cout << "Writing characters... ";
                std::vector<char> text = h.Decode(code);

                std::string buf;
                std::cout << text.size() << std::endl;

                for(auto it = text.begin(), end = text.end(); it != end; ++it)
                {
                    buf.push_back(*it);
                }
                os.write (buf.c_str(),buf.size());
                os.close();
            }
        }
    }


/*

    std::ifstream is ("mm_map.jpg", std::ifstream::binary);
    if (is)
    {
        // get length of file:
        is.seekg (0, is.end);
        int length = is.tellg();
        is.seekg (0, is.beg);

        char * buffer = new char [length];

        std::cout << "Reading " << length << " characters... ";
        // read data as a block:
        is.read (buffer,length);

        if (is)
          std::cout << "all characters read successfully.";
        else
          std::cout << "error: only " << is.gcount() << " could be read";
        is.close();

        std::vector<char> text;
        for(int i = 0; i < length; ++i)
        {
            text.push_back(buffer[i]);
        }

        std::cout << "Writing characters... ";
        std::vector<char> code = h.Code(text);

        text = h.Decode(code);
        char* buf = new char[text.size()];
        std::cout << text.size() << std::endl;

        int i = 0;
        for(auto it = text.begin(), end = text.end(); it != end; ++it)
        {
            buf[i] = *it;
            ++i;
        }

        std::ofstream os ("output_text.jpg", std::ofstream::binary);
        if (os)
        {

            std::cout << "Writing characters... ";
            os.write (buf,text.size());
            os.close();
        }
    }

*/

    return 0;
}
