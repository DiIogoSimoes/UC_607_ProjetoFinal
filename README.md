# RH AVANÇADO - Gestão de Recursos Humanos Diogo Simões (UC 607)

> Uma aplicação de console em C++ focada na gestão eficiente de colaboradores, marcação de ausências (Férias/Faltas), e análise de dados para o Projeto Final da Unidade Curricular 607.

---

## Detalhes Técnicos

| Componente | Detalhe |
| :--- | :--- |
| **Linguagem** | C++ |
| **IDE** | **Visual Studio 2026** |
| **Tipo de Aplicação** | Console |
| **Persistência** | Ficheiro Local (`data_rh.txt`) |
| **Segurança** | Cifragem de Dados Sensíveis (Cifra de César) |

---

## Funcionalidades Principais

O sistema oferece um menu principal robusto para diversas tarefas administrativas.

### 1. Gestão de Colaboradores

* **Listagem Completa:** Exibe todos os colaboradores (ID, Nome, Departamento).
* **Registo:** Adiciona novos colaboradores com verificação de nomes duplicados.
* **Pesquisa Detalhada:** Busca por ID ou nome e mostra todos os detalhes e o calendário mensal.

###  2. Marcações de Ausência

Controlo rigoroso da assiduidade, impedindo marcações em Fins de Semana.

* **Marcar Férias ('F'):** Regista dias de férias.
    > **Destaque:** Inclui verificação de **conflito de departamento**, alertando se houver colegas de férias no mesmo dia.
* **Marcar Faltas ('X'):** Registo de ausências.
* **Desmarcar:** Remove uma marcação existente.
* **Visualização:** Imprime o calendário mensal de um colaborador com todas as marcações.

### 3. Recursos & Desenvolvimento

Gerencie o perfil profissional e observações dos colaboradores.

* **Formações/Cursos:** Adicionar, editar e listar cursos concluídos.
* **Notas/Observações:** Adicionar e gerir notas internas e observações sobre o colaborador.

### 4. Relatórios e Análise de Dados

Extraia informações vitais para a tomada de decisões.

* **Relatórios Mensais:** Detalhes de Férias e Faltas para um mês/ano específico.
* **Estatísticas de Departamento:** Agrega os dados por departamento e identifica áreas com maior número de ausências.
* **Dashboard Resumido:** Apresenta o saldo de **Dias de Férias Restantes** e estatísticas anuais/mensais.

### 5. Exportação de Dados

* **Exportar Relatório (TXT/CSV):** Permite exportar mapas de marcações (individual ou por departamento) para análise externa.

---

## Como Colocar o Projeto a Correr

### Pré-requisitos

* **IDE:** **Visual Studio 2026** instalado.
* Compilador C++ compatível (MSVC).

### ⚙️ Compilação e Execução

1.  **Abrir:** Carregue a solução do projeto (`.sln`) no Visual Studio 2026.
2.  **Compilar:** Utilize a opção `Build` ou atalho `F7`.
3.  **Executar:** Inicie o projeto (`Debug > Start Debugging` ou `F5`).

> O sistema carrega os dados automaticamente do ficheiro `data_rh.txt` no arranque e guarda-os ao sair (Opção **0**).
