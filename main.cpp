#include<iostream>
#include<vector>
#include<fstream>

constexpr int MEMORY_SIZE = 1024 * 1024 * 128;

struct Cpu{
    uint64_t registers[32];
    uint64_t pc;
    std::vector<uint8_t> dram;
    
    Cpu(){
        this->registers[0] = 0;
        this->registers[2] = MEMORY_SIZE;
    }
    
    Cpu& set_dram(std::vector<uint8_t> code){
        this->dram = code;
        return *this;
    }

    uint32_t fetch(){
        uint64_t index = this->pc;

        return (this->dram[index]) | (this->dram[index + 1] << 8) |
                (this->dram[index + 2] << 16) | (this->dram[index + 3] << 24); 
    }
    
    uint32_t execute(uint32_t inst){
        uint32_t opcode = inst & 0x7f;
    
    }
};

int main(int argc, char const *argv[])
{
    using namespace std;
    
    if(argc != 2){
        cout<<"error"<<endl;
        exit(1);
    }
    ifstream fcin;
    fcin.open(argv[1], ifstream::in);

    if(fcin.bad()){
        cout<<"Error on opening file\n";
        exit(1);
    }

    fcin.seekg(0, fcin.end);
    int len = fcin.tellg();
    int total_bytes = len/8, remainder = len % 8;
    fcin.seekg(0, fcin.beg);


    Cpu *cpu = new Cpu;
    vector<uint8_t> code;

    while(1){
        char *buff = new char[8];
        if(total_bytes == 0 && remainder != 0){
            fcin.read(buff, remainder);
            remainder = 0;
        }
        else {
            fcin.read(buff, 8);
        }
        uint8_t byte_ = * (uint8_t *) buff;
        code.push_back(byte_);
        
        total_bytes--;

        delete[] buff;
        if(total_bytes == 0 && remainder == 0) break;
    }

    cpu->set_dram(code);
    //while(cpu.pc < cpu.dram.size());
    
    delete cpu;
    fcin.close();
    return 0;
}