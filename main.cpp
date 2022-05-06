#include <iostream>
#include <fstream>
#include <string>
#include <windows.data.json.h>
#include "json\json.h"
 
using namespace std;
using namespace Json;
 
class Materia
{
private:
    string nomeMateria;
    string horarioMateria;
    int idMateria;
    int maxVagas;
    int alunosVagas;
    int preRequisitos[21];
 
public:
 
    Materia(string nome = "", int id = NULL, string horario = "", int vagas = 5, int alunos = 0)
    {
        nomeMateria = nome;
        idMateria = id;
        horarioMateria = horario;
        maxVagas = vagas;
        alunosVagas = alunos;
    }
 
    //Setters
    void setPreReq(int req[], int numReq)   // Recebe o array de pre-requisitos direto do json e o número de requisitos
    {
        for (int i = 0; i < 21; i++)        // Percorre todo o array de pre requisitos da classe.
        {
            if (i < numReq)                 // Ex: Grafos tem 2 prerequisitos, ent ele vai encher o array da classe até a posição 1 (0 e 1).
            {
                preRequisitos[i] = req[i];
            }
            else                            // Dps disso, preenche o resto de -1. Ex de grafos: [362, 364, -1, -1 , -1, -1, ...]
            {
                preRequisitos[i] = -1;
            }
        }
    }
 
    // Getters
    string getNome() {                  // Usei os gets o valor na hora de printar, mas tbm vai servir pra checagem.
        return nomeMateria;
    }
    int getId() {
        return idMateria;
    }
    string getHorario() {
        return horarioMateria;
    }
    int getMaxVagas() {
        return maxVagas;
    }
    int getVagas() {
        return alunosVagas;
    }
};
 
class NoMateria
{
private:
    Materia elementoMateria;
    NoMateria* proximo;
 
public:
 
    NoMateria(Materia elem)
    {
        elementoMateria = elem;
        proximo = NULL;
    }
    // Getters
    Materia getElem() {
        return elementoMateria;
    }
    NoMateria* getProx() {
        return proximo;
    }
    // Setters
    void setProx(NoMateria* novo) {
        proximo = novo;
    }
};
 
class ListaMateria
{
private:
    NoMateria* cabeca;
 
public:
 
    ListaMateria()
    {
        cabeca = NULL;
    }
    ListaMateria(Materia elem)              // Talvez a gente n precise desse constructor.
    {
        cabeca = new NoMateria(elem);
    }
    virtual ~ListaMateria()                 // Destructor
    {
        delete cabeca;
    }
 
    bool checaVazia()                       // Acho que a gente tbm n precisa dessa função pra essa classe.
    {
        return (cabeca == NULL);
    }
 
    void printarLista()
    {
        NoMateria* atual = cabeca;
        cout << "\nExibindo todas as matérias...\n";
 
        if (checaVazia())                   // não precisa...
        {
            cout << "A lista nao possui elementos.\n";
        }
        else
        {
            int i = 1;
            while (atual != NULL)
            {
                cout << "Materia " << i << endl;
                cout << "\tNome: " << atual->getElem().getNome() << endl;
                cout << "\tId: COMP" << atual->getElem().getId() << endl;
                cout << "\tHorario: " << atual->getElem().getHorario() << endl;
                cout << "\tVagas: [" << atual->getElem().getVagas() << "/" << atual->getElem().getMaxVagas() << "]\n";
                atual = atual->getProx();
                i++;
            }
            cout << endl;
        }
    }
 
    void insereInicio(Materia materia)
    {
        NoMateria* novo = new NoMateria(materia);
 
        novo->setProx(cabeca);
        cabeca = novo;
    }
 
    void insereFinal(Materia novaMateria)
    {
        NoMateria* noAtual, * novoNo;
 
        if (checaVazia())
            return insereInicio(novaMateria);
 
        noAtual = cabeca;
        while (noAtual->getProx() != NULL)
            noAtual = noAtual->getProx();
 
        novoNo = new NoMateria(novaMateria);
        noAtual->setProx(novoNo);
        novoNo->setProx(NULL);
    }
 
    bool checaMateria(int id)               // Vamo precisar pra a lista de matérias pagas.
    {
        NoMateria* atual = cabeca;
 
        while (atual)
        {
            if (atual->getElem().getId() == id) {
                return true;
            }
            else {
                atual = atual->getProx();
            }
        }
    }
};
 
class Aluno
{
private:
    string nomeAluno;
    int idAluno;
    int fluxoAluno;
    int numMaterias;
public: 
    Aluno(string nome = "", int id = NULL, int fluxo = NULL, int num = 0)
    {
        nomeAluno = nome;
        idAluno = id;
        fluxoAluno = fluxo;
        numMaterias = num;
    }
 
    // Setters
    void setNumMaterias(int num) {
        numMaterias = num;
    }
 
    // Getters
    string getNome() {
        return nomeAluno;
    }
    int getId() {
        return idAluno;
    }
    int getFluxo() {
        return fluxoAluno;
    }
    int getNum() {
        return numMaterias;
    }
};
 
class NoAluno
{
private:
    Aluno elementoAluno;
    NoAluno* proximo;
public:
    NoAluno(Aluno elem)
    {
        elementoAluno = elem;
        proximo = NULL;
    }
 
    Aluno getElem() {
        return elementoAluno;
    }
    NoAluno* getProx() {
        return proximo;
    }
    void setProx(NoAluno* prox) {
        proximo = prox;
    }
};
 
class ListaAluno 
{
private:
    NoAluno* cabeca;
public:
    ListaAluno() {
        cabeca = NULL;
    }
 
    void inserir(Aluno novoAluno) {
        NoAluno* novo = new NoAluno(novoAluno);
        cabeca = novo;
    }
 
    void inserirAlunos(Aluno aluno) {           // Ele precisa inserir ordenadamente, em ordem de fluxo.
 
        NoAluno* novo = new NoAluno(aluno);
        NoAluno* atual = cabeca;
        //NoAluno* anterior = NULL;             Adicionei esse ponteiro agora.
 
        if (cabeca == NULL) {
            cabeca = novo;
            return;
        }
 
        int fluxoNovo = novo->getElem().getFluxo();
        int fluxoAtual = atual->getElem().getFluxo();
 
        while (true)        // É necessário inserir os elementos antes do checado.
        {
            if (fluxoNovo <= fluxoAtual)
            {
                novo->setProx(atual->getProx());
                atual->setProx(novo);
                return;
            }
            if (atual->getProx() == NULL) {
                break;
            }
 
            atual = atual->getProx();
            fluxoAtual = atual->getElem().getFluxo();
        }
 
        novo->setProx(atual->getProx());
        atual->setProx(novo);
    }
 
    void printarAlunos() 
    {
        NoAluno* atual = cabeca;
 
        cout << "Exibindo todos os alunos...\n";
 
        int i = 1;
        while (true) 
        {
            cout << "Aluno " << i << endl;
            cout << "\tNome: " << atual->getElem().getNome() << endl;
            cout << "\tId: " << atual->getElem().getId() << endl;
            cout << "\tFluxo: " << atual->getElem().getFluxo() << endl;
            cout << "\tMatriculado em: " << atual->getElem().getNum() << " materias.\n";
 
            if (atual->getProx() == NULL)
            {
                cout << "I = " << i << endl;
                break;
            }
 
            atual = atual->getProx();
            i++;
        }
    }
};
 
int main()
{
    ListaMateria materias;
 
    ifstream file("materias.json");
 
    Value json_archive;
    Reader reader;
 
    reader.parse(file, json_archive);
 
    for (int i = 359; i < 364; i++)
    {
        string id = to_string(i);
        const Value element = json_archive[id];
 
        string nome = element[0].asString();
        string horario = element[1].asString();
        int alunosMax = element[2].asInt();
        int alunosAtual = element[3].asInt();
 
        Materia nova_materia(nome, i, horario, alunosMax, alunosAtual);
        materias.insereFinal(nova_materia);
    };
 
    materias.printarLista();
 
    Aluno athus("Athus Henrique Marinho", 2501, 1, 0);
    Aluno eduardo("Eduardo Henrique Marques", 2502, 2, 0);
    Aluno fabio("Fabio Henrique Rosset", 2503, 1, 0);
    Aluno luis("Luis Henrique Vieira", 2504, 0, 0);
    ListaAluno alunos;
 
    alunos.inserirAlunos(athus);
    alunos.inserirAlunos(eduardo);
    alunos.inserirAlunos(fabio);
    alunos.inserirAlunos(luis);
    alunos.printarAlunos();
 
    cout << "Aperte qualquer botao para fechar o programa...\n";
    system("pause");
 
    return 0;
}