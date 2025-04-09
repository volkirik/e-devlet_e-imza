#!/bin/bash

html_url="https://giris.turkiye.gov.tr/Giris/Elektronik-Imza"
html_file="edevlet.html"
jnlp_file="edevlet.jnlp"
jar_file="edevlet.jar"
log_file="edevlet.log"

# Sayfa kaynağı içindeki tek .jnlp linkini çek
jnlp_url=$(curl -o "$html_file" "$html_url">"$log_file"  2>&1 | grep -oP 'https?://[^"]+\.jnlp' "$html_file" | head -n 1)

# Sonucu yazdır
echo "$jnlp_url" >> "$log_file"

# JNLP içindeki tek .jar linkini çek
jar_pathname=$(curl -o "$jnlp_file" "$jnlp_url">>"$log_file"  2>&1 | grep -oP 'https?://[^"]+\.gov\.tr/[^"]+/e-imza[^"]*' "$jnlp_file" | head -n 1)
jar_filename=$(grep -oP '[^"]+\.jar' "$jnlp_file" | head -n 1)
jar_absolute="$jar_pathname"/"$jar_filename"

# Sonucu yazdır
echo "$jar_absolute" >> "$log_file"

# jar dosyasını indirip çalıştır
curl -o "$jar_file" "$jar_absolute">>"$log_file" 2>&1 && java -jar "$jar_file" >> "$log_file" 2>&1 &
