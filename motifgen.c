#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

void generateRandomMotif(char motifString[], int length);
int createDNAMotif(char dnaSequence[], int dna_length, char motifString[], int length, int numMutations);
void printDNA(char dnaSequence[], int dna_length, int motifPos, int motifLength, int showMarks);


char nucleotides[] = {'A', 'T', 'G', 'C'};

int main(int argc, char const *argv[]) {
  char motifLength;  // Length of the hidden motif in each DNA sequence.
  int dnaLength;     // Length of each DNA sequence.
  int numSequences;  // Number of DNA sequences.
  int numMutations;

  if (argc < 5) {
    printf("\nUsage: > %s motif_length dna_length n_sequences n_mutations (¿marcas?)\n", argv[0]);
    printf("\n\t Para que salgan marcas que indiquen en que parte de las secuencias está el patrón oculto, como último parámetro pongan una M.\n\n");
    return 0;
  }

  sscanf(argv[1],"%d", &motifLength);
  sscanf(argv[2],"%d", &dnaLength);
  sscanf(argv[3],"%d", &numSequences);
  sscanf(argv[4],"%d", &numMutations);

  int marcas = FALSE;
  if (argc == 6) {
    if (argv[5][0] == 'M')
      marcas = TRUE;
  }

  if (dnaLength < motifLength) {
    printf("\nDNA's length must be greater than motif's length.\n");
    return 0;
  }

  srand(time(NULL));

  char motif[motifLength+1];
  char dnaSequence[dnaLength+1];
  motif[motifLength] = '\0';
  dnaSequence[dnaLength] = '\0';

  generateRandomMotif(motif, motifLength);

  printf("%d\n", motifLength);
  printf("%s\n", motif);
  printf("%d\n", dnaLength);
  printf("%d\n", numSequences);

  for (size_t i = 0; i < numSequences; i++) {
    int mPos = createDNAMotif(dnaSequence, dnaLength, motif, motifLength, numMutations);
    printDNA(dnaSequence, dnaLength, mPos, motifLength, marcas);
  }

  printf("\n\n");

  return 0;
}

void printDNA(char dnaSequence[], int dna_length, int motifPos, int motifLength, int showMarks){

  if (showMarks == TRUE) {
    for (size_t i = 0; i < motifPos; i++)
      printf(" ");
    printf("v");

    if (motifLength > 1) {
      for (size_t i = 0; i < motifLength-2; i++)
        printf(" ");

      printf("v");
    }
    printf("\n");
  }

  printf("%s\n", dnaSequence);
}

void generateRandomMotif(char motifString[], int length) {
    int r;

    for (size_t i = 0; i < length; i++) {
      r = rand()%4; // Select random nucleotide
      motifString[i] = nucleotides[r];
    }
}

int createDNAMotif(char dnaSequence[], int dnaLength, char motifString[], int motifLength, int numMutations) {
  int r;

  //printf("\n");
  for (size_t i = 0; i < dnaLength; i++) {
    r = rand() % 4; // Select random nucleotide
    dnaSequence[i] = nucleotides[r];
    //printf("%c", dnaSequence[i]);
  }
  //printf("\n");

  // Insert the motif in a middle point of the DNA sequence.
  int motifPosition = rand() % (dnaLength - motifLength);

  double pm = 1.0 * numMutations / motifLength;
  //printf("mut=%f\n", pm);

  for (size_t i = 0, p = motifPosition; i < motifLength; i++, p++) {
    double flip = (1.0*rand()) / RAND_MAX;
    //printf("r=%f\n", flip);
    if ( flip < pm) {
      dnaSequence[p] = nucleotides[rand()%4];
    }
    else
      dnaSequence[p] = motifString[i];
  }

  return motifPosition;
}
