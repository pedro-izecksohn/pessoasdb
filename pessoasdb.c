#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

typedef struct Pessoa
{
  char nome [32];
  unsigned short int idade;
  struct Pessoa *prox;
} Pessoa;
  
int main()
{
  Pessoa *ll = NULL;
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
        printf ("Não há memoria suficiente.\n");
        break;
      }
      char *pc;
      nova->prox = ll;
      printf ("Digite o nome:");
      if (NULL==fgets(nova->nome,32,stdin))    
      {
        printf ("fgets retornou NULL.\n");
        break;
      }
      pc = index (nova->nome,'\n');
      if (pc!=NULL)
      {
        *pc = 0;
      }
      printf ("Digite a idade:");    
      if (0 == scanf ("%hu",&nova->idade))
      {
       printf("Você não digitou a idade.\n");
       continue;
      }
      getchar();
      ll = nova;
    }
    else if (resposta == 'p')
    {
      printf ("Para pesquisar pelo nome digite n\nPara pesquisar pela idade digite i : ");
      resposta = getchar();
      getchar();
      if (resposta == 'n')
      {
        char nome [32];
        printf ("Digite o nome:");
        if (NULL==fgets (nome, 32, stdin))
        {
          printf ("fgets retornou NULL.\n");
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
            printf ("Nome:%s\nIdade:%hu\n\n",corrente->nome,corrente->idade);
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
      else printf ("O caractere %c não é reconhecido.\n",resposta);
    }
    else if (resposta == 'd')
    {
      char nome [32];
      unsigned short int idade = -1;
      Pessoa *corrente = ll, **anterior = &ll;
      printf ("Digite o nome:");
      if (NULL==fgets (nome, 32, stdin))
      {
        printf ("fgets retornou NULL.\n");
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
      printf ("O caractere %c não é reconhecido.\n",resposta);
    }
  }
  return EXIT_SUCCESS;
}
