#include <iostream>
#include <fstream>
#include <string>
// #include <windows.data.json.h>
// #include "json\json.h"

using namespace std;
// using namespace Json;

const int MIN_MATERIAS = 4;
const int MAX_MATERIAS = 8;
const int TOTAL_MATERIAS = 23;

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

        int i;
        for (i = 0; i < 21; i++)
        {
            preRequisitos[i] = -1;
        }
    }

    void printPreReq()
    {
        int i;
        cout << "Pre-requisitos de " << nomeMateria << ":\n[";
        for (i = 0; i < TOTAL_MATERIAS; i++)
        {
            if (preRequisitos[i] == -1) break;
            cout << preRequisitos[i] << " ";
        }
        cout << "]\n";
    }

    // Setters
    void setPreReq(int req[], int numReq) // Recebe o array de pre-requisitos direto do json e o número de requisitos
    {
        for (int i = 0; i < 21; i++) // Percorre todo o array de pre requisitos da classe.
        {
            if (i < numReq) // Ex: Grafos tem 2 prerequisitos, ent ele vai encher o array da classe até a posição 1 (0 e 1).
            {
                preRequisitos[i] = req[i];
            }
            else // Dps disso, preenche o resto de -1. Ex de grafos: [362, 364, -1, -1 , -1, -1, ...]
            {
                preRequisitos[i] = -1;
            }
        }
    }

    void setVagas(int vagas)    // Nao precisa utilizar mais essa
    {
        alunosVagas = vagas;
    }
    void addAluno()
    {
        alunosVagas++;
    }
    void removeAluno()
    {
        alunosVagas--;
    }
    void setNome(string novoNome)
    {
         nomeMateria = novoNome;
    }

    // Getters
    string getNome()
    {
        return nomeMateria;
    }
    int getId()
    {
        return idMateria;
    }
    string getHorario()
    {
        return horarioMateria;
    }
    int getMaxVagas()
    {
        return maxVagas;
    }
    int getVagas()
    {
        return alunosVagas;
    }
    int getPreReq(int i)
    {
        return preRequisitos[i];
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
    Materia getElem()
    {
        return elementoMateria;
    }
    Materia* getElemPointer()
    {
        return &elementoMateria;
    }
    NoMateria* getProx()
    {
        return proximo;
    }
    // Setters
    void setProx(NoMateria* novo)
    {
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
    ListaMateria(Materia elem) // Talvez a gente n precise desse constructor.
    {
        cabeca = new NoMateria(elem);
    }
    virtual ~ListaMateria() // Destructor
    {
        delete cabeca;
    }

    bool checaVazia() // Acho que a gente tbm n precisa dessa função pra essa classe.
    {
        return (cabeca == NULL);
    }

    NoMateria* getCabeca() 
    {
        return cabeca;
    }

    void printarLista()
    {
        NoMateria* atual = cabeca;

        if (checaVazia()) // não precisa...
        {
            cout << "A lista nao possui elementos.\n";
        }
        else
        {
            int i = 1;
            while (atual != NULL)
            {
                cout << "Materia " << i << ":\n";
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
        NoMateria *noAtual, *novoNo;

        if (checaVazia())
            return insereInicio(novaMateria);

        noAtual = cabeca;
        while (noAtual->getProx() != NULL)
            noAtual = noAtual->getProx();

        novoNo = new NoMateria(novaMateria);
        noAtual->setProx(novoNo);
        novoNo->setProx(NULL);
    }

    bool checaMateria(int id) // Vamo precisar pra a lista de matérias pagas.
    {
        NoMateria* atual = cabeca;

        while (atual)
        {
            if (atual->getElem().getId() == id)
            {
                return true;
            }
            else
            {
                atual = atual->getProx();
            }
        }
    }

    Materia getMateria(int id) // Retorna um ponteiro da matéria escolhida por id.
    {
        NoMateria* atual = cabeca;

        while (atual != NULL)
        {
            if (atual->getElem().getId() == id)
                return atual->getElem();

            atual = atual->getProx();
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
    int Materias[MAX_MATERIAS];

    int materiasPagas[TOTAL_MATERIAS];

public:
    Aluno(string nome = "", int id = NULL, int fluxo = NULL, int num = 0)
    {
        nomeAluno = nome;
        idAluno = id;
        fluxoAluno = fluxo;
        numMaterias = num;
        for (int i = 0; i < MAX_MATERIAS; i++)
            Materias[i] = -1;

        for (int i = 0; i < TOTAL_MATERIAS; i++)
            materiasPagas[i] = -1;

        //for ()
    }

    bool addMateria(int id)
    {
        for (int i = 0; i < MAX_MATERIAS; i++)
        {
            if (Materias[i] == -1)
            {
                Materias[i] = id;
                return true;
            }
        }
        return false;
    }

    void addMateriaPaga(int id)
    {
        for (int i = 0; i < TOTAL_MATERIAS; i++)
        {
            if (id == -1) break;
            if (materiasPagas[i] == -1) materiasPagas[i] = id;
        }
    }

    bool checaMateriaPaga(int id)
    {
        int i;

        for (i = 0; i < MAX_MATERIAS; i++)
        {
            if (materiasPagas[i] == id) return true;
        }
        return false;
    }

    bool removeMateria(int id)		// Adicionada
    {
        for (int i = 0; i < MAX_MATERIAS; i++)
        {
            if (Materias[i] == id)
            {
                Materias[i] = -1;
                return true;
            }
        }
        return false;
    }

    void printaMaterias()
    {
        int i;
        cout << "[";
        for (i = 0; i < numMaterias; i++)
        {
            if (i == numMaterias - 1)
            {
                cout << Materias[i] << "]\n\n";
                break;
            }
            cout << Materias[i] << ", ";
        }
    }

    bool checaMateriaAluno(int id) 		// Modificada
    {
        int i;
        for (i = 0; i < MAX_MATERIAS; i++)
        {
            if (Materias[i] == id) return true;
        }
        return false;
    }

    // Setters
    void setNumMaterias(int num)
    {
        numMaterias = num;
    }

    // Getters
    string getNomeAluno()
    {
        return nomeAluno;
    }
    int getId()
    {
        return idAluno;
    }
    int getFluxo()
    {
        return fluxoAluno;
    }
    int getNum()
    {
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

    Aluno getElem()
    {
        return elementoAluno;
    }
    NoAluno* getProx()
    {
        return proximo;
    }
    void setProx(NoAluno* prox)
    {
        proximo = prox;
    }
};

class ListaAluno
{
private:
    NoAluno* cabeca;

public:
    ListaAluno()
    {
        cabeca = NULL;
    }

    void inserirAlunos(Aluno aluno)
    {

        NoAluno* novo = new NoAluno(aluno);
        NoAluno* atual = cabeca;

        if (cabeca == NULL)
        {
            cabeca = novo;
            return;
        }

        while (true)
        {
            if (atual->getProx() == NULL)
            {
                novo->setProx(NULL);
                atual->setProx(novo);
                return;
            }
            atual = atual->getProx();
        }
    }

    void printarAlunos()
    {
        NoAluno* atual = cabeca;

        cout << "Exibindo todos os alunos...\n";

        int i = 1;
        while (true)
        {
            cout << "Aluno " << i << endl;
            cout << "\tNome: " << atual->getElem().getNomeAluno() << endl;
            cout << "\tId: " << atual->getElem().getId() << endl;
            cout << "\tFluxo: " << atual->getElem().getFluxo() << endl;
            cout << "\tMatriculado em: " << atual->getElem().getNum() << " materias.\n";

            if (atual->getProx() == NULL)
            {
                break;
            }

            atual = atual->getProx();
            i++;
        }
    }

    bool checaAluno(int id) // Vamo precisar pra a lista de matérias pagas.
    {
        NoAluno* atual = cabeca;

        while (atual)
        {
            if (atual->getElem().getId() == id)
            {
                return true;
            }
            else
            {
                atual = atual->getProx();
            }
        }
    }

    Aluno procuraAluno(int id)
    {
        NoAluno* atual = cabeca;

        while (atual)
        {
            if (atual->getElem().getId() == id)
            {
                return atual->getElem();
            }
            else
            {
                atual = atual->getProx();
            }
        }
    }
};

void inicializarAlunos(ListaAluno* l, int fluxos[4])
{
    Aluno luis("Luis Henrique Vieira", 2504, 0, 0);
    Aluno athus("Athus Henrique Marinho", 2501, 0, 0);
    Aluno fabio("Fabio Henrique Rosset", 2503, 1, 0);
    Aluno eduardo("Eduardo Henrique Marques", 2502, 2, 0);

    luis.addMateriaPaga(359);

    l->inserirAlunos(luis);
    fluxos[luis.getFluxo()] += 1;
    l->inserirAlunos(athus);
    fluxos[athus.getFluxo()] += 1;
    l->inserirAlunos(fabio);
    fluxos[fabio.getFluxo()] += 1;
    l->inserirAlunos(eduardo);
    fluxos[eduardo.getFluxo()] += 1;
}

bool checarPrioridade(int alunos_matriculados[4], int fluxo)
{
    if (alunos_matriculados[fluxo - 1] == 0 || fluxo == 0)
    {
        alunos_matriculados[fluxo] -= 1;
        return true;
    }
    else return false;
}

void addVaga(int id, ListaMateria* materias)
{
    NoMateria* atual = materias->getCabeca();

    while (atual != NULL)
    {
        if (atual->getElem().getId() == id)
        {
            atual->getElemPointer()->addAluno();
            return;
        }

        atual = atual->getProx();
    }
}

void removeVaga(int id, ListaMateria* materias)
{
    NoMateria* atual = materias->getCabeca();

    while (atual != NULL)
    {
        if (atual->getElem().getId() == id)
        {
            atual->getElemPointer()->removeAluno();
            return;
        }

        atual = atual->getProx();
    }
}

int main()
{
    Materia p1("Programacao 1", 359, "7:00", 20, 20);
    Materia logica("Logica para Computacao", 360, "11:00", 40, 37);
    Materia cse("Computacao, sociedade e etica", 361, "7:30", 30, 24);
    Materia md("Matematica discreta", 362, "11:00", 40, 37);
    Materia calculo("Calculo diferencial e integral", 363, "12:00", 80, 59);
    Materia estrutura("Estrutura de Dados", 364, "7:00", 20, 10);

    int aux[1] = {359};
    estrutura.setPreReq(aux, 1);

    // estrutura.printPreReq();

    ListaMateria materias;
    materias.insereFinal(p1);
    materias.insereFinal(logica);
    materias.insereFinal(cse);
    materias.insereFinal(md);
    materias.insereFinal(calculo);
    materias.insereFinal(estrutura);

    ListaAluno alunos;
    int alunosMatriculados[4];

    for (int i = 0; i < 4; i++)
        alunosMatriculados[i] = 0;

    inicializarAlunos(&alunos, alunosMatriculados);

    alunos.printarAlunos();

    // Função logar e fazer matricula:

    int userId;
    Aluno user;

    while (true)
    {
        cout << "Digite o usuario: ";
        cin >> userId;

        int validez = alunos.checaAluno(userId);

        if (validez)
        {
            user = alunos.procuraAluno(userId);

            if (!checarPrioridade(alunosMatriculados, user.getFluxo()))
            {
                cout << "\nSua matricula ainda nao e elegivel. Por favor, tente novamente mais tarde.\n";
                continue;
            }

            cout << "Bem-vindo, " << user.getNomeAluno() << ". Vamos fazer sua matricula.\n";
            break;
        }
        else
        {
            cout << "Usuario invalido. Digite novamente.\n";
        }
    }

    while (true)
    {
        if (!checarPrioridade(alunosMatriculados, user.getFluxo()))
            break;

        int op;
        int numMaterias = user.getNum();

        cout << "Por enquanto, voce escolheu " << numMaterias << " materias. O que deseja fazer?\n";
        cout << "[1] - Adicionar materia\n";
        cout << "[2] - Remover materia\n";
        cout << "[3] - Exibir materias escolhidas\n";
        cout << "[4] - Exibir todas materias \n";
        cout << "[0] - Concluir matricula\n";

        cin >> op;

        if (op == 1)
        {
            while (true)
            {
                if (numMaterias >= MAX_MATERIAS)
                {
                    cout << "Voce ja escolheu o maximo de materias possivel.\n";
                    break;
                }
                else
                {
                    int idMateria;

                    while (true)
                    {
                        cout << "Digite o id da materia que voce deseja se matricular: ";
                        cin >> idMateria;

                        int validez = materias.checaMateria(idMateria);

                        if (!validez)
                        {
                            cout << "Materia nao encontrada.\n";
                        }
                        else if (user.checaMateriaAluno(idMateria) == true)
                        {
                            cout << "Voce ja escolheu essa materia. Escolha outra.\n";
                        }
                        else
                        {
                            break;
                        }
                    }

                    Materia materiaChecada = materias.getMateria(idMateria);

                    if (materiaChecada.getMaxVagas() == materiaChecada.getVagas())
                    {
                        cout << "Essa materia ja esta com as vagas preenchidas.\n";
                        break;
                    }

                    int check = 0;

                    while(true)
                    {
                        int i;
                        int j;

                        for (i = 0; i < 21; i++)
                        {
                            j = materiaChecada.getPreReq(i);
                            if (j == -1)
                            {
                                check = 1;
                                break;
                            }
                            if (user.checaMateriaPaga(j) == false)
                            {
                                cout << "Voce nao pagou uma ou mais das materias de pre-requisito.\n";
                                break;
                            }
                        }
                        break;
                    }

                    if (check == 0) break;

                    numMaterias += 1;
                    user.setNumMaterias(numMaterias);
                    user.addMateria(idMateria);

                    cout << "Materia adicionada: " << materiaChecada.getNome() << endl;
                    addVaga(idMateria, &materias);

                    break;
                }
            }
        }
        else if (op == 2)
        {
            if (numMaterias <= 0)
            {
                cout << "Voce nao possui nenhuma materia para remover.\n";
            }
            else
            {
                int idMateria;

                while (true)
                {
                    cout << "Digite o id da materia que voce deseja remover da lista: ";
                    cin >> idMateria;

                    int validez = materias.checaMateria(idMateria);

                    if (!validez)
                    {
                        cout << "Essa materia nao existe.\n";
                    }
                    else if (user.checaMateriaAluno(idMateria) == false)
                    {
                        cout << "Voce nao adicionou essa materia na lista.\n";
                    }
                    else
                    {
                        break;
                    }
                }

                Materia materiaChecada = materias.getMateria(idMateria);

                numMaterias -= 1;
                user.setNumMaterias(numMaterias);
                user.removeMateria(idMateria);

                cout << "Materia removida: " << materiaChecada.getNome() << endl;
                removeVaga(idMateria, &materias);
            }
        }
        else if (op == 3)
        {
            if (numMaterias <= 0)
            {
                cout << "Voce ainda nao escolheu nenhuma materia.\n";
            }
            else 
            {
                int i;
                cout << "\nPrintando materias escolhidas: \n";
                user.printaMaterias();
            }
        }
        else if (op == 4)
        {
            cout << "Printando todas as materias: \n";
            materias.printarLista();
        }
        else if (op == 0)
        {
            if (numMaterias < MIN_MATERIAS) {
                cout << "Voce precisa escolher no minimo 4 materias para concluir a matricula.\n";
            }
            else {
                cout << "Salvando.\n";
                break;
            }
        }
    }
    cout << "Aperte qualquer botao para fechar o programa...\n";
    // system("pause");

    /*  Os Henriques:
            Luis Henrique Vieira
            Athus Henrique Marinho
            Fabio Henrique Rosset
            Eduardo Henrique Marques
    */
    return 0;
}