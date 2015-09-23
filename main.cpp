#include <iostream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>


//Classes
#include "verboseLog.hpp"
#include "defines.hpp"

void parseOpt(std::string *inputFile, std::string *outputFile, int argc, char* argv[])
{
    namespace po = boost::program_options;
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Print help mesages.")
        ("verbose,v", "Add verbosity to the program.")
        ("pre,p", po::value<std::vector<std::string> >()->multitoken(),
        "Preprocessing mode. Should receive 2 files as input.")
        ("macro,m", po::value<std::vector<std::string> >()->multitoken(),
        "Macro mode. Should receive 2 files as input.")
        ("output,o", po::value<std::vector<std::string> >()->multitoken(),
        "Translating mode. Should receive 2 files as input.");

    po::positional_options_description positionalOptions;
    po::variables_map vm; // Mapa contendo todas as opções, descrições e valores

    try
    {
        //pode lançar exceção
        po::store(po::command_line_parser(argc, argv).options(desc).positional(positionalOptions).run(), vm);

        //imprime texto de ajuda e termina o programa
        if (vm.count("help")) 
        { 
            std::cout << "Translator: an assembly translator."                     << std::endl;
            std::cout << "Works only with one ficticious assembly code."           << std::endl;
            std::cout                                                              << std::endl;
            std::cout << "Usage:"                                                  << std::endl;
            std::cout << "  " << argv[0] << " [options] [inputFile] [outputFile] " << std::endl;
            std::cout                                                              << std::endl;
            std::cout << desc                                                      << std::endl;

            exit(0); 
        }

        //pode lançar exceção
        po::notify(vm);

        //Checar as outras opções

        //Ativa o level de log para o modo de verbosidade
        if (vm.count("verbose") && GLOBAL_LEVEL < LOG_INFO) 
        { 
            log<LOG_INFO>("Verbose Printing.");
            GLOBAL_LEVEL = LOG_INFO;
        }

        //ativa o modo de pre compilação e salva os arquivos de input e output
        if (vm.count("pre"))
        {
            std::vector<std::string> fileNames;
            //Se a variável de entrada nao estiver vazia e seu tamanho for igual a 2, faça a função descrita acima
            if (!vm["pre"].empty() && (fileNames = vm["pre"].as<std::vector<std::string> >()).size() == 2) 
            {
                *inputFile = fileNames[0];
                *outputFile = fileNames[1];
                PROGRAM_MODE = MODE_PRE;     
            }
            //senão, lance um erro
            else
            {
                throw "the required argument for option '-p' is missing";
            }
        }

        //ativa o modo de leitura de macros e salva os arquivos de input e output
        if (vm.count("macro"))
        {
            std::vector<std::string> fileNames;
            if (!vm["macro"].empty() && (fileNames = vm["macro"].as<std::vector<std::string> >()).size() == 2) 
            {
                *inputFile = fileNames[0];
                *outputFile = fileNames[1];
                PROGRAM_MODE = MODE_MACRO;     
            }
            else
            {
                throw "the required argument for option '-m' is missing";
            }
        }

        //ativa o mode de tradutor e salva os arquivos de input e output
        if (vm.count("output"))
        {
            std::vector<std::string> fileNames;
            if (!vm["output"].empty() && (fileNames = vm["output"].as<std::vector<std::string> >()).size() == 2) 
            {
                *inputFile = fileNames[0];
                *outputFile = fileNames[1];
                PROGRAM_MODE = MODE_OUTPUT;     
            }
            else
            {
                throw "the required argument for option '-o' is missing";
            }
        }
    }
    //pega erros lançados pela boost program_options
    catch(po::error &e)
    {
        std::cerr << e.what() << std::endl;
        std::cout << desc << std::endl;
        exit(1);
    }
    //pega erros lançados manualmente
    catch(const char* e)
    {
        std::cerr << e << std::endl;
        std::cout << desc << std::endl;
        exit(1);
    }
}

int main(int argc, char* argv[])
{
//Se compilado em modo de Debug, level de verbosidade setado para o maximo.
#ifdef DEBUG_MODE
    GLOBAL_LEVEL = LOG_DEBUG;
#else
    GLOBAL_LEVEL = LOG_WARNING;
#endif

    std::string inputFile, outputFile;
    parseOpt(&inputFile, &outputFile, argc, argv);

    switch(PROGRAM_MODE)
    {
    case MODE_PRE:

        break;
    case MODE_MACRO:

        break;
    case MODE_OUTPUT:

        break;
    default:
        log<LOG_ERROR>("Mode not found.");
        return 1;
    };

    std::cout << inputFile << std::endl;
    std::cout << outputFile << std::endl;


    //Exeplos output
    log<LOG_DEBUG>("TO DEBUGANDO %1% %2%") % 5 % inputFile;
    log<LOG_INFO>("VERBOSANDO");

    return 0;
}