#include <iostream>
#include <string>
#include <fstream>
#include <vector>

class SACHeader{
    std::vector<float> float_headers;
    std::vector<int> int_headers;
    std::vector<int> enum_headers;
    std::vector<int> logic_headers;
    std::vector<std::string> string_headers;  
    const int num_float_headers{ 70 };
    const int num_int_headers{ 15 };
    const int num_enum_headers{ 20 };
    const int num_logic_headers{ 5 };
    const int num_string_headers{ 23 };

    public:

    const int size {632};

    SACHeader() = default;

    SACHeader(std::ifstream &sacf){
        // std::ifstream sacf{sacfile, std::ios::in | std::ios::binary};
        float_headers.reserve(num_float_headers);
        for (int i{ 0 }; i < num_float_headers; i++){
            float tmp{};
            sacf.read((char*) &tmp, 4);
            float_headers.push_back(tmp);
        }
        int_headers.reserve(num_int_headers);
        for (int i{ 0 }; i < num_int_headers; i++){
            int tmp{};
            sacf.read((char*) &tmp, 4);
            int_headers.push_back(tmp);
        }
        enum_headers.reserve(num_enum_headers);
        for (int i{ 0 }; i < num_enum_headers; i++){
            int tmp{};
            sacf.read((char*) &tmp, 4);
            enum_headers.push_back(tmp);
        }
        logic_headers.reserve(num_logic_headers);
        for (int i{ 0 }; i < num_logic_headers; i++){
            int tmp{};
            sacf.read((char*) &tmp, 4);
            logic_headers.push_back(tmp);
        }
        string_headers.reserve(num_string_headers);
        for (int i{ 0 }; i < num_string_headers; i++){
            if (i == 1){
                std::string tmp;
                tmp.resize(16);
                sacf.read(&tmp[0], 16);
                string_headers.push_back(tmp);
            } else {
                std::string tmp;
                tmp.resize(8);
                sacf.read(&tmp[0], 8);
                string_headers.push_back(tmp);
            }
            
        }
    }

    SACHeader& operator=(const SACHeader& other){
        if (&other == this)
            return *this;
        float_headers = other.float_headers;
        int_headers = other.int_headers;
        enum_headers = other.enum_headers;
        logic_headers = other.logic_headers;
        string_headers = other.string_headers;
        return *this;
    }

    int npts() const {
        return int_headers[9];
    }

    friend std::ostream& operator<<(std::ostream &out, const SACHeader &sachdr){
        out << "float headers: \n";
        for (int i{ 0 }; i < sachdr.num_float_headers; i++){   
            out << sachdr.float_headers.at(i) << ", ";
        }
        out << '\n';
        out << "int headers: \n";
        for (int i{ 0 }; i < sachdr.num_int_headers; i++){   
            out << sachdr.int_headers.at(i) << ", ";
        }
        out << '\n';
        out << "enum headers: \n";
        for (int i{ 0 }; i < sachdr.num_enum_headers; i++){   
            out << sachdr.enum_headers.at(i) << ", ";
        }
        out << '\n';
        out << "logic headers: \n";
        for (int i{ 0 }; i < sachdr.num_logic_headers; i++){   
            out << sachdr.logic_headers.at(i) << ", ";
        }
        out << '\n';
        out << "string headers: \n";
        for (int i{ 0 }; i < sachdr.num_string_headers; i++){   
            out << sachdr.string_headers.at(i) << ", ";
        }

        return out;
    }
};

struct SAC{
    SACHeader header;
    std::vector<float> data{};
    
    SAC(std::ifstream &sacf, const bool &header_only = false){
        //std::ifstream sacf{sacfile, std::ios::in | std::ios::binary};

        header = SACHeader{ sacf };
        if (header_only) return;

        int npts{ header.npts() };
        data.reserve(npts);
        sacf.seekg(header.size, std::ios::beg);
        for (int i{ 0 }; i < npts; i++){
            float tmp{};
            sacf.read((char*) &tmp, 4);
            data.push_back(tmp);
        }
    }

    friend std::ostream& operator<<(std::ostream &out, const SAC &sac){
        out << sac.header << '\n';
        out << "data (expected size = " << sac.header.npts() 
            << ", actual size = " << sac.data.size() << "): \n";
        out.precision(10);
        for (const auto &i : sac.data)
            out << i << ' ';
        return out;
    }
};

int main(){
    std::string sacfile = "test.sac";
    std::cout << "reading sac file: " 
        << sacfile << "\n";
    
    std::ifstream sacf{sacfile, std::ios::in | std::ios::binary};
    // SACHeader sachdr{ sacf };
    SAC sac{ sacf, false };
    std::cout << sac << '\n';
    // sachdr(sacfile);
    return 0;
}