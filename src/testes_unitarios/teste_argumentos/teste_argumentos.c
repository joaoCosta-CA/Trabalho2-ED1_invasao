#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "argumentos.h"

int main() {
    printf("[ARGUMENTOS] Testando parsing de argumentos...\n");
    
    // Simula argv
    char *argv[] = {"ted", "-e", "/entrada", "-f", "mapa.geo", "-o", "/saida", "-q", "consulta.qry", "-to", "m", "-i", "15"};
    int argc = 13;
    
    const char *e = get_option_value(argc, argv, "e");
    const char *f = get_option_value(argc, argv, "f");
    const char *o = get_option_value(argc, argv, "o");
    const char *q = get_option_value(argc, argv, "q");
    const char *to = get_option_value(argc, argv, "to");
    const char *i = get_option_value(argc, argv, "i");
    
    printf("   > -e: %s\n", e ? e : "(null)");
    printf("   > -f: %s\n", f ? f : "(null)");
    printf("   > -o: %s\n", o ? o : "(null)");
    printf("   > -q: %s\n", q ? q : "(null)");
    printf("   > -to: %s\n", to ? to : "(null)");
    printf("   > -i: %s\n", i ? i : "(null)");
    
    assert(e != NULL && strcmp(e, "/entrada") == 0);
    assert(f != NULL && strcmp(f, "mapa.geo") == 0);
    assert(o != NULL && strcmp(o, "/saida") == 0);
    assert(q != NULL && strcmp(q, "consulta.qry") == 0);
    assert(to != NULL && strcmp(to, "m") == 0);
    assert(i != NULL && strcmp(i, "15") == 0);
    
    // Teste de argumento inexistente
    const char *x = get_option_value(argc, argv, "x");
    assert(x == NULL);
    
    printf(">>> SUCESSO: Modulo Argumentos OK!\n");
    return 0;
}
