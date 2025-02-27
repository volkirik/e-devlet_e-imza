#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HTML_URL "https://giris.turkiye.gov.tr/Giris/Elektronik-Imza"
#define HTML_FILE "edevlet.html"
#define JNLP_FILE "edevlet.jnlp"
#define JAR_FILE "edevlet.jar"
#define LOG_FILE "edevlet.log"

void execute_command(const char *command) {
    printf("Executing: %s\n", command);
    system(command);
}

int main() {
    char command[512];
    char jnlp_url[256] = "";
    char jar_pathname[256] = "";
    char jar_filename[128] = "";
    char jar_absolute[512] = "";

    // Sayfa kaynağını indir
    snprintf(command, sizeof(command), "curl -s \"%s\" -o \"%s\"", HTML_URL, HTML_FILE);
    execute_command(command);

    // JNLP URL'sini çek
    snprintf(command, sizeof(command), "grep -oP 'https?://[^\"]+\\.jnlp' \"%s\" | head -n 1 > temp.txt", HTML_FILE);
    execute_command(command);
    FILE *file = fopen("temp.txt", "r");
    if (file) {
        fgets(jnlp_url, sizeof(jnlp_url), file);
        fclose(file);
    }
    remove("temp.txt");
    
    // Log dosyasına yaz
    snprintf(command, sizeof(command), "echo \"JNLP URL: %s\" >> \"%s\"", jnlp_url, LOG_FILE);
    execute_command(command);

    // JNLP dosyasını indir
    snprintf(command, sizeof(command), "curl -s \"%s\" -o \"%s\"", jnlp_url, JNLP_FILE);
    execute_command(command);

    // JNLP içindeki ilk .jar linkini çek
    snprintf(command, sizeof(command), "grep -oP 'https?://[^\"]+\\.gov\\.tr/[^\"]+/e-imza[^\"]*' \"%s\" | head -n 1 > temp.txt", JNLP_FILE);
    execute_command(command);
    file = fopen("temp.txt", "r");
    if (file) {
        fgets(jar_pathname, sizeof(jar_pathname), file);
        fclose(file);
    }
    remove("temp.txt");
    
    snprintf(command, sizeof(command), "grep -oP '[^\"]+\\.jar' \"%s\" | head -n 1 > temp.txt", JNLP_FILE);
    execute_command(command);
    file = fopen("temp.txt", "r");
    if (file) {
        fgets(jar_filename, sizeof(jar_filename), file);
        fclose(file);
    }
    remove("temp.txt");
    
    snprintf(jar_absolute, sizeof(jar_absolute), "%s/%s", jar_pathname, jar_filename);
    
    // Log dosyasına yaz
    snprintf(command, sizeof(command), "echo \"JAR URL: %s\" >> \"%s\"", jar_absolute, LOG_FILE);
    execute_command(command);

    // JAR dosyasını indir ve çalıştır
    snprintf(command, sizeof(command), "curl -s \"%s\" -o \"%s\"", jar_absolute, JAR_FILE);
    if (system(command) == 0) {
        snprintf(command, sizeof(command), "java -jar \"%s\" >> \"%s\" 2>&1 &", JAR_FILE, LOG_FILE);
        execute_command(command);
    } else {
        snprintf(command, sizeof(command), "echo \"JAR dosyası indirilemedi.\" >> \"%s\"", LOG_FILE);
        execute_command(command);
    }

    return 0;
}

