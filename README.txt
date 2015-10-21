Alunos: Raphael Arthur Barbosa Resende  (11/0070712)
        Yuri Gonçalves Rocha            (11/0067461)


Instruções:
    Para compilar o código são necessárias as seguintes bibliotecas:
        - Boost headers
        - Boost Program Options
        - Boost Regex
    Para instalar as bibliotecas:
        - sudo apt-get install libboost-dev
        - sudo apt-get install libboost-program-options1.55-dev 
        - sudo apt-get install libboost-regex1.55-dev
        
Além disso, é necessário ter pelo menos a versão C++11.
        
Existe um script para facilitar a compilação. Primeiramente habilite o script ser utilizado como executável:
        - chmod +x build_release.sh
Ou:
        - chmod 777 build_release.sh
        
Para compilar, basta então executar:
        - ./build_release.sh
        
Para executar o programa:
        - ./Tradutor <argumentos>
Para ler todas as opções pode-se utilizar o argumento -h ou --help:
        - ./Tradutor -h
        - ./Tradutor --help
        
Observações:

IMPORTANTE: Na detecção de erros, o número da linha é referente ao arquivo de entrada do módulo de processamento.
Ou seja, Erros de preprocessamento mostrarão linhas referentes ao arquivo .asm.
Erros de Macro mostrarão linhas referentes ao arquivo .pre.
Erros no montador mostrarão linhas referentes ao arquivo .mcr.
