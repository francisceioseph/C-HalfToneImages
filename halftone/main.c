//
//  main.c
//  halftone
//
//  Created by Francisco José A. C. Souza on 05/04/16.
//  Copyright © 2016 Francisco José A. C. Souza. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

unsigned char** matrixAlloc(int width, int height);
void matrixDealloc(unsigned char **m, int width, int height);
void printMatrix(unsigned char **m, int width, int height);
void writePGM(unsigned char **m, int width, int height);

void calculateHalftoneCategories(unsigned char **m, int width, int height);
unsigned char **buildHalftoneMatrix(unsigned char **m, int width, int height);

int main(int argc, const char * argv[])
{
    //file to be read
    
    FILE *file = fopen("child.pgm", "r");
    
    if (file != NULL)
    {
        unsigned char buffer = 0;
        
//        Consome cabeçalhos inuteis do arquivo pgm
        
        while(getc(file) != '\n');
        while(getc(file) == '#');
        while(getc(file) != '\n');
        
        int width  = 0;
        int height = 0;
        int maxval = 0;
        
//        Realiza leitura de metadados da imagem
//        Como altura, largura e valor máximo de cor.
        
        fscanf(file, "%d", &width);
        fscanf(file, "%d", &height);
        fscanf(file, "%d", &maxval);
        
        printf("%d x %d\n", width, height);
        
//        Contadores de linha e coluna
        int row, col;
        
//        Alocação de de matriz de bytes da matriz
        unsigned char **m = matrixAlloc(width, height);
        
        getc(file);
        
//        Leitura da imagem PGM
        for(row = 0; row < height; row++)
        {
            for(col = 0; col < width; col++)
            {
                buffer = getc(file);
                m[row][col] = buffer;
            }
        }
        
//        Cálculo do halftone 2x2
        calculateHalftoneCategories(m, width, height);
        
//        Create the halftone matrix
        unsigned char **m2 = buildHalftoneMatrix(m, width, height);
        
        printMatrix(m2, 2*width, 2*height);
        
//        Save it on a pgm file
        writePGM(m2, 2*width, 2*height);
        
//        Close open files and dealloc matrixes
        
        fclose(file);
        matrixDealloc(m, width, height);
        matrixDealloc(m2, 2*width, 2*height);
    }
    
    return 0;
}

void calculateHalftoneCategories(unsigned char **m, int width, int height)
{
    unsigned char c;
    int row, col;
    
    for(row = 0; row < height; row++)
    {
        for (col = 0; col < width; col++)
        {
            c = m[row][col];
            
            if (c >= 0 && c <= 51)
            {
                m[row][col] = 0;
            }
            else
            {
                if (c > 51 && c <= 102)
                {
                    m[row][col] = 1;
                }
                else
                {
                    if (c > 102 && c <= 153)
                    {
                        m[row][col] = 2;
                    }
                    else
                    {
                        if (c > 153 && c < 204)
                        {
                            m[row][col] = 3;
                        }
                        else
                        {
                            m[row][col] = 4;
                        }
                    }
                }
            }
        }
    }

}

unsigned char **buildHalftoneMatrix(unsigned char **m, int width, int height)
{
    int row, col;
    
    unsigned char **m2 = matrixAlloc(width*2, height*2);
    
    for(row = 0; row < height; row ++)
    {
        for (col = 0; col < width; col++)
        {
            switch (m[row][col]) {
                case 0:
                    m2[2*row][2*col] = 0;
                    m2[2*row][2*col + 1] = 0;
                    m2[2*row + 1][2*col] = 0;
                    m2[2*row + 1][2*col +1] = 0;
                    
                    break;
                    
                case 1:
                    m2[2*row][2*col] = 128;
                    m2[2*row][2*col + 1] = 0;
                    m2[2*row + 1][2*col] = 0;
                    m2[2*row + 1][2*col +1] = 0;
                    
                    break;
                    
                case 2:
                    m2[2*row][2*col] = 128;
                    m2[2*row][2*col + 1] = 0;
                    m2[2*row + 1][2*col] = 0;
                    m2[2*row + 1][2*col +1] = 128;
                    
                    break;
                    
                case 3:
                    m2[2*row][2*col] = 128;
                    m2[2*row][2*col + 1] = 128;
                    m2[2*row + 1][2*col] = 0;
                    m2[2*row + 1][2*col +1] = 128;
                    
                    break;
                    
                    
                default:
                    m2[2*row][2*col] = 128;
                    m2[2*row][2*col + 1] = 128;
                    m2[2*row + 1][2*col] = 128;
                    m2[2*row + 1][2*col +1] = 128;
                    
                    break;
            }
        }
    }
    
    return m2;
    
}

void printMatrix(unsigned char **m, int width, int height)
{
    int i, j;
    
    printf("\n");
    
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            printf("%03u ", m[i][j]);
        }
        
        printf("\n");
    }
}

unsigned char** matrixAlloc(int width, int height)
{
    unsigned char **m = malloc(sizeof(unsigned char*) * height);
    
    if (m!= NULL)
    {
        int i = 0;
        
        for(i = 0; i < height; i++)
        {
            m[i] = malloc(sizeof(unsigned char) * width);
        }
    }
    
    return m;
}

void matrixDealloc(unsigned char **m, int width, int height)
{
    if(m != NULL)
    {
        int i = 0;
        
        for(i = 0; i < height; i++)
        {
            if (m[i] != NULL)
            {
                free(m[i]);
            }
        }
        
        free(m);
    }
}

void writePGM(unsigned char **m, int width, int height)
{
    int i, j;
    
    FILE *file = fopen("child24.pgm", "w");
    
    fprintf(file, "P5\n");
    fprintf(file, "%d %d\n", width, height);
    fprintf(file, "128\n");
    
    for (i = 0; i < height; i++){
        for (j = 0; j < width; j++) {
            fputc(m[i][j], file);
        }
    }
    
    fflush(file);
    fclose(file);
}

