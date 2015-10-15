#include <iostream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>


//Classes
#include <verboseLog.hpp>
#include <defines.hpp>
#include <lexical.hpp>
#include <preprocessing.hpp>
#include <macro.hpp>
#include <parser.hpp>
#include <symbol.hpp>
#include <montador.hpp>

void parseOpt(std::string *inputFile, std::string *outputFile, int argc, char* argv[])
{
    namespace po = boost::program_options;
    po::options_description desc("Opções Permitidas");
    desc.add_options()
        ("help,h", "Imprime mensagem de ajuda.")
        ("verbose,v", "Adiciona verbosidade ao programa.")
        ("pre,p", po::value<std::vector<std::string> >()->multitoken(),
        "Modo de preprocessamento. Deve receber como entrada 2 nomes de arquivos: Arquivo de entrada e de saída.")
        ("macro,m", po::value<std::vector<std::string> >()->multitoken(),
        "Modo de processamento de Macros. Deve receber como entrada 2 nomes de arquivos: Arquivo de entrada e de saída.")
        ("output,o", po::value<std::vector<std::string> >()->multitoken(),
        "Modo de Tradução completa. Deve receber como entrada 2 nomes de arquivos: Arquivo de entrada e de saída.");

    po::positional_options_description positionalOptions;
    po::variables_map vm; // Mapa contendo todas as opções, descrições e valores

    try
    {
        //pode lançar exceção
        po::store(po::command_line_parser(argc, argv).options(desc).positional(positionalOptions).run(), vm);

        //imprime texto de ajuda e termina o programa
        if (vm.count("help")) 
        { 
            std::cout << "Tradutor. Traduz códigos em assembly inventado para binário."     << std::endl;
            std::cout << "Funciona apenas com assembly inventado."                          << std::endl;
            std::cout                                                                       << std::endl;
            std::cout << "Regras de Uso:"                                                   << std::endl;
            std::cout << "  " << argv[0] << " [opção] [arquivoDeEntrada] [arquivoDeSaida]"  << std::endl;
            std::cout                                                                       << std::endl;
            std::cout << desc                                                               << std::endl;

            exit(0); 
        }

        //pode lançar exceção
        po::notify(vm);

        //Checar as outras opções

        //Ativa o level de log para o modo de verbosidade
        if (vm.count("verbose") && GLOBAL_LEVEL < LOG_INFO) 
        { 
            log<LOG_INFO>("Verbosidade ativada.");
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
                throw "Os argumentos requeridos pela opção '-p' estão errados ou incompletos.";
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
                throw "Os argumentos requeridos pela opção '-m' estão errados ou incompletos.";
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
                throw "Os argumentos requeridos pela opção '-o' estão errados ou incompletos.";
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
        log<LOG_ERROR>("%1%") % e;
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
        {
            try
            {
                Preprocessing preprocessing(inputFile, outputFile);
            }
            catch(const char* e)
            {
                return 1;
            }
            break;
        }
        case MODE_MACRO:
        {
            try
            {
                Macro macro(inputFile, outputFile);
            }
            catch(const char* e)
            {
                return 1;
            }
            break;
        }
        case MODE_OUTPUT:
        {

            break;
        }
        default:
            log<LOG_ERROR>("Modo não encontrado.");
            return 1;
    };


    Lexical lexical;
    lexical.CheckLabel(outputFile, 1);
    Montador montador(inputFile,outputFile);
    //Exeplos output
    log<LOG_DEBUG>("TO DEBUGANDO %1% %2%") % 5 % inputFile;
    log<LOG_INFO>("VERBOSANDO");
    log<LOG_WARNING>("WARNINGZANDO");

    return 0;
}