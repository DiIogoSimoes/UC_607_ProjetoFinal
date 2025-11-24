⚙️ README do Projeto Final - RH Avançado (UC 607)
Este documento apresenta o projeto RH Avançado, uma aplicação de console desenvolvida em C++ para a Unidade Curricular 607, focada na gestão de recursos humanos, controle de assiduidade, férias e desenvolvimento profissional de colaboradores.


🚀 Visão Geral do Projeto
RH Avançado é um sistema completo de gestão de colaboradores, projetado para facilitar o registo, marcação e análise das ausências (férias e faltas) numa empresa. O sistema inclui funcionalidades de gestão de dados pessoais, cursos/formações, observações, e geração de diversos relatórios e dashboards.

💻 Tecnologia
Linguagem de Programação: C++ (Console Application).

Ambiente de Desenvolvimento: Visual Studio 2026 (Utilizado para desenvolvimento e compilação do projeto).
Persistência de Dados: Os dados são guardados e carregados localmente num ficheiro (data_rh.txt).
Segurança: Nomes e dados sensíveis dos colaboradores e cursos são cifrados (Cifra de César) antes de serem guardados no ficheiro, garantindo uma camada básica de segurança dos dados.

✨ Funcionalidades Principais
O projeto oferece um menu principal robusto com as seguintes opções:

1. Gestão de Colaboradores
Listar Colaboradores: Exibe todos os colaboradores com respetivo ID, nome e departamento.
Adicionar Colaborador: Permite adicionar novos colaboradores com verificação de nomes duplicados.
Buscar Colaborador: Procura um colaborador por ID ou nome (parcial) e exibe os seus detalhes e o calendário do mês atual.

2. Gestão de Marcações (Férias e Faltas)
Marcar Férias ('F') e Faltas ('X'): Permite registar ausências por dia com validação da data (formato YYYY-MM-DD).
Verificação de Conflito: Ao marcar férias, o sistema avisa se já existir outro colega do mesmo departamento de férias nessa data, permitindo ao utilizador confirmar ou cancelar a marcação.
Não é permitido marcar em Fins de Semana.
Dismarcar: Remove uma marcação existente.
Visualizar Calendário: Imprime o calendário mensal de um colaborador com as marcações visíveis (F=Férias, X=Falta, *=Fim de Semana).

3. Gestão de Recursos
Gerir Formações/Cursos: Interface dedicada para adicionar, listar, editar e remover cursos concluídos por um colaborador.
Gerir Notas/Observações: Interface para adicionar, listar, editar e remover notas internas ou observações sobre o colaborador.

4. Relatórios e Estatísticas
Relatórios Mensais: Gera uma lista detalhada das Férias e Faltas de cada colaborador para um mês específico.
Estatísticas por Departamento: Agrega o total de Férias e Faltas por departamento para o mês, e identifica o departamento com o maior número de ausências.
Dashboard Resumido: Apresenta um resumo anual/mensal por colaborador, mostrando as Férias tiradas no ano, as Faltas no mês e o saldo de Dias de Férias Restantes (com base num limite anual configurável).

5. Exportação de Dados
Exportar Relatório (TXT/CSV): Permite exportar os mapas de marcações para um ficheiro de texto, ideal para ser aberto em programas como o Bloco de Notas ou Excel (como CSV).
Exportação de calendário de um Colaborador (mensal).
Exportação do mapa de marcações de um Departamento (mensal) ou de Todos os departamentos.
