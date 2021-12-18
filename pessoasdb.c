// pessoasdb.c - Manages a local database of people names and ages.
// This is an interactive program that don't take argument.
// The database is kept in the local file pessoas.db
// If the file pessoas.db has permissions 2** then it is emptied.
// Written by: Pedro Izecksohn on 18-December-2021 18:01
// License: 1) This program is free to be distributed in source code.
//          2) No warranty is given.

#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
 
#define NOMELEN 32
 
typedef struct Pessoa
{
  char nome [NOMELEN];
  unsigned short int idade;
  struct Pessoa *prox;
} Pessoa; 
  
int main()
{
  FILE *file;
  if ((file=fopen("pessoas.db", "rb"))==NULL)
  {
    if ((file=fopen ("pessoas.db", "wb"))==NULL)
    {
      fprintf (stderr, "O arquivo pessoas.db não pode ser lido nem escrito.\n");
      return EXIT_FAILURE;
    }
    fclose (file);
  }
  else
  {
    fclose (file);
  }
  Pessoa *ll = NULL;
  file = fopen("pessoas.db","rb");
  if (file == NULL)
  {
   fprintf (stderr, "O arquivo pessoas.db não pode ser aberto.\n");
    return EXIT_FAILURE;
  }
  while (!(ferror(file)||(feof(file))))
  {
    Pessoa *nova = malloc (sizeof (Pessoa));
    if (NULL == nova)
    {
      fprintf (stderr, "malloc retornou NULL");
      return EXIT_FAILURE;
    }
    if (fgets (nova->nome,NOMELEN,file) == NULL)
    {
      if (ferror (file))
      {
        fprintf (stderr, "Um erro ocorreu enquanto fgets lia o arquivo pessoas.db\n");
        return EXIT_FAILURE;
      }
      free (nova);
      break;
    }
    char *pc = index (nova->nome ,'\n');
    if (pc!=NULL)
    {
      *pc = 0;
    }
    if (fscanf (file,"%hu",&nova->idade) != 1)
    {
      if (feof(file)&&(0==nova->nome[0]))
      {
        free (nova);
        break;
      }
      fprintf (stderr, "O arquivo pessoas.db está corrompido.\nfscanf falhou em ler a idade de %s.\n",nova->nome);
      fprintf (stderr, "nova->nome[0] = %u\nfeof(file) = %u\n", nova->nome[0], feof(file));
      return EXIT_FAILURE;
    }
    int C = fgetc(file);
    if (C != '\n')
    {
      fprintf (stderr, "O arquivo pessoas.db está corrompido:\nO caractere de nova linha era esperado, mas o caractere número %u foi encontrado.\n", C);
      return EXIT_FAILURE;
    }
    nova->prox = ll;
    ll = nova;
    //fprintf (stderr, "%s\t%u\tinserted.\n", nova->nome, nova->idade);
  }
  fclose(file);
  while (1)
  {
    char resposta = 0;
    printf ("Para inserir digite i\nPara pesquisar digite p\nPara deletar digite d :");
    resposta = getchar();
    if (EOF==resposta) break;
    if (resposta!='\n') getchar();
    if (resposta == 'i')
    {
      Pessoa *nova = malloc (sizeof (Pessoa));
      if (NULL == nova)
      {
       fprintf (stderr, "Não há memoria suficiente.\n");
        break;
      }
      printf ("Digite o nome:");
      if (NULL==fgets(nova->nome,NOMELEN,stdin))    
      {
        fprintf (stderr, "fgets retornou NULL.\n");
        break;
      }
      char *pc = index (nova->nome,'\n');
      if (pc!=NULL)
      {
        *pc = 0;
      }
      printf ("Digite a idade:");    
      if (0 == scanf ("%hu",&nova->idade))
      {
       fprintf (stderr, "Você não digitou a idade.\n");
       continue;
      }
      getchar();
      nova->prox = ll;
      ll = nova;
      FILE *file = fopen ("pessoas.db","ab");
      if (file == NULL)
      {
        fprintf (stderr, "Não consegui abrir o arquivo pessoas.db para edição.\n");
        return EXIT_FAILURE;
      }
      if (fprintf (file, "%s\n%hu\n", nova->nome, nova->idade) < 0)
      {
        fprintf (stderr, "fprintf falhou em escrever em pessoas.db .\n");
        return EXIT_FAILURE;
      }
      if (EOF == fclose(file))
      {
        fprintf (stderr, "fclose deu erro após inserir uma pessoa.\n");
        return EXIT_FAILURE;
      }
      printf ("Pessoa inserida.\n");
    }
    else if (resposta == 'p')
    {
      printf ("Para pesquisar pelo nome digite n\nPara pesquisar pela idade digite i : ");
      resposta = getchar();
      getchar();
      if (resposta == 'n')
      {
        char nome [NOMELEN];
        printf ("Digite o nome:");
        if (NULL==fgets (nome, NOMELEN, stdin))
        {
          fprintf (stderr, "fgets retornou NULL.\n");
          return EXIT_FAILURE;
        }
        char *pc = index (nome,'\n');
        if (pc!=NULL)
        {
          *pc = 0;
        }
        unsigned long int n = 0;
        Pessoa *corrente = ll;
        while (corrente!=NULL)
        {
          if (strcasestr(corrente->nome, nome))
          {
            printf ("Nome:%s\nIdade:%hu\n\n", corrente->nome, corrente->idade);
            n++;
          }
          corrente = corrente->prox;
        }
        printf ("Encontrei %lu resultados.\n",n);
      } 
      else if (resposta == 'i')
      {
        unsigned short int idade = -1;
        printf ("Digite a idade:");
        scanf ("%hd" , &idade);
        getchar();
        unsigned long int n = 0;
        Pessoa *corrente = ll;
        while (corrente!=NULL)
        {
          //printf ("Estou antes do if. corrente->nome = %sidade = %hd\n",corrente->nome, idade);
          if (idade==(unsigned short int)-1)
          {
            printf ("Nome:%s\nIdade:%hu\n\n",corrente->nome,corrente->idade);
            n++;
          }
          if (corrente->idade == idade)
          {
            printf ("Nome:%s\nIdade:%hu\n\n",corrente->nome,corrente->idade);
            n++;
          }
          corrente = corrente->prox;
        }
        printf ("Encontrei %lu resultados.\n",n);
      }
      else fprintf (stderr, "O caractere %c não é reconhecido.\n",resposta);
    }
    else if (resposta == 'd')
    {
      char nome [NOMELEN];
      unsigned short int idade = -1;
      Pessoa *corrente = ll, **anterior = &ll;
      printf ("Digite o nome:");
      if (NULL==fgets (nome, NOMELEN, stdin))
      {
        fprintf (stderr, "fgets retornou NULL.\n");
        return EXIT_FAILURE;
      }
      char *pc = index (nome,'\n');
      if (pc!=NULL)
      {
        *pc = 0;
      }
      printf ("Digite a idade:");
      scanf("%hu" , &idade);
      getchar();
      //printf ("ll = %p\n",ll);
      while (corrente!=NULL)
      {
        //printf ("corrente = %p\tanterior = %p\t*anterior = %p\n",corrente,anterior,*anterior);
        if (0 == strcasecmp (corrente->nome,nome))
        {
          if (corrente->idade == idade)
          {
            //printf ("Encontrado: corrente = %p\tcorrente->prox = %p\n",corrente,corrente->prox);
            //printf ("Linha 124: anterior = %p\t*anterior = %p\n",anterior,*anterior);
            *anterior = corrente->prox;
            free (corrente);
            corrente = *anterior;
            FILE *file = fopen ("pessoas.db~","wb");
            if (NULL==file)
            {
              fprintf (stderr, "Não pude criar o arquivo pessoas.db~ e por isto não pude deletar a pessoa determinada.\n");
              return EXIT_FAILURE;
            }
            {
              Pessoa *corrente = ll;
              while (corrente!=NULL)
              {
                if (fprintf (file, "%s\n%u\n", corrente->nome, corrente->idade) < 0)
                {
                  fprintf (stderr, "fprintf retornou erro ao escrever para pessoas.db~ portanto não pude deletar a pessoa determinada.\n");
                  return EXIT_FAILURE;
                }
                corrente = corrente->prox;
              }
            }
             if (EOF==fclose(file))
             {
               fprintf (stderr, "fclose retornou erro ao fechar pessoas.db~ portanto não pude deletar a pessoa determinada.\n");
               return EXIT_FAILURE;
             }
             if (-1==rename("pessoas.db~", "pessoas.db"))
             {
               fprintf (stderr, "Não pude mover pessoas.db~ para pessoas.db .\n");
               return EXIT_FAILURE;
             }
            printf ("Pessoa apagada.\n");
          }
          else
          {
            anterior = &corrente->prox;
            corrente = corrente->prox;
          }
        }
        else
        {
          anterior = &corrente->prox;
          corrente = corrente->prox;
        }
      }
    }
    else 
    {
      fprintf (stderr, "O caractere %c não é reconhecido.\n",resposta);
    }
  }
  return EXIT_SUCCESS;
}
