#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void imprimeEmBinario(short int num){
    for(int i = sizeof(num)*8 - 1; i >= 0; i--){
        printf("%d", (num>>i) & 1);
    }
}

void limpaEspacosEmBrancoAEsquerdaDaString(char *string){
    int j, k = 0;
    int len = strlen(string);
    for(k = 0; k < len && isspace(string[0]); k++){
        j = 0;
        while(j <= len){
            string[j] = string[j + 1];
            j++;
        }
    }
}

void separaPosicaoDaMemoriaDoValorDeEntrada(char *valorDeEntrada, char *posicaoDaMemoria){
    for(int i = 0; i < strlen(valorDeEntrada); i++){
        if(valorDeEntrada[i] != ';')
            posicaoDaMemoria[i] = valorDeEntrada[i];
        else
            break;
    }
    limpaEspacosEmBrancoAEsquerdaDaString(posicaoDaMemoria);
}

int converteHexaParaInt(char* valorEmHexa){
    char end;
    int posicaoEmDecimal = strtol(valorEmHexa, end, 16);
    return posicaoEmDecimal;
}

int retornaPosicaoDaProximaInstrucao(char* valorDeEntrada, int posicaoInicialAnterior, char divisor){
    char *pos = strchr(valorDeEntrada + posicaoInicialAnterior, divisor);
    int inicioProximaInstrucao = (pos-valorDeEntrada) + 1;
    return inicioProximaInstrucao;
}

char separaDataOrInstructionDoValorDeEntrada(char* valorDeEntrada, int inicioProximaInstrucao){
    char dataOrInstruction;
    char stringTemp[10] = "\0\0\0\0\0\0\0\0\0\0";
    int j = 0;
    for(int i = inicioProximaInstrucao; i < strlen(valorDeEntrada); i++){
        if(valorDeEntrada[i] != ';')
            stringTemp[j++] = valorDeEntrada[i];
        else
            break;
    }
    limpaEspacosEmBrancoAEsquerdaDaString(stringTemp);
    dataOrInstruction = stringTemp[0];
    return dataOrInstruction;
}

void preencheTodasAsPosicoesDoVetorComVazio(char *vetor, int tamanho){
    for(int i = 0; i < tamanho; i++){
        vetor[i] = 'a';
    }
}

void separaPrimeiraInstrucaoDoValorDeEntrada(char *valorDeEntrada, char *primeiraInstrucao, int inicioProximaInstrucao){
    int contador = 0;
    for(int i = inicioProximaInstrucao; i < strlen(valorDeEntrada); i++){
        if(valorDeEntrada[i] != '/'){
            primeiraInstrucao[contador++] = valorDeEntrada[i];
        }else{
            primeiraInstrucao[contador++] = valorDeEntrada[i];
            break;
        }
    }
    limpaEspacosEmBrancoAEsquerdaDaString(primeiraInstrucao);
}

void separaOperacaoDaInstrucao(char *primeiraInstrucao, char* op, int inicioEnderecoMemoria){
    int k = 0;
    for(int i = 0; i < inicioEnderecoMemoria; i++){
        if(primeiraInstrucao[i] != ' '){
            op[k++] = primeiraInstrucao [i];
        } else {
            break;
        }
    }
}

void separaEnderecoDaInstrucao(char *primeiraInstrucao, char *endereco, int inicioEnderecoMemoria){
    int j = 0;
    for(int i = inicioEnderecoMemoria; i < strlen(primeiraInstrucao); i++){
        if(primeiraInstrucao[i] != '/'){
            endereco[j++] = primeiraInstrucao[i];
        } else {
            break;
        }
    }

}

void separaSegundaInstrucaoDoValorDeEntrada(char *valorDeEntrada, char *segundaInstrucao, int inicioSegundaInstrucao){
    int contador = 0;
    for(int i = inicioSegundaInstrucao; i < strlen(valorDeEntrada); i++){
            segundaInstrucao[contador++] = valorDeEntrada[i];
    }
    limpaEspacosEmBrancoAEsquerdaDaString(segundaInstrucao);
}

int separaValorASerGuardado(char *valorDeEntrada, int inicioProximaInstrucao){
    int contador = 0;
    char valorASerGuardadoEmHexa[10] = "\0\0\0\0\0\0\0\0\0\0";
    for(int i = inicioProximaInstrucao; i < strlen(valorDeEntrada); i++){
            valorASerGuardadoEmHexa[contador++] = valorDeEntrada[i];
    }
    limpaEspacosEmBrancoAEsquerdaDaString(valorASerGuardadoEmHexa);
    return converteHexaParaInt(valorASerGuardadoEmHexa);
}

void executaLeituraDosDadosEGuardaNaMemoria(){
    char valorDeEntrada[30];

    char valorDaPosicaoDaMemoriaEmHexa[3];
    int posicaoNaMemoria;

    int inicioProximaInstrucao = 0;

    char dataOrInstruction;

    fgets(valorDeEntrada, 30, stdin);

    separaPosicaoDaMemoriaDoValorDeEntrada(valorDeEntrada, valorDaPosicaoDaMemoriaEmHexa);
    posicaoNaMemoria = converteHexaParaInt(valorDaPosicaoDaMemoriaEmHexa);

    inicioProximaInstrucao = retornaPosicaoDaProximaInstrucao(valorDeEntrada, 0, ';');

    dataOrInstruction = separaDataOrInstructionDoValorDeEntrada(valorDeEntrada, inicioProximaInstrucao);

    inicioProximaInstrucao = retornaPosicaoDaProximaInstrucao(valorDeEntrada, inicioProximaInstrucao, ';');


    if(dataOrInstruction == 'i'){

        char primeiraInstrucao[15];
        char pEnderecoDeMemoriaEmHexa[4] = "\0\0\0\0";
        int pEnderecoDeMemoria = 0;
        char pOp[7] = "\0\0\0\0\0\0\0";

        char segundaInstrucao[15];
        char sEnderecoDeMemoriaEmHexa[4] = "\0\0\0\0";
        int sEnderecoDeMemoria = 0;
        char sOp[7] = "\0\0\0\0\0\0\0";

        int inicioProximoComando = retornaPosicaoDaProximaInstrucao(valorDeEntrada, inicioProximaInstrucao, '/');
        preencheTodasAsPosicoesDoVetorComVazio(primeiraInstrucao, 15);
        separaPrimeiraInstrucaoDoValorDeEntrada(valorDeEntrada, primeiraInstrucao, inicioProximaInstrucao);
        if(strchr(primeiraInstrucao, ' ')){
            int inicioEnderecoMemoria = retornaPosicaoDaProximaInstrucao(primeiraInstrucao, 0, ' ');
            separaOperacaoDaInstrucao(primeiraInstrucao, pOp, inicioEnderecoMemoria);
            separaEnderecoDaInstrucao(primeiraInstrucao, pEnderecoDeMemoriaEmHexa, inicioEnderecoMemoria);
            pEnderecoDeMemoria = converteHexaParaInt(pEnderecoDeMemoriaEmHexa);
        }
        else{
            int inicioEnderecoMemoria = retornaPosicaoDaProximaInstrucao(primeiraInstrucao, 0, '/');
            separaOperacaoDaInstrucao(primeiraInstrucao, pOp, inicioEnderecoMemoria-1);
        }
        separaSegundaInstrucaoDoValorDeEntrada(valorDeEntrada, segundaInstrucao, inicioProximoComando);
        if(strchr(segundaInstrucao, ' ')){
            int inicioEnderecoMemoria = retornaPosicaoDaProximaInstrucao(segundaInstrucao, 0, ' ');
            separaOperacaoDaInstrucao(segundaInstrucao, sOp, inicioEnderecoMemoria);
            separaEnderecoDaInstrucao(segundaInstrucao, sEnderecoDeMemoriaEmHexa, inicioEnderecoMemoria);
            sEnderecoDeMemoria = converteHexaParaInt(sEnderecoDeMemoriaEmHexa);
        }else{
            int inicioEnderecoMemoria = strlen(segundaInstrucao) - 1 ;
            printf("%i\n", inicioEnderecoMemoria);
            separaOperacaoDaInstrucao(segundaInstrucao, sOp, inicioEnderecoMemoria);
        }
        printf("Primeira Instrucao:\n");
        printf("%s\n", pOp);
        printf("%i\n", pEnderecoDeMemoria);
        printf("Segunda Instrucao: \n");
        printf("%s\n", sOp);
        printf("%i\n", sEnderecoDeMemoria);
        imprimeEmBinario(sEnderecoDeMemoria);
    }
    else if(dataOrInstruction == 'd'){

        unsigned short int valorASerGuardado;

        valorASerGuardado = separaValorASerGuardado(valorDeEntrada, inicioProximaInstrucao);

        printf("Valor a ser guardado: %i", valorASerGuardado);
    }else{
        executaLeituraDosDadosEGuardaNaMemoria();
    }
}

int main()
{
    char memoria[154];
    for(int i = 0; i < 154; i++){
        memoria[i] = 2;
    }
    unsigned short int mbr, mar, ibr, imm, pc = 0;
    unsigned char ir;
    unsigned char equal, lower, greater;
    unsigned char lFlag, rFlag;
    unsigned short int aluA, aluB, aluT = 0;

    char conjuntoDeIntrucoes[32][6];
    strcpy(conjuntoDeIntrucoes[0], "hlt");strcpy(conjuntoDeIntrucoes[1], "nop");strcpy(conjuntoDeIntrucoes[2], "add");strcpy(conjuntoDeIntrucoes[3], "sub");
    strcpy(conjuntoDeIntrucoes[4], "mul");strcpy(conjuntoDeIntrucoes[5], "div");strcpy(conjuntoDeIntrucoes[6], "cmp");strcpy(conjuntoDeIntrucoes[7], "xchg");
    strcpy(conjuntoDeIntrucoes[8], "nad");strcpy(conjuntoDeIntrucoes[9], "or");strcpy(conjuntoDeIntrucoes[10], "xor");strcpy(conjuntoDeIntrucoes[11], "not");
    strcpy(conjuntoDeIntrucoes[12], "je");strcpy(conjuntoDeIntrucoes[13], "jne");strcpy(conjuntoDeIntrucoes[14], "jl");strcpy(conjuntoDeIntrucoes[15], "jle");
    strcpy(conjuntoDeIntrucoes[16], "jg");strcpy(conjuntoDeIntrucoes[17], "jge");strcpy(conjuntoDeIntrucoes[18], "jmp");strcpy(conjuntoDeIntrucoes[19], "lda");
    strcpy(conjuntoDeIntrucoes[20], "ldb");strcpy(conjuntoDeIntrucoes[21], "sta");strcpy(conjuntoDeIntrucoes[22], "stb");strcpy(conjuntoDeIntrucoes[23], "ldrb");
    strcpy(conjuntoDeIntrucoes[24], "movial");strcpy(conjuntoDeIntrucoes[25], "moviah");strcpy(conjuntoDeIntrucoes[26], "addia");strcpy(conjuntoDeIntrucoes[27], "subia");
    strcpy(conjuntoDeIntrucoes[28], "mulia");strcpy(conjuntoDeIntrucoes[29], "divia");strcpy(conjuntoDeIntrucoes[30], "lsh");strcpy(conjuntoDeIntrucoes[31], "rsh");

    executaLeituraDosDadosEGuardaNaMemoria();

    for(int i = 0; i < 154; i++){
        if(memoria[i] =! '\0'){
            memoria[i] = '\0';
        }
    }
    for(int i = 0; i < 154;i++){

        printf("|%c|", memoria[i]);
    }

    return 0;
}
