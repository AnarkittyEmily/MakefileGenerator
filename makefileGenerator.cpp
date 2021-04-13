#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>


/* 
    Course of the makefile generator:
        Provide the files you want to build for, and then make a make file. 
        Add arguments to the file you want aswell
*/
class makefile {
    public:
        std::fstream buildFile; 

        int writeToFile(const std::vector<std::string> &buildOutput) {
            this->buildFile.open(buildOutput[2] + "/makefile", std::fstream::out);

            if (!this->buildFile.is_open()) { 
                std::cout << "Could not create file :(" << std::endl;
                return 1; 
            } 
            this->buildFile << "install:\n"; 
            this->buildFile << buildOutput[1] << " " << buildOutput[3] << " " << buildOutput[0] << std::endl; 

            this->buildFile.close(); 
            return 0; 
        }

        int readFromFile(const std::string& pathToMake) { 
            this->buildFile.open(pathToMake, std::fstream::in);
            std::string contents;
            std::cout << "Current build file: \n"; 
            while (getline(buildFile, contents)) { 
                std::cout << contents << std::endl; 
            } 
            return 0;   
        }

        int modifyBuild(const std::string &pathToMake) { 
            this->buildFile.open(pathToMake, std::fstream::in);
            if (this->buildFile.is_open()) { 
                std::cout << "Could not open file";
                if (!std::filesystem::exists(pathToMake)) { 
                    std::cout << "\n" << "Reason: File does not exist"; 
                    return 1;
                } else if (pathToMake.find(getenv("HOME")) == std::string::npos) { 
                    std::cout << "\n" << "Reason: I do not have root permissions!"; 
                    return 1;
                }
            }
            this->buildFile.seekg(-2, std::ios::end); 
            bool loopThroughFile = true; 
            while (loopThroughFile == true) { 
                char getch; 
                this->buildFile.get(getch); 
                if (getch == '\n') { 
                    break; 
                } else { 
                    this->buildFile.seekg(-2, std::ios::cur); 
                }
            }

            std::string lastline; 
            getline(this->buildFile, lastline);
            this->buildFile.close();  
            int opt; 
            std::cout << "What do you want to modify?: 1 (Compiler) 2 (Options) 3 (Build files): ";
            std::cin >> opt; 
            if (opt == 1) { 
                std::string newCompiler;
                std::cout << "Enter your new compiler: "; 
                std::cin >> newCompiler; 
                lastline.replace(0, lastline.find(" "), newCompiler); 
                this->buildFile.open(pathToMake, std::fstream::out); 
                this->buildFile << "install:\n"; 
                this->buildFile << lastline; 
            } else if (opt == 2) {  
                std::string oldOptions; 
                std::cout << "Enter your old options: "; 
                std::cin.ignore(); 
                std::getline(std::cin, oldOptions);
                std::string newOptions; 
                std::cout << "Enter your new options: "; 
                std::cin.ignore(); 
                std::getline(std::cin, newOptions);

                lastline.replace(lastline.find("-"), oldOptions.length(), '-'+newOptions); 
                this->buildFile.open(pathToMake, std::fstream::out); 
                this->buildFile << "install:\n"; 
                this->buildFile << lastline; 
            } else if (opt == 3) { 
                std::string oldBuildFiles;
                std::string newBuildFiles;
                std::cin.ignore(); 
                std::cout << "Enter your old build files: ";
                getline(std::cin, oldBuildFiles); 

                std::cout << "Enter your new build files: "; 
                getline(std::cin, newBuildFiles);
                lastline.replace(lastline.find(oldBuildFiles), newBuildFiles.length(), newBuildFiles); 
                this->buildFile.open(pathToMake, std::fstream::out); 
                this->buildFile << "install:\n"; 
                this->buildFile << lastline; 
            }
            return 0; 
        }
};


void writeToFile(const std::vector<std::string>& arguments) { 
    makefile build; 
    std::string pathToMake = arguments[2]; 
    std::string compiler = arguments[1];

    if (!std::filesystem::exists("/usr/bin/" + compiler)) { 
        std::cout << "Warning, compiler does not exist"; 
    }
    if (!std::filesystem::exists(pathToMake)) { 
        if (pathToMake.find("~") != std::string::npos) { 
            pathToMake.replace(0, 1, getenv("HOME"));
            if (!std::filesystem::exists(pathToMake)) {
                std::cout << "Could not find path" << std::endl;
                return; 
            }
        } else {
            std::cout << "Could not find path" << std::endl; 
            return; 
        }
    }
    build.writeToFile(arguments);
}; 


int main(int argc, char* argv[]) { 
    std::vector<std::string> arguments;
    arguments.resize(4);
    // first index are the files to build  
    // second index is the compiler 
    // third index is the path to the  makefile we are going to create 
    // fourth index is are the options.
    
    int opt; 
    std::cout << "1 (Make a build task)\n"
              << "2 (Modify the build task)\n"
              << "3 (Show your current build task)\n";
    std::cin >> opt; 
    std::cin.ignore(); 

    switch (opt) {
        case 1: {

            std::cout << "List the files you want to build: "; 
            std::getline(std::cin, arguments[0]); 
            std::cout << "What compiler do you want to use?: "; 
            std::cin >> arguments[1];

            std::cout << "Enter the path you want to configure: "; 
            std::cin >> arguments[2];
            std::cin.ignore();  

            std::cout << "What options do you want to have for your build?: "; 
            std::getline(std::cin, arguments[3]);
            writeToFile(arguments);
            break;
        }
        case 2: {
            std::string path;
            std::cout << "Enter the path for your build file: "; 
            std::cin >> path; 
            std::cin.ignore(); 
            makefile modify; 
            modify.modifyBuild(path); 
            break; 
        }

        case 3: {
            std::string path;
            std::cout << "Enter the path for your build file: "; 
            std::cin >> path;
            makefile read; 
            read.readFromFile(path); 
            break;
        } 
    }       
}  