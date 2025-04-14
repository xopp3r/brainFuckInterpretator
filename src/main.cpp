#include <fstream>
#include <iostream>
#include <string>
#include <format>
#include <memory>





class Interpreter {
    public:
    Interpreter() = delete;
    Interpreter(std::string code, size_t tapeLength);
    ~Interpreter();

    int run();
    
    private:
    size_t tapeLength;
    size_t tapeHeadIndex;
    std::string code;
    std::unique_ptr<char[]> tape;

    int validate();

};



Interpreter::Interpreter(std::string code, size_t tapeLength)
    : code (code), tapeLength(tapeLength), tape(std::make_unique<char[]>(tapeLength)), tapeHeadIndex(0) {
        std::fill_n(tape.get(), tapeLength, 0);
}



Interpreter::~Interpreter(){}



int Interpreter::validate(){
    int bracesCount = 0;

    for (int i = 0; i < code.size(); i++){

        switch (code[i]){
        case '[':
            bracesCount++;
            break;
        case ']':
            bracesCount--;

            if (bracesCount < 0) 
                throw std::runtime_error(std::format("{} missmatched braces detected", bracesCount));

            break;
        case '<':
        case '>':
        case '+':
        case '-':
        case '.':
        case ',':
            break;
        default:
            throw 
                std::runtime_error(std::format("Unrecognized character: {} at index {}", code[i], i));

            break;
        }

    }

    if (bracesCount != 0) 
        throw std::runtime_error("Missmatched braces detected");

    return 0;
}



int Interpreter::run(){
    

    for (auto it = code.begin(); it != code.end(); it++){

        std::cout.put(*it);
        std::cout.flush();
        
        switch (*it) {
            case '[':
            
            if (tape[tapeHeadIndex] == 0){
                int braces = 0;
                
                while (!(*(++it) == ']' && braces == 0)) {
                    if (*it == '[') braces++;
                    if (*it == ']') braces--;
                }
                
                it++;
            }
            
            break;
            case ']':
            
            if (tape[tapeHeadIndex] != 0){
                int braces = 0;
                
                while (!(*(--it) == '[' && braces == 0)) {
                    if (*it == ']') braces++;
                    if (*it == '[') braces--;
                }
                
                it++;
            }
            
            break;
            case '<':
            tapeHeadIndex--;
            
            if (tapeHeadIndex < 0)
                throw std::runtime_error(std::format("Out of bounds indexing at {}", it - code.begin()));
            
            break;
            case '>':
            tapeHeadIndex++;
            
            if (tapeHeadIndex >= tapeLength)
                throw std::runtime_error(std::format("Out of bounds indexing at {}", it - code.begin()));
            
            break;
            case '+':
            
            tape[tapeHeadIndex]++;
            
            break;
            case '-':
            
            tape[tapeHeadIndex]--;
            
            break;
            case '.':
            
            std::cout.put(tape[tapeHeadIndex]);
            std::cout.flush();
            
            break;
            case ',':
            
            std::cin.read(tape.get() + tapeHeadIndex, 1);
            
            break;
        }

        std::cout << "\n\tcurrent = head: " << tapeHeadIndex << " value: " << (int)(tape[tapeHeadIndex]) << " char: " << it - code.begin() << std::endl;
        
    }

    
    return 0;
}







int main(int argc, char const *argv[]){
    std::cout << "XD" << std::endl;
    if (argc == 1) throw std::runtime_error("No input file specified");

    std::ifstream sourceCode(argv[1]);
    if (!sourceCode.is_open()) throw std::runtime_error("Can't open the file");

    int tapeLength = 30000;
    if (argc >= 3) {
        tapeLength = atoi(argv[3]);
        if (tapeLength < 1) throw std::runtime_error("Invalid tape lenght");
    }

    std::string code;
    std::string line;

    while (std::getline(sourceCode, line)){
        code.append(line);
    }

    Interpreter intrprtr(code, tapeLength);
    
    int returnCode = intrprtr.run();

    return returnCode;
}
