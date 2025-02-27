#!/bin/bash

html_url="https://giris.turkiye.gov.tr/Giris/Elektronik-Imza"
html_file="edevlet.html"
jnlp_file="edevlet.jnlp"
jar_file="edevlet.jar"
log_file="edevlet.log"

# Sayfa kaynağını indir
curl -s "$html_url" -o "$html_file"

# Sayfa kaynağı içindeki ilk .jnlp linkini çek
jnlp_url=$(grep -oP 'https?://[^"]+\.jnlp' "$html_file" | head -n 1)

# Sonucu log dosyasına yaz
echo "JNLP URL: $jnlp_url" >> "$log_file"

# JNLP dosyasını indir
curl -s "$jnlp_url" -o "$jnlp_file"

# JNLP içindeki ilk .jar linkini çek
jar_pathname=$(grep -oP 'https?://[^"]+\.gov\.tr/[^"]+/e-imza[^"]*' "$jnlp_file" | head -n 1)
jar_filename=$(grep -oP '[^"]+\.jar' "$jnlp_file" | head -n 1)
jar_absolute="$jar_pathname"/"$jar_filename"

# Sonucu log dosyasına yaz
echo "JAR URL: $jar_absolute" >> "$log_file"

# JAR dosyasını indir ve çalıştır
if curl -s "$jar_absolute" -o "$jar_file"; then
    java -jar "$jar_file" >> "$log_file" 2>&1 &
else
    echo "JAR dosyası indirilemedi." >> "$log_file"
fi

