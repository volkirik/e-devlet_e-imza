wget -O kep.jnlp https://hs01.kep.tr/download/pttkep-eimza.jnlp -o kep.log 2>&1
javaws -verbose kep.jnlp >> kep.log 2>&1 &
