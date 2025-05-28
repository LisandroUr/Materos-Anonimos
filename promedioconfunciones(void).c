#include <stdio.h>

int calcularpromedio(float sumatoria, int cantnotas){
	return sumatoria/ cantnotas;
}

	void imprimirnotas(int cantnotas, float promedio) {
		printf("***** Resultado de las notas ingresadas *****\n");
		printf("Cantidad de notas procesadas: %d\n", cantnotas);
		printf("Promedio: %.2f\n", promedio);
		printf("**********************************************\n");
	}
	
int main(int argc, char *argv[]) {
	float notas, promedio, acumul=0;
	int cantnotas=3;
	
	for (int i=0; i< cantnotas; i++){
		printf("Ingrese la nota: \n");
		scanf("%f", &notas);
		acumul+=notas;
	}
	
	promedio=calcularpromedio(acumul, cantnotas);
	
	imprimirnotas(cantnotas, promedio);

	return 0;
}
	
