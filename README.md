# Especificações Técnicas: 
- É necessário que inserções, alterações e buscas no cadastro de acidentados atendidos sejam muito eficientes
- Não é aceito operações lineares
- Tempo de resposta rápido
- Uso estrito de memória e armazenamento em disco

# Especificações do Problema: 
- Ao paciente dar entrada, será feita uma análise e classificação da prioridade do caso
- Atendimento com prioridade: Caso mais de um paciente estejam na mesma categoria, adota-se a ordem de chegada na emergência
    1) Emergência: caso gravíssimo com risco de morte; 
    2) Muito urgente: grave e risco de evoluir para morte; 
    3) Urgente: gravidade moderada e necessidade de atendimento médico, sem risco imediato; 
    4) Pouco Urgente: poderia ser atendido numa Unidade Básica; 
    5) Não Urgência: risco algum (resfriados, espinho no pé, etc)
- Interface: 
    1. Registrar paciente
    2. Remover paciente
    3. Listar pacientes
    4. Buscar paciente por ID
    5. Mostrar fila de espera
    6. Dar alta ao paciente.
    7. Sair

# Tipos Abstratos de Dados
## TAD | Paciente (AVL)
### Tipo de dado Paciente:
- Nome: tipo string
- Id (chave única de identificação): único, tipo int (string não é interessante, vamos ter que tratar nomes repetidos)

### Operações com paciente:
- Registrar: Caso seja a primeira ocorrência
- Remover: Deve ser possível só se o paciente não está na fila, 
- Listar: Mostrar todos os pacientes no hospital
- Buscar paciente por ID

## TAD | Fila de Prioridade
Nós escolhemos implementar a fila com ordem de prioridade através de uma Heap mínima sobre um Arranjo dinâmico. Tivemos um dilema ao pensar nessa implementação para equilibrar a economia de memória, eficiência computacional e tempo de resposta rápido, mas acreditamos que com as seguintes características conseguimos equilibrar:
- Com desempate de ordem de chegada quando as prioridades empatam;
- Sem operações lineares: encontrar/remover o elemento de maior prioridade com heap é O(log n);
- Arranjo dinâmico: O volume de pacientes é variável (picos de atendimento), usar vetor estático desperdiçaria memória. O uso de realloc com crescimento exponencial permite adaptação eficiente;
- Uso da struct ITEM em vez do nó NO: a heap armazena somente os dados necessários para priorização, evitando duplicar todo o conteúdo do paciente na fila e mantendo a separação entre as estruturas;
- Heap mínima: foi escolhida porque a menor chave representa a maior prioridade de atendimento, permitindo acesso direto ao próximo paciente a ser chamado na raiz da estrutura.
### Operações na fila:
- Colocar na fila: Verificação de cadastro prévio
- Mostrar fila: Listar paciente da maior para a menor prioridade
- Dar alta ao paciente: O paciente foi atendido e deixou o serviço de emergência. Ele permanece nos registros do hospital.

## TAD | Persistência dos Dados (IO - Input/Output)
- Ao sair do sistema, tudo deve ser armazenado uma única vez (lista de paciente, a fila de pacientes e os históricos de atendimento)
- Ao entrar no sistema, tudo deve ser recarregado
- Cada TAD além desse deve ter funções utilitárias do tipo save() e load()