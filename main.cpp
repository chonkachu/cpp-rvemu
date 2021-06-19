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

        return (uint32_t((this->dram[index])) | (uint32_t(this->dram[index + 1]) << 8) |
                (uint32_t(this->dram[index + 2]) << 16) | (uint32_t(this->dram[index + 3]) << 24)); 
    }
    
    void execute(uint32_t inst){
        uint32_t opcode = inst & 0x7f;
        uint32_t rd = (inst >> 7) & 0x1f;
        uint32_t rs1 = (inst >> 15) & 0x1f;
        uint32_t rs2 = (inst >> 20) & 0x1f;

        //std::cout<< opcode<<" "<<rd<< " "<< rs1<<" "<<rs2<<" "<<std::endl;
        switch(opcode){
            case 0x13:
            { 
                uint64_t imm = (inst >>20) & 0xfff;
                //std::cout<<imm<<std::endl;
                this->registers[rd] = this->registers[rs1] + imm;
                break;
            }
            case 0x33:
            {
                this->registers[rd] = this->registers[rs1] + this->registers[rs2];
                break;
            }
            default:
                break;
        }
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
    int total_bytes = len;
    fcin.seekg(0, fcin.beg);


    Cpu *cpu = new Cpu();
    vector<uint8_t> code;

    while(1){
        char buff[1];
        fcin.read(buff, 1);
        uint8_t byte_ = (uint8_t) buff[0];
        code.push_back(byte_);
        
        total_bytes--;

        if(total_bytes == 0 ) break;
    }

    cpu->set_dram(code);
    while(cpu->pc < uint64_t(cpu->dram.size())){
        auto inst = cpu->fetch();
        cpu->pc += 4;
        cpu->execute(inst);
        

        for(int i = 0; i < 32; i++){
              
            printf("x%d= 0x%lx\n", i, cpu->registers[i]);
        }
        cout<<"-------------------------\n";
    }
    
    delete cpu;
    fcin.close();
    return 0;
}