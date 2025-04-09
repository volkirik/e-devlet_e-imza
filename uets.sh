wget -O uets.jnlp https://api.etebligat.gov.tr/v1/auth/_eimza/uets-eimza.jnlp -o uets.log 2>&1
javaws -verbose uets.jnlp >> uets.log 2>&1 &
